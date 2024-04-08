#include "../Entity.h"



ControledEntity::ControledEntity() :
	DynamicEntity(),
	controle_flags_pointer(nullptr),
	heat_box_vertexes_array(),
	model_matrix(),
	player_number(0),
	player_team_number(0)
{
	SetDefaultMatrix();
}

ControledEntity::ControledEntity(const ControledEntity& controled_entity) :
	DynamicEntity(controled_entity),
	controle_flags_pointer(controled_entity.controle_flags_pointer),
	heat_box_vertexes_array(controled_entity.heat_box_vertexes_array),
	model_matrix(controled_entity.model_matrix),
	player_number(controled_entity.player_number),
	player_team_number(controled_entity.player_team_number)
{
}

ControledEntity::ControledEntity(
	const Vec2F& position,
	const Vec2F& velocity,
	float radius,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags_pointer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
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
	controle_flags_pointer(controle_flags_pointer),
	heat_box_vertexes_array(heat_box_vertexes_array),
	player_number(player_number),
	player_team_number(player_team_number)
{
	if (this->heat_box_vertexes_array.size() < 3)
	{
		this->heat_box_vertexes_array.clear();
		SetDefaultMatrix();
	}

	UpdateMatrix();
}

bool ControledEntity::Collision(const Map::MapData& map)
{
	bool collision = false;
	for (auto& element : map.cyrcles_array)
	{
		collision |= CollisionWithElement(element);
	}
	for (auto& element : map.polygons_array)
	{
		collision |= CollisionWithElement(element);
	}
	for (auto& element : map.rectangles_array)
	{
		collision |= CollisionWithElement(element);
	}
	return collision;
}

template<typename MapElementT>
bool ControledEntity::CollisionWithElement(MapElementT& element)
{
	auto collision = false;
	if (element.exist && DynamicEntity::Collision(element))
	{
		if (element.IsKiller())
		{
			exist = false;
			return true;
		}
		collision = true;
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

const GameTypes::control_flags_t* ControledEntity::GetControleFlagsP() const
{
	return controle_flags_pointer;
}

std::vector<Segment> ControledEntity::GetGlobalHeatBox() const
{
	auto out_array = std::vector<Segment>(heat_box_vertexes_array.size());

	auto p = heat_box_vertexes_array.back() * model_matrix;
	size_t i = 0;
	for (const auto& v : heat_box_vertexes_array)
	{
		const auto p2 = p;
		p = v * model_matrix;
		out_array[i].Set(p, p2, true);
		++i;
	}

	return out_array;
}

std::vector<Segment> ControledEntity::GetLocalHeatBox() const
{
	auto out_array = std::vector<Segment>(heat_box_vertexes_array.size());

	auto p = heat_box_vertexes_array.back();
	size_t i = 0;
	for (const auto& v : heat_box_vertexes_array)
	{
		const auto p2 = p;
		p = v;
		out_array[i].Set(p, p2, true);
		++i;
	}

	return out_array;
}

Mat3x2F ControledEntity::GetModelMatrix() const
{
	return model_matrix;
}

Mat3x2F* ControledEntity::GetModelMatrixPointer()
{
	return &model_matrix;
}

const Mat3x2F* ControledEntity::GetModelMatrixPointerConst() const
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

template <typename Type>
bool ControledEntity::IsCollision(const Type& thing) const
{
	auto point2 = heat_box_vertexes_array.back() * model_matrix;
	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point1 = point2;
		point2 = vertex * model_matrix;
		const auto& hb_side = Segment(point1, point2, true);

		if (hb_side.IsIntersection(thing)) return true;
	}

	if (velocity.LengthPow2() < radius * radius / 4.0f) return false;

	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto& point = vertex * model_matrix;
		const auto& hb_side = Segment(point, velocity);

		if (hb_side.IsIntersection(thing)) return true;
	}

	return false;
}
template bool ControledEntity::IsCollision<Line>(const Line& line) const;
template bool ControledEntity::IsCollision<Beam>(const Beam& beam) const;
template bool ControledEntity::IsCollision<Segment>(const Segment& segment) const;

template <>
bool ControledEntity::IsCollision<DynamicEntity>(const DynamicEntity& entity) const
{	
	auto point2 = heat_box_vertexes_array.back() * model_matrix;
	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point1 = point2;
		point2 = vertex * model_matrix;
		const auto& hb_side = Segment(point1, point2, true);

		if (hb_side.Distance(entity.GetPosition()) < entity.radius)
			return true;
	}

	const auto& relative_velocity = entity.GetVelocity() - velocity;
	const float& sum_radius = radius + entity.radius;

	if (relative_velocity.LengthPow2() < sum_radius * sum_radius / 100.0f) return false;
	
	point2 = heat_box_vertexes_array.back() * model_matrix;
	const auto& relative_treck = Segment(entity.GetPosition(), relative_velocity);
	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point1 = point2;
		point2 = vertex * model_matrix;
		const auto& hb_side = Segment(point1, point2, true);

		if (hb_side.Distance(relative_treck) < entity.radius)
		{
			return true;
		}
	}

	return false;
}

template <>
bool ControledEntity::IsCollision<Knife>(const Knife& knife) const
{
	//Frame of reference set to this entity.
	const Segment& segment = knife.GetSegment();
	//controled entity side
	Segment ce_side;
	auto point2 = heat_box_vertexes_array.back() * model_matrix;
	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point1 = point2;
		point2 = vertex * model_matrix;
		const auto& hb_side = Segment(point1, point2, true);

		if (hb_side.IsIntersection(segment)) return true;
	}
	return false;
}

template <>
bool ControledEntity::IsCollision<Laser>(const Laser& laser) const
{
	//Frame of reference set to this entity.
	const Segment& segment = laser.GetSegment();
		//controled entity side
		Segment ce_side;
	auto point2 = heat_box_vertexes_array.back() * model_matrix;
	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point1 = point2;
		point2 = vertex * model_matrix;
		const auto& hb_side = Segment(point1, point2, true);

		if (hb_side.Distance(segment) < laser.width) return true;
	}
	return false;
}

bool ControledEntity::IsColectEntity(const DynamicEntity& stored_entity) const
{
	if (IsCollision(stored_entity))
	{
		return true;
	}

	Vec2F point1 = heat_box_vertexes_array.back() * 2.0f * model_matrix;

	const Beam& test_beam = Beam(stored_entity.GetPosition(), Vec2F(1.0f, 0.0f));
	EngineTypes::ControledEntity::heat_box_vertexes_count_t intersections_count = 0;

	for (const auto& vertex : heat_box_vertexes_array)
	{
		const auto point2 = point1;
		point1 = vertex * 2.0f * model_matrix;
		const auto& ce_side = Segment(point1, point2, true);
		if (ce_side.IsIntersection(test_beam))
		{
			intersections_count++;
		}
	}
	return (intersections_count & 1) ? true : false;

}

bool ControledEntity::IsSameTeams(const ControledEntity& second_entity) const
{
	return player_team_number == second_entity.player_team_number;
}

bool ControledEntity::IsTooSlow() const
{
	return velocity.LengthPow2() < radius * radius / 100.0f;
}

void ControledEntity::Set(const ControledEntity* controled_entity)
{
	DynamicEntity::Set(controled_entity);

	controle_flags_pointer = controled_entity->controle_flags_pointer;
	heat_box_vertexes_array = controled_entity->heat_box_vertexes_array;
	model_matrix = controled_entity->model_matrix;
	player_number = controled_entity->player_number;
	player_team_number = controled_entity->player_team_number;
}

void ControledEntity::Set(
	const Vec2F& position, 
	const Vec2F& velocity,
	float radius, 
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags_pointer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient, 
	bool exist)
{
	DynamicEntity::Set(
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist);

	this->controle_flags_pointer = controle_flags_pointer;
	this->heat_box_vertexes_array = heat_box_vertexes_array;
	this->player_number = player_number;
	this->player_team_number = player_team_number;

	UpdateMatrix();
}

void ControledEntity::SetDefaultMatrix()
{
	heat_box_vertexes_array = 
	{
		Vec2F(0.40f, 0.0f), 
		Vec2F(-0.35f, sqrt(3.0f) / 4.0f), 
		Vec2F(-0.35f, sqrt(3.0f) / -4.0f)
	};
}

bool ControledEntity::ShouldDash() const
{
	return controle_flags_pointer->burnout_flags[player_number];
}

bool ControledEntity::ShouldRotate() const
{
	return controle_flags_pointer->rotate_flags[player_number];
}

bool ControledEntity::ShouldShoot() const
{
	return controle_flags_pointer->shoot_flags[player_number];
}

void ControledEntity::UpdateMatrix()
{
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(1.0f, 1.0f) * radius);
}

void ControledEntity::operator=(const ControledEntity& controled_entity)
{
	DynamicEntity::operator=(controled_entity);

	controle_flags_pointer = controled_entity.controle_flags_pointer;
	heat_box_vertexes_array = controled_entity.heat_box_vertexes_array;
	model_matrix = controled_entity.model_matrix;
	player_number = controled_entity.player_number;
	player_team_number = controled_entity.player_team_number;
}

ControledEntity::~ControledEntity()
{
}