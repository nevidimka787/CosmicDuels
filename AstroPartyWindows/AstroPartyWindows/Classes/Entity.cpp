#include "Entity.h"
#include <math.h>

#define M_PI 3.14159265358979323846

Entity::Entity() :
	angle(0.0),
	direction(Vec2F(1.0, 0.0)),
	exist(false),
	radius(0.0)
{
}

Entity::Entity(Vec2F position, float radius, float angle, bool exist) :
	angle(angle),
	direction(Vec2F(cosf(angle), -sinf(angle))),
	exist(exist),
	position(position),
	radius(radius)
{
}

Entity::Entity(const Vec2F* position, float radius, float angle, bool exist) :
	angle(angle),
	direction(Vec2F(cosf(angle), -sinf(angle))),
	exist(exist),
	position(*position),
	radius(radius)
{
}

Entity::Entity(const Entity& entity) :
	angle(entity.angle),
	direction(Vec2F(cosf(entity.angle), -sinf(entity.angle))),
	exist(entity.exist),
	position(entity.position),
	radius(entity.radius)
{

}

float Entity::GetAngle() const
{
	return angle;
}

float Entity::GetDistance(const Beam* beam) const
{
	return beam->Distance(&position) - radius, 0.0f;
}

float Entity::GetDistance(const Cyrcle* cyrcle, bool* is_inside) const
{
	Vec2F temp = cyrcle->GetPosition();
	float dist = GetDistance(&temp) - cyrcle->GetRadius();
	if (dist < -2.0f * radius)
	{
		if (is_inside != nullptr)
		{
			*is_inside = true;
		}
		dist = cyrcle->GetRadius() - temp.Distance(&position) - 3.0f * radius;
	}
	else if (is_inside != nullptr)
	{
		*is_inside = false;
	}
	return dist;
}

float Entity::GetDistance(const DecelerationArea* deceler_area) const
{
	return deceler_area->GetDistance(position) - radius;
}

float Entity::GetDistance(const DynamicEntity* entity) const
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(const Entity* entity) const
{
	return entity->position.Distance(&position) - entity->radius - radius;
}

float Entity::GetDistance(const Line* line) const
{
	return line->Distance(&position) - radius;
}

float Entity::GetDistance(const Segment* segment) const
{
	return segment->Distance(&position) - radius;
}

float Entity::GetDistance(const StaticEntity* entity) const
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(const Polygon* polygon) const
{
	return 0.0;
}

float Entity::GetDistance(const Rectangle* rectangle) const
{
	Segment temp = rectangle->UpSide();
	float dist1 = GetDistance(&temp);
	if (dist1 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->DownSide();
	float dist2 = GetDistance(&temp);
	if (dist2 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->LeftSide();
	float dist3 = GetDistance(&temp);
	if (dist3 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->RightSide();
	float dist4 = GetDistance(&temp);
	if (dist4 == 0.0)
	{
		return 0.0;
	}
	if (dist1 < dist2)
	{
		if (dist3 < dist4)
		{
			if (dist1 < dist3)
			{
				return dist1;
			}
			return dist3;
		}
		if (dist1 < dist4)
		{
			return dist1;
		}
		return dist4;
	}
	if (dist3 < dist4)
	{
		if (dist2 < dist3)
		{
			return dist2;
		}
		return dist3;
	}
	if (dist2 < dist4)
	{
		return dist2;
	}
	return dist4;
}

float Entity::GetDistance(Vec2F point) const
{
	return point.Distance(&position) - radius;
}

float Entity::GetDistance(const Vec2F* point) const
{
	return point->Distance(&position) - radius;
}

Vec2F Entity::GetDirectionNotNormalize() const
{
	return direction;
}

float Entity::GetFrameSize(Entity* entity, float scale) const
{
	return fmaxf(fabs((entity->position - position).x), fabs((entity->position - position).y * scale)) + radius + entity->radius;
}

Vec2F Entity::GetDirection() const
{
	return direction.Normalize();
}

Vec2F Entity::GetPosition() const
{
	return position;
}

const Vec2F* Entity::GetPositionPointer() const
{
	return &position;
}

bool Entity::IsCollision(Beam* beam) const
{
	return beam->Distance(position) <= radius;
}

bool Entity::IsCollision(Cyrcle* cyrcle) const
{
	if (cyrcle->exist == false)
	{
		return false;
	}
	Vec2F temp = cyrcle->GetPosition();
	return GetDistance(&temp) < radius + cyrcle->GetRadius();
}

bool Entity::IsCollision(DecelerationArea* deceler_area) const
{
	return GetDistance(deceler_area) <= 0.0f;
}

bool Entity::IsCollision(DynamicEntity* dynamic_entity) const
{
	return GetDistance(&dynamic_entity->position) <= dynamic_entity->radius;
}

bool Entity::IsCollision(Entity* entity) const
{
	return GetDistance(&entity->position) <= entity->radius;
}

bool Entity::IsCollision(Line* line) const
{
	return line->Distance(&position) <= radius;
}

bool Entity::IsCollision(Laser* laser) const
{
	return laser->GetBeam().Distance(position) < radius + laser->width;
}

bool Entity::IsCollision(MegaLaser* mega_laser) const
{
	Line perpendicular = Line(position, mega_laser->GetDirection().Perpendicular());
	Segment segment = mega_laser->GetSegment();
	Segment start = Segment(
		segment.point + mega_laser->GetDirection().PerpendicularClockwise() * mega_laser->width,
		mega_laser->GetDirection().Perpendicular() * mega_laser->width * 2.0f);
	Vec2F intersect;
	return 
		segment.Intersection(&perpendicular, &intersect) && intersect.Distance(position) < radius + mega_laser->width ||
		start.Distance(position) < radius || 
		Segment(start.point + segment.vector, start.vector).Distance(position) < radius;
}

bool Entity::IsCollision(Map* map) const
{
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		if (IsCollision(map->RectanglePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		if (IsCollision(map->CyrclePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		if (IsCollision(map->PolygonPointer(i)))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Polygon* polygon) const
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
	Segment side = Segment(polygon->points_array[p_count - 1], polygon->points_array[0], true);
	if (p_count > 2 && polygon->IsClosed())
	{
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		side.Set(polygon->points_array[p - 1], polygon->points_array[p], true);
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Rectangle* rectangle) const
{
	if (rectangle->exist == false)
	{
		return false;
	}
	Segment side;
	side = rectangle->UpSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->DownSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->RightSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->LeftSide();
	if (IsCollision(&side))
	{
		return true;
	}
	return false;
}

bool Entity::IsCollision(Segment* segment) const
{
	return segment->Distance(&position) <= radius;
}

bool Entity::IsCollision(StaticEntity* static_entity) const
{
	return GetDistance(static_entity->GetPosition()) <= static_entity->radius;
}

bool Entity::IsCollision(Vec2F point) const
{
	return GetDistance(point) <= 0.0f;
}

bool Entity::IsCollision(Vec2F* point) const
{
	return GetDistance(point) <= 0.0f;
}

void Entity::Rotate(float angle)
{
	this->angle += angle;
	UpdateDirection();
}

void Entity::Set(Entity* entity)
{
	angle = entity->angle;
	direction = entity->direction;
	exist = entity->exist;
	position = entity->position;
	radius = entity->radius;
}

void Entity::Set(Vec2F* position, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
}

void Entity::SetAngle(float angle)
{
	this->angle = angle;
	UpdateDirection();
}

void Entity::SetDirection(Vec2F direction)
{
	this->direction = direction.Normalize();
}

void Entity::SetDirection(Vec2F* direction)
{
	this->direction = direction->Normalize();
}

void Entity::SetDirectionNotNormalize(Vec2F direction)
{
	if (direction.x != 0 || direction.y != 0)
	{
		this->direction = direction;
	}
}

void Entity::SetDirectionNotNormalize(Vec2F* direction)
{
	if (direction->x != 0 || direction->y != 0)
	{
		this->direction = *direction;
	}
}

void Entity::SetPosition(Vec2F position)
{
	this->position = position;
}

void Entity::SetPosition(const Vec2F* position)
{
	this->position.Set(position);
}

void Entity::Move(Vec2F delta)
{
	position += delta;
}

void Entity::Move(Vec2F* delta)
{
	position += *delta;
}

void Entity::UpdateAngle()
{
	angle = -atan2f(direction.y, direction.x);
}

void Entity::UpdateDirection()
{
	direction = Vec2F(1.0f, 0.0f).Rotate(angle);
}

void Entity::operator=(Entity entity)
{
	angle = entity.angle;
	direction = entity.direction;
	exist = entity.exist;
	position = entity.position;
	radius = entity.radius;
}

Entity::~Entity()
{
}



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

DynamicEntity::DynamicEntity(Vec2F position, Vec2F velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Entity(position, radius, angle, exist),
	angular_velocity(angular_velocity),
	force_collision_coeffisient(force_collision_coeffisient),
	force_resistance_air_coefficient(force_resistance_air_coefficient),
	velocity(velocity)
{
}

DynamicEntity::DynamicEntity(const Vec2F* position, const Vec2F* velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Entity(position, radius, angle, exist),
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

void DynamicEntity::AddForce(Vec2F* force)
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

void DynamicEntity::AddGravityForce(float gravity_coeffisient, Vec2F* forced_point)
{
	Vec2F grav_vec = (*forced_point - position).Normalize();
	float* x_temp_coeff = &grav_vec.x;
	float* y = &grav_vec.y;
	*x_temp_coeff = gravity_coeffisient / (*x_temp_coeff * *x_temp_coeff + *y * *y);
	force += grav_vec * *x_temp_coeff;
}

void DynamicEntity::AddGravityForce(float gravity_coeffisient, Vec2F forced_point)
{
	float distance = (forced_point - position).Length();
	Vec2F grav_vec = (forced_point - position).Normalize();
	grav_vec *= gravity_coeffisient / distance / distance;
	force += grav_vec;// * *x_temp_coeff;
}

void DynamicEntity::AddVelocity(Vec2F velocity)
{
	this->velocity += velocity;
}

void DynamicEntity::AddVelocity(Vec2F* velocity)
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
		local_velocity = collision_direction.Project(&local_velocity);
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
		local_velocity = collision_direction.Project(&local_velocity);
		velocity -= local_velocity;
		position -= collision_direction.Normalize() * distance;
	}


	return true;
}

bool DynamicEntity::Collision(const Rectangle* rectangle)
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
			position.y += radius - distance + force_collision_delta;
		}
		else
		{
			position.y -= radius - distance + force_collision_delta;
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
			position.y += radius - distance + force_collision_delta;
		}
		else
		{
			position.y -= radius - distance + force_collision_delta;
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
			position.x += radius - distance + force_collision_delta;
		}
		else
		{
			position.x -= radius - distance + force_collision_delta;
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
			position.x += radius - distance + force_collision_delta;
		}
		else
		{
			position.x -= radius - distance + force_collision_delta;
		}
		velocity -= collision_direction.ProjectSign(&velocity);
		velocity += Vec2F(-1.0f, 0.0f).ProjectSign(rectangle_velocity - velocity);

		collision = true;
	}
	return collision;
}

bool DynamicEntity::Collision(const Cyrcle* cyrcle)
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
		position = cyrcle->GetPosition() - collision_direction.Normalize() * (cyrcle->GetRadius() - radius - force_collision_delta);
	}
	else
	{
		position = cyrcle->GetPosition() - collision_direction.Normalize() * (cyrcle->GetRadius() + radius + force_collision_delta);
	}

	velocity -= collision_direction.Project(&velocity) / 2.0f;
	velocity += collision_direction.ProjectSign(cyrcle->GetVelocity() - velocity);
	return true;
}

bool DynamicEntity::Collision(const Polygon* polygon)
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

	if (p_count > 2 && polygon->IsClosed())
	{
		distance = side.Distance(position, &collision_direction);
		if (distance < radius || side.IsIntersection(&treck))
		{
			position += (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_delta);
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
			position += (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_delta);
			collision_direction -= position;
			velocity -= collision_direction.ProjectSign(velocity);
			velocity += (-collision_direction).ProjectSign(polygon->Velocity(collision_direction + position) - velocity);

			collision = true;
		}
	}
	return collision;
}

bool DynamicEntity::Collision(const Map* map)
{
	bool collision = false;
	void* map_element;
	for (EngineTypes::Map::array_length_t i = 0; i < map->cyrcles_array_length; i++)
	{
		map_element = (void*)map->CyrclePointer(i);
		if (((Cyrcle*)map_element)->exist)
		{
			collision |= Collision((Cyrcle*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->polygons_array_length; i++)
	{
		map_element = (void*)map->PolygonPointer(i);
		if (((Polygon*)map_element)->exist)
		{
			collision |= Collision((Polygon*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->rectangles_array_length; i++)
	{
		map_element = (void*)map->RectanglePointer(i);
		if (((Rectangle*)map_element)->exist)
		{
			collision |= Collision((Rectangle*)map_element);
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
	return laser->GetBeam().Distance(&last) < radius + laser->width;
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

bool DynamicEntity::IsCollision(const Rectangle* rectangle) const
{
	return 
		IsCollision(rectangle->UpSide()) ||
		IsCollision(rectangle->DownSide()) ||
		IsCollision(rectangle->RightSide()) ||
		IsCollision(rectangle->LeftSide());
}

bool DynamicEntity::IsCollision(const Cyrcle* cyrcle) const
{
	return Segment(position, -velocity).Distance(cyrcle->GetPosition()) < radius + cyrcle->GetRadius();
}

bool DynamicEntity::IsCollision(const Polygon* polygon) const
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

bool DynamicEntity::IsCollision(const Map* map) const
{
	void* element;
	if (map->cyrcles_array_length > 0)
	{
		for (EngineTypes::Map::array_length_t cyrcle = 0, found_cyrcle = 0; found_cyrcle < map->cyrcles_array_length; cyrcle++)
		{
			if (((Cyrcle*)(element = (void*)map->CyrclePointer(cyrcle)))->exist)
			{
				if (IsCollision((Cyrcle*)element))
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
			if (((Polygon*)(element = (void*)map->PolygonPointer(polygon)))->exist)
			{
				if (IsCollision((Polygon*)element))
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
			if (((Rectangle*)(element = (void*)map->RectanglePointer(rectangle)))->exist)
			{
				if (IsCollision((Rectangle*)element))
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

	velocity += force;
	velocity *= 1.0f - force_resistance_air_coefficient;
	force.Set(0.0f, 0.0f);
	Move(&velocity);
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

void DynamicEntity::Set(Vec2F position, Vec2F velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
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

void DynamicEntity::Set(const Vec2F* position, const Vec2F* velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
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



StaticEntity::StaticEntity() :
	Entity()
{
}

StaticEntity::StaticEntity(const StaticEntity& static_entity) :
	Entity(static_entity),
	last_position(static_entity.last_position)
{
}

StaticEntity::StaticEntity(Vec2F position, float radius, float angle, bool exist) :
	Entity(position, radius, angle, exist),
	last_position(position)
{
}

StaticEntity::StaticEntity(const Vec2F* position, float radius, float angle, bool exist) :
	Entity(position, radius, angle, exist),
	last_position(*position)
{
}

Vec2F StaticEntity::GetVelocity() const
{
	return position - last_position;
}

void StaticEntity::Set(StaticEntity* static_entity)
{
	angle = static_entity->angle;
	direction = static_entity->direction;
	exist = static_entity->exist;
	last_position = static_entity->last_position;
	position = static_entity->position;
	radius = static_entity->radius;
}

void StaticEntity::Set(Vec2F position, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = position;
	this->radius = radius;
}

void StaticEntity::Set(const Vec2F* position, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
}

void StaticEntity::SetPosition(Vec2F position)
{
	this->position = position;
	last_position = position;
}

void StaticEntity::SetPosition(const Vec2F* position)
{
	this->position = *position;
	last_position = *position;
}

void StaticEntity::Update()
{
	last_position = position;
}

void StaticEntity::operator=(StaticEntity static_entity)
{
	angle = static_entity.angle;
	direction = static_entity.direction;
	exist = static_entity.exist;
	last_position = static_entity.last_position;
	position = static_entity.position;
	radius = static_entity.radius;
}

StaticEntity::~StaticEntity()
{
}


Particle::Particle() :
	Entity(),
	type(0),
	animation_period(PARTICLE_PROPERTY_AUTO),
	animation_postpone(PARTICLE_PROPERTY_AUTO),
	finish_tic(PARTICLE_PROPERTY_AUTO),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(0)
{
}

Particle::Particle(GameTypes::tic_t current_tic, const Entity* pointer_to_host, EngineTypes::Particle::type_t type, Color3F color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	Entity(*pointer_to_host),
	color(color),
	finish_tic(finish_tic),
	linked(true),
	pointer_to_entity(pointer_to_host),
	spawn_tic(current_tic),
	type(type)
{
	if (animation_period == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPeriod(type);
	}
	else
	{
		this->animation_period = animation_period;
	}

	if (animation_postpone == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPostpone(type);
	}
	else
	{
		this->animation_postpone = animation_postpone;
	}
}

Particle::Particle(GameTypes::tic_t current_tic, const Entity* pointer_to_host, EngineTypes::Particle::type_t type, const Color3F* color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	Entity(*pointer_to_host),
	color(*color),
	finish_tic(finish_tic),
	linked(true),
	pointer_to_entity(pointer_to_host),
	spawn_tic(current_tic),
	type(type)
{
	if (animation_period == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPeriod(type);
	}
	else
	{
		this->animation_period = animation_period;
	}

	if (animation_postpone == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPostpone(type);
	}
	else
	{
		this->animation_postpone = animation_postpone;
	}
}

Particle::Particle(GameTypes::tic_t current_tic, Vec2F position, float angle, float radius, EngineTypes::Particle::type_t type, Color3F color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	Entity(position, radius, angle, exist),
	type(type),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(color),
	finish_tic(finish_tic),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(current_tic)
{
}

Particle::Particle(GameTypes::tic_t current_tic, const Vec2F* position, float angle, float radius, EngineTypes::Particle::type_t type, const Color3F* color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	Entity(*position, radius, angle, exist),
	type(type),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(*color),
	finish_tic(finish_tic),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(current_tic)
{
}

void Particle::Activate(GameTypes::tic_t current_tic)
{
	if (!active && current_tic == spawn_tic + animation_postpone)
	{
		active = true;
	}
}

bool Particle::CanRemove(GameTypes::tic_t current_tic) const
{
	if (linked)
	{
		return !pointer_to_entity->exist || finish_tic != PARTICLE_ANIMATION_NOT_FINISH && current_tic == finish_tic;
	}
	return finish_tic != PARTICLE_ANIMATION_NOT_FINISH && current_tic == finish_tic;
}

EngineTypes::Particle::type_t Particle::GetType() const
{
	return type;
}

bool Particle::IsActive() const
{
	return active;
}

void Particle::Link(Entity* new_pointer)
{
	pointer_to_entity = new_pointer;
	linked = true;
}

void Particle::SetAutoPeriod(EngineTypes::Particle::type_t type)
{
	switch (type)
	{
	case PARTICLE_TYPE_BACKGROUND:
		this->animation_period = PARTICLE_PERIOD_BACKGROUND;
		break;
	case PARTICLE_TYPE_EXAUST_BOMB_BOOM:
		this->animation_period = PARTICLE_PERIOD_EXAUST_BOMB_BOOM;
		break;
	case PARTICLE_TYPE_EXAUST_BURNOUT:
		this->animation_period = PARTICLE_PERIOD_EXAUST_BURNOUT;
		break;
	case PARTICLE_TYPE_EXAUST_ENGINE:
		this->animation_period = PARTICLE_PERIOD_EXAUST_ENGINE;
		break;
	case PARTICLE_TYPE_EXAUST_SHOOT:
		this->animation_period = PARTICLE_PERIOD_EXAUST_SHOOT;
		break;
	case PARTICLE_TYPE_PORTAL_IN:
		this->animation_period = PARTICLE_PERIOD_PORTAL_IN;
		break;
	case PARTICLE_TYPE_PORTAL_OUT:
		this->animation_period = PARTICLE_PERIOD_PORTAL_OUT;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID:
		this->animation_period = PARTICLE_PERIOD_SHARDS_ASTEROID;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID_POWERED:
		this->animation_period = PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED;
		break;
	case PARTICLE_TYPE_SHARDS_MAP_ELEMENT:
		this->animation_period = PARTICLE_PERIOD_SHARDS_MAP_ELEMENT;
		break;
	case PARTICLE_TYPE_SHARDS_PILOT:
		this->animation_period = PARTICLE_PERIOD_SHARDS_PILOT;
		break;
	case PARTICLE_TYPE_SHARDS_SHIP:
		this->animation_period = PARTICLE_PERIOD_SHARDS_SHIP;
		break;
	default:
		std::cout << "ERROR::PARTICLE::SET_AUTO_PERIOD::Animation period seted failed. Type of the patricle undeclarates." << std::endl;
		exist = false;
		return;
	}
}

void Particle::SetAutoPostpone(EngineTypes::Particle::type_t type)
{
	switch (type)
	{
	case PARTICLE_TYPE_BACKGROUND:
		this->animation_postpone = PARTICLE_POSTPONE_BACKGROUND;
		break;
	case PARTICLE_TYPE_EXAUST_BOMB_BOOM:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_BOMB_BOOM;
		break;
	case PARTICLE_TYPE_EXAUST_BURNOUT:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_BURNOUT;
		break;
	case PARTICLE_TYPE_EXAUST_ENGINE:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_ENGINE;
		break;
	case PARTICLE_TYPE_EXAUST_SHOOT:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_SHOOT;
		break;
	case PARTICLE_TYPE_PORTAL_IN:
		this->animation_postpone = PARTICLE_POSTPONE_PORTAL_IN;
		break;
	case PARTICLE_TYPE_PORTAL_OUT:
		this->animation_postpone = PARTICLE_POSTPONE_PORTAL_OUT;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_ASTEROID;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID_POWERED:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_ASTEROID_POWERED;
		break;
	case PARTICLE_TYPE_SHARDS_MAP_ELEMENT:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_MAP_ELEMENT;
		break;
	case PARTICLE_TYPE_SHARDS_PILOT:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_PILOT;
		break;
	case PARTICLE_TYPE_SHARDS_SHIP:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_SHIP;
		break;
	default:
		std::cout << "ERROR::PARTICLE::SET_AUTO_POSTPONE::Animation postond seted failed. Type of the patricle undeclarates." << std::endl;
		exist = false;
		return;
	}
}

void Particle::Unlink()
{
	linked = false;
	pointer_to_entity = nullptr;
}

void Particle::Update(GameTypes::tic_t current_tic)
{
	if (pointer_to_entity != nullptr && pointer_to_entity->exist)
	{
		position = pointer_to_entity->GetPosition();
		angle = pointer_to_entity->GetAngle();
		radius = pointer_to_entity->radius;
	}
	
	if (active)
	{
		animation = (float)((current_tic - spawn_tic - animation_postpone) % animation_period) / (float)animation_period;
	}
}

void Particle::operator=(Particle particle)
{
	active = particle.active;
	angle = particle.angle;
	animation = particle.animation;
	animation_period = particle.animation_period;
	animation_postpone = particle.animation_postpone;
	color = particle.color;
	exist = particle.exist;
	finish_tic = particle.finish_tic;
	linked = particle.linked;
	pointer_to_entity = particle.pointer_to_entity;
	position = particle.position;
	radius = particle.radius;
	spawn_tic = particle.spawn_tic;
	type = particle.type;
}

Particle::~Particle()
{
}



DynamicParticle::DynamicParticle() :
	DynamicEntity(),
	active(false),
	animation(0.0f),
	animation_period(0),
	animation_postpone(0),
	finish_tic(0),
	properties(DYNAMIC_PARTICLE_PROPERTY_NULL),
	spawn_tic(0),
	type(0)
{
}

DynamicParticle::DynamicParticle(GameTypes::tic_t current_tic, Vec2F position, Vec2F velocisy, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, EngineTypes::Particle::type_t type, EngineTypes::DynamicParticle::property_t properties, Color3F color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	DynamicEntity(position, velocisy, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	active(false),
	animation(0.0f),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(color),
	finish_tic(finish_tic),
	properties(properties),
	spawn_tic(current_tic),
	type(type)
{
}

DynamicParticle::DynamicParticle(GameTypes::tic_t current_tic, const Vec2F* position, const Vec2F* velocisy, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, EngineTypes::Particle::type_t type, EngineTypes::DynamicParticle::property_t properties, const Color3F* color, GameTypes::tic_t animation_period, GameTypes::tic_t animation_postpone, GameTypes::tic_t finish_tic, bool exist) :
	DynamicEntity(position, velocisy, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	active(false),
	animation(0.0f),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(*color),
	finish_tic(finish_tic),
	properties(properties),
	spawn_tic(current_tic),
	type(type)
{
}

#pragma warning (disable : 6282)
bool DynamicParticle::Activate(GameTypes::tic_t current_tic)
{
	return (!active && current_tic == spawn_tic + animation_postpone) ? (active = true) : false;
}

bool DynamicParticle::CanRemove(GameTypes::tic_t current_tic) const
{
	return current_tic == finish_tic;
}

EngineTypes::Particle::type_t DynamicParticle::GetType() const
{
	return type;
}

bool DynamicParticle::IsActive() const
{
	return active;
}

void DynamicParticle::Update(GameTypes::tic_t current_tic)
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

	velocity += force;
	if (properties & DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE)
	{
		velocity *= 1.0f - force_resistance_air_coefficient;
	}
	force.Set(0.0f, 0.0f);
	Move(&velocity);
	if (active)
	{
		animation = (float)((current_tic - spawn_tic - animation_postpone) % animation_period) / (float)animation_period;
	}
}

void DynamicParticle::operator=(DynamicParticle dynamical_entity)
{
	active = dynamical_entity.active;
	angle = dynamical_entity.angle;
	angular_velocity = dynamical_entity.angular_velocity;
	animation = dynamical_entity.animation;
	animation_period = dynamical_entity.animation_period;
	animation_postpone = dynamical_entity.animation_postpone;
	color = dynamical_entity.color;
	UpdateDirection();
	exist = dynamical_entity.exist;
	finish_tic = dynamical_entity.finish_tic;
	force = dynamical_entity.force;
	force_collision_coeffisient = dynamical_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = dynamical_entity.force_resistance_air_coefficient;
	position = dynamical_entity.position;
	properties = dynamical_entity.properties;
	radius = dynamical_entity.radius;
	spawn_tic = dynamical_entity.spawn_tic;
	type = dynamical_entity.type;
	velocity = dynamical_entity.velocity;
}

DynamicParticle::~DynamicParticle()
{
}



Bonus::Bonus() :
	DynamicEntity(),
	bonus_inventory(0)
{
	radius = BONUS_DEFAULT_RADIUS;
}

Bonus::Bonus(bool exist) :
	Bonus()
{
	this->exist = exist;
}

Bonus::Bonus(const Bonus& bonus) :
	DynamicEntity(bonus),
	bonus_inventory(bonus.bonus_inventory)
{
}

Bonus::Bonus(Vec2F position, Vec2F velocity, EngineTypes::Bonus::inventory_t bonus_inventory, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	bonus_inventory(bonus_inventory)
{
}

Bonus::Bonus(const Vec2F* position, const Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	bonus_inventory(bonus_inventory)
{
}

EngineTypes::Bonus::inventory_t Bonus::BonusInfo() const
{
	return bonus_inventory;
}

bool Bonus::CanDivision() const
{
	bool bonus_found = false;
	for (uint8_t cell = 0; cell < BONUS_CELLS_COUNT; cell++)
	{
		if ((bonus_inventory & (BONUS_CELL << (cell * 2))) > (1 << (cell * 2)))
		{
			return true;
		}
		else if (bonus_inventory & (BONUS_CELL << (cell * 2)))
		{
			if (bonus_found)
			{
				return true;
			}
			bonus_found = true;
		}
	}
	return false;
}

Bonus Bonus::Division()
{
	Bonus found_bonus;
	for (uint8_t cell = 0; cell < BONUS_CELLS_COUNT; cell++)
	{
		if ((bonus_inventory & (BONUS_CELL << (cell * 2))) > (1 << (cell * 2)))
		{
			bonus_inventory -= 1 << (cell * 2);
			return Bonus(&position, &velocity, 1 << (cell * 2), 0.0f, 0.0f);
		}
		else if(bonus_inventory & (BONUS_CELL << (cell * 2)))
		{
			if (found_bonus.exist)
			{
				bonus_inventory -= found_bonus.bonus_inventory;
				return found_bonus;
			}
			found_bonus = Bonus(&position, &velocity, 1 << (cell * 2), 0.0f, 0.0f);
		}
	}
	return Bonus(false);
}

uint16_t Bonus::GetType()
{
	return bonus_inventory;
}

uint8_t Bonus::GetBonusesCount() const
{
	uint8_t count = 0;
	EngineTypes::Bonus::inventory_t inventory_cell;
	for (uint8_t i = BONUS_BONUS_FIRST_CELL; i <= BONUS_BONUS_LAST_CELL; i++)
	{
		if (inventory_cell = (bonus_inventory & (BONUS_CELL << (i * 2))))
		{
			count += inventory_cell >> (i * 2);
		}
	}
	return count;
}

uint8_t Bonus::GetBuffsCount() const
{
	uint8_t count = 0;
	for (uint8_t i = BONUS_BUFF_FIRST_CELL; i <= BONUS_BUFF_LAST_CELL; i++)
	{
		if (bonus_inventory & (BONUS_CELL << (i << 1)))
		{
			count++;
		}
	}
	return count;
}

uint8_t Bonus::GetGameRulesCount() const
{
	uint8_t count = 0;
	for (uint8_t i = BONUS_RULE_FIRST_CELL; i <= BONUS_RULE_LAST_CELL; i++)
	{
		if (bonus_inventory & (BONUS_CELL << (i << 1)))
		{
			count++;
		}
	}
	return count;
}

void Bonus::Set(Bonus* bonus)
{
	angle = bonus->angle;
	angular_velocity = bonus->angular_velocity;
	bonus_inventory = bonus->bonus_inventory;
	direction = bonus->direction;
	exist = bonus->exist;
	force_collision_coeffisient = bonus->force_collision_coeffisient;
	force_resistance_air_coefficient = bonus->force_resistance_air_coefficient;
	position = bonus->position;
	radius = bonus->radius;
	velocity = bonus->velocity;
}

void Bonus::Set(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void Bonus::operator=(Bonus bonus)
{
	angle = bonus.angle;
	angular_velocity = bonus.angular_velocity;
	bonus_inventory = bonus.bonus_inventory;
	direction = bonus.direction;
	exist = bonus.exist;
	force_collision_coeffisient = bonus.force_collision_coeffisient;
	force_resistance_air_coefficient = bonus.force_resistance_air_coefficient;
	position = bonus.position;
	radius = bonus.radius;
	velocity = bonus.velocity;
}

Bonus::~Bonus()
{

}



Asteroid::Asteroid() :
	Bonus(),
	size(ASTEROID_DEFAULT_SIZE)
{
}

Asteroid::Asteroid(const Asteroid& asteroid) :
	Bonus(asteroid),
	size(asteroid.size)
{
}

Asteroid::Asteroid(Vec2F position, Vec2F velocity, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Asteroid::size_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Bonus(position, velocity, bonus_inventory, 0.0f, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	size(size)
{
	UpdateRadius();
	this->bonus_inventory = bonus_inventory;
	exist = true;
}

Asteroid::Asteroid(const Vec2F* position, const Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Asteroid::size_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Bonus(position, velocity, bonus_inventory, 0.0f, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	size(size)
{
	UpdateRadius();
	this->bonus_inventory = bonus_inventory;
	exist = true;
}

DynamicParticle Asteroid::CreateShards(GameTypes::tic_t current_tic) const
{
	if (bonus_inventory & BONUS_BUFF)
	{
		return DynamicParticle(
			current_tic,
			position,
			velocity,
			radius,
			angle,
			angular_velocity,
			force_collision_coeffisient,
			force_resistance_air_coefficient,
			PARTICLE_TYPE_SHARDS_ASTEROID_POWERED,
			DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
			Color3F(0.0f, 0.7f, 0.7f),
			PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED,
			PARTICLE_POSTPONE_SHARDS_ASTEROID_POWERED,
			current_tic + PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED);
	}
	if (bonus_inventory)
	{
		return DynamicParticle(
			current_tic,
			position,
			velocity,
			radius,
			angle,
			angular_velocity,
			force_collision_coeffisient,
			force_resistance_air_coefficient,
			PARTICLE_TYPE_SHARDS_ASTEROID_POWERED,
			DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
			Color3F(0.7f, 0.2f, 0.0f),
			PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED,
			PARTICLE_POSTPONE_SHARDS_ASTEROID_POWERED,
			current_tic + PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED);
	}
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_SHARDS_ASTEROID,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		Color3F(0.4f),
		PARTICLE_PERIOD_SHARDS_ASTEROID,
		PARTICLE_POSTPONE_SHARDS_ASTEROID,
		current_tic + PARTICLE_PERIOD_SHARDS_ASTEROID);
}

bool Asteroid::Collision(const Map* map)
{
	bool collision = false;
	void* element_p;
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		element_p = (void*)map->RectanglePointer(i);
		if (((Rectangle*)element_p)->exist && DynamicEntity::Collision(((Rectangle*)element_p)))
		{
			if (((Rectangle*)element_p)->IsAggressive())
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
			if (((Cyrcle*)element_p)->IsAggressive())
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
			if (((Rectangle*)element_p)->IsAggressive())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	return collision;
}

void Asteroid::DecrementSize()
{
	if (size > ASTEROID_SIZE_SMALL)
	{
		size--;
		UpdateRadius();
	}
}

Bonus Asteroid::Destroy()
{
	if (bonus_inventory)
	{
		return Bonus(&position, &velocity, bonus_inventory);
	}
	return Bonus();
}

Asteroid Asteroid::Division()
{
	if (size == ASTEROID_SIZE_SMALL)
	{
		return Asteroid();
	}

	EngineTypes::Bonus::inventory_t return_bonus = BONUS_NOTHING;
	uint8_t found_bonuses_count = 0;

	switch (size)
	{
	case ASTEROID_SIZE_MEDIUM:
		for (EngineTypes::Bonus::inventory_t cell = 0; cell < 8; cell++)
		{
			if (bonus_inventory & (BONUS_CELL << (cell * 2)))
			{
				bonus_inventory -= 1 << (cell * 2);
				return_bonus += 1 << (cell * 2);
				found_bonuses_count++;
				if (found_bonuses_count >= 1)
				{
					break;
				}
				cell--;
			}
		}
		break;
	case ASTEROID_SIZE_BIG:
		for (EngineTypes::Bonus::inventory_t cell = 0; cell < 8; cell++)
		{
			if (bonus_inventory & (BONUS_CELL << (cell * 2)))
			{
				bonus_inventory -= 1 << (cell * 2);
				return_bonus += 1 << (cell * 2);
				found_bonuses_count++;
				if (found_bonuses_count >= 2)
				{
					break;
				}
				cell--;
			}
		}
		break;
	default:
		return Asteroid();
	}

	Vec2F asteroid_velocity = direction.Rotate(((float)rand() / (float)RAND_MAX) * (float)M_PI * 2.0f) * ASTEROID_DEEFAULT_VELOCITY;
	
	return Asteroid(
		position + asteroid_velocity.Normalize() * radius,
		asteroid_velocity,
		return_bonus,
		size - 1);
}

EngineTypes::Asteroid::size_t Asteroid::GetSize() const
{
	return size;
}

EngineTypes::Bonus::inventory_t Asteroid::GetBuffBonus() const
{
	return bonus_inventory;
}

void Asteroid::Set(Asteroid* asteroid)
{
	angle = asteroid->angle;
	angular_velocity = asteroid->angular_velocity;
	bonus_inventory = asteroid->bonus_inventory;
	direction = asteroid->direction;
	exist = asteroid->exist;
	force = asteroid->force;
	force_collision_coeffisient = asteroid->force_collision_coeffisient;
	force_resistance_air_coefficient = asteroid->force_resistance_air_coefficient;
	position = asteroid->position;
	radius = asteroid->radius;
	size = asteroid->size;
	velocity = asteroid->velocity;
}

void Asteroid::Set(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Asteroid::size_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	UpdateDirection();
	this->exist = exist;
	this->force = force;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->position = *position;
	this->size = size;
	UpdateRadius();
	this->velocity = *velocity;
}

void Asteroid::UpdateRadius()
{
	switch (this->size)
	{
	case ASTEROID_SIZE_SMALL:
		radius = ASTEROID_RADIUS_SMALL;
		break;
	case ASTEROID_SIZE_MEDIUM:
		radius = ASTEROID_RADIUS_MEDIUM;
		break;
	case ASTEROID_SIZE_BIG:
		radius = ASTEROID_RADIUS_BIG;
		break;
	default:
		this->size = ASTEROID_DEFAULT_SIZE;
		radius = ASTEROID_DEFAULT_RADIUS;
		break;
	}
}

void Asteroid::operator=(Asteroid asteroid)
{
	angle = asteroid.angle;
	angular_velocity = asteroid.angular_velocity;
	bonus_inventory = asteroid.bonus_inventory;
	direction = asteroid.direction;
	exist = asteroid.exist;
	force = asteroid.force;
	force_collision_coeffisient = asteroid.force_collision_coeffisient;
	force_resistance_air_coefficient = asteroid.force_resistance_air_coefficient;
	position = asteroid.position;
	radius = asteroid.radius;
	size = asteroid.size;
	velocity = asteroid.velocity;
}

Asteroid::~Asteroid()
{

}



KillerEntity::KillerEntity() :
	DynamicEntity(),
	host_number(0),
	host_team_number(0)
{
}

KillerEntity::KillerEntity(const KillerEntity& killer_entity) :
	DynamicEntity(killer_entity),
	host_number(killer_entity.host_number),
	host_team_number(killer_entity.host_team_number)
{
}

KillerEntity::KillerEntity(Vec2F position, Vec2F velocity, float radius, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	host_number(player_master_number),
	host_team_number(player_master_team_number)
{
}

KillerEntity::KillerEntity(const Vec2F* position, const Vec2F* velocity, float radius, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	host_number(player_master_number),
	host_team_number(player_master_team_number)
{
}

bool KillerEntity::CreatedBy(ControledEntity* controled_entity)
{
	return host_number == controled_entity->GetPlayerNumber();
}

bool KillerEntity::CreatedByTeam(ControledEntity* controled_entity)
{
	return host_team_number == controled_entity->GetTeamNumber();
}

GameTypes::players_count_t KillerEntity::GetHostNumber()
{
	return host_number;
}

GameTypes::players_count_t KillerEntity::GetHostTeamNumber()
{
	return host_team_number;
}

bool KillerEntity::SameTeam(KillerEntity* killer_entity)
{
	return host_team_number == killer_entity->host_team_number;
}

void KillerEntity::Set(KillerEntity* killer_entity)
{
	angle = killer_entity->angle;
	angular_velocity = killer_entity->angular_velocity;
	direction = killer_entity->direction;
	exist = killer_entity->exist;
	force_collision_coeffisient = killer_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = killer_entity->force_resistance_air_coefficient;
	host_number = killer_entity->host_number;
	host_team_number = killer_entity->host_team_number;
	position = killer_entity->position;
	radius = killer_entity->radius;
	velocity = killer_entity->velocity;
}

void KillerEntity::Set(Vec2F* position, Vec2F* velocity, float radius, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void KillerEntity::operator=(KillerEntity killer_entity)
{
	angle = killer_entity.angle;
	angular_velocity = killer_entity.angular_velocity;
	direction = killer_entity.direction;
	exist = killer_entity.exist;
	force = killer_entity.force;
	force_collision_coeffisient = killer_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = killer_entity.force_resistance_air_coefficient;
	host_number = killer_entity.host_number;
	host_team_number = killer_entity.host_team_number;
	position = killer_entity.position;
	radius = killer_entity.radius;
	velocity = killer_entity.velocity;
}

KillerEntity::~KillerEntity()
{
}



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

ControledEntity::ControledEntity(Vec2F position, Vec2F velocity, float radius, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
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

ControledEntity::ControledEntity(const Vec2F* position, const Vec2F* velocity, float radius, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
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
	if (segment->Distance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment->Distance(&ce_side) < radius)
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

void ControledEntity::Set(Vec2F* position, Vec2F* velocity, float radius, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
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



Ship::Ship() :
	ControledEntity(),
	bonus_inventory(BONUS_NOTHING),
	buff_inventory(BONUS_NOTHING),
	burnout(0),
	burnout_input_value_pointer(nullptr),
	bullets_in_magazine(0),
	magazine_size(0),
	unbrakable(0),
	objects_in_loop(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

Ship::Ship(const Ship& ship) :
	ControledEntity(ship),
	bonus_inventory(ship.bonus_inventory),
	buff_inventory(ship.buff_inventory),
	burnout(ship.burnout),
	burnout_input_value_pointer(ship.burnout_input_value_pointer),
	bullets_in_magazine(ship.bullets_in_magazine),
	magazine_size(ship.magazine_size),
	unbrakable(ship.unbrakable),
	objects_in_loop(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

Ship::Ship(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	void* burnout_input_value_pointer,
	void* rotate_input_value_pointer,
	void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t start_bullets_count,
	bool exist)
	:
ControledEntity(
	position,
	velocity,
	radius,
	player_number,
	player_team_number,
	rotate_input_value_pointer,
	shoot_input_value_pointer,
	heat_box_vertexes_array,
	heat_box_vertexes_array_length,
	angle,
	angular_velocity,
	force_collision_coeffisient,
	force_resistance_air_coefficient,
	exist
),
bonus_inventory(bonus_inventory),
buff_inventory(buff_inventory),
burnout(burnout),
burnout_input_value_pointer(burnout_input_value_pointer),
bullets_in_magazine(start_bullets_count),
magazine_size(max_bullets_count),
unbrakable(unbrakable),
objects_in_loop(0),
element_type(GAME_OBJECT_TYPE_NULL)
{
}

Ship::Ship(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	void* burnout_input_value_pointer,
	void* rotate_input_value_pointer,
	void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t start_bullets_count,
	bool exist)
	:
ControledEntity(
	position,
	velocity,
	radius,
	player_number,
	player_team_number,
	rotate_input_value_pointer,
	shoot_input_value_pointer,
	heat_box_vertexes_array,
	heat_box_vertexes_array_length,
	angle,
	angular_velocity,
	force_collision_coeffisient,
	force_resistance_air_coefficient,
	exist
),
	bonus_inventory(bonus_inventory),
	buff_inventory(buff_inventory),
	burnout(burnout),
	burnout_input_value_pointer(burnout_input_value_pointer),
	bullets_in_magazine(start_bullets_count),
	magazine_size(max_bullets_count),
	unbrakable(unbrakable),
	objects_in_loop(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

bool Ship::ActivateAvailableBuffs()
{
	bool update = false;
	if (!(buff_inventory & SHIP_BUFF_TRIPLE) && (bonus_inventory & (BONUS_BUFF_TRIPLE * 3)))
	{
		buff_inventory += SHIP_BUFF_TRIPLE;
		bonus_inventory -= BONUS_BUFF_TRIPLE;
		update = true;
	}
	if (!(buff_inventory & SHIP_BUFF_SHIELD) && (bonus_inventory & (BONUS_BUFF_SHIELD * 3)))
	{
		buff_inventory += SHIP_BUFF_SHIELD;
		bonus_inventory -= BONUS_BUFF_SHIELD;
		update = true;
	}
	return update;
}

void Ship::ActivateBuffNoCheck(EngineTypes::Ship::inventory_t buff)
{
	buff_inventory |= buff;
}

void Ship::AddBullet()
{
	if (bullets_in_magazine < magazine_size)
	{
		bullets_in_magazine++;
	}
}

void Ship::AddBullets(GameTypes::entities_count_t bullets_count)
{
	if (bullets_count < magazine_size - bullets_in_magazine)
	{
		bullets_in_magazine += bullets_count;
	}
	else
	{
		bullets_in_magazine = magazine_size;
	}
}

void Ship::AddBulletsToMax()
{
	bullets_in_magazine = magazine_size;
}

EngineTypes::Bonus::inventory_t Ship::BonusInfo()
{
	return bonus_inventory;
}

void Ship::Burnout(float power, bool rotate_clockwise, GameTypes::tic_t burnout_period)
{
	if (rotate_clockwise)
	{
		force -= direction.PerpendicularClockwise().Normalize() * power;
		angle += (float)M_PI / 2.0f;
	}
	else
	{
		force += direction.PerpendicularClockwise().Normalize() * power;
		angle += -(float)M_PI / 2.0f;
	}
	UpdateDirection();
	burnout = burnout_period;
}

bool Ship::CanCreatingBullet() const
{
	return bullets_in_magazine > 0;
}

bool Ship::CanCreatingLoop() const
{
	return objects_in_loop > 0;
}

Bullet Ship::CreateBullet()
{
	if (bullets_in_magazine == 0)
	{
		return Bullet();
	}

	AddForceAlongDirection(-SHIP_SHOOT_FORCE);
	bullets_in_magazine--;
	reoading_dellay = SHIP_DEFAULT_REALOADING_DELLAY;

	return Bullet(
		position + direction.Normalize() * radius,
		direction * BULLET_DEFAULT_VELOCITY + velocity - direction.PerpendicularClockwise() * angular_velocity * radius,
		player_number,
		player_team_number,
		true,
		angle);
}

DynamicParticle Ship::CreateBurnoutExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_BURNOUT,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_ALL | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_EXAUST_ENGINE,
		PARTICLE_PERIOD_EXAUST_ENGINE,
		current_tic + PARTICLE_PERIOD_EXAUST_ENGINE);
}

DynamicParticle Ship::CreateEnginExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity - direction * 0.001f,
		radius,
		angle,
		0.0f,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_ENGINE,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_ALL | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_EXAUST_ENGINE,
		PARTICLE_POSTPONE_EXAUST_ENGINE,
		current_tic + PARTICLE_PERIOD_EXAUST_ENGINE);
}

DynamicParticle Ship::CreateShards(GameTypes::tic_t current_tic)
{
	
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_SHARDS_SHIP,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_SHARDS_SHIP,
		PARTICLE_POSTPONE_SHARDS_SHIP,
		current_tic + PARTICLE_PERIOD_SHARDS_SHIP + PARTICLE_POSTPONE_SHARDS_SHIP);
}

void Ship::ClearInventory()
{
	buff_inventory = 0x0;
	bonus_inventory = 0x0;
	bullets_in_magazine = 0x0;
	magazine_size = SHIP_DEFAULT_MAGAZINE_SIZE;
}

Bullet Ship::CreateTriple(uint8_t bullet_number)
{
	if (bullets_in_magazine < 3)
	{
		return Bullet();
	}
	Vec2F bullet_velocity, bullet_position;
	switch (bullet_number)
	{
	case 0:
		bullet_position = position + direction * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	case 1:
		bullet_position = position - direction.PerpendicularClockwise() * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	case 2:
		bullet_position = position + direction.PerpendicularClockwise() * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		bullets_in_magazine -= 3;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	default:
		return Bullet();
	}
}

bool Ship::CreatingEntities(
	GameTypes::entities_count_t objects_in_loop,	//count of objects in creating loop
	GameTypes::objects_types_count_t element_type	//type of elemnts in creating loop
)
{
	if (this->objects_in_loop > 0)
	{
		return false;
	}

	this->objects_in_loop = objects_in_loop;
	this->element_type = element_type;

	return true;
}

GameTypes::entities_count_t Ship::GetElemntFromList()
{
	if (objects_in_loop == 0)
	{
		return 0;
	}
	objects_in_loop--;
	return objects_in_loop + (uint16_t)1;
}

Bullet Ship::CreateLoop(GameTypes::entities_count_t bullet_number)
{ 
	printf("WARNING::Ship::CreateLoop::The function is overdate. Use Ship::CreeatingEntities().");

	Vec2F bullet_velocity = velocity + Vec2F(0.0f, 1.0f).Rotate(2.0f * (float)M_PI / (float)SHIP_BULLETS_IN_LOOP * (float)bullet_number) * BULLET_DEFAULT_VELOCITY;
	return Bullet(&position, &bullet_velocity, 
		player_number, player_team_number, true,
		0.0f, 0.0f, 
		DEFAULT_FORCE_COLLISION_COEFFICIENT, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT / 2.0f, 
		BULLET_DEFAULT_RADIUS / 2.0f);
}

Bomb Ship::CreateBomb()
{
	return Bomb(
		Vec2F(-0.25f, 0.0f) * model_matrix,
		velocity,
		player_team_number,
		player_team_number);
}

Laser Ship::CreateLaser()
{
	Beam laser_beam = LASER_DEFAULT_LOCAL_BEAM;
	AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	return Laser(this, &laser_beam);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Segment new_knife_segment;
	switch (knife_number)
	{
	case 0:
		new_knife_segment = Segment(Vec2F(-0.35f, sqrt(3.0f) / 4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	case 1:
		new_knife_segment = Segment(Vec2F(-0.35f, sqrt(3.0f) / -4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	default:
		return Knife();
	}
}

Particle Ship::CreateShootingExaust(GameTypes::tic_t current_tic)
{
	return Particle(
		current_tic,
		this,
		PARTICLE_TYPE_EXAUST_SHOOT,
		Color3F(1.0f, 0.0f, 0.0f));
}

void Ship::DecrementSizeOfMagasize(GameTypes::entities_count_t cells_count)
{
	if (magazine_size >= cells_count)
	{
		magazine_size -= cells_count;
		return;
	}
	magazine_size = 0;
	return;
}

Pilot Ship::Destroy()
{
	Vec2F zero_velocity;
	Pilot new_pilot = Pilot(&position, &zero_velocity, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, PILOT_DEFAULT_RESPAWN_TIMER, nullptr, 0, angle);
	new_pilot.AddForce(velocity * 1.5f);
	return new_pilot;
}

int Ship::GetBonusInventoryAsBoolList() const
{
	return
		(int)(
			((bonus_inventory & (BONUS_LOOP * BONUS_CELL)) ? (1u << 0u) : 0u) +		//zero bit is a loop cell
			((bonus_inventory & (BONUS_LASER * BONUS_CELL)) ? (1u << 1u) : 0u) +	//first bit is a laser cell
			((bonus_inventory & (BONUS_BOMB * BONUS_CELL)) ? (1u << 2u) : 0u) +		//second bit is a bomb cell
			((bonus_inventory & (BONUS_KNIFE * BONUS_CELL)) ? (1u << 3u) : 0u));	//third bit is a knife cell
}

GameTypes::entities_count_t Ship::GetBulletsCountInMagasine() const
{
	return bullets_in_magazine;
}

GameTypes::entities_count_t Ship::GetSizeOfMagazine() const
{
	return magazine_size;
}

GameTypes::objects_types_count_t Ship::GetTypeOfElemntInLoop() const
{
	return element_type;
}

bool Ship::IsHaveBonus(EngineTypes::Bonus::inventory_t bonus) const
{
	return bonus_inventory & (bonus * 3);
}

bool Ship::IsHaveBuff(EngineTypes::Ship::inventory_t buff) const
{
	return buff_inventory & buff;
}

void Ship::IncrementSizeOfMagazine(GameTypes::entities_count_t cells_count)
{
	if (magazine_size <= SHIP_MAGAZINE_MAX_SIZE - cells_count)
	{
		magazine_size += cells_count;
		return;
	}
	magazine_size = SHIP_MAGAZINE_MAX_SIZE;
	return;
}

bool Ship::IsUnbrakable() const
{
	return unbrakable;
}

Bonus Ship::LoseBonus()
{
	bonus_inventory &= BONUS_BONUS;
	if (bonus_inventory)
	{
		return Bonus(&position, &velocity, bonus_inventory);
	}
	return Bonus();
}

void Ship::Set(const Ship* ship)
{
	angle = ship->angle;
	angular_velocity = ship->angular_velocity;
	bonus_inventory = ship->bonus_inventory;
	buff_inventory = ship->buff_inventory;
	burnout = ship->burnout;
	burnout_input_value_pointer = ship->burnout_input_value_pointer;
	direction = ship->direction;
	exist = ship->exist;
	force_collision_coeffisient = ship->force_collision_coeffisient;
	force_resistance_air_coefficient = ship->force_resistance_air_coefficient;
	heat_box_vertexes_array_length = ship->heat_box_vertexes_array_length;
	player_number = ship->player_number;
	player_team_number = ship->player_team_number;
	position = ship->position;
	radius = ship->radius;
	rotate_input_value_pointer = ship->rotate_input_value_pointer;
	shoot_input_value_pointer = ship->shoot_input_value_pointer;
	unbrakable = ship->unbrakable;
	velocity = ship->velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = ship->heat_box_vertexes_array[vertex];
	}
}

void Ship::Set(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	void* burnout_input_value_pointer,
	void* rotate_input_value_pointer,
	void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t current_bullets_count,
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->burnout = burnout;
	this->bullets_in_magazine = current_bullets_count;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->magazine_size = max_bullets_count;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->unbrakable = unbrakable;
	this->velocity = velocity;

	delete[] this->heat_box_vertexes_array;
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
	}
	else
	{
		this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
		for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
		{
			this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
		}
	}
}

void Ship::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	void* burnout_input_value_pointer,
	void* rotate_input_value_pointer,
	void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t current_bullets_count,
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->burnout = burnout;
	this->bullets_in_magazine = current_bullets_count;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->magazine_size = max_bullets_count;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = *position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->unbrakable = unbrakable;
	this->velocity = *velocity;

	delete[] this->heat_box_vertexes_array;
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
	}
	else
	{
		this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
		for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
		{
			this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
		}
	}
}

void Ship::SetSizeOfMagazine(GameTypes::entities_count_t bullets_count)
{
	magazine_size = bullets_count;
	if (bullets_in_magazine > magazine_size)
	{
		bullets_in_magazine = magazine_size;
	}
}

void Ship::SetUnbrakablePeriod(GameTypes::tic_t period)
{
	unbrakable = period;
}

bool Ship::SpendBonus(EngineTypes::Bonus::inventory_t bonus)
{
	if (IsHaveBonus(bonus))
	{
		bonus_inventory -= bonus;
		return true;
	}
	return false;
}

void Ship::SpendBonusNoCheck(EngineTypes::Bonus::inventory_t bonus)
{
	bonus_inventory -= bonus;
}

bool Ship::SpendBuff(EngineTypes::Ship::inventory_t buff)
{
	if (IsHaveBuff(buff))
	{
		buff_inventory &= SHIP_BUFF_ALL - buff;
		return true;
	}
	return false;
}

void Ship::SpendBuffNoCheck(EngineTypes::Ship::inventory_t buff)
{
	buff_inventory &= SHIP_BUFF_ALL - buff;
}

void Ship::StopCreatingLoop()
{
	objects_in_loop = 0;
}

void Ship::TakeBonus(Bonus* bonus, bool as_triple)
{
	EngineTypes::Bonus::inventory_t new_inventory = bonus->bonus_inventory;
	if (as_triple)
	{
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_CELLS_COUNT * 2; i += 2)
		{
			if (new_inventory & (BONUS_CELL << i))
			{
				bonus_inventory |= BONUS_CELL << i;
			}
		}
	}
	else
	{
		EngineTypes::Bonus::inventory_t count;
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_CELLS_COUNT * 2; i += 2)
		{
			count = (new_inventory & (BONUS_CELL << i)) >> i;
			if (count)
			{
				count += (bonus_inventory & (BONUS_CELL << i)) >> i;
				if (count > BONUS_CELL)
				{
					bonus_inventory |= BONUS_CELL << i;
				}
				else
				{
					bonus_inventory &= BONUS_ALL - (BONUS_CELL << i);
					bonus_inventory |= count << i;
				}
			}
		}
	}
}

void Ship::Update()
{
	if (burnout > 0)
	{
		AddForceAlongDirection(SHIP_BURNOUT_FORCE);
		burnout--;
	}
	
	if (unbrakable > 0)
	{
		unbrakable--;
	}

	if (reoading_dellay > 0)
	{
		reoading_dellay--;
	}
	else
	{
#if SHIP_DEFAULT_RELOADING_PERIOD == 0
		AddBulletsToMax();
#else
		AddBullet();
		reoading_dellay = SHIP_DEFAULT_RELOADING_PERIOD;
#endif
	}

	DynamicEntity::Update();
}

void Ship::UpdateMatrix()
{
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}


void Ship::operator=(Ship ship)
{
	angle = ship.angle;
	angular_velocity = ship.angular_velocity;
	bonus_inventory = ship.bonus_inventory;
	buff_inventory = ship.buff_inventory;
	bullets_in_magazine = ship.bullets_in_magazine;
	direction = ship.direction;
	exist = ship.exist;
	force = ship.force;
	force_collision_coeffisient = ship.force_collision_coeffisient;
	force_resistance_air_coefficient = ship.force_resistance_air_coefficient;
	heat_box_vertexes_array_length = ship.heat_box_vertexes_array_length;
	magazine_size = ship.magazine_size;
	player_number = ship.player_number;
	player_team_number = ship.player_team_number;
	position = ship.position;
	radius = ship.radius;
	rotate_input_value_pointer = ship.rotate_input_value_pointer;
	shoot_input_value_pointer = ship.shoot_input_value_pointer;
	unbrakable = ship.unbrakable;
	velocity = ship.velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = ship.heat_box_vertexes_array[vertex];
	}
}

Ship::~Ship()
{
}



Pilot::Pilot():
	ControledEntity(),
	respawn_timer(0)
{
}

Pilot::Pilot(const Pilot& pilot) :
	ControledEntity(pilot),
	respawn_timer(pilot.respawn_timer)
{
}

Pilot::Pilot(Vec2F position, Vec2F velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, GameTypes::tic_t respawn_timer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::inventory_t buffs_bonuses, EngineTypes::Bonus::inventory_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	respawn_timer(respawn_timer)
{
}

Pilot::Pilot(const Vec2F* position, const Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, GameTypes::tic_t respawn_timer,  Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::inventory_t buffs_bonuses, EngineTypes::Bonus::inventory_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	respawn_timer(respawn_timer)
{
}

bool Pilot::CanRespawn() const
{
	return respawn_timer == 0;
}

DynamicParticle Pilot::CreateShards(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_SHARDS_PILOT,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_SHARDS_PILOT,
		PARTICLE_POSTPONE_SHARDS_PILOT,
		current_tic + PARTICLE_PERIOD_SHARDS_PILOT);
}

GameTypes::tic_t Pilot::GetRespawnDellay() const
{
	return respawn_timer;
}

Ship Pilot::Respawn()
{
	Vec2F temp = Vec2F();
	return Ship(&position, &velocity, player_number, player_team_number, nullptr, rotate_input_value_pointer, shoot_input_value_pointer, nullptr, 0, angle);
}

void Pilot::Set(Pilot* pilot)
{
	angle = pilot->angle;
	angular_velocity = pilot->angular_velocity;
	direction = pilot->direction;
	exist = pilot->exist;
	force = pilot->force;
	heat_box_vertexes_array_length = pilot->heat_box_vertexes_array_length;
	player_number = pilot->player_number;
	player_team_number = pilot->player_team_number;
	position = pilot->position;
	radius = pilot->radius;
	respawn_timer = pilot->respawn_timer;
	rotate_input_value_pointer = pilot->rotate_input_value_pointer;
	shoot_input_value_pointer = pilot->shoot_input_value_pointer;
	velocity = pilot->velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = pilot->heat_box_vertexes_array[vertex];
	}
}

void Pilot::Update()
{
	DynamicEntity::Update();
	if (respawn_timer > 0)
	{
		respawn_timer--;
	}
}

void Pilot::UpdateMatrix()
{
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}


void Pilot::operator=(Pilot pilot)
{
	angle = pilot.angle;
	angular_velocity = pilot.angular_velocity;
	direction = pilot.direction;
	exist = pilot.exist;
	force = pilot.force;
	heat_box_vertexes_array_length = pilot.heat_box_vertexes_array_length;
	player_number = pilot.player_number;
	player_team_number = pilot.player_team_number;
	position = pilot.position;
	radius = pilot.radius;
	respawn_timer = pilot.respawn_timer;
	rotate_input_value_pointer = pilot.rotate_input_value_pointer;
	shoot_input_value_pointer = pilot.shoot_input_value_pointer;
	velocity = pilot.velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = pilot.heat_box_vertexes_array[vertex];
	}
}

Pilot::~Pilot()
{

}


SupportEntity::SupportEntity() :
	StaticEntity(),
	host_p(nullptr),
	host_matrix_p(nullptr),
	local_angle(0),
	host_number(0),
	host_team(0)
{
}

SupportEntity::SupportEntity(const SupportEntity& support_entity) :
	StaticEntity(support_entity),
	host_p(support_entity.host_p),
	host_matrix_p(support_entity.host_matrix_p),
	host_number(support_entity.host_number),
	host_team(support_entity.host_team),
	local_angle(support_entity.local_angle),
	local_direction(support_entity.local_direction),
	local_position(support_entity.local_position)
{
	this->position *= *host_matrix_p;
}

SupportEntity::SupportEntity(const ControledEntity* host, Vec2F position, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle + host->GetAngle(), exist),
	host_p(host),
	host_matrix_p(host->GetModelMatrixPointer()),
	host_number(host->GetPlayerNumber()),
	host_team(host->GetTeamNumber()),
	local_angle(angle),
	local_direction(Vec2F(1.0f, 0.0f).Rotate(angle)),
	local_position(position)
{
	this->position *= *host_matrix_p;
}

SupportEntity::SupportEntity(const ControledEntity* host, const Vec2F* position, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle + host->GetAngle(), exist),
	host_p(host),
	host_matrix_p(host->GetModelMatrixPointer()),
	host_number(host->GetPlayerNumber()),
	host_team(host->GetTeamNumber()),
	local_angle(angle),
	local_direction(Vec2F(1.0f, 0.0f).Rotate(angle)),
	local_position(*position)
{
	this->position *= *host_matrix_p;
}

bool SupportEntity::IsCreatedBy(ControledEntity* potential_host) const
{
	return host_number == potential_host->GetPlayerNumber();
}

bool SupportEntity::IsCreatedByTeam(ControledEntity* potential_host) const
{
	return host_team == potential_host->GetTeamNumber();
}

float SupportEntity::GetAngle() const
{
	return local_angle;
}

Vec2F SupportEntity::GetDirection() const
{
	return local_direction;
}

Vec2F SupportEntity::GetNormalizeDirection() const
{
	return local_direction.Normalize();
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterNumber() const
{
	return host_number;
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterTeamNumber() const
{
	return host_team;
}

Vec2F SupportEntity::GetPosition() const
{
	return local_position;
}

void SupportEntity::Set(const SupportEntity* support_entity)
{
	angle = support_entity->angle;
	direction = support_entity->direction;
	exist = support_entity->exist;
	host_p = support_entity->host_p;
	host_matrix_p = support_entity->host_matrix_p;
	host_number = support_entity->host_number;
	host_team = support_entity->host_team;
	local_angle = support_entity->local_angle;
	local_direction = support_entity->local_direction;
	local_position = support_entity->local_position;
	position = support_entity->position;
	radius = support_entity->radius;
}

void SupportEntity::Set(const ControledEntity* host, Vec2F position, float radius, float angle, bool exist)
{
	if (host == nullptr)
	{
		return;
	}
	this->angle = host->GetAngle();
	this->direction = host->GetDirectionNotNormalize();
	this->exist = exist;
	this->host_number = host->GetPlayerNumber();
	this->host_team = host->GetTeamNumber();
	this->local_angle = angle;
	this->local_direction = Vec2F(1.0f, 0.0f).Rotate(angle);
	this->local_position = position;
	this->position = host->GetPosition();
	this->radius = radius;

	this->host_p = host;
	this->host_matrix_p = host->GetModelMatrixPointer();
}

void SupportEntity::Set(const ControledEntity* host, const Vec2F* position, float radius, float angle, bool exist)
{
	if (host == nullptr)
	{
		return;
	}
	this->angle = host->GetAngle();
	this->direction = host->GetDirectionNotNormalize();
	this->exist = exist;
	this->host_number = host->GetPlayerNumber();
	this->host_team = host->GetTeamNumber();
	this->local_angle = angle;
	this->local_direction = Vec2F(1.0f, 0.0f).Rotate(angle);
	this->local_position = *position;
	this->position = host->GetPosition();
	this->radius = radius;

	this->host_p = host;
	this->host_matrix_p = host->GetModelMatrixPointer();
}

void SupportEntity::SetAngle(float angle)
{
	local_angle = angle;
}

void SupportEntity::SetDirection(Vec2F direction)
{
	local_direction = direction.Normalize();
}

void SupportEntity::SetDirection(const Vec2F* direction)
{
	local_direction = direction->Normalize();
}

void SupportEntity::SetNotNormalizeDirection(Vec2F direction)
{
	local_direction = direction;
}

void SupportEntity::SetNotNormalizeDirection(const Vec2F* direction)
{
	local_direction = *direction;
}

void SupportEntity::SetHost(const ControledEntity* host)
{
	host_p = host;
	host_matrix_p = host->GetModelMatrixPointer();
}

void SupportEntity::SetPosition(Vec2F position)
{
	local_position = position;
}

void SupportEntity::SetPosition(const Vec2F* position)
{
	local_position = *position;
}

void SupportEntity::Update()
{
	if (host_p == nullptr)
	{
		return;
	}
	angle = local_angle + host_p->GetAngle();
	direction = local_direction * (Mat2F)*host_matrix_p;
	position = local_position * *host_matrix_p;
	StaticEntity::Update();
}

void SupportEntity::UpdateDirection()
{
	local_direction = Vec2F(direction.Length(), 0.0f).Rotate(local_angle);
}

void SupportEntity::operator=(SupportEntity support_entity)
{
	angle = support_entity.angle;
	direction = support_entity.direction;
	exist = support_entity.exist;
	host_p = support_entity.host_p;
	host_matrix_p = support_entity.host_matrix_p;
	host_number = support_entity.host_number;
	host_team = support_entity.host_team;
	local_angle = support_entity.local_angle;
	local_direction = support_entity.local_direction;
	local_position = support_entity.local_position;
	position = support_entity.position;
	radius = support_entity.radius;
}

SupportEntity::~SupportEntity()
{
}



AggressiveEntity::AggressiveEntity() :
	StaticEntity(),
	attack_dellay(0),
	attack_period(AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD),
	inactive_period(AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD),
	shoots_count(AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT)
{
}

AggressiveEntity::AggressiveEntity(const AggressiveEntity& aggressive_entity) :
	StaticEntity(aggressive_entity),
	attack_dellay(aggressive_entity.attack_dellay),
	attack_period(aggressive_entity.attack_period),
	inactive_period(aggressive_entity.inactive_period),
	shoots_count(aggressive_entity.shoots_count)
{
}

AggressiveEntity::AggressiveEntity(Vec2F position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist) :
	StaticEntity(position, radius, angle, exist),
	attack_dellay(attack_dellay),
	attack_period(attack_period),
	inactive_period(inactive_period),
	shoots_count(shoots_count)
{
}

AggressiveEntity::AggressiveEntity(const Vec2F* position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist) :
	StaticEntity(position, radius, angle, exist),
	attack_dellay(attack_dellay),
	attack_period(attack_period),
	inactive_period(inactive_period),
	shoots_count(shoots_count)
{
}

bool AggressiveEntity::CanShoot(GameTypes::tic_t current_tic) const
{
	GameTypes::tic_t local_tic = (current_tic - attack_dellay) % (attack_period + inactive_period);
	if (local_tic < attack_period)
	{
		return false;
	}
	if (attack_period < shoots_count)
	{
		return true;
	}
	return (local_tic - attack_period) % (attack_period / shoots_count) == 0;
}

void AggressiveEntity::PostponeAttack(GameTypes::tic_t dellay)
{
	attack_dellay += dellay;
}

void AggressiveEntity::Set(const AggressiveEntity* aggressive_entity)
{
	angle = aggressive_entity->angle;
	attack_dellay = aggressive_entity->attack_dellay;
	attack_period = aggressive_entity->attack_period;
	direction = aggressive_entity->direction;
	exist = aggressive_entity->exist;
	inactive_period = aggressive_entity->inactive_period;
	last_position = aggressive_entity->last_position;
	position = aggressive_entity->position;
	shoots_count = aggressive_entity->shoots_count;
}

void AggressiveEntity::Set(Vec2F position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = position;
	this->shoots_count = shoots_count;
}

void AggressiveEntity::Set(const Vec2F* position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = *position;
	this->shoots_count = shoots_count;
}

void AggressiveEntity::operator=(AggressiveEntity aggressive_entity)
{
	angle = aggressive_entity.angle;
	attack_dellay = aggressive_entity.attack_dellay;
	attack_period = aggressive_entity.attack_period;
	direction = aggressive_entity.direction;
	exist = aggressive_entity.exist;
	inactive_period = aggressive_entity.inactive_period;
	last_position = aggressive_entity.last_position;
	position = aggressive_entity.position;
	shoots_count = aggressive_entity.shoots_count;
}

AggressiveEntity::~AggressiveEntity()
{

}



Turel::Turel() :
	AggressiveEntity()
{
}

Turel::Turel(const Turel& turel) :
	AggressiveEntity(turel)
{
}

Turel::Turel(Vec2F position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist) :
	AggressiveEntity(position, radius, angle, attack_dellay, attack_period, inactive_period, shoots_count, exist)
{
}

Turel::Turel(const Vec2F* position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist) :
	AggressiveEntity(position, radius, angle, attack_dellay, attack_period, inactive_period, shoots_count, exist)
{
}

DynamicParticle Turel::CreateShards(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		Vec2F(),
		radius,
		angle,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		PARTICLE_TYPE_SHARDS_ASTEROID,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		PARTICLE_PERIOD_SHARDS_ASTEROID,
		PARTICLE_POSTPONE_SHARDS_ASTEROID,
		current_tic + PARTICLE_PERIOD_SHARDS_ASTEROID);

	std::cout << "Turel::CreateShards::Not realisated." << std::endl;
}

Bullet Turel::Shoot()
{
	Vec2F new_bullet_velocity = direction * BULLET_DEFAULT_VELOCITY;
	Vec2F new_bullet_position = position + direction * radius * 2.0f;
	return Bullet(&new_bullet_position, &new_bullet_velocity, AGGRESIVE_ENTITY_HOST_ID, AGGRESIVE_ENTITY_HOST_ID, false);
}

void Turel::Set(const Turel* turel)
{
	angle = turel->angle;
	attack_dellay = turel->attack_dellay;
	attack_period = turel->attack_period;
	direction = turel->direction;
	exist = turel->exist;
	inactive_period = turel->inactive_period;
	last_position = turel->last_position;
	position = turel->position;
	radius = turel->radius;
	shoots_count = turel->shoots_count;
}

void Turel::Set(Vec2F position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
	this->shoots_count = shoots_count;
}

void Turel::Set(const Vec2F* position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
	this->shoots_count = shoots_count;
}

void Turel::operator=(Turel turel)
{
	angle = turel.angle;
	attack_dellay = turel.attack_dellay;
	attack_period = turel.attack_period;
	direction = turel.direction;
	exist = turel.exist;
	inactive_period = turel.inactive_period;
	last_position = turel.last_position;
	position = turel.position;
	radius = turel.radius;
	shoots_count = turel.shoots_count;
}

Turel::~Turel()
{
}



DecelerationArea::DecelerationArea() :
	StaticEntity(),
	deceleration_parameter(DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT)
{
}

DecelerationArea::DecelerationArea(const DecelerationArea& deceleration_area):
	StaticEntity(deceleration_area),
	deceleration_parameter(deceleration_area.deceleration_parameter)
{
}

DecelerationArea::DecelerationArea(Vec2F position, float deceleration_parameter, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	deceleration_parameter(deceleration_parameter)
{
}

DecelerationArea::DecelerationArea(const Vec2F* position, float deceleration_parameter, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	deceleration_parameter(deceleration_parameter)
{
}

void DecelerationArea::Set(const DecelerationArea* deceleration_area)
{
	angle = deceleration_area->angle;
	deceleration_parameter = deceleration_area->deceleration_parameter;
	exist = deceleration_area->exist;
	last_position = deceleration_area->last_position;
	position = deceleration_area->position;
	radius = deceleration_area->radius;
}

void DecelerationArea::Set(Vec2F position, float deceleration_parameter, float radius, float angle, bool exist)
{
	this->angle = angle;
	this->deceleration_parameter = deceleration_parameter;
	this->exist = exist;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
}

void DecelerationArea::Set(const Vec2F* position, float deceleration_parameter, float radius, float angle, bool exist)
{
	this->angle = angle;
	this->deceleration_parameter = deceleration_parameter;
	this->exist = exist;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
}

void DecelerationArea::operator=(DecelerationArea deceleration_area)
{
	angle = deceleration_area.angle;
	deceleration_parameter = deceleration_area.deceleration_parameter;
	direction = deceleration_area.direction;
	exist = deceleration_area.exist;
	last_position = deceleration_area.last_position;
	position = deceleration_area.position;
	radius = deceleration_area.radius;
}

DecelerationArea::~DecelerationArea()
{
}



GravGen::GravGen() :
	gravity(GRAVITY_GENERATOR_DEFAULT_GRAVITY)
{
}

GravGen::GravGen(const GravGen& grav_gen) :
	StaticEntity(grav_gen),
	gravity(grav_gen.gravity)
{
}

GravGen::GravGen(Vec2F position, float gravity, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	gravity(gravity)
{
}

GravGen::GravGen(const Vec2F* position, float gravity, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	gravity(gravity)
{
}

void GravGen::Set(const GravGen* grav_gen)
{
	angle = grav_gen->angle;
	direction = grav_gen->direction;
	exist = grav_gen->exist;
	gravity = grav_gen->gravity;
	last_position = grav_gen->last_position;
	position = grav_gen->position;
}

void GravGen::Set(Vec2F position, float gravity, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->gravity = gravity;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
}

void GravGen::Set(const Vec2F* position, float gravity, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->gravity = gravity;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
}

void GravGen::operator=(GravGen grav_gen)
{
	angle = grav_gen.angle;
	direction = grav_gen.direction;
	exist = grav_gen.exist;
	gravity = grav_gen.gravity;
	last_position = grav_gen.last_position;
	position = grav_gen.position;
	radius = grav_gen.radius;
}

GravGen::~GravGen()
{
}



Portal::Portal()
	:
	StaticEntity(),
	tp_mode(0),
	tp_position(Vec2F()),
	tp_position_pointer(nullptr)
{
}

Portal::Portal(const Portal& portal)
	:
	StaticEntity(portal.position, portal.radius, portal.angle, portal.exist),
	tp_mode(portal.tp_mode),
	tp_position(portal.tp_position),
	tp_position_pointer(portal.tp_position_pointer)
{
}

Portal::Portal(
	Vec2F position,
	Vec2F tp_position,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode(PORTAL_MODE_POSITION),
	tp_position(tp_position),
	tp_position_pointer(nullptr)
{
}

Portal::Portal(
	const Vec2F* position,
	Vec2F tp_position,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode(PORTAL_MODE_POSITION),
	tp_position(tp_position),
	tp_position_pointer(nullptr)
{
}

Portal::Portal(
	Vec2F position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode(PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(tp_position_pointer)
{
}

Portal::Portal(
	const Vec2F* position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode(PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(tp_position_pointer)
{
}

Portal::Portal(
	Vec2F position,
	const Entity* entity,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode((EngineTypes::Portal::mode_t)PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(entity->GetPositionPointer())
{
}

Portal::Portal(
	const Vec2F* position,
	const Entity* entity,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode((EngineTypes::Portal::mode_t)PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(entity->GetPositionPointer())
{
}

void Portal::Connect(const Vec2F* pointer)
{
	tp_position_pointer = pointer;
}

void Portal::Connect(const Entity* entity)
{
	tp_position_pointer = entity->GetPositionPointer();
}

Particle Portal::CreateParticles(GameTypes::tic_t current_tic)
{
	return Particle(
		current_tic,
		position,
		angle,
		radius,
		PARTICLE_TYPE_PORTAL_IN,
		Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f),
		PARTICLE_PERIOD_PORTAL_IN,
		PARTICLE_POSTPONE_PORTAL_IN,
		current_tic + PARTICLE_POSTPONE_PORTAL_IN + PARTICLE_PERIOD_PORTAL_IN);
}

Particle Portal::CreateParticlesTP(GameTypes::tic_t current_tic, float radius)
{
	if (tp_mode == PORTAL_MODE_POSITION)
	{
		return Particle(
			current_tic,
			tp_position,
			angle,
			radius,
			PARTICLE_TYPE_PORTAL_OUT,
			Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f),
			PARTICLE_PERIOD_PORTAL_OUT,
			PARTICLE_POSTPONE_PORTAL_OUT,
			current_tic + PARTICLE_POSTPONE_PORTAL_OUT + PARTICLE_PERIOD_PORTAL_OUT);
	}
	if (tp_mode == PORTAL_MODE_POINTER)
	{
		return Particle(
			current_tic,
			*tp_position_pointer,
			angle,
			radius,
			PARTICLE_TYPE_PORTAL_OUT,
			Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f),
			PARTICLE_PERIOD_PORTAL_OUT,
			PARTICLE_POSTPONE_PORTAL_OUT,
			current_tic + PARTICLE_POSTPONE_PORTAL_OUT + PARTICLE_PERIOD_PORTAL_OUT);
	}
	return Particle();
}

void Portal::Disconnect()
{
	tp_position_pointer = nullptr;
}

bool Portal::IsConnected() const
{
	return 
		tp_mode == PORTAL_MODE_POSITION || 
		tp_mode == PORTAL_MODE_POINTER && tp_position_pointer != nullptr;
}

void Portal::Set(
	Vec2F position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
	this->tp_position_pointer = tp_position_pointer;
}

void Portal::Set(
	const Vec2F* position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
	this->tp_mode = PORTAL_MODE_POINTER;
	this->tp_position_pointer = tp_position_pointer;
}

void Portal::SetMode(EngineTypes::Portal::mode_t mode)
{
	if (mode == PORTAL_MODE_POINTER && tp_position_pointer != nullptr)
	{
		tp_mode = PORTAL_MODE_POINTER;
		return;
	}
	if (mode == PORTAL_MODE_POSITION)
	{
		tp_mode = PORTAL_MODE_POSITION;
		return;
	}
}

template <typename EntityType>
void Portal::Teleport(EntityType* entity)
{
	if (tp_mode == PORTAL_MODE_POINTER)
	{
		entity->SetPosition(tp_position_pointer);
	}
	else if (tp_mode == PORTAL_MODE_POSITION)
	{
		entity->SetPosition(tp_position);
	}
	else
	{
		std::cout << "tp_mode: " << (unsigned)tp_mode << std::endl;
		return;
	}

	entity->Rotate(angle);
	entity->SetVelocity(entity->GetVelocity().RotateClockwise(angle));
	entity->ClearForce();
}
template void Portal::Teleport<Asteroid>(Asteroid* asteroid);
template void Portal::Teleport<Bomb>(Bomb* asteroid);
template void Portal::Teleport<Bonus>(Bonus* asteroid);
template void Portal::Teleport<Bullet>(Bullet* asteroid);
template void Portal::Teleport<DynamicParticle>(DynamicParticle* asteroid);
template void Portal::Teleport<Pilot>(Pilot* asteroid);
template void Portal::Teleport<Ship>(Ship* asteroid);

void Portal::operator=(Portal portal)
{
	angle = portal.angle;
	direction = portal.direction;
	exist = portal.exist;
	last_position = portal.last_position;
	position = portal.position;
	radius = portal.radius;
	tp_mode = portal.tp_mode;
	tp_position = portal.tp_position;
	tp_position_pointer = portal.tp_position_pointer;
}

Portal::~Portal()
{
}



MegaLaser::MegaLaser() :
	AggressiveEntity(),
	active(false),
	width(0.0f)
{
}

MegaLaser::MegaLaser(const MegaLaser& mega_laser) :
	AggressiveEntity(mega_laser),
	active(mega_laser.active),
	width(mega_laser.width)
{
}

MegaLaser::MegaLaser(const Segment* segment, float width, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool active, bool exist) :
	AggressiveEntity(&segment->point, segment->vector.Length(), segment->vector.AbsoluteAngle(), attack_dellay, attack_period, inactive_period, shoots_count, exist),
	active(active),
	width(width)
{
}

Segment MegaLaser::GetSegment() const
{
	return Segment(position, direction * radius);
}

bool MegaLaser::IsCollision(const Beam* beam) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);
	Line perpendicular = Line(
		beam->point,
		beam->vector.Perpendicular());
	Vec2F intersect;

	return
		main.Intersection(&perpendicular, &intersect) && intersect.Distance(beam->point) < width ||
		start.IsIntersection(beam) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(beam);
}

bool MegaLaser::IsCollision(const Line* line) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);

	return
		Line(line->point, line->vector.Perpendicular()).IsIntersection(&main) ||
		start.IsIntersection(line) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(line);
}

bool MegaLaser::IsCollision(const Segment* segment) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);
	Line perpendicular = Line(
		segment->point,
		segment->vector.Perpendicular());
	Vec2F intersect;

	if (main.Intersection(&perpendicular, &intersect) && intersect.Distance(segment->point) < width)
	{
		return true;
	}

	perpendicular.point += segment->vector;

	return
		main.Intersection(&perpendicular, &intersect) && intersect.Distance(segment->SecondPoint()) < width ||
		start.IsIntersection(segment) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(segment);
}

void MegaLaser::Rotate(float angle)
{
	direction.RotateThis(angle);
	this->angle += angle;
}

void MegaLaser::Set(const MegaLaser* mega_laser)
{
	active = mega_laser->active;
	angle = mega_laser->angle;
	attack_dellay = mega_laser->attack_dellay;
	attack_period = mega_laser->attack_period;
	direction = mega_laser->direction;
	exist = mega_laser->exist;
	inactive_period = mega_laser->inactive_period;
	last_position = mega_laser->last_position;
	position = mega_laser->position;
	radius = mega_laser->radius;
	shoots_count = mega_laser->shoots_count;
}

void MegaLaser::Set(const Segment* segment, float width, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool active, bool exist)
{
	this->active = active;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	this->direction = segment->vector.Normalize();
	UpdateAngle();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->position = segment->point;
	this->radius = segment->vector.Length();
	this->shoots_count = shoots_count;
	this->width = width;

	direction *= radius;
}

bool MegaLaser::IsShooting(GameTypes::tic_t current_tic) const
{
	if (current_tic < attack_dellay) return false;
	return ((current_tic - attack_dellay) % attack_period) > inactive_period;
}

void MegaLaser::operator=(MegaLaser mega_laser)
{
	active = mega_laser.active;
	angle = mega_laser.angle;
	attack_dellay = mega_laser.attack_dellay;
	attack_period = mega_laser.attack_period;
	direction = mega_laser.direction;
	exist = mega_laser.exist;
	inactive_period = mega_laser.inactive_period;
	last_position = mega_laser.last_position;
	position = mega_laser.position;
	radius = mega_laser.radius;
	shoots_count = mega_laser.shoots_count;
	width = mega_laser.width;
}

MegaLaser::~MegaLaser()
{
}



Bomb::Bomb() :
	KillerEntity(),
	animation_tic(0),
	activation_period(0),
	blinking_period(0),
	status(BOMB_STATUS_INACTIVE)
{
}

Bomb::Bomb(const Bomb& bomb) :
	KillerEntity(bomb),
	activation_period(bomb.activation_period),
	animation_tic(bomb.animation_tic),
	blinking_period(bomb.blinking_period),
	status(bomb.status)
{
}

Bomb::Bomb(Vec2F position, Vec2F velocity, GameTypes::players_count_t master1_team_number, GameTypes::players_count_t master2_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, EngineTypes::Bomb::status_t status, GameTypes::tic_t activation_period, GameTypes::tic_t blinking_period, bool exist) :
	KillerEntity(position, velocity, radius, master1_team_number, master2_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	activation_period(activation_period),
	animation_tic(animation_tic),
	blinking_period(blinking_period),
	status(status)
{
}

Bomb::Bomb(const Vec2F* position, const Vec2F* velocity, GameTypes::players_count_t master1_team_number, GameTypes::players_count_t master2_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, EngineTypes::Bomb::status_t status, GameTypes::tic_t activation_period, GameTypes::tic_t blinking_period, bool exist) :
	KillerEntity(position, velocity, radius, master1_team_number, master2_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	activation_period(activation_period),
	animation_tic(animation_tic),
	blinking_period(blinking_period),
	status(status)
{
}

void Bomb::Activate()
{
	status |= BOMB_STATUS_ACTIVE;
	animation_tic = activation_period;
	blinking_period = activation_period;
}

void Bomb::Boom(GameTypes::tic_t period)
{
	animation_tic = period;
	blinking_period = period;
	status &= BOMB_STATUS_ALL - BOMB_STATUS_ACTIVE;
	radius *= BOMB_BOOM_RADIUS_COEF;
	status |= BOMB_STATUS_BOOM;
}

bool Bomb::CanRemove() const
{
	return status & BOMB_STATUS_CAN_REMOVE;
}

bool Bomb::Collision(const Map* map)
{
	void* map_element;
	bool collision = false;

	if (status & BOMB_STATUS_BOOM)
	{
		for (EngineTypes::Map::array_length_t i = 0; i < map->cyrcles_array_length; i++)
		{
			map_element = (void*)map->CyrclePointer(i);
			if (((Cyrcle*)map_element)->exist && !(((Cyrcle*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Cyrcle*)map_element))
			{
				((Cyrcle*)map_element)->exist = false;
				collision = true;
			}
		}
		for (EngineTypes::Map::array_length_t i = 0; i < map->polygons_array_length; i++)
		{
			map_element = (void*)map->PolygonPointer(i);
			if (((Polygon*)map_element)->exist && !(((Polygon*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Polygon*)map_element))
			{
				((Polygon*)map_element)->exist = false;
				collision = true;
			}
		}
		for (EngineTypes::Map::array_length_t i = 0; i < map->rectangles_array_length; i++)
		{
			map_element = (void*)map->RectanglePointer(i);
			if (((Rectangle*)map_element)->exist && !(((Rectangle*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Rectangle*)map_element))
			{
				((Rectangle*)map_element)->exist = false;
				collision = true;
			}
		}
		return collision;
	}

	for (EngineTypes::Map::array_length_t i = 0; i < map->cyrcles_array_length; i++)
	{
		map_element = (void*)map->CyrclePointer(i);
		if (((Cyrcle*)map_element)->exist)
		{
			collision |= DynamicEntity::Collision((Cyrcle*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->polygons_array_length; i++)
	{
		map_element = (void*)map->PolygonPointer(i);
		if (((Polygon*)map_element)->exist)
		{
			collision |= DynamicEntity::Collision((Polygon*)map_element);
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map->rectangles_array_length; i++)
	{
		map_element = (void*)map->RectanglePointer(i);
		if (((Rectangle*)map_element)->exist)
		{
			collision |= DynamicEntity::Collision((Rectangle*)map_element);
		}
	}
	return collision;
}

GameTypes::tic_t Bomb::GetAnimationTic() const
{
	return animation_tic;
}

bool Bomb::IsActive() const
{
	return status & BOMB_STATUS_ACTIVE;
}

bool Bomb::IsBoom() const
{
	return status & BOMB_STATUS_BOOM;
}

bool Bomb::IsCreatedByAggressiveTeam() const
{
	return host_number == AGGRESIVE_ENTITY_HOST_ID || host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByAggressiveTeamOnly() const
{
	return host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByAggressiveTeamNotOnly() const
{
	return
		host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number != AGGRESIVE_ENTITY_HOST_ID ||
		host_number != AGGRESIVE_ENTITY_HOST_ID && host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByTeam(const ControledEntity* host) const
{
	GameTypes::players_count_t host_team = host->GetTeamNumber();
	return host_team_number == host_team;
}

GameTypes::players_count_t Bomb::GetTeamNumber(const ControledEntity* not_host) const
{
	if (host_number == not_host->GetTeamNumber() || host_number == AGGRESIVE_ENTITY_HOST_ID)
	{
		return host_team_number;
	}
	return host_number;
}

bool Bomb::IsAggressiveFor(const ControledEntity* host) const
{
	GameTypes::players_count_t host_team = host->GetTeamNumber();
	return
		host_number == host_team && host_team_number == host_team ||
		host_number != host_team && host_team_number != host_team && host_number != host_team_number ||
		host_number == host_team && host_team_number == AGGRESIVE_ENTITY_HOST_ID ||
		host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number == host_team ||
		host_team_number == AGGRESIVE_ENTITY_HOST_ID && host_number == AGGRESIVE_ENTITY_HOST_ID;
}

void Bomb::Set(const Bomb* bomb)
{
	activation_period = bomb->activation_period;
	angle = bomb->angle;
	angular_velocity = bomb->angular_velocity;
	animation_tic = bomb->animation_tic;
	blinking_period = bomb->blinking_period;
	direction = bomb->direction;
	exist = bomb->exist;
	force_collision_coeffisient = bomb->force_collision_coeffisient;
	force_resistance_air_coefficient = bomb->force_resistance_air_coefficient;
	host_number = bomb->host_number;
	host_team_number = bomb->host_team_number;
	position = bomb->position;
	radius = bomb->radius;
	status = bomb->status;
	velocity = bomb->velocity;
}

void Bomb::Set(Vec2F position, Vec2F velocity, GameTypes::players_count_t payer_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, EngineTypes::Bomb::status_t status, GameTypes::tic_t activation_period, GameTypes::tic_t blinking_period, bool exist)
{
	this->activation_period = activation_period;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	this->blinking_period = blinking_period;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = host_number;
	this->host_team_number = player_master_team_number;
	this->position = position;
	this->radius = radius;
	this->status = status;
	this->velocity = velocity;
}

void Bomb::Set(const Vec2F* position, const Vec2F* velocity, GameTypes::players_count_t payer_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, EngineTypes::Bomb::status_t status, GameTypes::tic_t activation_period, GameTypes::tic_t blinking_period, bool exist)
{
	this->activation_period = activation_period;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	this->blinking_period = blinking_period;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = host_number;
	this->host_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->status = status;
	this->velocity = *velocity;
}

void Bomb::Update()
{
	if (status & BOMB_STATUS_CAN_REMOVE)
	{
		return;
	}
	DynamicEntity::Update();
	if (status & (BOMB_STATUS_ACTIVE | BOMB_STATUS_BOOM))
	{
		if (status & BOMB_STATUS_ACTIVE)
		{
			angular_velocity += BOMB_DEFAULT_ANGULAR_VELOCITY_INCREMENTATION / (float)activation_period;
		}
		if (animation_tic == 0)
		{
			if (status & BOMB_STATUS_BOOM)
			{
				status |= BOMB_STATUS_CAN_REMOVE;
				return;
			}
			Boom();
			return;
		}
		animation_tic--;
		return;
	}
	else
	{
		if (animation_tic == 0)
		{
			animation_tic = blinking_period;
			return;
		}
		animation_tic--;
		return;
	}
}

void Bomb::operator=(Bomb bomb)
{
	activation_period = bomb.activation_period;
	angle = bomb.angle;
	angular_velocity = bomb.angular_velocity;
	animation_tic = bomb.animation_tic;
	blinking_period = bomb.blinking_period;
	direction = bomb.direction;
	exist = bomb.exist;
	force = bomb.force;
	force_collision_coeffisient = bomb.force_collision_coeffisient;
	force_resistance_air_coefficient = bomb.force_resistance_air_coefficient;
	host_number = bomb.host_number;
	host_team_number = bomb.host_team_number;
	position = bomb.position;
	radius = bomb.radius;
	status = bomb.status;
	velocity = bomb.velocity;
}

Bomb::~Bomb()
{
}



Bullet::Bullet() :
	KillerEntity(),
	is_ignore(BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER),
	min_velocity(BULLET_DEFAULT_MIN_VELOCITY)
{
}

Bullet::Bullet(const Bullet& bullet) :
	KillerEntity(bullet),
	is_ignore(bullet.is_ignore),
	min_velocity(bullet.min_velocity)
{
}

Bullet::Bullet(Vec2F position, Vec2F velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velocity, bool exist) :
	KillerEntity(position, velocity, radius, player_master_number, player_master_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	is_ignore((is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING),
	min_velocity(min_velocity)
{
}

Bullet::Bullet(const Vec2F* position, const Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velocity, bool exist) :
	KillerEntity(position, velocity, radius, player_master_number, player_master_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	is_ignore((is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING),
	min_velocity(min_velocity)
{
}

bool Bullet::Collision(Map* map)
{
	void* element_p;
	EngineTypes::Map::array_length_t element;
	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Cyrcle*)element_p)->exist && DynamicEntity::IsCollision((Cyrcle*)element_p))
		{
			if (!((Cyrcle*)element_p)->IsUnbreacable())
			{
				((Cyrcle*)element_p)->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Polygon*)element_p)->exist && DynamicEntity::IsCollision((Polygon*)element_p))
		{
			if (!((Polygon*)element_p)->IsUnbreacable())
			{
				((Polygon*)element_p)->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Rectangle*)element_p)->exist && DynamicEntity::IsCollision((Rectangle*)element_p))
		{
			if (!((Rectangle*)element_p)->IsUnbreacable())
			{
				((Rectangle*)element_p)->exist = false;
			}
			return true;
		}
	}
	return false;
}

void Bullet::Set(const Bullet* bullet)
{
	angle = bullet->angle;
	angular_velocity = bullet->angular_velocity;
	UpdateDirection();
	exist = bullet->exist;
	force_collision_coeffisient = bullet->force_collision_coeffisient;
	force_resistance_air_coefficient = bullet->force_resistance_air_coefficient;
	is_ignore = bullet->is_ignore;
	min_velocity = bullet->min_velocity;
	host_number = bullet->host_number;
	host_team_number = bullet->host_team_number;
	position = bullet->position;
	radius = bullet->radius;
	velocity = bullet->velocity;
}

void Bullet::Set(Vec2F position, Vec2F velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velosity, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->is_ignore = (is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING;
	this->min_velocity = min_velocity;
	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
	this->position = position;
	this->radius = radius;
	this->velocity = velocity;
}

void Bullet::Set(const Vec2F* position, const Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velosity, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->is_ignore = (is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING;
	this->min_velocity = min_velocity;
	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void Bullet::Update()
{
	DynamicEntity::Update();
	if (velocity.Length() < min_velocity)
	{
		velocity = velocity.Normalize() * min_velocity;
	}
}

void Bullet::operator=(Bullet bullet)
{
	angle = bullet.angle;
	angular_velocity = bullet.angular_velocity;
	direction = bullet.direction;
	exist = bullet.exist;
	force = bullet.force;
	force_collision_coeffisient = bullet.force_collision_coeffisient;
	force_resistance_air_coefficient = bullet.force_resistance_air_coefficient;
	is_ignore = bullet.is_ignore;
	min_velocity = bullet.min_velocity;
	host_number = bullet.host_number;
	host_team_number = bullet.host_team_number;
	position = bullet.position;
	radius = bullet.radius;
	velocity = bullet.velocity;
}

Bullet::~Bullet()
{

}



Knife::Knife() :
	SupportEntity(),
	health(KNIFE_DEFAULT_HEALTH)
{
	Update();
}

Knife::Knife(const Knife& knife) :
	SupportEntity(knife),
	health(knife.health)
{
	Update();
}

Knife::Knife(const ControledEntity* host, const Segment* local_segment, EngineTypes::Knife::knife_health_t health, bool exist) :
	SupportEntity(host, &local_segment->point, local_segment->vector.Length(), local_segment->vector.AbsoluteAngleClockwise(), exist),
	health(health)
{
	Update();
}

bool Knife::Collision(Map* map)
{
	if (health == 0)
	{
		exist = false;
		return false;
	}
	Segment segment = GetSegment();
	bool collision = false;
	void* element_p;
	EngineTypes::Map::array_length_t element;

	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Cyrcle*)element_p)->exist &&
			!((Cyrcle*)element_p)->IsUnbreacable() &&
			((Cyrcle*)element_p)->IsCollision(&segment))
		{
			((Cyrcle*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Polygon*)element_p)->exist &&
			!((Polygon*)element_p)->IsUnbreacable() &&
			((Polygon*)element_p)->IsCollision(&segment))
		{
			((Polygon*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Rectangle*)element_p)->exist &&
			!((Rectangle*)element_p)->IsUnbreacable() &&
			((Rectangle*)element_p)->IsCollision(&segment))
		{
			((Rectangle*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	return collision;
}

Segment Knife::GetSegment() const
{
	return Segment(position, direction * radius);
}

void Knife::Set(const Knife* knife)
{
	angle = knife->angle;
	direction = knife->direction;
	exist = knife->exist;
	health = knife->health;
	host_matrix_p = knife->host_matrix_p;
	host_number = knife->host_number;
	host_p = knife->host_p;
	host_team = knife->host_team;
	last_position = knife->last_position;
	local_angle = knife->local_angle;
	local_direction = knife->local_direction;
	local_position = knife->local_position;
	position = knife->position;
	radius = knife->radius;
}

void Knife::Set(const ControledEntity* host, const Segment* local_segment, EngineTypes::Knife::knife_health_t health, bool exist)
{
	this->exist = exist;
	this->health = health;
	host_matrix_p = host->GetModelMatrixPointer();
	host_p = host;
	host_number = host->GetPlayerNumber();
	host_team = host->GetTeamNumber();
	local_direction = local_segment->vector;
	local_angle = local_direction.AbsoluteAngle();
	local_position = local_segment->point;
	radius = local_segment->vector.Length();
	SupportEntity::Update();
}

bool Knife::LoseHealth()
{
	if (health > 1)
	{
		health--;
		return true;
	}
	if (health == 1)
	{
		health--;
		return false;
	}
	return false;
}

void Knife::operator=(Knife knife)
{
	angle = knife.angle;
	direction = knife.direction;
	exist = knife.exist;
	health = knife.health;
	host_matrix_p = knife.host_matrix_p;
	host_number = knife.host_number;
	host_p = knife.host_p;
	host_team = knife.host_team;
	last_position = knife.last_position;
	local_angle = knife.local_angle;
	local_direction = knife.local_direction;
	local_position = knife.local_position;
	position = knife.position;
	radius = knife.radius;
}

Knife::~Knife()
{

}



Laser::Laser() :
	SupportEntity(),
	properties(false),
	shoot_time(0),
	width(0.0f)
{
}

Laser::Laser(const Laser& laser) :
	SupportEntity(laser),
	properties(laser.properties),
	shoot_time(laser.shoot_time),
	width(laser.width)
{
	Update();
}

Laser::Laser(const ControledEntity* host, const Beam* local_beam, float width, GameTypes::tic_t shoot_time, EngineTypes::Laser::property_t properties, bool exist) :
	SupportEntity(host, &local_beam->point, 0.0f, local_beam->vector.AbsoluteAngle(), exist),
	properties(properties),
	shoot_time(shoot_time),
	width(width)
{
	Update();
}

bool Laser::Collision(Map* map)
{
	Beam beam = GetBeam();
	bool collision = false;
	void* element_p;
	EngineTypes::Map::array_length_t element;

	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Cyrcle*)element_p)->exist &&
			!((Cyrcle*)element_p)->IsUnbreacable() &&
			((Cyrcle*)element_p)->IsCollision(&beam))
		{
			((Cyrcle*)element_p)->exist = false;
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Polygon*)element_p)->exist &&
			!((Polygon*)element_p)->IsUnbreacable() &&
			((Polygon*)element_p)->IsCollision(&beam))
		{
			((Polygon*)element_p)->exist = false;
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Rectangle*)element_p)->exist &&
			!((Rectangle*)element_p)->IsUnbreacable() &&
			((Rectangle*)element_p)->IsCollision(&beam))
		{
			((Rectangle*)element_p)->exist = false;
			collision = true;
		}
	}
	return collision;
}

bool Laser::IsActive() const
{
	return shoot_time > 0;
}

bool Laser::CreatedBy(ControledEntity* controled_entity)
{
	return host_number == controled_entity->GetPlayerNumber();
}

Beam Laser::GetBeam() const
{
	return Beam(position, direction);
}

GameTypes::tic_t Laser::GetLifeTime() const
{
	return shoot_time;
}

GameTypes::players_count_t Laser::GetPlayerMasterNumber() const
{
	return host_number;
}

GameTypes::players_count_t Laser::GetPlayerMasterTeamNumber() const
{
	return host_team;
}

bool Laser::GetProperty(EngineTypes::Laser::property_t property) const
{
	return (properties & property) != LASER_PROPERTY_NOTHING;
}

bool Laser::IsCollision(const Beam* beam) const
{
	return GetBeam().Distance(beam) < width;
}

bool Laser::IsCollision(const Line* line) const
{
	return GetBeam().Distance(line) < width;
}

bool Laser::IsCollision(const Segment* segment) const
{
	return GetBeam().Distance(segment) < width;
}

void Laser::Set(const Laser* laser)
{
	angle = laser->angle;
	direction = laser->direction;
	exist = laser->exist;
	host_matrix_p = laser->host_matrix_p;
	host_number = laser->host_number;
	host_p = laser->host_p;
	host_team = laser->host_team;
	last_position = laser->last_position;
	local_angle = laser->local_angle;
	local_direction = laser->local_direction;
	local_position = laser->local_position;
	position = laser->position;
	properties = laser->properties;
	shoot_time = laser->shoot_time;
	width = laser->width;
}

void Laser::Set(const ControledEntity* host, const Beam* local_beam, float width, GameTypes::tic_t shoot_time, bool can_create_loops, bool exist)
{
	UpdateDirection();
	this->properties = can_create_loops;
	this->exist = exist;
	this->host_number = host_number;
	this->host_team = host_team;
	local_angle = local_beam->vector.AbsoluteAngle();
	local_position = local_beam->point;
	shoot_time = shoot_time;
	this->width = width;
	SupportEntity::Update();
}

void Laser::Update()
{
	if (host_p == nullptr)
	{
		return;
	}
	direction = local_direction.Rotate(angle);
	position = local_position * *host_matrix_p;
	StaticEntity::Update();

	if (shoot_time > 0)
	{
		shoot_time--;
	}
}

void Laser::operator=(Laser laser)
{
	angle = laser.angle;
	direction = laser.direction;
	exist = laser.exist;
	host_matrix_p = laser.host_matrix_p;
	host_number = laser.host_number;
	host_p = laser.host_p;
	host_team = laser.host_team;
	last_position = laser.last_position;
	local_angle = laser.local_angle;
	local_direction = laser.local_direction;
	local_position = laser.local_position;
	position = laser.position;
	properties = laser.properties;
	shoot_time = laser.shoot_time;
	width = laser.width;
}

Laser::~Laser()
{
}
