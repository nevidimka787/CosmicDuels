#include "Entity.h"
#include <math.h>

#define M_PI 3.14159265358979323846

Entity::Entity() :
	angle(0.0),
	direction(Vec2F(1.0, 0.0)),
	exist(false),
	radius(0.0)
{
	exist = false;
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

float Entity::GetDistance(Entity* entity)
{
	return entity->position.GetDistance(&position) - entity->radius - radius, 0.0f;
}

float Entity::GetDistance(Vec2F* point)
{
	return point->GetDistance(&position) - radius;
}

float Entity::GetDistance(Line* line)
{
	return line->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(Beam* beam)
{
	return beam->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(Segment* segment)
{
	return segment->GetDistance(&position) - radius, 0.0f;
}

float Entity::GetDistance(DynamicEntity* entity)
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(StaticEntity* entity)
{
	return entity->GetDistance(&position) - radius, 0.0f;
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

float Entity::GetDistance(Cyrcle* cyrcle)
{
	Vec2F temp = cyrcle->GetPosition();
	return GetDistance(&temp) - cyrcle->GetRadius();
}

float Entity::GetDistance(Polygon* polygon)
{
	return 0.0;
}

Vec2F Entity::GetDirection()
{
	return direction;
}

Vec2F Entity::GetPosition()
{
	return position;
}

bool Entity::IsCollision(Vec2F* point)
{
	return GetDistance(point) <= 0.0;
}

bool Entity::IsCollision(Line* line)
{
	return line->GetDistance(&position) <= 0.0;
}

bool Entity::IsCollision(Beam* beam)
{
	return beam->GetDistance(&position) <= 0.0;
}

bool Entity::IsCollision(Segment* segment)
{
	return segment->GetDistance(&position) <= radius;
}

bool Entity::IsCollision(DynamicEntity* dynamic_entity)
{
	return GetDistance(&dynamic_entity->position) <= 0.0;
}

bool Entity::IsCollision(StaticEntity* static_entity)
{
	Vec2F temp = static_entity->GetPosition();
	return GetDistance(&temp) <= 0.0;
}

bool Entity::IsCollision(Rectangle* rectangle)
{
	if (rectangle->exist == false)
	{
		return false;
	}
	Segment side;
	if (rectangle->collision_sides & RECTANGLE_UP_SIDE)
	{
		side = rectangle->GetUpSide();
		if (IsCollision(&side))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_DOWN_SIDE)
	{
		side = rectangle->GetDownSide();
		if (IsCollision(&side))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_RIGHT_SIDE)
	{
		side = rectangle->GetRightSide();
		if (IsCollision(&side))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_LEFT_SIDE)
	{
		side = rectangle->GetLeftSide();
		if (IsCollision(&side))
		{
			return true;
		}
	}
	return false;
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

bool Entity::IsCollision(Polygon* polygon)
{
	if (polygon->exist == false)
	{
		return false;
	}
	return false;
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

void Entity::Rotate(float angle)
{
	this->angle += angle;
	this->direction.x = cos(angle);
	this->direction.y = -sin(angle);
}

void Entity::Set(Entity* entity)
{
	angle = entity->angle;
	direction = entity->direction;
	exist = entity->exist;
	model_matrix = entity->model_matrix;
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

void Entity::SetDirection(Vec2F* direction)
{
	this->direction = direction->Normalize();
}

void Entity::SetPosition(Vec2F* position)
{
	this->position.Set(position);
}

void Entity::Move(Vec2F* delta)
{
	position += *delta;
	model_matrix.TransportThis(delta);
}

void Entity::UpdateAngle()
{
	angle = -atan2f(direction.y, direction.x);
}

void Entity::UpdateDirection()
{
	direction = Vec2F(1.0f, 0.0f).Rotate(angle);
}

void Entity::UpdateMatrix()
{
	model_matrix.SetByAngle(angle);
	model_matrix.TransportThis(position);
	model_matrix.Scale(Vec2F(4.5f, 3.0f));
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
	std::cout << "Grav vec len: " << grav_vec.GetLength() << std::endl;
	force += grav_vec;// * *x_temp_coeff;
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
	Vec2F vec = cyrcle->GetPosition();
	if (GetDistance(&vec) > radius + cyrcle->GetRadius())
	{
		return false;
	}
	Vec2F force_vec = position - cyrcle->GetPosition();
	velocity -= force_vec.Project(&velocity);
	force += force_vec * force_collision_coeffisient;

	if (cyrcle->IsUnbreacable() == false)
	{
		cyrcle->exist = false;
	}
	return true;
}

bool DynamicEntity::Collision(Polygon* polygon)
{
	return false;
}

bool DynamicEntity::Collision(Map* map)
{
	bool collision = false;
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		Rectangle temp = map->GetRectangle(i);
		if (temp.exist)
		{
			collision |= Collision(&temp);
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		Cyrcle temp = map->GetCyrcle(i);
		if (temp.exist)
		{
			collision |= Collision(&temp);
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		Polygon temp = map->GetPolygon(i);
		if (temp.exist)
		{
			collision |= Collision(&temp);
		}
	}
	return collision;
}

float DynamicEntity::GetAngularVelocity()
{
	return angular_velocity;
}

Vec2F DynamicEntity::GetVelocity()
{
	return velocity;
}

Segment DynamicEntity::GetTreck()
{
	return Segment(&position, &velocity);
}

bool DynamicEntity::IsCollision(Vec2F* point)
{
	return Segment(position, velocity).GetDistance(point) < radius;
}

bool DynamicEntity::IsCollision(Line* line)
{
	return Segment(position, velocity).GetDistance(line) < radius;
}

bool DynamicEntity::IsCollision(Beam* beam)
{
	return Segment(position, velocity).GetDistance(beam) < radius;
}

bool DynamicEntity::IsCollision(Segment* segment)
{
	return Segment(position, velocity).GetDistance(segment) < radius;
}

bool DynamicEntity::IsCollision(DynamicEntity* entity)
{
	return Segment(position, velocity).GetDistance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(StaticEntity* entity)
{
	return Segment(position, velocity).GetDistance(entity->GetPosition()) < radius + entity->radius;
}

bool DynamicEntity::IsCollision(Rectangle* rectangle)
{
	Segment side = rectangle->GetUpSide();
	if (Segment(position, velocity).GetDistance(&side) < radius)
	{
		std::cout << "Distance to up side: " << Segment(position, velocity).GetDistance(&side) << std::endl;
		return true;
	}
	side = rectangle->GetLeftSide();
	if (Segment(position, velocity).GetDistance(&side) < radius)
	{	
		std::cout << "Distance to left side: " << Segment(position, velocity).GetDistance(&side) << std::endl;
		return true;
	}
	side = rectangle->GetDownSide();
	if (Segment(position, velocity).GetDistance(&side) < radius)
	{
		std::cout << "Distance to down side: " << Segment(position, velocity).GetDistance(&side) << std::endl;
		return true;
	}
	side = rectangle->GetRightSide();
	if (Segment(position, velocity).GetDistance(&side) < radius)
	{
		std::cout << "Distance to lrighteft side: " << Segment(position, velocity).GetDistance(&side) << std::endl;
		return true;
	}
	return false;
}

bool DynamicEntity::IsCollision(Cyrcle* cyrcle)
{
	return Segment(position, velocity).GetDistance(cyrcle->GetPosition()) < radius + cyrcle->GetRadius();
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
	if (angle > M_PI)
	{
		angle -= M_PI * 2.0f;
	}
	else if (angle < -M_PI)
	{
		angle += M_PI * 2.0f;
	}
	UpdateDirection();


	velocity += force;
	velocity *= 1.0f - force_resistance_air_coefficient;
	force.Set(0.0f, 0.0f);
	Move(&velocity);
}

void DynamicEntity::Set(DynamicEntity* dynamic_entity)
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

void DynamicEntity::Set(Vec2F* position, Vec2F* velocity, float radius, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
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

void StaticEntity::Recalculate()
{
	last_position = position;
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



Bonus::Bonus() :
	DynamicEntity(),
	bonus_type(0)
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
	bonus_type(bonus.bonus_type)
{
}

Bonus::Bonus(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::bonus_t bonus_type, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	DynamicEntity(position, velocity, radius, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	bonus_type(bonus_type)
{
}

Bonus Bonus::Division()
{
	EngineTypes::Bonus::bonus_t temp_bonus;
	bool last = false;
	for (uint8_t i = 0; i < 6; i++)
	{
		temp_bonus = 0x11 << (i << 1);
		if (bonus_type & temp_bonus)
		{
			if (last)
			{
				temp_bonus = bonus_type & temp_bonus;
				bonus_type &= BUFF_BONUS_ALL - temp_bonus;
				return Bonus(&this->position, &this->velocity, temp_bonus);
			}
			else
			{
				last = true;
			}
		}
	}
	return Bonus(false);
}

uint16_t Bonus::GetType()
{
	return bonus_type;
}

uint8_t Bonus::GetBonusesCount()
{
	uint8_t count = 0;
	for (uint8_t i = 0; i < BONUS_BONUSES_COUNT; i++)
	{
		if (bonus_type & (0x11 << (i << 1)))
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
		if (bonus_type & (0x11 << (i << 1)))
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
		if (bonus_type & (0x11 << (i << 1)))
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
		if (bonus_type & (0x11 << (i << 1)))
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
	bonus_type = bonus->bonus_type;
	direction = bonus->direction;
	exist = bonus->exist;
	force_collision_coeffisient = bonus->force_collision_coeffisient;
	force_resistance_air_coefficient = bonus->force_resistance_air_coefficient;
	position = bonus->position;
	radius = bonus->radius;
	velocity = bonus->velocity;
}

void Bonus::Set(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::bonus_t bonus_type, float angle, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_type = bonus_type;
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
	bonus_type = bonus.bonus_type;
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

Asteroid::Asteroid(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::bonus_t bonus_type, uint8_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	Bonus(position, velocity, bonus_type, 0.0f, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	size(size)
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
	this->bonus_type = bonus_type;
	exist = true;
}

Bonus Asteroid::Destroy()
{
	return Bonus(&position, &velocity, bonus_type);
}

Asteroid Asteroid::Division()
{
	if (size > ASTEROID_SIZE_SMALL)
	{
		return Asteroid();
	}
	Vec2F temp_position = position + Vec2F(((float)rand() - (float)RAND_MAX / 2.0f) / ((float)RAND_MAX * 10.0f), ((float)rand() - (float)RAND_MAX / 2.0f) / ((float)RAND_MAX * 10.0f));
	Asteroid temp_asteroid = Asteroid(&temp_position, &velocity, bonus_type, size - 1);

	EngineTypes::Bonus::bonus_t temp;
	for (uint8_t i = 0; i < BONUS_BUFFS_COUNT + BONUS_BONUSES_COUNT; i++)
	{
		temp = 0x11 << (i << 1);
		if (bonus_type & temp)
		{
			break;
		}
	}
	temp_asteroid.bonus_type = temp;
	return temp_asteroid;
}

uint8_t Asteroid::GetSize()
{
	return size;
}

uint16_t Asteroid::GetBuffBonus()
{
	return bonus_type;
}

void Asteroid::Set(Asteroid* entity)
{
	angle = entity->angle;
	angular_velocity = entity->angular_velocity;
	bonus_type = entity->bonus_type;
	direction = entity->direction;
	exist = entity->exist;
	force = entity->force;
	force_collision_coeffisient = entity->force_collision_coeffisient;
	force_resistance_air_coefficient = entity->force_resistance_air_coefficient;
	position = entity->position;
	radius = entity->radius;
	size = entity->size;
	velocity = entity->velocity;
}

void Asteroid::Set(Vec2F* position, Vec2F* velocity, EngineTypes::Bonus::bonus_t bonus_type, uint8_t size, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_type = bonus_type;
	UpdateDirection();
	this->exist = exist;
	this->force = force;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->position = *position;
	this->radius = radius;
	this->size = size;
	this->velocity = *velocity;
}

void Asteroid::operator=(Asteroid entity)
{
	angle = entity.angle;
	angular_velocity = entity.angular_velocity;
	bonus_type = entity.bonus_type;
	direction = entity.direction;
	exist = entity.exist;
	force = entity.force;
	force_collision_coeffisient = entity.force_collision_coeffisient;
	force_resistance_air_coefficient = entity.force_resistance_air_coefficient;
	position = entity.position;
	radius = entity.radius;
	size = entity.size;
	velocity = entity.velocity;
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

GameTypes::players_count_t KillerEntity::GetPlayerMasterNumber()
{
	return player_master_number;
}

GameTypes::players_count_t KillerEntity::GetPlayerMasterTeamNumber()
{
	return player_master_team_number;
}

void KillerEntity::Set(KillerEntity* killer_entity)
{
	angle = killer_entity->angle;
	angular_velocity = killer_entity->angular_velocity;
	direction = killer_entity->direction;
	exist = killer_entity->exist;
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
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	heat_box_vertexes_array[0].Set(radius, 0.0f);
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
		this->heat_box_vertexes_array[0].Set(radius, 0.0f);
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

bool ControledEntity::HeatBoxIsCollision(Bullet* bullet)
{
	//Frame of reference set to this entity.
	Segment segment = Segment(bullet->GetPosition(), bullet->GetVelocity() - velocity);
	//controled entity side
	Segment ce_side;
	Vec2F 
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;
	ce_side.Set(point1, point2);
	if (segment.GetDistance(&ce_side) < bullet->radius + radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2);
		if (segment.GetDistance(&ce_side) < bullet->radius + radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::HeatBoxIsCollision(Knife* knife)
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

bool ControledEntity::HeatBoxIsCollision(Laser* laser)
{

	//Frame of reference set to this entity.
	Beam beam = laser->GetBeam();
	//controled entity side
	Segment ce_side;
	Vec2F
		point1 = heat_box_vertexes_array[heat_box_vertexes_array_length - 1] * model_matrix,
		point2 = heat_box_vertexes_array[0] * model_matrix;

	ce_side.Set(point1, point2);
	if (beam.GetDistance(&ce_side) < radius)
	{
		return true;
	}
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 1; vertex < heat_box_vertexes_array_length; vertex++)
	{
		point1 = point2;
		point2 = heat_box_vertexes_array[vertex] * model_matrix;
		ce_side.Set(point1, point2);
		if (beam.GetDistance(&ce_side) < radius)
		{
			return true;
		}
	}
	return false;
}

bool ControledEntity::HeatBoxIsCollision(MegaLaser* mega_laser)
{
	//Frame of reference set to this entity.
	Segment segment = mega_laser->GetSegment();
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


void ControledEntity::Set(ControledEntity* controled_entity)
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
	buffs_bonuses(0),
	active_baffs(0),
	unbrakable(0)
{
}

Ship::Ship(const Ship& ship) :
	ControledEntity(ship),
	buffs_bonuses(ship.buffs_bonuses),
	active_baffs(ship.active_baffs),
	unbrakable(ship.unbrakable)
{
}

Ship::Ship(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::bonus_t buffs_bonuses, EngineTypes::Bonus::bonus_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	buffs_bonuses(buffs_bonuses),
	active_baffs(active_baffs),
	unbrakable(unbrakable)
{
}

void Ship::ActivateBuffs()
{
	active_baffs |= buffs_bonuses & BONUS_BUFF;
}

void Ship::BreakShield()
{
	active_baffs &= (0xFFFFFFFF - BUFF_SHIELD * 3);
	unbrakable = SHIP_UNBRAKABLE_PERIOD;
}

Bullet Ship::CreateBullet()
{
	Vec2F temp_position = direction * (radius + BULLET_DEFAULT_RADIUS) + position;
	Vec2F temp_velocity = direction * BULLET_DEFAULT_VELOCITY + velocity;
	AddForceAlongDirection(-SHIP_SHOOT_FORCE);
	return Bullet(&temp_position, &temp_velocity, player_number, player_team_number);
}

Bullet Ship::CreateTriple(uint8_t bullet_number)
{
	Vec2F temp1, temp2;
	switch (bullet_number)
	{
	case 0:
		temp1 = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&position, &temp1, player_number, player_team_number);
	case 1:
		temp1 = position + direction.Rotate(0.5) * radius;
		temp2 = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&temp1, &temp2, player_number, player_team_number);
	case 2:
		temp1 = position + direction.Rotate(-0.5) * radius;
		temp2 = direction * BULLET_DEFAULT_VELOCITY + velocity;
		return Bullet(&temp1, &temp2, player_number, player_team_number);
	default:
		return Bullet();
	}
}

Bullet Ship::CreateLoop(GameTypes::entities_count_t bullet_number)
{
	Vec2F new_dir = direction.Rotate(2.0f * (float)M_PI / BULLETS_IN_LOOP * bullet_number);
	Vec2F temp = position + new_dir * radius;
	new_dir = new_dir * BULLET_DEFAULT_VELOCITY + velocity;
	return Bullet(&temp, &new_dir, player_number, player_team_number);
}

Bomb Ship::CreateBomb()
{
	Vec2F temp = Vec2F();
	return Bomb(&position, &temp, player_number, player_team_number);
}

Laser Ship::CreateLaser()
{
	Vec2F new_laser_position = position + direction * radius;
	Beam new_laser_beam = Beam(&new_laser_position, &direction, false);
	return Laser(&new_laser_beam, player_number, player_team_number);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Vec2F new_knife_point1;
	Vec2F new_knife_point2;
	Segment new_knife_segment;
	switch (knife_number)
	{
	case 0:
		new_knife_point1 = position - direction + direction.Perpendicular();
		new_knife_point2 = direction * 2.0 + new_knife_point1;
		new_knife_segment = Segment(&new_knife_point1, &new_knife_point2, true);
		return Knife(&new_knife_segment, &velocity, player_number, player_team_number);
	case 1:
		new_knife_point1 = position - direction - direction.Perpendicular();
		new_knife_point2 = direction * 2.0f;
		new_knife_segment = Segment(&new_knife_point1, &new_knife_point2, true);
		return Knife(&new_knife_segment, &velocity, player_number, player_team_number);
	default:
		return Knife();
	}
}

Pilot Ship::Destroy()
{
	Vec2F pilot_new_velocity = velocity * 2.0f;
	return Pilot(&position, &pilot_new_velocity, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, nullptr, 0, angle);
}

EngineTypes::Bonus::bonus_t Ship::GetActiveBaffs()
{
	return active_baffs;
}

bool Ship::HaveBonus(EngineTypes::Bonus::bonus_t bonus)
{
	EngineTypes::Bonus::bonus_t temp;
	for (EngineTypes::Bonus::bonus_t i = 0; i < BONUS_BONUSES_COUNT; i++)
	{
		temp = 0x11 * (i << 1);
		if ((bonus & temp) && !(buffs_bonuses & temp))
		{
			return false;
		}
	}
	return true;
}

Bonus Ship::LoseBonus()
{
	return Bonus(&position, &velocity, buffs_bonuses);
}

void Ship::Recalculate()
{
	DynamicEntity::Update();
	if (unbrakable > 0)
	{
		unbrakable--;
	}
}

void Ship::Set(Ship* ship)
{
	active_baffs = ship->active_baffs;
	angle = ship->angle;
	angular_velocity = ship->angular_velocity;
	buffs_bonuses = ship->buffs_bonuses;
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

void Ship::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::bonus_t buffs_bonuses, EngineTypes::Bonus::bonus_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->active_baffs = active_baffs;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->buffs_bonuses = buffs_bonuses;
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
	this->unbrakable = unbrakable;
	this->velocity = *velocity;

	delete[] this->heat_box_vertexes_array;
	this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

bool Ship::SpendBonus(EngineTypes::Bonus::bonus_t bonus)
{
	if (HaveBonus(bonus))
	{
		buffs_bonuses -= bonus;
		return true;
	}
	return false;
}

void Ship::SpendBonusNoCheck(EngineTypes::Bonus::bonus_t bonus)
{
	buffs_bonuses -= bonus;
}

void Ship::TakeBonus(Bonus* bonus)
{
	if (buffs_bonuses & BUFF_TRIPLE)
	{
		uint16_t temp;
		for (uint8_t i = 0; i < BONUS_BONUS_DATA_LENGTH; i++)
		{
			temp = 0x11 << (i << 1);
			if (bonus->bonus_type & temp)
			{
				buffs_bonuses |= temp;
			}
		}
		return;
	}
	buffs_bonuses |= bonus->bonus_type;
}

void Ship::operator=(Ship ship)
{
	active_baffs = ship.active_baffs;
	angle = ship.angle;
	angular_velocity = ship.angular_velocity;
	buffs_bonuses = ship.buffs_bonuses;
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



Pilot::Pilot()
{
}

Pilot::Pilot(const Pilot& pilot) :
	ControledEntity(pilot)
{
}

Pilot::Pilot(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* heat_box_vertexes_array, EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length, float angle, EngineTypes::Bonus::bonus_t buffs_bonuses, EngineTypes::Bonus::bonus_t active_baffs, GameTypes::tic_t unbrakable, float angular_velocity, float radius, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	ControledEntity(position, velocity, radius, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, heat_box_vertexes_array, heat_box_vertexes_array_length, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist)
{
}

Ship Pilot::Respawn()
{
	Vec2F temp = Vec2F();
	return Ship(&position, &velocity, player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, nullptr, 0, angle);
}

void Pilot::Set(Pilot* pilot)
{
	angle = pilot->angle;
	angular_velocity = pilot->angular_velocity;
	direction = pilot->direction;
	exist = pilot->exist;
	heat_box_vertexes_array_length = pilot->heat_box_vertexes_array_length;
	player_number = pilot->player_number;
	player_team_number = pilot->player_team_number;
	position = pilot->position;
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
	Vec2F new_bullet_position = position + direction * radius;
	return Bullet(&new_bullet_position, &new_bullet_velocity, AGGRESIVE_ENTITY_HOST_ID, AGGRESIVE_ENTITY_HOST_ID);
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
	shoots_count = turel.shoots_count;
}

Turel::~Turel()
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
	return Segment(position, direction * radius);
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
	StaticEntity(),
	player_master_number(0),
	player_master_team_number(0),
	shoot_time(0)
{
}

Laser::Laser(const Laser& laser) :
	StaticEntity(laser),
	player_master_number(laser.player_master_number),
	player_master_team_number(laser.player_master_team_number),
	shoot_time(laser.shoot_time)
{
}

Laser::Laser(Beam* beam, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t shoot_time, bool exist) :
	StaticEntity(&beam->point, 0.0f, beam->vector.GetAbsoluteAngle(), exist),
	player_master_number(player_master_number),
	player_master_team_number(player_master_team_number),
	shoot_time(shoot_time)
{
}

bool Laser::CanShoot(GameTypes::tic_t current_tic)
{
	return shoot_time > 0;
}

Beam Laser::GetBeam()
{
	return Beam(&position, &direction, false);
}

GameTypes::players_count_t Laser::GetPlayerMasterNumber()
{
	return player_master_number;
}

GameTypes::players_count_t Laser::GetPlayerMasterTeamNumber()
{
	return player_master_team_number;
}

void Laser::Recalculate()
{
	StaticEntity::Recalculate();
	if (shoot_time > 0)
	{
		shoot_time--;
	}
}

void Laser::Set(Laser* laser)
{
	angle = laser->angle;
	direction = laser->direction;
	exist = laser->exist;
	last_position = laser->last_position;
	player_master_number = laser->player_master_number;
	player_master_team_number = laser->player_master_team_number;
	position = laser->position;
	shoot_time = laser->shoot_time;
}

void Laser::Set(Beam* beam, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t shoot_time, bool exist)
{
	angle = beam->vector.GetAbsoluteAngle();
	UpdateDirection();
	exist = exist;
	player_master_number = player_master_number;
	player_master_team_number = player_master_team_number;
	position = beam->point;
	shoot_time = shoot_time;
}

void Laser::operator=(Laser laser)
{
	angle = laser.angle;
	direction = laser.direction;
	exist = laser.exist;
	last_position = laser.last_position;
	player_master_number = laser.player_master_number;
	player_master_team_number = laser.player_master_team_number;
	position = laser.position;
	shoot_time = laser.shoot_time;
}

Laser::~Laser()
{
}



Bullet::Bullet() :
	KillerEntity()
{
}

Bullet::Bullet(const Bullet& bullet) :
	KillerEntity(bullet)
{
}

Bullet::Bullet(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, bool exist) :
	KillerEntity(position, velocity, radius, player_master_number, player_master_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist)
{
}

void Bullet::Set(Bullet* bullet)
{
	angle = bullet->angle;
	angular_velocity = bullet->angular_velocity;
	UpdateDirection();
	exist = bullet->exist;
	force_collision_coeffisient = bullet->force_collision_coeffisient;
	force_resistance_air_coefficient = bullet->force_resistance_air_coefficient;
	player_master_number = bullet->player_master_number;
	player_master_team_number = bullet->player_master_team_number;
	position = bullet->position;
	radius = bullet->radius;
	velocity = bullet->velocity;
}

void Bullet::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
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
	KillerEntity(),
	health(KNIFE_DEFAULT_HEALTH)
{

}

Knife::Knife(const Knife& knife) : KillerEntity(knife),
health(knife.health)
{
}

Knife::Knife(Segment* segment, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, EngineTypes::Knife::knife_health_t health, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist) :
	KillerEntity(&segment->point, velocity, segment->vector.GetLength(), player_master_number, player_master_team_number, segment->vector.GetAbsoluteAngle(), angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	health(health)
{
}

Segment Knife::GetSegment()
{
	Vec2F temp = direction * radius;
	return Segment(&position, &temp, false);
}

void Knife::Set(Knife* knife)
{
	angle = knife->angle;
	angular_velocity = knife->angular_velocity;
	UpdateDirection();
	exist = knife->exist;
	force_collision_coeffisient = knife->force_collision_coeffisient;
	force_resistance_air_coefficient = knife->force_resistance_air_coefficient;
	health = knife->health;
	player_master_number = knife->player_master_number;
	player_master_team_number = knife->player_master_team_number;
	position = knife->position;
	radius = knife->radius;
	velocity = knife->velocity;
}

void Knife::Set(Segment* segment, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, EngineTypes::Knife::knife_health_t health, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, bool exist)
{
	this->angle = segment->vector.GetAbsoluteAngle();
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
	this->position = segment->point;
	this->radius = segment->vector.GetLength();
	this->velocity = *velocity;
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
	angular_velocity = knife.angular_velocity;
	direction = knife.direction;
	exist = knife.exist;
	force = knife.force;
	force_collision_coeffisient = knife.force_collision_coeffisient;
	force_resistance_air_coefficient = knife.force_resistance_air_coefficient;
	health = knife.health;
	player_master_number = knife.player_master_number;
	player_master_team_number = knife.player_master_team_number;
	position = knife.position;
	radius = knife.radius;
	velocity = knife.velocity;
}

Knife::~Knife()
{

}



Bomb::Bomb() :
	KillerEntity(),
	animation_tic(BOMB_DEFAULT_BOOM_DELLAY),
	active(false),
	boom(false)
{
}

Bomb::Bomb(const Bomb& bomb) :
	KillerEntity(bomb),
	animation_tic(bomb.animation_tic),
	active(bomb.active),
	boom(bomb.boom)
{
}

Bomb::Bomb(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, bool active, bool boom, bool exist) :
	KillerEntity(position, velocity, radius, player_master_number, player_master_team_number, angle, angular_velocity, force_collision_coeffisient, force_resistance_air_coefficient, exist),
	active(active),
	animation_tic(animation_tic),
	boom(boom)
{
}

void Bomb::Activate()
{
	active = true;
}

void Bomb::Boom()
{
	animation_tic = BOMB_BOOM_TIME;
	active = false;
	boom = true;
}

bool Bomb::IsActive()
{
	return active;
}

bool Bomb::IsBoom()
{
	return boom;
}

bool Bomb::CanRemove()
{
	return animation_tic == 0 && boom;
}

void Bomb::Recalculate()
{
	DynamicEntity::Update();
	if (animation_tic > 0 && (active || boom))
	{
		animation_tic--;
		return;
	}
	if (animation_tic == 0 && !boom)
	{
		Boom();
		return;
	}
}

void Bomb::Set(Bomb* bomb)
{
	active = bomb->active;
	angle = bomb->angle;
	angular_velocity = bomb->angular_velocity;
	animation_tic = bomb->animation_tic;
	boom = bomb->boom;
	direction = bomb->direction;
	exist = bomb->exist;
	force_collision_coeffisient = bomb->force_collision_coeffisient;
	force_resistance_air_coefficient = bomb->force_resistance_air_coefficient;
	player_master_number = bomb->player_master_number;
	player_master_team_number = bomb->player_master_team_number;
	position = bomb->position;
	radius = bomb->radius;
	velocity = bomb->velocity;
}

void Bomb::Set(Vec2F* position, Vec2F* velocity, GameTypes::players_count_t payer_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t animation_tic, float angle, float angular_velocity, float force_collision_coeffisient, float force_resistance_air_coefficient, float radius, bool active, bool boom, bool exist)
{
	this->active = active;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	this->boom = boom;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->player_master_number = player_master_number;
	this->player_master_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void Bomb::operator=(Bomb bomb)
{
	active = bomb.active;
	angle = bomb.angle;
	angular_velocity = bomb.angular_velocity;
	animation_tic = bomb.animation_tic;
	boom = bomb.boom;
	direction = bomb.direction;
	exist = bomb.exist;
	force = bomb.force;
	force_collision_coeffisient = bomb.force_collision_coeffisient;
	force_resistance_air_coefficient = bomb.force_resistance_air_coefficient;
	player_master_number = bomb.player_master_number;
	player_master_team_number = bomb.player_master_team_number;
	position = bomb.position;
	radius = bomb.radius;
	velocity = bomb.velocity;
}

Bomb::~Bomb()
{

}