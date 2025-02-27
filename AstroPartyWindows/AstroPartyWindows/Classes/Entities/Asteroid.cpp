#include "../Entity.h"



Asteroid::Asteroid() 
	:
	Bonus(),
	size(ASTEROID_DEFAULT_SIZE)
{
}

Asteroid::Asteroid(const Asteroid& asteroid) 
	:
	Bonus(asteroid),
	size(asteroid.size)
{
}

Asteroid::Asteroid(
	const Vec2F& position,
	const Vec2F& velocity,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Asteroid::size_t size,
	float angle,
	float angular_velocity, 
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist) 
	:
	Bonus(
		position,
		velocity,
		bonus_inventory,
		0.0f,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
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

bool Asteroid::Collision(const Map::MapData& map)
{
	bool collision = false;
	for (auto& element : map.polygons_array)
	{
		if (element.exist && DynamicEntity::Collision(element))
		{
			if (element.IsAggressive())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	for (auto& element : map.cyrcles_array)
	{
		if (element.exist && DynamicEntity::Collision(element))
		{
			if (element.IsAggressive())
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	for (auto& element : map.rectangles_array)
	{
		if (element.exist && DynamicEntity::Collision(element))
		{
			if (element.IsAggressive())
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
		return Bonus(position, velocity / 2.0f, bonus_inventory);
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

void Asteroid::Set(const Asteroid* asteroid)
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

void Asteroid::Set(
	const Vec2F& position,
	const Vec2F& velocity,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Asteroid::size_t size,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	Bonus::Set(
		position, 
		velocity,
		bonus_inventory,
		angle, angular_velocity,
		0.0f,
		force_collision_coeffisient, 
		force_resistance_air_coefficient, 
		exist);

	this->size = size;
	UpdateRadius();
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

void Asteroid::operator=(const Asteroid& asteroid)
{
	Bonus::operator=(asteroid);

	size = asteroid.size;
}

Asteroid::~Asteroid()
{

}