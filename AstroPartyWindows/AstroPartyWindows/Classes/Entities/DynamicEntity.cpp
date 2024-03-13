#include "../Entity.h"



DynamicEntity::DynamicEntity() :
	Entity(),
	angular_velocity(0.0f),
	force_collision_coeffisient(DEFAULT_FORCE_COLLISION_COEFFICIENT),
	force_resistance_air_coefficient(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
}

DynamicEntity::DynamicEntity(const DynamicEntity& dynamic_entity) :
	Entity(dynamic_entity),
	angular_velocity(dynamic_entity.angular_velocity),
	force(dynamic_entity.force),
	force_collision_coeffisient(dynamic_entity.force_collision_coeffisient),
	force_resistance_air_coefficient(dynamic_entity.force_resistance_air_coefficient),
	velocity(dynamic_entity.velocity)
{
}

DynamicEntity::DynamicEntity(
	Vec2F position, 
	Vec2F velocity, 
	float radius,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient,
	bool exist) 
	:
	Entity(
		position,
		radius,
		angle, 
		exist),
	angular_velocity(angular_velocity),
	force_collision_coeffisient(force_collision_coeffisient),
	force_resistance_air_coefficient(force_resistance_air_coefficient),
	velocity(velocity)
{
}

DynamicEntity::DynamicEntity(
	const Vec2F* position, 
	const Vec2F* velocity, 
	float radius,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist) 
	:
	Entity(
		position,
		radius,
		angle, 
		exist),
	angular_velocity(angular_velocity),
	force_collision_coeffisient(force_collision_coeffisient),
	force_resistance_air_coefficient(force_resistance_air_coefficient),
	velocity(*velocity)
{
}

void DynamicEntity::AddForce(Vec2F force)
{
	this->force += force;
}

void DynamicEntity::AddForce(const Vec2F* force)
{
	this->force += *force;
}

void DynamicEntity::AddForceAlongDirection(float force)
{
	this->force += direction * force;
}

void DynamicEntity::AddAngularVelocity(float angular_velocity)
{
	this->angular_velocity += angular_velocity;
}

void DynamicEntity::AddGravityForce(float gravity_coeffisient, Vec2F forced_point)
{
	Vec2F gvnn = forced_point - position;//grav_vec_not_normalize
	force += gvnn.Normalize() * gravity_coeffisient / (gvnn.x * gvnn.x + gvnn.y * gvnn.y);
}

void DynamicEntity::AddGravityForce(float gravity_coeffisient, const Vec2F* forced_point)
{
	Vec2F gvnn = *forced_point - position;//grav_vec_not_normalize
	force += gvnn.Normalize() * gravity_coeffisient / (gvnn.x * gvnn.x + gvnn.y * gvnn.y);
}

void DynamicEntity::AddVelocity(Vec2F velocity)
{
	this->velocity += velocity;
}

void DynamicEntity::AddVelocity(const Vec2F* velocity)
{
	this->velocity += *velocity;
}

void DynamicEntity::ClearForce()
{
	force.Set(0.0f, 0.0f);
}

bool DynamicEntity::Collision(DynamicEntity* entity)
{
	float distance;
	if ((distance = GetDistance(entity)) > 0.0f)
	{
		return false;
	}
	Vec2F collision_direction = position - entity->position;
	Vec2F local_velocity = velocity - entity->velocity;

	if (collision_direction * local_velocity < 0.0f)
	{
		local_velocity = collision_direction.Project(local_velocity);
		velocity -= local_velocity / 2.0f;
		entity->velocity += local_velocity / 2.0f;
		position -= collision_direction.Normalize() * distance;
		entity->position += collision_direction.Normalize() * distance;
	}

	return true;
}

bool DynamicEntity::Collision(const StaticEntity* entity)
{
	float distance;
	if ((distance = GetDistance(entity)) > 0.0f)
	{
		return false;
	}
	Vec2F collision_direction = position - entity->GetPosition();
	Vec2F local_velocity = velocity - entity->GetVelocity();

	if (collision_direction * local_velocity < 0.0f)
	{
		local_velocity = collision_direction.Project(local_velocity);
		velocity -= local_velocity;
		position -= collision_direction.Normalize() * distance;
	}


	return true;
}

bool DynamicEntity::Collision(const Map::Rectangle* rectangle)
{
	Vec2F nearest_position1;
	Vec2F collision_direction;
	Vec2F rectangle_velocity = rectangle->GetVelocity();
	float distance;
	bool collision = false;

	int i = 0;
	Segment side = rectangle->UpSide();

	if ((distance = side.Distance(&position, &nearest_position1)) < radius || GetTreck().IsIntersection(&side))
	{
		collision_direction = nearest_position1 - position;
		if (position.y > side.point.y)
		{
			position.y += radius - distance + force_collision_coeffisient;
			if (force.y < 0.0f)
			{
				force.y = 0.0f;
			}
		}
		else
		{
			position.y -= radius - distance + force_collision_coeffisient;
			if (force.y > 0.0f)
			{
				force.y = 0.0f;
			}
		}
		velocity -= collision_direction.ProjectSign(&velocity);
		velocity += Vec2F(0.0f, 1.0f).ProjectSign(rectangle_velocity - velocity);

		collision = true;
	}

	side = rectangle->DownSide();
	if ((distance = side.Distance(&position, &nearest_position1)) < radius || GetTreck().IsIntersection(&side))
	{
		collision_direction = nearest_position1 - position;
		if (position.y > side.point.y)
		{
			position.y += radius - distance + force_collision_coeffisient;
			if (force.y < 0.0f)
			{
				force.y = 0.0f;
			}
		}
		else
		{
			position.y -= radius - distance + force_collision_coeffisient;
			if (force.y > 0.0f)
			{
				force.y = 0.0f;
			}
		}
		velocity -= collision_direction.ProjectSign(&velocity);
		velocity += Vec2F(0.0f, -1.0f).ProjectSign(rectangle_velocity - velocity);

		collision = true;
	}

	side = rectangle->RightSide();
	if ((distance = side.Distance(&position, &nearest_position1)) < radius || GetTreck().IsIntersection(&side))
	{
		collision_direction = nearest_position1 - position;
		if (position.x > side.point.x)
		{
			position.x += radius - distance + force_collision_coeffisient;
			if (force.x < 0.0f)
			{
				force.x = 0.0f;
			}
		}
		else
		{
			position.x -= radius - distance + force_collision_coeffisient;
			if (force.x > 0.0f)
			{
				force.x = 0.0f;
			}
		}
		velocity -= collision_direction.ProjectSign(&velocity);
		velocity += Vec2F(1.0f, 0.0f).ProjectSign(rectangle_velocity - velocity);

		collision = true;
	}

	side = rectangle->LeftSide();
	if ((distance = side.Distance(&position, &nearest_position1)) < radius || GetTreck().IsIntersection(&side))
	{
		collision_direction = nearest_position1 - position;
		if (position.x > side.point.x)
		{
			position.x += radius - distance + force_collision_coeffisient;
			if (force.x < 0.0f)
			{
				force.x = 0.0f;
			}
		}
		else
		{
			position.x -= radius - distance + force_collision_coeffisient;
			if (force.x > 0.0f)
			{
				force.x = 0.0f;
			}
		}
		velocity -= collision_direction.ProjectSign(&velocity);
		velocity += Vec2F(-1.0f, 0.0f).ProjectSign(rectangle_velocity - velocity);

		collision = true;
	}
	return collision;
}

bool DynamicEntity::Collision(const Map::Cyrcle* cyrcle)
{
	float distance;
	bool inside;
	if ((distance = GetDistance(cyrcle, &inside)) > 0.0f)
	{
		return false;
	}

	//from this to cyrcle
	Vec2F collision_direction = cyrcle->GetPosition() - position;
	if (inside)
	{
		Vec2F d_pos = cyrcle->GetPosition() - collision_direction.Normalize() * (cyrcle->GetRadius() - radius - force_collision_coeffisient);
		force -= force.ProjectSign(position - d_pos);
		position = d_pos;
	}
	else
	{
		Vec2F d_pos = cyrcle->GetPosition() - collision_direction.Normalize() * (cyrcle->GetRadius() + radius + force_collision_coeffisient);
		force -= force.ProjectSign(position - d_pos);
		position = d_pos;
	}

	velocity -= collision_direction.Project(&velocity) / 2.0f;
	velocity += collision_direction.ProjectSign(cyrcle->GetVelocity() - velocity);
	return true;
}

bool DynamicEntity::Collision(const Map::Polygon* polygon)
{
	if (!polygon->exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon->PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment treck = GetLastTreck();
	Segment side = Segment(polygon->points_array[p_count - 1], polygon->points_array[0], true);
	bool collision = false;
	Vec2F collision_direction;//direction from position to collision point
	float distance;

	Vec2F d_pos;

	if (p_count > 2 && polygon->IsClosed())
	{
		distance = side.Distance(position, &collision_direction);
		if (distance < radius || side.IsIntersection(&treck))
		{
			d_pos = (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_coeffisient);
			force -= force.ProjectSign(d_pos);
			position += d_pos;
			collision_direction -= position;
			velocity -= collision_direction.ProjectSign(velocity);
			velocity += (-collision_direction).ProjectSign(polygon->Velocity(collision_direction + position) - velocity);

			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		side.Set(polygon->points_array[p - 1], polygon->points_array[p], true);

		distance = side.Distance(position, &collision_direction);
		if (distance < radius || side.IsIntersection(&treck))
		{
			d_pos = (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_coeffisient);
			force -= force.ProjectSign(d_pos);
			position += d_pos;
			collision_direction -= position;
			velocity -= collision_direction.ProjectSign(velocity);
			velocity += (-collision_direction).ProjectSign(polygon->Velocity(collision_direction + position) - velocity);

			collision = true;
		}
	}
	return collision;
}

bool DynamicEntity::Collision(const Map::MapData* map)
{
	bool collision = false;
	void* map_element;
	for (EngineTypes::Map::array_length_t i = 0; i < map->cyrcles_array_length; i++)
	{
		map_element = (void*)map->CyrclePointer(i);
		if (((Map::Cyrcle*)map_element)->exist)
		{
			collision |= Collision((Map::Cyrcle*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->polygons_array_length; i++)
	{
		map_element = (void*)map->PolygonPointer(i);
		if (((Map::Polygon*)map_element)->exist)
		{
			collision |= Collision((Map::Polygon*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->rectangles_array_length; i++)
	{
		map_element = (void*)map->RectanglePointer(i);
		if (((Map::Rectangle*)map_element)->exist)
		{
			collision |= Collision((Map::Rectangle*)map_element);
		}
	}
	return collision;
}

float DynamicEntity::GetAngularVelocity() const
{
	return angular_velocity;
}

Segment DynamicEntity::GetLastTreck() const
{
	return Segment(position, -velocity);
}

Segment DynamicEntity::GetTreck() const
{
	return Segment(&position, &velocity);
}

Segment DynamicEntity::GetTreckDouble() const
{
	return Segment(position, velocity * 2.0f);
}

Segment DynamicEntity::GetTreckDoubleRelative(const DynamicEntity* entity) const
{
	return Segment(position, (velocity - entity->velocity) * 2.0f);
}

Segment DynamicEntity::GetTreckRelative(const DynamicEntity* entity) const
{
	return Segment(position, velocity - entity->velocity);
}

Segment DynamicEntity::GetTreckRelativeDouble(const DynamicEntity* entity) const
{
	return Segment(position, velocity - entity->velocity * 2.0f);
}

Vec2F DynamicEntity::GetVelocity() const
{
	return velocity;
}

bool DynamicEntity::IsCollision(Vec2F point) const
{
	return Segment(position, -velocity).Distance(point) < radius;
}

bool DynamicEntity::IsCollision(const Vec2F* point) const
{
	return Segment(position, -velocity).Distance(point) < radius;
}

bool DynamicEntity::IsCollision(Line line) const
{
	return Segment(position, -velocity).Distance(&line) < radius;
}

bool DynamicEntity::IsCollision(const Line* line) const
{
	return Segment(position, -velocity).Distance(line) < radius;
}

bool DynamicEntity::IsCollision(Beam beam) const
{
	return Segment(position, -velocity).Distance(&beam) < radius;
}

bool DynamicEntity::IsCollision(const Beam* beam) const
{
	return Segment(position, -velocity).Distance(beam) < radius;
}

bool DynamicEntity::IsCollision(Segment segment) const
{
	return segment.Distance(position) < radius || GetTreck().IsIntersection(&segment);
}

bool DynamicEntity::IsCollision(const Segment* segment) const
{
	return segment->Distance(position) < radius || GetTreck().IsIntersection(segment);
}

bool DynamicEntity::IsCollision(DynamicEntity* entity) const
{
	return Segment(position, -velocity).Distance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(const StaticEntity* entity) const
{
	return Segment(position, -velocity).Distance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(const Laser* laser) const
{
	Segment last = GetTreck();
	return laser->GetSegment().Distance(&last) < radius + laser->width;
}

bool DynamicEntity::IsCollision(const MegaLaser* mega_laser) const
{
	Segment last = GetLastTreck();
	Line perpendicular = Line(position, mega_laser->GetDirection().Perpendicular());
	Segment segment = mega_laser->GetSegment();
	Segment start = Segment(
		segment.point + mega_laser->GetDirection().PerpendicularClockwise() * mega_laser->width,
		mega_laser->GetDirection().Perpendicular() * mega_laser->width * 2.0f);
	Vec2F intersect;
	return
		last.IsIntersection(&segment) ||
		segment.Intersection(&perpendicular, &intersect) && intersect.Distance(position) < radius + mega_laser->width ||
		start.Distance(&last) < radius ||
		Segment(start.point + segment.vector, start.vector).Distance(&last) < radius;
}

bool DynamicEntity::IsCollision(const Map::Rectangle* rectangle) const
{
	return
		IsCollision(rectangle->UpSide()) ||
		IsCollision(rectangle->DownSide()) ||
		IsCollision(rectangle->RightSide()) ||
		IsCollision(rectangle->LeftSide());
}

bool DynamicEntity::IsCollision(const Map::Cyrcle* cyrcle) const
{
	const Segment trace(position, -velocity);
	const Vec2F& cyrcle_position = cyrcle->GetPosition();
	const float& cyrcle_radius = cyrcle->GetRadius();

	return !(trace.Distance(cyrcle_position) > cyrcle_radius ||
		trace.point.Distance(cyrcle_position) < cyrcle_radius - radius &&
		trace.SecondPoint().Distance(cyrcle_position) < cyrcle_radius - radius);
}

bool DynamicEntity::IsCollision(const Map::Polygon* polygon) const
{
	if (!polygon->exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon->PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment treck = GetTreck();

	if (p_count > 2 && polygon->IsClosed())
	{
		if (IsCollision(Segment(polygon->points_array[p_count - 1], polygon->points_array[0], true)))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		if (IsCollision(Segment(polygon->points_array[p - 1], polygon->points_array[p], true)))
		{
			return true;
		}
	}
	return false;
}

bool DynamicEntity::IsCollision(const Portal* portal) const
{
	return Segment(position, -velocity).Distance(portal->GetPosition()) < portal->radius + radius;
}

bool DynamicEntity::IsCollision(const Map::MapData* map) const
{
	void* element;
	if (map->cyrcles_array_length > 0)
	{
		for (EngineTypes::Map::array_length_t cyrcle = 0, found_cyrcle = 0; found_cyrcle < map->cyrcles_array_length; cyrcle++)
		{
			if (((Map::Cyrcle*)(element = (void*)map->CyrclePointer(cyrcle)))->exist)
			{
				if (IsCollision((Map::Cyrcle*)element))
				{
					return true;
				}
				found_cyrcle++;
			}
		}
	}
	if (map->polygons_array_length > 0)
	{
		for (EngineTypes::Map::array_length_t polygon = 0, found_polygon = 0; found_polygon < map->polygons_array_length; polygon++)
		{
			if (((Map::Polygon*)(element = (void*)map->PolygonPointer(polygon)))->exist)
			{
				if (IsCollision((Map::Polygon*)element))
				{
					return true;
				}
				found_polygon++;
			}
		}
	}
	if (map->rectangles_array_length > 0)
	{
		for (EngineTypes::Map::array_length_t rectangle = 0, found_rectangle = 0; found_rectangle < map->rectangles_array_length; rectangle++)
		{
			if (((Map::Rectangle*)(element = (void*)map->RectanglePointer(rectangle)))->exist)
			{
				if (IsCollision((Map::Rectangle*)element))
				{
					return true;
				}
				found_rectangle++;
			}
		}
	}
	return false;
}

//The function updates position and velocity of entity and clears forces' data.
void DynamicEntity::Update()
{
	angle += angular_velocity;
	if (angle > (float)M_PI)
	{
		angle -= (float)M_PI * 2.0f;
	}
	else if (angle < -(float)M_PI)
	{
		angle += (float)M_PI * 2.0f;
	}
	UpdateDirection();

	velocity *= 1.0f - force_resistance_air_coefficient;
	velocity += force;
	Move(&velocity);
	force.Set(0.0f, 0.0f);
}

void DynamicEntity::Set(const DynamicEntity* dynamic_entity)
{
	angle = dynamic_entity->angle;
	angular_velocity = dynamic_entity->angular_velocity;
	direction = dynamic_entity->direction;
	exist = dynamic_entity->exist;
	force_collision_coeffisient = dynamic_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = dynamic_entity->force_resistance_air_coefficient;
	position = dynamic_entity->position;
	radius = dynamic_entity->radius;
	velocity = dynamic_entity->velocity;
}

void DynamicEntity::Set(
	Vec2F position,
	Vec2F velocity,
	float radius, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->force_resistance_air_coefficient = angular_velocity;
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->position = position;
	this->radius = radius;
	this->velocity = velocity;
}

void DynamicEntity::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	float radius,
	float angle, 
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->force_resistance_air_coefficient = angular_velocity;
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void DynamicEntity::SetAngularVelocity(float angular_velocity)
{
	this->angular_velocity = angular_velocity;
}

void DynamicEntity::SetCoefficients(float force_collision_coeffisient, float force_resistance_air_coefficient)
{
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
}

void DynamicEntity::SetVelocity(Vec2F velocity)
{
	this->velocity = velocity;
}

void DynamicEntity::SetVelocity(const Vec2F* velocity)
{
	this->velocity = *velocity;
}

void DynamicEntity::operator=(DynamicEntity dynamic_entity)
{
	angle = dynamic_entity.angle;
	angular_velocity = dynamic_entity.angular_velocity;
	direction = dynamic_entity.direction;
	exist = dynamic_entity.exist;
	force = dynamic_entity.force;
	force_collision_coeffisient = dynamic_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = dynamic_entity.force_resistance_air_coefficient;
	position = dynamic_entity.position;
	radius = dynamic_entity.radius;
	velocity = dynamic_entity.velocity;
}

DynamicEntity::~DynamicEntity()
{
}