#include "../Entity.h"



AnigAreaGen::AnigAreaGen()
	:
	SupportEntity(),
	buff_inventory(SHIP_BUFF_NOTHING)
{
}

AnigAreaGen::AnigAreaGen(const AnigAreaGen& anig_area_generator)
	:
	SupportEntity(anig_area_generator),
	buff_inventory(anig_area_generator.buff_inventory)
{
}

AnigAreaGen::AnigAreaGen(
	const ControledEntity* host,
	Vec2F position,
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

AnigAreaGen::AnigAreaGen(
	const ControledEntity* host,
	const Vec2F* position,
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

bool AnigAreaGen::IsHaveShield() const
{
	return buff_inventory & SHIP_BUFF_SHIELD;
}

Bomb AnigAreaGen::Shoot()
{
	return Bomb(
		position + direction * (radius + BOMB_DEFAULT_RADIUS) + host_p->GetVelocity() * 2.0f,
		host_p->GetVelocity() + direction * BULLET_DEFAULT_VELOCITY * 3.0f,
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

void AnigAreaGen::operator=(AnigAreaGen anig_area_gen)
{
	angle = anig_area_gen.angle;
	buff_inventory = anig_area_gen.buff_inventory;
	direction = anig_area_gen.direction;
	exist = anig_area_gen.exist;
	host_matrix_p = anig_area_gen.host_matrix_p;
	host_number = anig_area_gen.host_number;
	host_p = anig_area_gen.host_p;
	host_team = anig_area_gen.host_team;
	last_position = anig_area_gen.last_position;
	local_angle = anig_area_gen.local_angle;
	local_direction = anig_area_gen.local_direction;
	local_position = anig_area_gen.local_position;
	position = anig_area_gen.position;
	radius = anig_area_gen.radius;
}

AnigAreaGen::~AnigAreaGen()
{
}