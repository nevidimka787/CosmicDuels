#include "../Entity.h"



Bullet::Bullet() 
	:
	KillerEntity(),
	is_ignore(BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER),
	min_velocity(BULLET_DEFAULT_MIN_VELOCITY)
{
}

Bullet::Bullet(const Bullet& bullet)
	:
	KillerEntity(bullet),
	is_ignore(bullet.is_ignore),
	min_velocity(bullet.min_velocity)
{
}

Bullet::Bullet(
	Vec2F position,
	Vec2F velocity, 
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	bool is_collision_master,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius, float min_velocity, bool exist) 
	:
	KillerEntity(
		position,
		velocity,
		radius,
		player_master_number,
		player_master_team_number,
		angle, angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	is_ignore((is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING),
	min_velocity(min_velocity)
{
}

Bullet::Bullet(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_master_number, 
	GameTypes::players_count_t player_master_team_number,
	bool is_collision_master, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient, 
	float radius,
	float min_velocity,
	bool exist) 
	:
	KillerEntity(
		position,
		velocity,
		radius,
		player_master_number,
		player_master_team_number, 
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	is_ignore((is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING),
	min_velocity(min_velocity)
{
}

bool Bullet::Collision(Map::MapData* map)
{
	void* element_p;
	EngineTypes::Map::array_length_t element;
	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Map::Cyrcle*)element_p)->exist && DynamicEntity::IsCollision((Map::Cyrcle*)element_p))
		{
			if (!((Map::Cyrcle*)element_p)->IsUnbreacable())
			{
				((Map::Cyrcle*)element_p)->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Map::Polygon*)element_p)->exist && DynamicEntity::IsCollision((Map::Polygon*)element_p))
		{
			if (!((Map::Polygon*)element_p)->IsUnbreacable())
			{
				((Map::Polygon*)element_p)->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Map::Rectangle*)element_p)->exist && DynamicEntity::IsCollision((Map::Rectangle*)element_p))
		{
			if (!((Map::Rectangle*)element_p)->IsUnbreacable())
			{
				((Map::Rectangle*)element_p)->exist = false;
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

void Bullet::Set(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	bool is_collision_master,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius,
	float min_velosity,
	bool exist)
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

void Bullet::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	bool is_collision_master, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius, 
	float min_velosity, 
	bool exist)
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