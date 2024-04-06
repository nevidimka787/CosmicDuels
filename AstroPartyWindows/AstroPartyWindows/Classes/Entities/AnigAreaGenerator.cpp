#include "../Entity.h"



AnnihAreaGen::AnnihAreaGen()
	:
	SupportEntity(),
	buff_inventory(SHIP_BUFF_NOTHING)
{
}

AnnihAreaGen::AnnihAreaGen(const AnnihAreaGen& annih_area_generator)
	:
	SupportEntity(annih_area_generator),
	buff_inventory(annih_area_generator.buff_inventory)
{
}

AnnihAreaGen::AnnihAreaGen(
	const ControledEntity* host,
	const Vec2F& position,
	EngineTypes::Ship::inventory_t buff_inventory,
	float radius,
	float angle,
	bool exist)
	:
	SupportEntity(host, position, radius, angle, exist),
	buff_inventory(buff_inventory)
{
	Update();
}

bool AnnihAreaGen::IsHaveShield() const
{
	return buff_inventory & SHIP_BUFF_SHIELD;
}

const Bomb& AnnihAreaGen::Shoot() const
{
	return Bomb(
		position + direction * (radius + BOMB_DEFAULT_RADIUS) + host_p->GetVelocity() * 2.0f,
		host_p->GetVelocity() + direction * BULLET_DEFAULT_VELOCITY * 3.0f,
		host_p->GetTeamNumber(),
		host_p->GetTeamNumber(),
		SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
		0.0f,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		0.0f,
		BOMB_DEFAULT_RADIUS,
		BOMB_STATUS_BOOM,
		SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
		SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD);
}

void AnnihAreaGen::operator=(const AnnihAreaGen& annih_area_gen)
{
	SupportEntity::operator=(annih_area_gen);

	buff_inventory = annih_area_gen.buff_inventory;
}

AnnihAreaGen::~AnnihAreaGen()
{
}