#include "../Entity.h"



AnigAreaGenerator::AnigAreaGenerator()
	:
	SupportEntity(),
	buff_inventory(SHIP_BUFF_NOTHING),
	from_rigth_side(false)
{
}

AnigAreaGenerator::AnigAreaGenerator(const AnigAreaGenerator& anig_area_generator)
	:
	SupportEntity(anig_area_generator),
	buff_inventory(anig_area_generator.buff_inventory),
	from_rigth_side(anig_area_generator.from_rigth_side)
{
}

AnigAreaGenerator::AnigAreaGenerator(
	const ControledEntity* host,
	Vec2F position,
	EngineTypes::Ship::inventory_t buff_inventory,
	bool from_rigth_side,
	float radius,
	float angle,
	bool exist)
	:
	SupportEntity(host, position, radius, angle, exist),
	buff_inventory(buff_inventory),
	from_rigth_side(from_rigth_side)
{
}

AnigAreaGenerator::AnigAreaGenerator(
	const ControledEntity* host,
	const Vec2F* position,
	EngineTypes::Ship::inventory_t buff_inventory,
	bool from_rigth_side,
	float radius,
	float angle,
	bool exist)
	:
	SupportEntity(host, position, radius, angle, exist),
	buff_inventory(buff_inventory),
	from_rigth_side(from_rigth_side)
{
}

bool AnigAreaGenerator::IsHaveShield()
{
	return buff_inventory & SHIP_BUFF_SHIELD;
}

Bomb AnigAreaGenerator::Shoot()
{
	return Bomb(
		position + direction * (radius + BOMB_DEFAULT_RADIUS) + host_p->GetVelocity() * 2.0f,
		host_p->GetVelocity() + direction * BULLET_DEFAULT_VELOCITY,
		host_p->GetTeamNumber(),
		host_p->GetTeamNumber(),
		SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD,
		0.0f,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		0.0f,
		BOMB_DEFAULT_RADIUS,
		BOMB_STATUS_BOOM,
		SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD,
		SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD);
}

AnigAreaGenerator::~AnigAreaGenerator()
{
}