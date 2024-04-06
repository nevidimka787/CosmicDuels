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
	const Vec2F& position,
	const Vec2F& velocity, 
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

bool Bullet::Collision(Map::MapData& map)
{
	EngineTypes::Map::array_length_t element;
	for (element = 0; element < map.cyrcles_array_length; element++)
	{
		const auto element_p = map.CyrclePointer(element);
		if (element_p->exist && DynamicEntity::IsCollision(*element_p))
		{
			if (!element_p->IsUnbreacable())
			{
				element_p->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map.polygons_array_length; element++)
	{
		const auto element_p =map.PolygonPointer(element);
		if (element_p->exist && DynamicEntity::IsCollision(*element_p))
		{
			if (!element_p->IsUnbreacable())
			{
				element_p->exist = false;
			}
			return true;
		}
	}
	for (element = 0; element < map.rectangles_array_length; element++)
	{
		const auto element_p = map.RectanglePointer(element);
		if (element_p->exist && DynamicEntity::IsCollision(*element_p))
		{
			if (!element_p->IsUnbreacable())
			{
				element_p->exist = false;
			}
			return true;
		}
	}
	return false;
}

void Bullet::Set(const Bullet* bullet)
{
	KillerEntity::Set(bullet);

	is_ignore = bullet->is_ignore;
	min_velocity = bullet->min_velocity;
}

void Bullet::Set(
	const Vec2F& position,
	const Vec2F& velocity,
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
	KillerEntity::Set(
		position,
		velocity,
		radius,
		player_master_number,
		player_master_team_number,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist);

	this->is_ignore = (is_collision_master) ? (BULLET_IGNORE_MUSTER | BULLET_IGNORE_KNIFES_OF_MASTER) : BULLET_IGNORE_NOTHING;
	this->min_velocity = min_velocity;
}

void Bullet::Update()
{
	DynamicEntity::Update();
	if (velocity.Length() < min_velocity)
	{
		velocity = velocity.Normalize() * min_velocity;
	}
}

void Bullet::operator=(const Bullet& bullet)
{
	KillerEntity::operator=(bullet);

	is_ignore = bullet.is_ignore;
	min_velocity = bullet.min_velocity;
}

Bullet::~Bullet()
{
}