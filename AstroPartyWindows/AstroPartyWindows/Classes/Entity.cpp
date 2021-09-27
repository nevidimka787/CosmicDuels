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

Entity::Entity(Vec2F* position, float radius, float angle, bool exist) :
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

float Entity::GetAngle()
{
	return angle;
}

float Entity::GetDistance(Beam* beam)
{
	return beam->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(Cyrcle* cyrcle)
{
	Vec2F temp = cyrcle->GetPosition();
	float dist = GetDistance(&temp) - cyrcle->GetRadius();
	if (dist < -2.0f * radius)
	{
		dist = cyrcle->GetRadius() - temp.GetDistance(&position) - 3.0f * radius;
	}
	return dist;
}

float Entity::GetDistance(DecelerationArea* deceler_area)
{
	return deceler_area->GetDistance(position) - radius;
}

float Entity::GetDistance(DynamicEntity* entity)
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(Entity* entity)
{
	return entity->position.GetDistance(&position) - entity->radius - radius, 0.0f;
}

float Entity::GetDistance(Line* line)
{
	return line->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(Segment* segment)
{
	return segment->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(StaticEntity* entity)
{
	return entity->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(Polygon* polygon)
{
	return 0.0;
}

float Entity::GetDistance(Rectangle* rectangle)
{
	Segment temp = rectangle->GetUpSide();
	float dist1 = GetDistance(&temp);
	if (dist1 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetDownSide();
	float dist2 = GetDistance(&temp);
	if (dist2 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetLeftSide();
	float dist3 = GetDistance(&temp);
	if (dist3 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetRightSide();
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

float Entity::GetDistance(Vec2F point)
{
	return point.GetDistance(&position) - radius;
}

float Entity::GetDistance(Vec2F* point)
{
	return point->GetDistance(&position) - radius;
}

Vec2F Entity::GetDirectionNotNormalize()
{
	return direction;
}

float Entity::GetFrameSize(Entity* entity, float scale)
{
	return fmaxf(fabs((entity->position - position).x), fabs((entity->position - position).y * scale)) + radius + entity->radius;
}

Vec2F Entity::GetDirection()
{
	return direction.Normalize();
}

Vec2F Entity::GetPosition()
{
	return position;
}

bool Entity::IsCollision(Beam* beam)
{
	return beam->GetDistance(&position) <= radius;
}

bool Entity::IsCollision(Cyrcle* cyrcle)
{
	if (cyrcle->exist == false)
	{
		return false;
	}
	Vec2F temp = cyrcle->GetPosition();
	return GetDistance(&temp) < radius + cyrcle->GetRadius();
}

bool Entity::IsCollision(DecelerationArea* deceler_area)
{
	return GetDistance(deceler_area) <= 0.0f;
}

bool Entity::IsCollision(DynamicEntity* dynamic_entity)
{
	return GetDistance(&dynamic_entity->position) <= dynamic_entity->radius;
}

bool Entity::IsCollision(Entity* entity)
{
	return GetDistance(&entity->position) <= entity->radius;
}

bool Entity::IsCollision(Line* line)
{
	return line->GetDistance(&position) <= radius;
}

bool Entity::IsCollision(Map* map)
{
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		if (IsCollision(map->GetRectanglePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		if (IsCollision(map->GetCyrclePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		if (IsCollision(map->GetPolygonPointer(i)))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Polygon* polygon)
{
	if (polygon->exist == false)
	{
		return false;
	}
	return false;
}

bool Entity::IsCollision(Rectangle* rectangle)
{
	if (rectangle->exist == false)
	{
		return false;
	}
	Segment side;
	side = rectangle->GetUpSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->GetDownSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->GetRightSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->GetLeftSide();
	if (IsCollision(&side))
	{
		return true;
	}
	return false;
}

bool Entity::IsCollision(Segment* segment)
{
	return segment->GetDistance(&position) <= radius;
}

bool Entity::IsCollision(StaticEntity* static_entity)
{
	return GetDistance(static_entity->GetPosition()) <= static_entity->radius;
}

bool Entity::IsCollision(Vec2F point)
{
	return GetDistance(point) <= 0.0f;
}

bool Entity::IsCollision(Vec2F* point)
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

void Entity::SetPosition(Vec2F* position)
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

DynamicEntity::DynamicEntity(Vec2F* position, Vec2F* velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
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
	float distance = (forced_point - position).GetLength();
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

bool DynamicEntity::Collision(StaticEntity* entity)
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

bool DynamicEntity::Collision(Rectangle* rectangle)
{
	Vec2F nearest_position1;
	float distance1;

	if ((distance1 = rectangle->GetUpSide().GetDistance(&position, &nearest_position1)) < radius)
	{
		Vec2F nearest_position2;
		Vec2F collision_direction;
		float distance2;

		if ((distance2 = rectangle->GetRightSide().GetDistance(&position, &nearest_position2)) < radius)
		{
			collision_direction = nearest_position2 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance2 * radius);

			collision_direction = nearest_position1 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
			return true;
		}

		if ((distance2 = rectangle->GetLeftSide().GetDistance(&position, &nearest_position2)) < radius)
		{
			collision_direction = nearest_position2 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance2 * radius);

			collision_direction = nearest_position1 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
			return true;
		}

		collision_direction = nearest_position1 - position;
		velocity -= collision_direction.Project(&velocity);
		force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
		return true;
	}

	if ((distance1 = rectangle->GetDownSide().GetDistance(&position, &nearest_position1)) < radius)
	{
		Vec2F nearest_position2;
		Vec2F collision_direction;
		float distance2;

		if ((distance2 = rectangle->GetRightSide().GetDistance(&position, &nearest_position2)) < radius)
		{
			collision_direction = nearest_position2 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance2 * radius);

			collision_direction = nearest_position1 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
			return true;
		}

		if ((distance2 = rectangle->GetLeftSide().GetDistance(&position, &nearest_position2)) < radius)
		{
			collision_direction = nearest_position2 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance2 * radius);

			collision_direction = nearest_position1 - position;
			velocity -= collision_direction.Project(&velocity);
			force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
			return true;
		}

		collision_direction = nearest_position1 - position;
		velocity -= collision_direction.Project(&velocity);
		force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
		return true;
	}

	if ((distance1 = rectangle->GetRightSide().GetDistance(&position, &nearest_position1)) < radius)
	{
		Vec2F collision_direction = nearest_position1 - position;
		velocity -= collision_direction.Project(&velocity);
		force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
		return true;
	}

	if ((distance1 = rectangle->GetLeftSide().GetDistance(&position, &nearest_position1)) < radius)
	{
		Vec2F collision_direction = nearest_position1 - position;
		velocity -= collision_direction.Project(&velocity);
		force -= collision_direction * (force_collision_coeffisient / distance1 * radius);
		return true;
	}
	return false;
}

bool DynamicEntity::Collision(Cyrcle* cyrcle)
{
	float distance;
	if ((distance = GetDistance(cyrcle)) > 0.0f)
	{
		return false;
	}

	Vec2F collision_direction = cyrcle->GetPosition() - position;
	velocity -= collision_direction.Project(&velocity);
	force -= collision_direction * (force_collision_coeffisient / (distance + radius) * radius) / 10.0f;
	return true;
}

bool DynamicEntity::Collision(Polygon* polygon)
{
	return false;
}

bool DynamicEntity::Collision(Map* map)
{
	bool collision = false;
	void* map_element;
	for (EngineTypes::Map::elements_array_length_t i = 0; i < map->cyrcles_array_length; i++)
	{
		map_element = (void*)map->GetCyrclePointer(i);
		if (((Cyrcle*)map_element)->exist)
		{
			collision |= Collision((Cyrcle*)map_element);
		}
	}
	for (EngineTypes::Map::elements_array_length_t i = 0; i < map->polygons_array_length; i++)
	{
		map_element = (void*)map->GetPolygonPointer(i);
		if (((Polygon*)map_element)->exist)
		{
			collision |= Collision((Polygon*)map_element);
		}
	}
	for (EngineTypes::Map::elements_array_length_t i = 0; i < map->rectangles_array_length; i++)
	{
		map_element = (void*)map->GetRectanglePointer(i);
		if (((Rectangle*)map_element)->exist)
		{
			collision |= Collision((Rectangle*)map_element);
		}
	}
	return collision;
}

float DynamicEntity::GetAngularVelocity()
{
	return angular_velocity;
}

Segment DynamicEntity::GetLastTreck()
{
	return Segment(position - velocity, position, true);
}

Segment DynamicEntity::GetTreck()
{
	return Segment(&position, &velocity);
}

Vec2F DynamicEntity::GetVelocity()
{
	return velocity;
}

bool DynamicEntity::IsCollision(Vec2F point)
{
	return Segment(position, -velocity).GetDistance(point) < radius;
}

bool DynamicEntity::IsCollision(Vec2F* point)
{
	return Segment(position, -velocity).GetDistance(point) < radius;
}

bool DynamicEntity::IsCollision(Line line)
{
	return Segment(position, -velocity).GetDistance(&line) < radius;
}

bool DynamicEntity::IsCollision(Line* line)
{
	return Segment(position, -velocity).GetDistance(line) < radius;
}

bool DynamicEntity::IsCollision(Beam beam)
{
	return Segment(position, -velocity).GetDistance(&beam) < radius;
}

bool DynamicEntity::IsCollision(Beam* beam)
{
	return Segment(position, -velocity).GetDistance(beam) < radius;
}

bool DynamicEntity::IsCollision(Segment segment)
{
	return Segment(position, -velocity).GetDistance(&segment) < radius;
}

bool DynamicEntity::IsCollision(Segment* segment)
{
	return Segment(position, -velocity).GetDistance(segment) < radius;
}

bool DynamicEntity::IsCollision(DynamicEntity* entity)
{
	return Segment(position, -velocity).GetDistance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(StaticEntity* entity)
{
	return Segment(position, -velocity).GetDistance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(Rectangle* rectangle)
{
	Segment side = rectangle->GetUpSide();
	if (Segment(position, -velocity).GetDistance(&side) < radius)
	{
		return true;
	}
	side = rectangle->GetLeftSide();
	if (Segment(position, -velocity).GetDistance(&side) < radius)
	{	
		return true;
	}
	side = rectangle->GetDownSide();
	if (Segment(position, -velocity).GetDistance(&side) < radius)
	{
		return true;
	}
	side = rectangle->GetRightSide();
	if (Segment(position, -velocity).GetDistance(&side) < radius)
	{
		return true;
	}
	return false;
}

bool DynamicEntity::IsCollision(Cyrcle* cyrcle)
{
	return Segment(position, -velocity).GetDistance(cyrcle->GetPosition()) < radius + cyrcle->GetRadius();
}

bool DynamicEntity::IsCollision(Polygon* polygon)
{
	return false;
}

bool DynamicEntity::IsCollision(Map* map)
{
	void* element;
	if (map->cyrcles_array_length > 0)
	{
		for (EngineTypes::Map::elements_array_length_t cyrcle = 0, found_cyrcle = 0; found_cyrcle < map->cyrcles_array_length; cyrcle++)
		{
			if (((Cyrcle*)(element = (void*)map->GetCyrclePointer(cyrcle)))->exist)
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
		for (EngineTypes::Map::elements_array_length_t polygon = 0, found_polygon = 0; found_polygon < map->polygons_array_length; polygon++)
		{
			if (((Polygon*)(element = (void*)map->GetPolygonPointer(polygon)))->exist)
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
		for (EngineTypes::Map::elements_array_length_t rectangle = 0, found_rectangle = 0; found_rectangle < map->rectangles_array_length; rectangle++)
		{
			if (((Rectangle*)(element = (void*)map->GetRectanglePointer(rectangle)))->exist)
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

	bool not_nan = true;
	if (velocity.x != velocity.x)
	{
		int i = 0;
	}
	if (force.x != force.x)
	{
		int i = 0;
	}
	velocity += force;
	velocity *= 1.0f - force_resistance_air_coefficient;
	force.Set(0.0f, 0.0f);
	if (not_nan && velocity.x != velocity.x)
	{
		int i = 0;
	}
	Move(&velocity);
}

void DynamicEntity::Set(DynamicEntity* dynamic_entity)
{
	angle = dynamic_entity->angle;
	angular_velocity = dynamic_entity->angular_velocity;
	direction = dynamic_entity->direction;
	exist = dynamic_entity->exist;
	force = dynamic_entity->force;
	force_collision_coeffisient = dynamic_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = dynamic_entity->force_resistance_air_coefficient;
	position = dynamic_entity->position;
	radius = dynamic_entity->radius;
	velocity = dynamic_entity->velocity;
}

void DynamicEntity::Set(Vec2F* position, Vec2F* velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->force_resistance_air_coefficient = angular_velocity;
	this->exist = exist;
	this->force.Set(0.0f, 0.0f);
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

void DynamicEntity::SetVelocity(Vec2F* velocity)
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

StaticEntity::StaticEntity(Vec2F* position, float radius, float angle, bool exist) :
	Entity(position, radius, angle, exist),
	last_position(*position)
{
}

Vec2F StaticEntity::GetVelocity()
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

void StaticEntity::Set(Vec2F* position, float radius, float angle, bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
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
	DynamicEntity(),
	life_time(0)
{

}

Particle::~Particle()
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

Bonus::Bonus(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	bonus_inventory(bonus_inventory)
{
}

EngineTypes::Bonus::inventory_t Bonus::BonusInfo()
{
	return bonus_inventory;
}

Bonus Bonus::Division()
{
	EngineTypes::Bonus::inventory_t temp_bonus_inventory;
	for (uint8_t i = 0; i < BONUS_BONUSES_COUNT; i++)
	{
		temp_bonus_inventory = 0x03 << (i * 2);
		if (bonus_inventory & temp_bonus_inventory)
		{
			temp_bonus_inventory &= bonus_inventory;
			bonus_inventory &= BONUS_ALL - temp_bonus_inventory;
			if (bonus_inventory & (1u << (BONUS_BONUSES_COUNT * 2)) - 1)
			{
				Vec2F new_position = position + direction.Rotate((float)rand() / (float)RAND_MAX * (float)M_PI * 2.0f) * radius;
				return Bonus(&new_position, &velocity, temp_bonus_inventory);
			}
			return Bonus(false);
		}
	}
	return Bonus(false);
}

uint16_t Bonus::GetType()
{
	return bonus_inventory;
}

uint8_t Bonus::GetBonusesCount()
{
	uint8_t count = 0;
	for (uint8_t i = 0; i < BONUS_BONUSES_COUNT; i++)
	{
		if (bonus_inventory & (0x11 << (i << 1)))
		{
			count++;
		}
	}
	return count;
}

uint8_t Bonus::GetBuffsCount()
{
	uint8_t count = 0;
	for (uint8_t i = BONUS_BONUSES_COUNT; i < BONUS_BONUSES_COUNT + BONUS_BUFFS_COUNT; i++)
	{
		if (bonus_inventory & (0x11 << (i << 1)))
		{
			count++;
		}
	}
	return count;
}

uint8_t Bonus::GetGameRulesCount()
{
	uint8_t count = 0;
	for (uint8_t i = BONUS_BONUSES_COUNT + BONUS_BUFFS_COUNT; i < BONUS_BONUSES_COUNT + BONUS_BUFFS_COUNT + BONUS_GAME_RULES_COUNT; i++)
	{
		if (bonus_inventory & (0x11 << (i << 1)))
		{
			count++;
		}
	}
	return count;
}

uint8_t Bonus::GetTypesCount()
{
	uint8_t count = 0;
	for (uint8_t i = 0; i < BONUS_TYPES_COUNT; i++)
	{
		if (bonus_inventory & (0x11 << (i << 1)))
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
	force = bonus->force;
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
	this->force.Set(0.0f, 0.0f);
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
	force = bonus.force;
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

Asteroid::Asteroid(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Asteroid::size_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Bonus(position, velocity, bonus_inventory, 0.0f, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	size(size)
{
	UpdateRadius();
	this->bonus_inventory = bonus_inventory;
	exist = true;
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
	if (bonus_inventory & (BONUS_BONUS | BONUS_RULE_REVERSE))
	{
		return Bonus(&position, &velocity, bonus_inventory & (BONUS_BONUS | BONUS_RULE_REVERSE));
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
	EngineTypes::Bonus::inventory_t temp_bonus;
	for (uint8_t i = 0; i < BONUS_BONUSES_COUNT; i++)
	{
		temp_bonus = 0x11 << (i * 2);
		if (bonus_inventory & temp_bonus)
		{
			temp_bonus &= bonus_inventory;
			bonus_inventory &= BONUS_ALL - temp_bonus;
			return_bonus = temp_bonus;
			goto end_of_cycle;
		}
	}
end_of_cycle:;

	Vec2F asteroid_velocity = direction.Rotate(((float)rand() / (float)RAND_MAX) * (float)M_PI * 2.0f) * ASTEROID_DEEFAULT_VELOCITY;
	Vec2F asteroid_position = position + asteroid_velocity.Normalize() * radius;
	
	return Asteroid(&asteroid_position, &asteroid_velocity, return_bonus, size - 1);
}

EngineTypes::Asteroid::size_t Asteroid::GetSize()
{
	return size;
}

EngineTypes::Bonus::inventory_t Asteroid::GetBuffBonus()
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
	player_master_number(0),
	player_master_team_number(0)
{
}

KillerEntity::KillerEntity(const KillerEntity& killer_entity) :
	DynamicEntity(killer_entity),
	player_master_number(killer_entity.player_master_number),
	player_master_team_number(killer_entity.player_master_team_number)
{
}

KillerEntity::KillerEntity(Vec2F* position, Vec2F* velocity, float radius, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	player_master_number(player_master_number),
	player_master_team_number(player_master_team_number)
{
}

bool KillerEntity::CreatedBy(ControledEntity* controled_entity)
{
	return player_master_number == controled_entity->GetPlayerNumber();
}

bool KillerEntity::CreatedByTeam(ControledEntity* controled_entity)
{
	return player_master_team_number == controled_entity->GetTeamNumber();
}

GameTypes::players_count_t KillerEntity::GetPlayerMasterNumber()
{
	return player_master_number;
}

GameTypes::players_count_t KillerEntity::GetPlayerMasterTeamNumber()
{
	return player_master_team_number;
}

bool KillerEntity::SameTeam(KillerEntity* killer_entity)
{
	return player_master_team_number == killer_entity->player_master_team_number;
}

void KillerEntity::Set(KillerEntity* killer_entity)
{
	angle = killer_entity->angle;
	angular_velocity = killer_entity->angular_velocity;
	direction = killer_entity->direction;
	exist = killer_entity->exist;
	force = killer_entity->force;
	force_collision_coeffisient = killer_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = killer_entity->force_resistance_air_coefficient;
	player_master_number = killer_entity->player_master_number;
	player_master_team_number = killer_entity->player_master_team_number;
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
	this->force.Set(0.0f, 0.0f);
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
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
	player_master_number = killer_entity.player_master_number;
	player_master_team_number = killer_entity.player_master_team_number;
	position = killer_entity.position;
	radius = killer_entity.radius;
	velocity = killer_entity.velocity;
}

KillerEntity::~KillerEntity()
{
}



ControledEntity::ControledEntity() :
	DynamicEntity(),
	player_number(0),
	rotate_input_value_pointer(nullptr),
	shoot_input_value_pointer(nullptr),
	player_team_number(0)
{
	heat_box_vertexes_array_length = 3;
	heat_box_vertexes_array = new Vec2F[3];
	heat_box_vertexes_array[0].Set(0.5f, 0.0f);
	heat_box_vertexes_array[1].Set(-0.25f, sqrt(3.0f) / 4.0f);
	heat_box_vertexes_array[2].Set(-0.25f, sqrt(3.0f) / -4.0f);
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

ControledEntity::ControledEntity(Vec2F* position, Vec2F* velocity, float radius, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	player_number(player_number),
	player_team_number(player_team_number),
	rotate_input_value_pointer(rotate_input_value_pointer),
	shoot_input_value_pointer(shoot_input_value_pointer)
{
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array_length = 3;
		this->heat_box_vertexes_array = new Vec2F[3];
		this->heat_box_vertexes_array[0].Set(0.5f, 0.0f);
		this->heat_box_vertexes_array[1].Set(-0.25f, sqrt(3.0f) / 4.0f);
		this->heat_box_vertexes_array[2].Set(-0.25f, sqrt(3.0f) / -4.0f);
		return;
	}

	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];

	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

Mat3x2F ControledEntity::GetModelMatrix()
{
	return model_matrix;
}

Mat3x2F* ControledEntity::GetModelMatrixPointer()
{
	return &model_matrix;
}

GameTypes::players_count_t ControledEntity::GetPlayerNumber()
{
	return player_number;
}

GameTypes::players_count_t ControledEntity::GetTeamNumber()
{
	return player_team_number;
}

bool ControledEntity::GetRotateInputValue()
{
	return *(bool*)rotate_input_value_pointer;
}

bool ControledEntity::GetShootInputValue()
{
	return *(bool*)shoot_input_value_pointer;
}

bool ControledEntity::IsCollision(Beam* beam)
{
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (beam->GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (beam->GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(Bomb* bomb)
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
	if (segment.GetDistance(&ce_side) < bomb->radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.GetDistance(&ce_side) < bomb->radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(Bullet* bullet)
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
	if (segment.GetDistance(&ce_side) < bullet->radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.GetDistance(&ce_side) < bullet->radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(Knife* knife)
{
	//Frame of reference set to this entity.
	Segment segment = knife->GetSegment();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2);
	if (segment.GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2);
		if (segment.GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(Laser* laser)
{
	//Frame of reference set to this entity.
	Beam beam = laser->GetBeam();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (beam.GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (beam.GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(MegaLaser* mega_laser)
{
	//Frame of reference set to this entity.
	Segment segment = mega_laser->GetSegment();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (segment.GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment.GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsCollision(Segment* segment)
{
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2, true);
	if (segment->GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2, true);
		if (segment->GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::IsColectEntity(Entity* stored_entity)
{
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	Beam test_beam;
	test_beam.Set(stored_entity->GetPosition(), Vec2F(1.0f, 0.0f));
	EngineTypes::ControledEntity::heat_box_vertexes_count_t intersections_count = 0;

	ce_side.Set(point1, point2, true);
	if (stored_entity->GetDistance(&ce_side) < radius)
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
		if (stored_entity->GetDistance(&ce_side) < radius)
		{
			return true;
		}
		else if (test_beam.IsIntersection(&ce_side))
		{
			intersections_count++;
		}
	}
	return intersections_count % 2;

}

bool ControledEntity::SameTeams(ControledEntity* second_entity)
{
	return player_team_number == second_entity->player_team_number;
}

void ControledEntity::Set(ControledEntity* controled_entity)
{
	angle = controled_entity->angle;
	angular_velocity = controled_entity->angular_velocity;
	direction = controled_entity->direction;
	exist = controled_entity->exist;
	force = controled_entity->force;
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
	this->force.Set(0.0f, 0.0f);
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
	unbrakable(0)
{
}

Ship::Ship(const Ship& ship) :
	ControledEntity(ship),
	bonus_inventory(ship.bonus_inventory),
	buff_inventory(ship.buff_inventory),
	burnout(ship.burnout),
	burnout_input_value_pointer(ship.burnout_input_value_pointer),
	unbrakable(ship.unbrakable)
{
}

Ship::Ship(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* burnout_input_value_pointer, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Ship::inventory_t buff_inventory, GameTypes::tic_t unbrakable, GameTypes::tic_t burnout, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	bonus_inventory(bonus_inventory),
	buff_inventory(buff_inventory),
	burnout(burnout),
	burnout_input_value_pointer(burnout_input_value_pointer),
	unbrakable(unbrakable)
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

EngineTypes::Bonus::inventory_t Ship::BonusInfo()
{
	return bonus_inventory;
}

void Ship::Burnout(float power, bool rotate_clockwise, GameTypes::tic_t burnout_period)
{
	if (rotate_clockwise)
	{
		force -= direction.Perpendicular().Normalize() * power;
		angle += (float)M_PI / 2.0f;
	}
	else
	{
		force += direction.Perpendicular().Normalize() * power;
		angle += -(float)M_PI / 2.0f;
	}
	UpdateDirection();
	burnout = burnout_period;
}

Bullet Ship::CreateBullet()
{
	Vec2F bullet_position = position +  direction.Normalize() * radius;
	Vec2F bullet_velosity = direction * BULLET_DEFAULT_VELOCITY + velocity - direction.Perpendicular() * angular_velocity * radius;
	AddForceAlongDirection(-SHIP_SHOOT_FORCE);
	return Bullet(&bullet_position, &bullet_velosity, player_number, player_team_number);
}

Bullet Ship::CreateTriple(uint8_t bullet_number)
{
	Vec2F bullet_velocity, bullet_position;
	switch (bullet_number)
	{
	case 0:
		bullet_position = position + direction * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	case 1:
		bullet_position = position - direction.Perpendicular() * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	case 2:
		bullet_position = position + direction.Perpendicular() * radius;
		bullet_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&bullet_position, &bullet_velocity, player_number, player_team_number);
	default:
		return Bullet();
	}
}

Bullet Ship::CreateLoop(GameTypes::entities_count_t bullet_number)
{ 
	float loc_angle = 2.0f * (float)M_PI / (float)SHIP_BULLETS_IN_LOOP * (float)bullet_number;
	Vec2F bullet_velocity = direction.Rotate(loc_angle);
	Vec2F bullet_position = position + bullet_velocity * (radius + BULLET_DEFAULT_RADIUS);
	bullet_velocity = bullet_velocity * BULLET_DEFAULT_VELOCITY + velocity;
	return Bullet(&bullet_position, &bullet_velocity, 
		player_number, player_team_number, true,
		0.0f, 0.0f, 
		DEFAULT_FORCE_COLLISION_COEFFICIENT, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT / 2.0f, 
		BULLET_DEFAULT_RADIUS / 2.0f);
}

Bomb Ship::CreateBomb()
{
	Vec2F bomb_position = Vec2F(-0.25f, 0.0f) * model_matrix;
	return Bomb(&bomb_position, &velocity, player_team_number, player_team_number);
}

Laser Ship::CreateLaser()
{
	Beam laser_beam = Beam(Vec2F(0.5f, 0.0f), Vec2F(1.0f, 0.0f));
	AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	return Laser(this, &laser_beam);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Segment new_knife_segment;
	switch (knife_number)
	{
	case 0:
		new_knife_segment = Segment(Vec2F(-0.25f, sqrt(3.0f) / 4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	case 1:
		new_knife_segment = Segment(Vec2F(-0.25f, sqrt(3.0f) / -4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	default:
		return Knife();
	}
}

Pilot Ship::Destroy()
{
	Vec2F pilot_new_velocity = velocity * 1.5f;
	return Pilot(&position, &pilot_new_velocity, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, PILOT_DEFAULT_RESPAWN_TIMER, nullptr, 0, angle);
}

bool Ship::HaveBonus(EngineTypes::Bonus::inventory_t bonus)
{
	return bonus_inventory & (bonus * 3);
}

bool Ship::HaveBuff(EngineTypes::Ship::inventory_t buff)
{
	return buff_inventory & buff;
}

bool Ship::IsUnbrakable()
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

void Ship::Set(Ship* ship)
{
	angle = ship->angle;
	angular_velocity = ship->angular_velocity;
	bonus_inventory = ship->bonus_inventory;
	buff_inventory = ship->buff_inventory;
	burnout = ship->burnout;
	burnout_input_value_pointer = ship->burnout_input_value_pointer;
	direction = ship->direction;
	exist = ship->exist;
	force = ship->force;
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

void Ship::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* burnout_input_value_pointer, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::inventory_t bonus_inventory, EngineTypes::Ship::inventory_t buff_inventory, GameTypes::tic_t unbrakable, GameTypes::tic_t burnout, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->burnout = burnout;
	UpdateDirection();
	this->exist = exist;
	this->force.Set(0.0f, 0.0f);
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
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
		this->heat_box_vertexes_array_length = 3;
		this->heat_box_vertexes_array = new Vec2F[3];
		this->heat_box_vertexes_array[0].Set(0.5f, 0.0f);
		this->heat_box_vertexes_array[1].Set(-0.25f, sqrt(3.0f) / 4.0f);
		this->heat_box_vertexes_array[2].Set(-0.25f, sqrt(3.0f) / -4.0f);
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

void Ship::SetUnbrakablePeriod(GameTypes::tic_t period)
{
	unbrakable = period;
}

bool Ship::SpendBonus(EngineTypes::Bonus::inventory_t bonus)
{
	if (HaveBonus(bonus))
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
	if (HaveBuff(buff))
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

void Ship::TakeBonus(Bonus* bonus, bool as_triple)
{
	EngineTypes::Bonus::inventory_t new_inventory = bonus->bonus_inventory;
	if (as_triple)
	{
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_BONUS_DATA_LENGTH * 2; i += 2)
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
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_BONUS_DATA_LENGTH * 2; i += 2)
		{
			count = (new_inventory & (BONUS_CELL << i)) >> i;
			if (count)
			{
				count += (bonus_inventory & (BONUS_CELL << i)) >> i;
				if (count > 3)
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
	direction = ship.direction;
	exist = ship.exist;
	force = ship.force;
	force_collision_coeffisient = ship.force_collision_coeffisient;
	force_resistance_air_coefficient = ship.force_resistance_air_coefficient;
	heat_box_vertexes_array_length = ship.heat_box_vertexes_array_length;
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

Pilot::Pilot(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, GameTypes::tic_t respawn_timer,  Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::inventory_t buffs_bonuses, EngineTypes::Bonus::inventory_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	respawn_timer(respawn_timer)
{
}

bool Pilot::CanRespawn()
{
	return respawn_timer == 0;
}

GameTypes::tic_t Pilot::GetRespawnDellay()
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
	force_collision_coeffisient = pilot->force_collision_coeffisient;
	force_resistance_air_coefficient = pilot->force_resistance_air_coefficient;
	heat_box_vertexes_array_length = pilot->heat_box_vertexes_array_length;
	player_number = pilot->player_number;
	player_team_number = pilot->player_team_number;
	position = pilot->position;
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
	force_collision_coeffisient = pilot.force_collision_coeffisient;
	force_resistance_air_coefficient = pilot.force_resistance_air_coefficient;
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
}

SupportEntity::SupportEntity(ControledEntity* host, Vec2F* position, float radius, float angle, bool exist) :
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

bool SupportEntity::CreatedBy(ControledEntity* potential_host)
{
	return host_number == potential_host->GetPlayerNumber();
}

bool SupportEntity::CreatedByTeam(ControledEntity* potential_host)
{
	return host_team == potential_host->GetTeamNumber();
}

float SupportEntity::GetAngle()
{
	return local_angle;
}

Vec2F SupportEntity::GetDirection()
{
	return local_direction;
}

Vec2F SupportEntity::GetNormalizeDirection()
{
	return local_direction.Normalize();
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterNumber()
{
	return host_number;
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterTeamNumber()
{
	return host_team;
}

Vec2F SupportEntity::GetPosition()
{
	return local_position;
}

void SupportEntity::Set(SupportEntity* support_entity)
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

void SupportEntity::Set(ControledEntity* host, Vec2F position, float radius, float angle, bool exist)
{
	if (host_p == nullptr)
	{
		return;
	}
	angle = host->GetAngle();
	direction = host->GetDirectionNotNormalize();
	this->exist = exist;
	host_number = host->GetPlayerNumber();
	host_team = host->GetTeamNumber();
	local_angle = angle;
	local_direction = Vec2F(1.0f, 0.0f).Rotate(angle);
	local_position = position;
	position = host->GetPosition();
	this->radius = radius;

	host_p = host;
	host_matrix_p = host->GetModelMatrixPointer();
}

void SupportEntity::SetAngle(float angle)
{
	local_angle = angle;
}

void SupportEntity::SetDirection(Vec2F direction)
{
	local_direction = direction.Normalize();
}

void SupportEntity::SetDirection(Vec2F* direction)
{
	local_direction = direction->Normalize();
}

void SupportEntity::SetNotNormalizeDirection(Vec2F direction)
{
	local_direction = direction;
}

void SupportEntity::SetNotNormalizeDirection(Vec2F* direction)
{
	local_direction = *direction;
}

void SupportEntity::SetHost(ControledEntity* host)
{
	host_p = host;
	host_matrix_p = host->GetModelMatrixPointer();
}

void SupportEntity::SetPosition(Vec2F position)
{
	local_position = position;
}

void SupportEntity::SetPosition(Vec2F* position)
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
	local_direction = Vec2F(direction.GetLength(), 0.0f).Rotate(local_angle);
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

AggressiveEntity::AggressiveEntity(Vec2F* position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist) :
	StaticEntity(position, radius, angle, exist),
	attack_dellay(attack_dellay),
	attack_period(attack_period),
	inactive_period(inactive_period),
	shoots_count(shoots_count)
{
}

bool AggressiveEntity::CanShoot(GameTypes::tic_t current_tic)
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

void AggressiveEntity::Set(AggressiveEntity* aggressive_entity)
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

void AggressiveEntity::Set(Vec2F* position, float radius, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool exist)
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

Turel::Turel(Vec2F* position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist) :
	AggressiveEntity(position, radius, angle, attack_dellay, attack_period, inactive_period, shoots_count, exist)
{
}

Bullet Turel::Shoot()
{
	Vec2F new_bullet_velocity = direction * BULLET_DEFAULT_VELOCITY;
	Vec2F new_bullet_position = position + direction * radius * 2.0f;
	return Bullet(&new_bullet_position, &new_bullet_velocity, AGGRESIVE_ENTITY_HOST_ID, AGGRESIVE_ENTITY_HOST_ID, false);
}

void Turel::Set(Turel* turel)
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

void Turel::Set(Vec2F* position, float angle, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, float radius, bool exist)
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

DecelerationArea::DecelerationArea(Vec2F* position, float deceleration_parameter, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	deceleration_parameter(deceleration_parameter)
{
}

void DecelerationArea::Set(DecelerationArea* deceleration_area)
{
	angle = deceleration_area->angle;
	deceleration_parameter = deceleration_area->deceleration_parameter;
	exist = deceleration_area->exist;
	last_position = deceleration_area->last_position;
	position = deceleration_area->position;
	radius = deceleration_area->radius;
}

void DecelerationArea::Set(Vec2F* position, float deceleration_parameter, float radius, float angle, bool exist)
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

GravGen::GravGen(Vec2F* position, float gravity, float radius, float angle, bool exist) :
	StaticEntity(position, radius, angle, exist),
	gravity(gravity)
{
}

void GravGen::Set(GravGen* grav_gen)
{
	angle = grav_gen->angle;
	direction = grav_gen->direction;
	exist = grav_gen->exist;
	gravity = grav_gen->gravity;
	last_position = grav_gen->last_position;
	position = grav_gen->position;
}

void GravGen::Set(Vec2F* position, float gravity, float radius, float angle, bool exist)
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



MegaLaser::MegaLaser() :
	AggressiveEntity(),
	active(false)
{
}

MegaLaser::MegaLaser(const MegaLaser& mega_laser) :
	AggressiveEntity(mega_laser),
	active(mega_laser.active)
{
}

MegaLaser::MegaLaser(Segment* segment, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool active, bool exist) :
	AggressiveEntity(&segment->point, segment->vector.GetLength(), segment->vector.GetAbsoluteAngle(), attack_dellay, attack_period, inactive_period, shoots_count, exist),
	active(active)
{
}

Segment MegaLaser::GetSegment()
{
	return Segment(position, direction);
}

void MegaLaser::Rotate(float angle)
{
	direction.RotateThis(angle);
	this->angle += angle;
}

void MegaLaser::Set(MegaLaser* mega_laser)
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

void MegaLaser::Set(Segment* segment, GameTypes::tic_t attack_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t inactive_period, EngineTypes::AgressiveEntity::shoots_count_t shoots_count, bool active, bool exist)
{
	this->active = active;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	this->direction = segment->vector.Normalize();
	UpdateAngle();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->position = segment->point;
	this->radius = segment->vector.GetLength();
	this->shoots_count = shoots_count;

	direction *= radius;
}

bool MegaLaser::IsShooting()
{
	return active;
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
}

MegaLaser::~MegaLaser()
{
}



Laser::Laser() :
	SupportEntity(),
	shoot_time(0)
{
}

Laser::Laser(const Laser& laser) :
	SupportEntity(laser),
	shoot_time(laser.shoot_time)
{
}

Laser::Laser(ControledEntity* host, Beam* local_beam, GameTypes::tic_t shoot_time, bool exist) :
	SupportEntity(host, &local_beam->point, 0.0f, local_beam->vector.GetAbsoluteAngle(), exist),
	shoot_time(shoot_time)
{
}

bool Laser::CanShoot()
{
	return shoot_time > 0;
}

bool Laser::CreatedBy(ControledEntity* controled_entity)
{
	return host_number == controled_entity->GetPlayerNumber();
}

Beam Laser::GetBeam()
{
	return Beam(position, direction);
}

GameTypes::tic_t Laser::GetLifeTime()
{
	return shoot_time;
}

GameTypes::players_count_t Laser::GetPlayerMasterNumber()
{
	return host_number;
}

GameTypes::players_count_t Laser::GetPlayerMasterTeamNumber()
{
	return host_team;
}

void Laser::Set(Laser* laser)
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
	shoot_time = laser->shoot_time;
}

void Laser::Set(ControledEntity* host, Beam* local_beam, GameTypes::tic_t shoot_time, bool exist)
{
	UpdateDirection();
	this->exist = exist;
	this->host_number = host_number;
	this->host_team = host_team;
	local_angle = local_beam->vector.GetAbsoluteAngle();
	local_position = local_beam->point;
	shoot_time = shoot_time;
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
	shoot_time = laser.shoot_time;
}

Laser::~Laser()
{
}



Bullet::Bullet() :
	KillerEntity(),
	is_collision(BULLET_MUSTER_ONLY | BULLET_MUSTER_KNIFES),
	min_velocity(BULLET_DEFAULT_MIN_VELOCITY)
{
}

Bullet::Bullet(const Bullet& bullet) :
	KillerEntity(bullet),
	is_collision(bullet.is_collision),
	min_velocity(bullet.min_velocity)
{
}

Bullet::Bullet(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velocity, bool exist) :
	KillerEntity(position, velocity, radius, player_master_number, player_master_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	is_collision((is_collision_master) ? BULLET_MUSTER_ONLY | BULLET_MUSTER_KNIFES : BULLET_MUSTER_NOTHING),
	min_velocity(min_velocity)
{
}

void Bullet::Set(Bullet* bullet)
{
	angle = bullet->angle;
	angular_velocity = bullet->angular_velocity;
	UpdateDirection();
	exist = bullet->exist;
	force = bullet->force;
	force_collision_coeffisient = bullet->force_collision_coeffisient;
	force_resistance_air_coefficient = bullet->force_resistance_air_coefficient;
	is_collision = bullet->is_collision;
	min_velocity = bullet->min_velocity;
	player_master_number = bullet->player_master_number;
	player_master_team_number = bullet->player_master_team_number;
	position = bullet->position;
	radius = bullet->radius;
	velocity = bullet->velocity;
}

void Bullet::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, bool is_collision_master, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, float min_velosity, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force.Set(0.0f, 0.0f);
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->is_collision = is_collision_master;
	this->min_velocity = min_velocity;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void Bullet::Update()
{
	DynamicEntity::Update();
	if (velocity.GetLength() < min_velocity)
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
	is_collision = bullet.is_collision;
	min_velocity = bullet.min_velocity;
	player_master_number = bullet.player_master_number;
	player_master_team_number = bullet.player_master_team_number;
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
}

Knife::Knife(const Knife& knife) :
	SupportEntity(knife),
	health(knife.health)
{
}

Knife::Knife(ControledEntity* host, Segment* local_segment, EngineTypes::Knife::knife_health_t health, bool exist) :
	SupportEntity(host, &local_segment->point, local_segment->vector.GetLength(), local_segment->vector.GetAbsoluteAngle(), exist),
	health(health)
{
}

Segment Knife::GetSegment()
{
	return Segment(position, direction * radius);
}

void Knife::Set(Knife* knife)
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

void Knife::Set(ControledEntity* host, Segment* local_segment, EngineTypes::Knife::knife_health_t health, bool exist)
{
	this->exist = exist;
	this->health = health;
	host_matrix_p = host->GetModelMatrixPointer();
	host_p = host;
	host_number = host->GetPlayerNumber();
	host_team = host->GetTeamNumber();
	local_direction = local_segment->vector;
	local_angle = local_direction.GetAbsoluteAngle();
	local_position = local_segment->point;
	radius = local_segment->vector.GetLength();
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



Bomb::Bomb() :
	KillerEntity(),
	animation_tic(BOMB_DEFAULT_BOOM_DELLAY),
	status(BOMB_INACTIVE)
{
}

Bomb::Bomb(const Bomb& bomb) :
	KillerEntity(bomb),
	animation_tic(bomb.animation_tic),
	status(bomb.status)
{
}

Bomb::Bomb(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t master1_team_number, GameTypes::players_count_t master2_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius,EngineTypes::Bomb::status_t status, bool exist) :
	KillerEntity(position, velocity, radius, master1_team_number, master2_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	animation_tic(animation_tic), 
	status(status)
{
}

void Bomb::Activate()
{
	status |= BOMB_ACTIVE;
}

void Bomb::Boom()
{
	animation_tic = BOMB_BOOM_TIME;
	status &= BOMB_ALL - BOMB_ACTIVE;
	radius = BOMB_BOOM_RADIUS;
	status |= BOMB_BOOM;
}

bool Bomb::CanRemove()
{
	return status & BOMB_CAN_REMOVE;
}

GameTypes::tic_t Bomb::GetAnimationTic()
{
	return animation_tic;
}

bool Bomb::IsActive()
{
	return status & BOMB_ACTIVE;
}

bool Bomb::IsBoom()
{
	return status & BOMB_BOOM;
}

bool Bomb::CreatedByAggressiveTeam()
{
	return player_master_number == AGGRESIVE_ENTITY_HOST_ID || player_master_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::CreatedByAggressiveTeamOnly()
{
	return player_master_number == AGGRESIVE_ENTITY_HOST_ID && player_master_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::CreatedByAggressiveTeamNotOnly()
{
	return
		player_master_number == AGGRESIVE_ENTITY_HOST_ID && player_master_team_number != AGGRESIVE_ENTITY_HOST_ID ||
		player_master_number != AGGRESIVE_ENTITY_HOST_ID && player_master_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::CreatedByTeam(ControledEntity* host)
{
	GameTypes::players_count_t host_team = host->GetTeamNumber();
	return player_master_team_number == host_team;
}

GameTypes::players_count_t Bomb::GetTeamNumber(ControledEntity* not_host)
{
	if (player_master_number == not_host->GetTeamNumber() || player_master_number == AGGRESIVE_ENTITY_HOST_ID)
	{
		return player_master_team_number;
	}
	return player_master_number;
}

bool Bomb::IsAggressiveFor(ControledEntity* host)
{
	GameTypes::players_count_t host_team = host->GetTeamNumber();
	return
		player_master_number == host_team && player_master_team_number == host_team ||
		player_master_number != host_team && player_master_team_number != host_team && player_master_number != player_master_team_number ||
		player_master_number == host_team && player_master_team_number == AGGRESIVE_ENTITY_HOST_ID ||
		player_master_number == AGGRESIVE_ENTITY_HOST_ID && player_master_team_number == host_team ||
		player_master_team_number == AGGRESIVE_ENTITY_HOST_ID && player_master_number == AGGRESIVE_ENTITY_HOST_ID;
}

void Bomb::Set(Bomb* bomb)
{
	angle = bomb->angle;
	angular_velocity = bomb->angular_velocity;
	animation_tic = bomb->animation_tic;
	direction = bomb->direction;
	exist = bomb->exist;
	force = bomb->force;
	force_collision_coeffisient = bomb->force_collision_coeffisient;
	force_resistance_air_coefficient = bomb->force_resistance_air_coefficient;
	player_master_number = bomb->player_master_number;
	player_master_team_number = bomb->player_master_team_number;
	position = bomb->position;
	radius = bomb->radius;
	status = bomb->status;
	velocity = bomb->velocity;
}

void Bomb::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t payer_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, EngineTypes::Bomb::status_t status, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	UpdateDirection();
	this->exist = exist;
	this->force.Set(0.0f, 0.0f);
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->status = status;
	this->velocity = *velocity;
}

void Bomb::Update()
{
	if (status & BOMB_CAN_REMOVE)
	{
		return;
	}
	DynamicEntity::Update();
	if (status && (BOMB_ACTIVE | BOMB_BOOM))
	{
		animation_tic--;
		if (animation_tic == 0)
		{
			if (status & BOMB_BOOM)
			{
				status |= BOMB_CAN_REMOVE;
				return;
			}
			Boom();
			return;
		}
		return;
	}
}

void Bomb::operator=(Bomb bomb)
{
	angle = bomb.angle;
	angular_velocity = bomb.angular_velocity;
	animation_tic = bomb.animation_tic;
	direction = bomb.direction;
	exist = bomb.exist;
	force = bomb.force;
	force_collision_coeffisient = bomb.force_collision_coeffisient;
	force_resistance_air_coefficient = bomb.force_resistance_air_coefficient;
	player_master_number = bomb.player_master_number;
	player_master_team_number = bomb.player_master_team_number;
	position = bomb.position;
	radius = bomb.radius;
	status = bomb.status;
	velocity = bomb.velocity;
}

Bomb::~Bomb()
{

}