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

AnnihAreaGen::AnnihAreaGen(
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

bool AnnihAreaGen::IsHaveShield() const
{
	return buff_inventory & SHIP_BUFF_SHIELD;
}

Bomb AnnihAreaGen::Shoot()
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

void AnnihAreaGen::operator=(AnnihAreaGen annih_area_gen)
{
	angle = annih_area_gen.angle;
	buff_inventory = annih_area_gen.buff_inventory;
	direction = annih_area_gen.direction;
	exist = annih_area_gen.exist;
	host_matrix_p = annih_area_gen.host_matrix_p;
	host_number = annih_area_gen.host_number;
	host_p = annih_area_gen.host_p;
	host_team = annih_area_gen.host_team;
	last_position = annih_area_gen.last_position;
	local_angle = annih_area_gen.local_angle;
	local_direction = annih_area_gen.local_direction;
	local_position = annih_area_gen.local_position;
	position = annih_area_gen.position;
	radius = annih_area_gen.radius;
}

AnnihAreaGen::~AnnihAreaGen()
{
}