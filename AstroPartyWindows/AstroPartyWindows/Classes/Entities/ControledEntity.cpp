#include "../Entity.h"



ControledEntity::ControledEntity() :
	DynamicEntity(),
	heat_box_vertexes_array(nullptr),
	player_number(0),
	rotate_input_value_pointer(nullptr),
	shoot_input_value_pointer(nullptr),
	player_team_number(0)
{
	SetDefaultMatrix();
}

ControledEntity::ControledEntity(const ControledEntity& controled_entity) :
	DynamicEntity(controled_entity),
	heat_box_vertexes_array(new Vec2F[controled_entity.heat_box_vertexes_array_length]),
	heat_box_vertexes_array_length(controled_entity.heat_box_vertexes_array_length),
	player_number(controled_entity.player_number),
	player_team_number(controled_entity.player_team_number),
	rotate_input_value_pointer(controled_entity.rotate_input_value_pointer),
	shoot_input_value_pointer(controled_entity.shoot_input_value_pointer)
{
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = controled_entity.heat_box_vertexes_array[vertex];
	}
}

ControledEntity::ControledEntity(
	Vec2F position,
	Vec2F velocity,
	float radius,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle, float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
	:
	DynamicEntity(
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	player_number(player_number),
	player_team_number(player_team_number),
	rotate_input_value_pointer(rotate_input_value_pointer),
	shoot_input_value_pointer(shoot_input_value_pointer)
{
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
		return;
	}

	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];

	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

ControledEntity::ControledEntity(
	const Vec2F* position,
	const Vec2F* velocity,
	float radius,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
	:
	DynamicEntity(
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	player_number(player_number),
	player_team_number(player_team_number),
	rotate_input_value_pointer(rotate_input_value_pointer),
	shoot_input_value_pointer(shoot_input_value_pointer)
{
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
		return;
	}

	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];

	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

bool ControledEntity::Collision(const Map* map)
{
	bool collision = false;
	void* element_p;
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		element_p = (void*)map->RectanglePointer(i);
		if (((Rectangle*)element_p)->exist && DynamicEntity::Collision(((Rectangle*)element_p)))
		{
			if (((Rectangle*)element_p)->IsKiller())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		element_p = (void*)map->CyrclePointer(i);
		if (((Cyrcle*)element_p)->exist && DynamicEntity::Collision(((Cyrcle*)element_p)))
		{
			if (((Cyrcle*)element_p)->IsKiller())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		element_p = (void*)map->PolygonPointer(i);
		if (((Polygon*)element_p)->exist && DynamicEntity::Collision(((Polygon*)element_p)))
		{
			if (((Rectangle*)element_p)->IsKiller())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	return collision;
}

Color3F ControledEntity::GetColor() const
{
	switch (player_team_number)
	{
	case SHIPS_SELECT_BUTTONS_TEAM_RED:
		return Color3F(1.0f, 0.0f, 0.0f);
	case SHIPS_SELECT_BUTTONS_TEAM_GREEN:
		return Color3F(0.0f, 1.0f, 0.0f);
	case SHIPS_SELECT_BUTTONS_TEAM_BLUE:
		return Color3F(0.0f, 0.0f, 1.0f);
	case SHIPS_SELECT_BUTTONS_TEAM_PURPURE:
		return Color3F(1.0f, 0.0f, 1.0f);
	}
	return Color3F();
}

Mat3x2F ControledEntity::GetModelMatrix() const
{
	return model_matrix;
}

const Mat3x2F* ControledEntity::GetModelMatrixPointer() const
{
	return &model_matrix;
}

GameTypes::players_count_t ControledEntity::GetPlayerNumber() const
{
	return player_number;
}

GameTypes::players_count_t ControledEntity::GetTeamNumber() const
{
	return player_team_number;
}

bool ControledEntity::GetRotateInputValue() const
{
	return *(bool*)rotate_input_value_pointer;
}

bool ControledEntity::GetShootInputValue() const
{
	return *(bool*)shoot_input_value_pointer;
}

bool ControledEntity::IsCollision(const Beam* beam) const
{
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (beam->Distance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (beam->Distance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const Bomb* bomb) const
{
	//Frame of reference set to this entity.
	Segment segment = bomb->GetLastTreck();
	segment.vector -= velocity;
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;
	ce_side.Set(point1, point2, true);
	if (segment.Distance(&ce_side) < bomb->radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.Distance(&ce_side) < bomb->radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const Bullet* bullet) const
{
	//Frame of reference set to this entity.
	Segment segment = bullet->GetLastTreck();
	segment.vector -= velocity;
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;
	ce_side.Set(point1, point2, true);
	if (segment.Distance(&ce_side) < bullet->radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.Distance(&ce_side) < bullet->radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const DynamicEntity* dynamic_entity) const
{
	//Frame of reference set to this entity.
	Segment segment = dynamic_entity->GetLastTreck();
	segment.vector -= velocity;
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;
	ce_side.Set(point1, point2, true);
	if (segment.Distance(&ce_side) < dynamic_entity->radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.Distance(&ce_side) < dynamic_entity->radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const Knife* knife) const
{
	//Frame of reference set to this entity.
	Segment segment = knife->GetSegment();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2);
	if (segment.Distance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2);
		if (segment.Distance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const Laser* laser) const
{
	//Frame of reference set to this entity.
	Beam beam = laser->GetBeam();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (beam.Distance(&ce_side) < radius + laser->width)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (beam.Distance(&ce_side) < radius + laser->width)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const MegaLaser* mega_laser) const
{
	//Frame of reference set to this entity.
	Segment segment = mega_laser->GetSegment();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	Vec2F nearest;
	Line perpendicular = Line(position, segment.vector.Perpendicular());

	if (perpendicular.Intersection(&segment, &nearest) && nearest.Distance(position) < radius + mega_laser->width)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (perpendicular.Intersection(&segment, &nearest) && nearest.Distance(position) < radius + mega_laser->width)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(const Segment* segment) const
{
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (segment->IsIntersection(&ce_side))
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment->IsIntersection(&ce_side))
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsColectEntity(const Entity* stored_entity) const
{
	Segment ce_side;//side of the heatbox of the controled entity
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	Beam test_beam;
	test_beam.Set(stored_entity->GetPosition(), Vec2F(1.0f, 0.0f));
	EngineTypes::ControledEntity::heat_box_vertexes_count_t intersections_count = 0;

	ce_side.Set(point1, point2, true);
	if (stored_entity->GetDistance(&ce_side) < stored_entity->radius)
	{
		return true;
	}
	else if (test_beam.IsIntersection(&ce_side))
	{
		intersections_count++;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (stored_entity->GetDistance(&ce_side) < stored_entity->radius)
		{
			return true;
		}
		else if (test_beam.IsIntersection(&ce_side))
		{
			intersections_count++;
		}
	}
	return (intersections_count & 1) ? true : false;

}

bool ControledEntity::IsColectEntity(const DynamicEntity* stored_entity) const
{
	if (IsCollision(stored_entity))
	{
		return true;
	}
	Segment ce_side;//side of the heatbox of the controled entity
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * 2.0f * model_matrix,
		point2 = heat_box_vertexes_array[0] * 2.0f * model_matrix;

	Beam test_beam;
	test_beam.Set(stored_entity->GetPosition(), Vec2F(1.0f, 0.0f));
	EngineTypes::ControledEntity::heat_box_vertexes_count_t intersections_count = 0;

	ce_side.Set(point1, point2, true);
	if (test_beam.IsIntersection(&ce_side))
	{
		intersections_count++;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * 2.0f * model_matrix;
		ce_side.Set(point1, point2, true);
		if (test_beam.IsIntersection(&ce_side))
		{
			intersections_count++;
		}
	}
	return (intersections_count & 1) ? true : false;

}

bool ControledEntity::IsSameTeams(const ControledEntity* second_entity) const
{
	return player_team_number == second_entity->player_team_number;
}

void ControledEntity::Set(const ControledEntity* controled_entity)
{
	angle = controled_entity->angle;
	angular_velocity = controled_entity->angular_velocity;
	direction = controled_entity->direction;
	exist = controled_entity->exist;
	force_collision_coeffisient = controled_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = controled_entity->force_resistance_air_coefficient;
	heat_box_vertexes_array_length = controled_entity->heat_box_vertexes_array_length;
	player_number = controled_entity->player_number;
	player_team_number = controled_entity->player_team_number;
	position = controled_entity->position;
	radius = controled_entity->radius;
	rotate_input_value_pointer = controled_entity->rotate_input_value_pointer;
	shoot_input_value_pointer = controled_entity->shoot_input_value_pointer;
	velocity = controled_entity->velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = controled_entity->heat_box_vertexes_array[vertex];
	}
}

void ControledEntity::Set(
	Vec2F position, 
	Vec2F velocity,
	float radius, 
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient, 
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->velocity = velocity;

	delete[] this->heat_box_vertexes_array;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

void ControledEntity::Set(
	const Vec2F* position, 
	const Vec2F* velocity,
	float radius, 
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* rotate_input_value_pointer, 
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient, 
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = *position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->velocity = *velocity;

	delete[] this->heat_box_vertexes_array;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

void ControledEntity::SetDefaultMatrix()
{
	heat_box_vertexes_array_length = 3;
	if (heat_box_vertexes_array != nullptr)
	{
		delete[] heat_box_vertexes_array;
	}
	heat_box_vertexes_array = new Vec2F[3];
	heat_box_vertexes_array[0].Set(0.40f, 0.0f);
	heat_box_vertexes_array[1].Set(-0.35f, sqrt(3.0f) / 4.0f);
	heat_box_vertexes_array[2].Set(-0.35f, sqrt(3.0f) / -4.0f);
}

bool ControledEntity::ShouldRotate()
{
	return *(bool*)rotate_input_value_pointer;
}

bool ControledEntity::ShouldShoot()
{
	return *(bool*)shoot_input_value_pointer;
}

void ControledEntity::UpdateMatrix()
{
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(1.0f, 1.0f) * radius);
}

void ControledEntity::operator=(ControledEntity controled_entity)
{
	angle = controled_entity.angle;
	angular_velocity = controled_entity.angular_velocity;
	direction = controled_entity.direction;
	exist = controled_entity.exist;
	force = controled_entity.force;
	force_collision_coeffisient = controled_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = controled_entity.force_resistance_air_coefficient;
	heat_box_vertexes_array_length = controled_entity.heat_box_vertexes_array_length;
	player_number = controled_entity.player_number;
	player_team_number = controled_entity.player_team_number;
	position = controled_entity.position;
	radius = controled_entity.radius;
	rotate_input_value_pointer = controled_entity.rotate_input_value_pointer;
	shoot_input_value_pointer = controled_entity.shoot_input_value_pointer;
	velocity = controled_entity.velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = controled_entity.heat_box_vertexes_array[vertex];
	}
}

ControledEntity::~ControledEntity()
{
	delete[] heat_box_vertexes_array;
}