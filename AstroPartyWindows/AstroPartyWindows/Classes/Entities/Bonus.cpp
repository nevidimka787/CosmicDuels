#include "../Entity.h"



Bonus::Bonus()
	:
	DynamicEntity(),
	bonus_inventory(0)
{
	radius = BONUS_DEFAULT_RADIUS;
}

Bonus::Bonus(bool exist)
	:
	Bonus()
{
	this->exist = exist;
}

Bonus::Bonus(const Bonus& bonus) 
	:
	DynamicEntity(bonus),
	bonus_inventory(bonus.bonus_inventory)
{
}

Bonus::Bonus(
	const Vec2F& position,
	const Vec2F& velocity,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	float angle,
	float angular_velocity,
	float radius, 
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
	bonus_inventory(bonus_inventory)
{
}

Bonus::Bonus(
	const Vec2F* position,
	const Vec2F* velocity,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	float angle,
	float angular_velocity,
	float radius,
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
		else if (bonus_inventory & (BONUS_CELL << (cell * 2)))
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