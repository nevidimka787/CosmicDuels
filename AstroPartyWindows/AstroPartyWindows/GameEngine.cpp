#include "GameEngine.h"
#include "GameRealisation.h"
#include <math.h>

Entity::Entity() : position(new Vec2F()), radius(0.0), angle(0.0), direction(new Vec2F(1.0, 0.0)), exist(false)
{
}

Entity::Entity(Vec2F* position, float radius, float angle) : position(new Vec2F(*position)), radius(radius), angle(angle), direction(new Vec2F(cos(angle), -sin(angle))), exist(true)
{
}

float Entity::GetAngle()
{
	return angle;
}

float Entity::GetDistance(Entity* entity)
{
	return entity->position->GetDistance(position) - entity->radius - radius, 0.0f;
}

float Entity::GetDistance(Vec2F* point)
{
	return point->GetDistance(position) - radius, 0.0f;
}

float Entity::GetDistance(Line* line)
{
	return line->GetDistance(position) - radius, 0.0f;
}

float Entity::GetDistance(Beam* beam)
{
	return beam->GetDistance(position) - radius, 0.0f;
}

float Entity::GetDistance(Segment* segment)
{
	return segment->GetDistance(position) - radius, 0.0f;
}

float Entity::GetDistance(DynamicEntity* entity)
{
	return entity->GetDistance(position) - radius, 0.0f;
}

float Entity::GetDistance(StaticEntity* entity)
{
	return entity->GetDistance(position) - radius, 0.0f;
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
	return *direction;
}

Vec2F Entity::GetPosition()
{
	return *position;
}

bool Entity::IsCollision(Vec2F* point)
{
	return GetDistance(point) <= 0.0;
}

bool Entity::IsCollision(Line* line)
{
	return line->GetDistance(position) <= 0.0;
}

bool Entity::IsCollision(Beam* beam)
{
	return beam->GetDistance(position) <= 0.0;
}

bool Entity::IsCollision(Segment* segment)
{
	return segment->GetDistance(position) <= 0.0;
}

bool Entity::IsCollision(DynamicEntity* entity)
{
	return GetDistance(entity->position) <= 0.0;
}

bool Entity::IsCollision(StaticEntity* entity)
{
	Vec2F temp = entity->GetPosition();
	return GetDistance(&temp) <= 0.0;
}

bool Entity::IsCollision(Rectangle* rectangle)
{
	if (rectangle->is_exist == false)
	{
		return false;
	}
	Segment temp;
	if (rectangle->collision_sides & RECTANGLE_UP_SIDE)
	{
		temp = rectangle->GetUpSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_DOWN_SIDE)
	{
		temp = rectangle->GetDownSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_RIGHT_SIDE)
	{
		temp = rectangle->GetRightSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_LEFT_SIDE)
	{
		temp = rectangle->GetLeftSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Cyrcle* cyrcle)
{
	if (cyrcle->is_exist == false)
	{
		return false;
	}
	Vec2F temp = cyrcle->GetPosition();
	return GetDistance(&temp) < radius + cyrcle->GetRadius();
}

bool Entity::IsCollision(Polygon* polygon)
{
	if (polygon->is_exist == false)
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
	this->direction->x = cos(angle);
	this->direction->y = -sin(angle);
}

void Entity::Set(Entity* entity)
{
	angle = entity->angle;
	*position = *entity->position;
	*direction = *entity->direction;
	exist = entity->exist;
	radius = entity->radius;
}

void Entity::SetAngle(float angle)
{
	this->angle = angle;
}

void Entity::SetDirection(Vec2F* direction)
{
	*this->direction = direction->Normalize();
}

void Entity::SetPosition(Vec2F* position)
{
	*(this->position) = *position;
}

void Entity::Move(Vec2F* delta)
{
	*position += *delta;
}

void Entity::UpdateAngle()
{
	angle = -atan2f(direction->y, direction->x);
}

void Entity::UpdateDirection()
{
	*direction = Vec2F(1.0f, 0.0f).Rotate(angle);
}

void Entity::operator=(Entity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;
}

Entity::~Entity()
{
	delete this->position;
}



DynamicEntity::DynamicEntity() : angular_velocity(0.0f), velocity(new Vec2F()), force(new Vec2F()), FORCE_COLLISION_COEFFICIENT(DEFAULT_FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	Vec2F temp = Vec2F();
}

DynamicEntity::DynamicEntity(Vec2F* position, float angle, float radius) : angular_velocity(0.0f), velocity(new Vec2F()), force(new Vec2F()), FORCE_COLLISION_COEFFICIENT(DEFAULT_FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	SetPosition(position);
	exist = true;
}

DynamicEntity::DynamicEntity(Vec2F* position, Vec2F* velosity, float angle, float angular_velosity, float radius, float FORCE_COLLISION_COEFFICIENT, float FORCE_RESISTANSE_AIR_COEFFICIENT) : angular_velocity(angular_velosity), velocity(new Vec2F(*velosity)), force(new Vec2F()), FORCE_COLLISION_COEFFICIENT(FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	SetPosition(position);
	exist = true;
}

void DynamicEntity::AddForce(Vec2F* force)
{
	*this->force += *force;
}

bool DynamicEntity::Collision(DynamicEntity* entity)
{
	if (GetDistance(entity) > radius + entity->radius)
	{
		return false;
	}
	Vec2F line = *position - *entity->position;
	*velocity -= line.Project(velocity);
	*entity->velocity -= line.Project(entity->velocity);

	return true;
}

bool DynamicEntity::Collision(StaticEntity* entity)
{
	if (GetDistance(entity) > radius + entity->radius)
	{
		return false;
	}
	*velocity -= (*position - entity->GetPosition()).Project(velocity);
	return true;
}

bool DynamicEntity::Collision(Rectangle* rectangle)
{
	Vec2F temp1 = rectangle->GetUpRightPoint();
	Vec2F temp2 = rectangle->GetUpLeftPoint();
	Segment up_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetDownRightPoint();
	temp2 = rectangle->GetDownLeftPoint();
	Segment down_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetUpRightPoint();
	temp2 = rectangle->GetDownRightPoint();
	Segment right_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetUpLeftPoint();
	temp2 = rectangle->GetDownLeftPoint();
	Segment left_side = Segment(&temp1, &temp1, true);

	if (up_side.GetDistance(position) < radius)
	{
		if(right_side.GetDistance(position) < radius)
		{
			Vec2F force_vec = *position - rectangle->GetUpRightPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			if (rectangle->IsUnbreacable() == false)
			{
				rectangle->is_exist = false;
			}
			return true;
		}
		if (left_side.GetDistance(position) < radius)
		{
			Vec2F force_vec = *position - rectangle->GetUpLeftPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			if (rectangle->IsUnbreacable() == false)
			{
				rectangle->is_exist = false;
			}
			return true;
		}
		*velocity += rectangle->GetVelocity() - Vec2F(0.0, 1.0).Project(velocity);
		*force += Vec2F(0.0, (radius - (position->y - up_side.point->y)) * FORCE_COLLISION_COEFFICIENT);
		if (rectangle->IsUnbreacable() == false)
		{
			rectangle->is_exist = false;
		}
		return true;
	}
	if (down_side.GetDistance(position) < radius)
	{
		if (right_side.GetDistance(position) < radius)
		{
			Vec2F force_vec = *position - rectangle->GetDownRightPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			if (rectangle->IsUnbreacable() == false)
			{
				rectangle->is_exist = false;
			}
			return true;
		}
		if (left_side.GetDistance(position) < radius)
		{
			Vec2F force_vec = *position - rectangle->GetDownLeftPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			if (rectangle->IsUnbreacable() == false)
			{
				rectangle->is_exist = false;
			}
			return true;
		}
		*velocity += rectangle->GetVelocity() - Vec2F(0.0, 1.0).Project(velocity);
		*force += Vec2F(0.0, (radius - (up_side.point->y - position->y)) * FORCE_COLLISION_COEFFICIENT);
		if (rectangle->IsUnbreacable() == false)
		{
			rectangle->is_exist = false;
		}
		return true;
	}
	if (right_side.GetDistance(position) < radius)
	{
		*velocity += rectangle->GetVelocity() - Vec2F(1.0, 0.0).Project(velocity);
		*force += Vec2F((radius - (position->x - up_side.point->x)) * FORCE_COLLISION_COEFFICIENT, 0.0);
		if (rectangle->IsUnbreacable() == false)
		{
			rectangle->is_exist = false;
		}
		return true;
	}
	if (left_side.GetDistance(position) < radius)
	{
		*velocity += rectangle->GetVelocity() - Vec2F(1.0, 0.0).Project(velocity);
		*force += Vec2F((radius - (up_side.point->x - position->x)) * FORCE_COLLISION_COEFFICIENT, 0.0);
		if (rectangle->IsUnbreacable() == false)
		{
			rectangle->is_exist = false;
		}
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
	Vec2F force_vec = *position - cyrcle->GetPosition();
	*velocity -= force_vec.Project(velocity);
	*force += force_vec * FORCE_COLLISION_COEFFICIENT;

	if (cyrcle->IsUnbreacable() == false)
	{
		cyrcle->is_exist = false;
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
		if (temp.is_exist)
		{
			collision |= Collision(&temp);
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		Cyrcle temp = map->GetCyrcle(i);
		if (temp.is_exist)
		{
			collision |= Collision(&temp);
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		Polygon temp = map->GetPolygon(i);
		if (temp.is_exist)
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
	return *velocity;
}

void DynamicEntity::Recalculate()
{
	angle += angular_velocity;

	*velocity += *force;
	*velocity *= 1.0f - FORCE_RESISTANSE_AIR_COEFFICIENT;
	*force = Vec2F();
	Move(velocity);
}

void DynamicEntity::Set(DynamicEntity* entity)
{
	angle = entity->angle;
	*position = *entity->position;
	*direction = *entity->direction;
	exist = entity->exist;
	radius = entity->radius;

	angular_velocity = entity->angular_velocity;
	*velocity = *entity->velocity;
	*force = *entity->force;
}

void DynamicEntity::operator=(DynamicEntity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*velocity = *entity.velocity;
}

DynamicEntity::~DynamicEntity()
{
	delete velocity;
	delete force;
}



StaticEntity::StaticEntity() : last_position(new Vec2F())
{

}

StaticEntity::StaticEntity(Vec2F* position, float angle) : last_position(new Vec2F(*position))
{
	SetPosition(position);
	SetAngle(angle);
	exist = true;
}

Vec2F StaticEntity::GetVelosity()
{
	return *position - *last_position;
}

void StaticEntity::Recalculate()
{
	*last_position = *position;
}

void StaticEntity::Set(StaticEntity* entity)
{
	angle = entity->angle;
	*position = *entity->position;
	*direction = *entity->direction;
	exist = entity->exist;
	radius = entity->radius;

	*last_position = *entity->last_position;
}

void StaticEntity::operator=(StaticEntity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;
}

StaticEntity::~StaticEntity()
{
	delete last_position;
}



Bonus::Bonus() : bonus_type(0)
{
	radius = BONUS_DEFAULT_RADIUS;
}

Bonus::Bonus(bonus_t bonus_type, Vec2F* position, Vec2F* velocity) : bonus_type(bonus_type)
{
	*this->position = *position;
	*this->velocity = *velocity;
	radius = BONUS_DEFAULT_RADIUS;
	exist = true;
}

Bonus Bonus::Division()
{
	bonus_t temp;
	bool last = false;
	for (uint8_t i = 0; i < 6; i++)
	{
		temp = 0x11 << (i << 1);
		if (bonus_type & temp)
		{
			if (last)
			{
				return Bonus(bonus_type & temp, position, velocity);
			}
			else
			{
				last = true;
			}
		}
	}
	return Bonus();
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

void Bonus::Set(Bonus* entity)
{
	exist = entity->exist;
	*position = *entity->position;

	*velocity = *entity->velocity;

	bonus_type = entity->bonus_type;
}

void Bonus::operator=(Bonus entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*velocity = *entity.velocity;

	bonus_type = entity.bonus_type;
}

Bonus::~Bonus()
{

}



Asteroid::Asteroid() : size(ASTEROID_DEFAULT_SIZE)
{

}

Asteroid::Asteroid(bonus_t bonus_type, uint8_t size) : size(size)
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
	return Bonus(bonus_type, position, velocity);
}

Asteroid Asteroid::Division()
{
	if (size > ASTEROID_SIZE_SMALL)
	{
		return Asteroid();
	}
	Asteroid temp_asteroid = Asteroid(bonus_type, size - 1);
	*temp_asteroid.position = *position + Vec2F(((float)rand() - (float)RAND_MAX / 2.0f) / ((float)RAND_MAX * 10.0f), ((float)rand() - (float)RAND_MAX / 2.0f) / ((float)RAND_MAX * 10.0f));
	*temp_asteroid.velocity = *velocity;

	bonus_t temp;
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
	exist = entity->exist;
	*position = *entity->position;

	*velocity = *entity->velocity;

	bonus_type = entity->bonus_type;

	size = entity->size;
}

void Asteroid::operator=(Asteroid entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	bonus_type = entity.bonus_type;
}

Asteroid::~Asteroid()
{

}



KillerEntity::KillerEntity() : player_master_number(0), player_master_team_number(0)
{

}

KillerEntity::KillerEntity(Game::players_count_t player_master_number, Game::players_count_t player_master_team_number) : player_master_number(player_master_number), player_master_team_number(player_master_team_number)
{
	exist = true;
}

uint8_t KillerEntity::GetPlayerMasterNumber()
{
	return player_master_number;
}

void KillerEntity::Set(KillerEntity* entity)
{
	exist = entity->exist;
	*position = *entity->position;

	*velocity = *entity->velocity;

	player_master_number = entity->player_master_number;
	player_master_team_number = entity->player_master_team_number;
}

void KillerEntity::operator=(KillerEntity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_master_number = entity.player_master_number;player_master_team_number = entity.player_master_team_number;
}

KillerEntity::~KillerEntity()
{

}



ControledEntity::ControledEntity() : player_number(0), rotate_input_value_pointer(nullptr), shoot_input_value_pointer(nullptr), player_team_number(0)
{

}

ControledEntity::ControledEntity(Game::players_count_t player_number, Game::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity) : player_number(player_number), player_team_number(player_team_number), rotate_input_value_pointer(rotate_input_value_pointer), shoot_input_value_pointer(shoot_input_value_pointer)
{
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	exist = true;
}

uint8_t ControledEntity::GetPlayerNumber()
{
	return player_number;
}

bool ControledEntity::GetRotateInputValue()
{
	return *(bool*)rotate_input_value_pointer;
}

bool ControledEntity::GetShootInputValue()
{
	return *(bool*)shoot_input_value_pointer;
}

void ControledEntity::Set(ControledEntity* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;
	radius = entity->radius;

	angular_velocity = entity->angular_velocity;
	*velocity = *entity->velocity;

	player_number = entity->player_number;
	rotate_input_value_pointer = entity->rotate_input_value_pointer;
	shoot_input_value_pointer = entity->shoot_input_value_pointer;
}

void ControledEntity::operator=(ControledEntity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_number = entity.player_number;
	rotate_input_value_pointer = entity.rotate_input_value_pointer;
	shoot_input_value_pointer = entity.shoot_input_value_pointer;
}

ControledEntity::~ControledEntity()
{

}



Ship::Ship() : buffs_bonuses(0), active_baffs(0), unbrakable(0)
{

}

Ship::Ship(Game::players_count_t player_number, Game::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity) : buffs_bonuses(0x00), active_baffs(0x00), unbrakable(SHIP_UNBRAKABLE_PERIOD)
{
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	exist = true;
}

Ship::Ship(Game::players_count_t player_number, Game::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity, Bonus::bonus_t buffs_bonuses) : buffs_bonuses(buffs_bonuses), active_baffs(0x00), unbrakable(SHIP_UNBRAKABLE_PERIOD)
{
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	exist = true;
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
	Vec2F temp_position = *direction * radius + *position;
	Vec2F temp_velocity = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
	return Bullet(&temp_position, &temp_velocity, player_number);
}

Bullet Ship::CreateTriple(uint8_t bullet_number)
{
	Vec2F temp1, temp2;
	switch (bullet_number)
	{
	case 0:
		temp1 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return Bullet(position, &temp1, player_number);
	case 1:
		temp1 = *position + direction->Rotate(0.5) * radius;
		temp2 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return Bullet(&temp1, &temp2, player_number);
	case 2:
		temp1 = *position + direction->Rotate(-0.5) * radius;
		temp2 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return Bullet(&temp1, &temp2, player_number);
	default:
		return Bullet();
	}
}

Bullet Ship::CreateLoop(Game::entities_count_t bullet_number)
{
	Vec2F new_dir = direction->Rotate(2.0f * (float)M_PI / BULLETS_IN_LOOP * bullet_number);
	Vec2F temp = *position + new_dir * radius;
	new_dir = new_dir * BULLET_DEFAULT_VELOCITY + *velocity;
	return Bullet(&temp, &new_dir, player_number);
}

Bomb Ship::CreateBomb()
{
	Vec2F temp = Vec2F();
	return Bomb(position, &temp, 0.0f, 0.0f, player_number);
}

Laser Ship::CreateLazer()
{
	Vec2F temp = *position + *direction * radius;
	return Laser(&temp, direction, player_number, player_team_number);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Vec2F temp1, temp2;
	switch (knife_number)
	{
	case 0:
		temp1 = *position - *direction + direction->Perpendicular();
		temp2 = *direction * 2.0 + temp1;
		return Knife(&temp1, &temp2, player_number);
	case 1:
		temp1 = *position - *direction - direction->Perpendicular();
		temp2 = *direction * 2.0f;
		return Knife(&temp1, &temp2, player_number);
	default:
		return Knife();
	}
}

Pilot Ship::Destroy()
{
	Vec2F temp = *velocity * 2.0f;
	return Pilot(player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, position, &temp, angle, angular_velocity);
}

Bonus::bonus_t Ship::GetActiveBaffs()
{
	return active_baffs;
}

bool Ship::HaveBonus(Bonus::bonus_t bonus)
{
	Bonus::bonus_t temp;
	for (Bonus::bonus_t i = 0; i < BONUS_BONUSES_COUNT; i++)
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
	return Bonus(buffs_bonuses, position, velocity);
}

void Ship::Recalculate()
{
	DynamicEntity::Recalculate();
	if (unbrakable > 0)
	{
		unbrakable--;
	}
}

void Ship::Set(Ship* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;

	angular_velocity = entity->angular_velocity;
	*velocity = *entity->velocity;

	player_number = entity->player_number;
	rotate_input_value_pointer = entity->rotate_input_value_pointer;
	shoot_input_value_pointer = entity->shoot_input_value_pointer;

	buffs_bonuses = entity->buffs_bonuses;
	active_baffs = entity->active_baffs;
	unbrakable = entity->unbrakable;
}

bool Ship::SpendBonus(Bonus::bonus_t bonus)
{
	if (HaveBonus(bonus))
	{
		buffs_bonuses -= bonus;
		return true;
	}
	return false;
}

void Ship::SpendBonusNoCheck(Bonus::bonus_t bonus)
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

void Ship::operator=(Ship entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_number = entity.player_number;
	rotate_input_value_pointer = entity.rotate_input_value_pointer;
	shoot_input_value_pointer = entity.shoot_input_value_pointer;

	buffs_bonuses = entity.buffs_bonuses;
	active_baffs = entity.active_baffs;
}

Ship::~Ship()
{

}



Pilot::Pilot()
{

}

Pilot::Pilot(Game::players_count_t player_number, Game::players_count_t player_team_number, void* rotate_keyboard_key_pointer, void* move_keyboard_key_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity)
{
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	rotate_input_value_pointer = rotate_keyboard_key_pointer;
	shoot_input_value_pointer = move_keyboard_key_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	exist = true;
}

Ship Pilot::Respawn()
{
	Vec2F temp = Vec2F();
	return Ship(player_number, player_team_number, rotate_input_value_pointer, shoot_input_value_pointer, position, &temp, angle, 0.0f);
}

void Pilot::Set(Pilot* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;

	angular_velocity = entity->angular_velocity;
	*velocity = *entity->velocity;

	player_number = entity->player_number;
	rotate_input_value_pointer = entity->rotate_input_value_pointer;
	shoot_input_value_pointer = entity->shoot_input_value_pointer;
}

void Pilot::operator=(Pilot entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_number = entity.player_number;
	rotate_input_value_pointer = entity.rotate_input_value_pointer;
	shoot_input_value_pointer = entity.shoot_input_value_pointer;
}

Pilot::~Pilot()
{

}



AggressiveEntity::AggressiveEntity() : attack_dellay(0), attack_period(AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD), inactive_period(AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD), shoots_count(AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT)
{

}

AggressiveEntity::AggressiveEntity(Game::tic_t current_tic, Game::tic_t first_activation_dellay, Game::tic_t attack_period, Game::tic_t passive_period, uint8_t shoots_count) : attack_dellay(current_tic + first_activation_dellay), attack_period(attack_period), inactive_period(passive_period), shoots_count(shoots_count)
{
	exist = true;
}

bool AggressiveEntity::CanShoot(Game::tic_t current_tic)
{
	uint32_t local_tic = (current_tic - attack_dellay) % (attack_period + inactive_period);
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

void AggressiveEntity::PostponeAttack(uint32_t dellay)
{
	attack_dellay += dellay;
}

void AggressiveEntity::Set(AggressiveEntity* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;
	*last_position = *entity->last_position;

	attack_dellay = entity->attack_dellay;
	attack_period = entity->attack_period;
	inactive_period = entity->inactive_period;
}

void AggressiveEntity::operator=(AggressiveEntity entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	attack_dellay = entity.attack_dellay;
	attack_period = entity.attack_period;
	inactive_period = entity.inactive_period;
	shoots_count = entity.shoots_count;
}

AggressiveEntity::~AggressiveEntity()
{

}



Turel::Turel(Vec2F* position, float angle)
{
	this->position = position;
	this->angle = angle;
}

Bullet Turel::Shoot()
{
	Vec2F temp1 = *direction * BULLET_DEFAULT_VELOCITY;
	Vec2F temp2 = *position + *direction * radius;
	return Bullet(&temp2, &temp1, AGGRESIVE_ENTITY_HOST_ID);
}

void Turel::Set(Turel* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;
	*last_position = *entity->last_position;

	attack_dellay = entity->attack_dellay;
	attack_period = entity->attack_period;
	inactive_period = entity->inactive_period;
}

void Turel::operator=(Turel entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	attack_dellay = entity.attack_dellay;
	attack_period = entity.attack_period;
	inactive_period = entity.inactive_period;
	shoots_count = entity.shoots_count;
}

Turel::~Turel()
{

}



GravGen::GravGen() : gravity(GRAVITY_GENERATOR_DEFAULT_GRAVITY)
{

}

GravGen::GravGen(Vec2F* position, float gravity) : gravity(gravity)
{
	*this->position = *position;
}

void GravGen::Set(GravGen* entity)
{
	angle = entity->angle;
	*direction = *entity->direction;
	exist = entity->exist;
	*position = *entity->position;
	*last_position = *entity->last_position;

	gravity = entity->gravity;
}

void GravGen::operator=(GravGen entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	gravity = entity.gravity;
}

GravGen::~GravGen()
{

}



MegaLaser::MegaLaser(Segment* lazer_segment, float angle) : active(false)
{
	*this->position = *lazer_segment->point;
	*this->point2 = lazer_segment->GetSecondPoint();
	this->angle = angle;
	exist = true;
}



void MegaLaser::operator=(MegaLaser entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	attack_dellay = entity.attack_dellay;
	attack_period = entity.attack_period;
	inactive_period = entity.inactive_period;
	shoots_count = entity.shoots_count;

	active = entity.active;
	*point2 = *entity.point2;
}

MegaLaser::~MegaLaser()
{

}



Laser::Laser() : player_master_number(0), player_master_team_number(0)
{

}

Laser::Laser(Vec2F* position, Vec2F* velocity, Game::players_count_t player_master_number, Game::players_count_t player_master_team_number) : player_master_number(player_master_number), player_master_team_number(player_master_team_number)
{
	*this->position = *position;
	*this->direction = *direction;
	exist = true;
}

Beam Laser::GetBeam()
{
	return Beam(position, direction, false);
}

void Laser::Set(Laser* laser)
{
	*position = *laser->position;
	*direction = *laser->direction;
	player_master_number = laser->player_master_number;
	player_master_team_number = laser->player_master_team_number;
	exist = laser->exist;
}

void Laser::operator=(Laser entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	player_master_number = entity.player_master_number;player_master_team_number = entity.player_master_team_number;
}

Laser::~Laser()
{

}



Bullet::Bullet()
{

}

Bullet::Bullet(Vec2F* position, Vec2F* velocity, uint8_t host_number)
{
	*this->position = *position;
	*this->velocity = *velocity;
	player_master_number = host_number;
	exist = true;
}

bool Bullet::IsCollision(Map* map)
{
	void* pointer;
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		pointer = (void*)map->GetRectanglePointer(i);
		if (Entity::IsCollision((Rectangle*)pointer))
		{
			((Rectangle*)pointer)->is_exist = false;
			return true;
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		pointer = (void*)map->GetCyrclePointer(i);
		if (Entity::IsCollision(map->GetCyrclePointer(i)))
		{
			((Cyrcle*)pointer)->is_exist = false;
			return true;
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		pointer = (void*)map->GetPolygonPointer(i);
		if (Entity::IsCollision(map->GetPolygonPointer(i)))
		{
			((Polygon*)pointer)->is_exist = false;
			return true;
		}
	}
	return false;
}

void Bullet::Set(Bullet* bullet)
{
	*position = *bullet->position;
	*velocity = *bullet->velocity;
	player_master_number = bullet->player_master_number;
	player_master_team_number = bullet->player_master_team_number;
	exist = bullet->exist;
}

void Bullet::operator=(Bullet entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_master_number = entity.player_master_number;player_master_team_number = entity.player_master_team_number;
}

Bullet::~Bullet()
{

}



Knife::Knife() : health(3)
{

}

Knife::Knife(Vec2F* point1, Vec2F* point2, uint8_t host_number) : health(3)
{
	*position = *point1;
	radius = point1->GetDistance(point2);
	*direction = *point2 - *point1;
	direction->NormalizeThis();
	exist = true;
}

Segment Knife::GetSegment()
{
	Vec2F temp = *direction * radius;
	return Segment(position, &temp, false);
}

void Knife::Set(Knife* knife)
{
	*position = *knife->position;
	*direction = *knife->direction;
	radius = knife->radius;
	exist = knife->exist;
	player_master_number = knife->player_master_number;
	player_master_team_number = knife->player_master_team_number;
}

void Knife::operator=(Knife entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_master_number = entity.player_master_number;player_master_team_number = entity.player_master_team_number;
}

Knife::~Knife()
{

}



Bomb::Bomb() : animation_tic(MINE_DEFAULT_TIMER), active(false), boom(false)
{

}

Bomb::Bomb(Vec2F* position, Vec2F* velosity, float angle, float angular_velosity, uint8_t player_master_number) : animation_tic(MINE_DEFAULT_TIMER), active(false), boom(false)
{
	this->player_master_number = player_master_number;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	exist = true;
}

void Bomb::Activate()
{
	active = true;
}

void Bomb::Boom()
{
	animation_tic = MINE_BOOM_TIMER;
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
	DynamicEntity::Recalculate();
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
	*position = *bomb->position;
	*velocity = *bomb->velocity;
	angle = bomb->angle;
	*direction = *bomb->direction;
	angular_velocity = bomb->angular_velocity;
	player_master_number = bomb->player_master_number;
	player_master_team_number = bomb->player_master_team_number;
	exist = bomb->exist;
}

void Bomb::operator=(Bomb entity)
{
	angle = entity.angle;
	*direction = *entity.direction;
	exist = entity.exist;
	*position = *entity.position;
	radius = entity.radius;

	angular_velocity = entity.angular_velocity;
	*force = *entity.force;
	*velocity = *entity.velocity;

	player_master_number = entity.player_master_number;player_master_team_number = entity.player_master_team_number;

	animation_tic = entity.animation_tic;
	active = entity.active;
	boom = entity.boom;
}

Bomb::~Bomb()
{

}



MapElement::MapElement() : position(new Vec2F()), last_position(new Vec2F()), unbreakable(true), is_exist(true)
{

}

Vec2F MapElement::GetPosition()
{
	return *position;
}

Vec2F MapElement::GetVelocity()
{
	return *position - *last_position;
}

bool MapElement::IsUnbreacable()
{
	return unbreakable;
}

void MapElement::Move(Vec2F* move_vector)
{
	*position += *move_vector;
}

void MapElement::Set(MapElement* element)
{
	*position = *element->position;
	*last_position = *element->last_position;
	unbreakable = element->unbreakable;
}

void MapElement::SetPosition(Vec2F* position)
{
	*this->position = *position;
}

MapElement::~MapElement()
{
	delete position;
	delete last_position;
}



Rectangle::Rectangle() : point2(new Vec2F()), show_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE), collision_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE)
{

}

Rectangle::Rectangle(Vec2F* point1, Vec2F* point2, bool unbreakable) : point2(new Vec2F(*point2)), show_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE), collision_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE)
{
	*position = *point1;
	this->unbreakable = unbreakable;
}

Vec2F Rectangle::GetUpRightPoint()
{
	return Vec2F(point2->x, position->y);
}

Vec2F Rectangle::GetDownRightPoint()
{
	return *point2;
}


Vec2F Rectangle::GetDownLeftPoint()
{
	return Vec2F(position->x, point2->y);;
}


Vec2F Rectangle::GetUpLeftPoint()
{
	return *position;
}

Segment Rectangle::GetUpSide()
{
	Vec2F temp = Vec2F(point2->x, position->y);
	return Segment(position, &temp, true);
}

Segment Rectangle::GetDownSide()
{
	Vec2F temp = Vec2F(position->x, point2->y);
	return Segment(&temp, point2, true);
}

Segment Rectangle::GetRightSide()
{
	Vec2F temp = Vec2F(point2->x, position->y);
	return Segment(&temp, point2, true);
}

Segment Rectangle::GetLeftSide()
{
	Vec2F temp = Vec2F(position->x, point2->y);
	return Segment(position, &temp, true);
}

void Rectangle::Move(Vec2F* move_vector)
{
	MapElement::Move(move_vector);
	*point2 += *move_vector;
}

void Rectangle::Set(Rectangle* parent)
{
	*position = *parent->position;
	*last_position = *parent->last_position;
	*point2 = *parent->point2;
	show_sides = parent->show_sides;
	collision_sides = parent->collision_sides;

}

Rectangle::~Rectangle()
{
	delete point2;
}



Cyrcle::Cyrcle() : radius(0.0f)
{

}

Cyrcle::Cyrcle(Vec2F* position, float radius, bool unbreakable) : radius(radius)
{
	*this->position = *position;
	this->unbreakable = unbreakable;
}

float Cyrcle::GetRadius()
{
	return radius;
}

void Cyrcle::SetRadius(float radius)
{
	this->radius = radius;
}

void Cyrcle::Set(Cyrcle* parent)
{
	*this->last_position = *parent->last_position;
	*this->position = *parent->position;
	this->radius = parent->radius;
}

Cyrcle::~Cyrcle()
{

}



Polygon::Polygon() : points_array(nullptr), default_points_array(nullptr), points_array_length(0)
{

}

Polygon::Polygon(Vec2F* position, Vec2F* default_points_array, uint32_t points_array_length, bool unbreakable) : points_array_length(points_array_length), default_points_array(new Vec2F[points_array_length]), points_array(new Vec2F[points_array_length - 1])
{
	*this->position = *position;
	for (uint32_t i = 0; i < points_array_length; i++)
	{
		this->default_points_array[i] = default_points_array[i];
	}
	ToDefault();
	this->unbreakable = unbreakable;
}

void Polygon::Rotate(float angle, Vec2F* rotating_point)
{
	Vec2F rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - *rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = *rotating_point + rot_vec;
	}

	rot_vec = *position - *rotating_point;
	rot_vec.RotateThis(angle);
	*position = *rotating_point + rot_vec;
}

void Polygon::Move(Vec2F* move_vector)
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] += *move_vector;
	}
	MapElement::Move(move_vector);
}

void Polygon::ToDefault()
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] = default_points_array[i];
	}
	*position = default_points_array[points_array_length - 1];
}

void Polygon::Set(Polygon* parent)
{
	delete points_array;
	delete default_points_array;
	points_array_length = parent->points_array_length;
	points_array = new Vec2F[points_array_length - 1];
	default_points_array = new Vec2F[points_array_length];
	for (uint32_t i = 1; i < points_array_length; i++)
	{
		points_array[i] = parent->points_array[i];
	}
}

Polygon::~Polygon()
{

}



Map::Map(Rectangle* rectangles_array, uint8_t rectangles_array_length, Cyrcle* cyrcles_array, uint8_t cyrcles_array_length, Polygon* polygons_array, uint8_t polygons_array_length, Vec2F* size) : rectangles_array_length(rectangles_array_length), cyrcles_array_length(cyrcles_array_length), polygons_array_length(polygons_array_length), size(*size)
{
	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}
	else
	{
		this->rectangles_array = nullptr;
	}

	if (rectangles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->cyrcles_array[i].Set(&cyrcles_array[i]);
		}
	}
	else
	{
		this->cyrcles_array = nullptr;
	}

	if (cyrcles_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (uint8_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}
	else
	{
		this->polygons_array = nullptr;
	}
}

Vec2F Map::GetSize()
{
	return size;
}

Rectangle Map::GetRectangle(uint8_t number)
{
	if (number >= rectangles_array_length)
	{
		return rectangles_array[number % rectangles_array_length];
	}
	return rectangles_array[number];
}

Cyrcle Map::GetCyrcle(uint8_t number)
{
	if (number >= cyrcles_array_length)
	{
		return cyrcles_array[number % cyrcles_array_length];
	}
	return cyrcles_array[number];
}

Polygon Map::GetPolygon(uint8_t number)
{
	if (number >= polygons_array_length)
	{
		return polygons_array[number % polygons_array_length];
	}
	return polygons_array[number];
}

Rectangle* Map::GetRectanglePointer(uint8_t number)
{
	if (number >= rectangles_array_length)
	{
		return &rectangles_array[number % rectangles_array_length];
	}
	return &rectangles_array[number];
}

Cyrcle* Map::GetCyrclePointer(uint8_t number)
{
	if (number >= cyrcles_array_length)
	{
		return &cyrcles_array[number % cyrcles_array_length];
	}
	return &cyrcles_array[number];
}

Polygon* Map::GetPolygonPointer(uint8_t number)
{
	if (number >= polygons_array_length)
	{
		return &polygons_array[number % polygons_array_length];
	}
	return &polygons_array[number];
}

Map::~Map()
{
	delete rectangles_array;
	delete cyrcles_array;
	delete polygons_array;
}