#include "../Entity.h"



Turret::Turret() :
	AggressiveEntity()
{
}

Turret::Turret(const Turret& turret)
	:
	AggressiveEntity(turret)
{
}

Turret::Turret(
	Vec2F position,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	float radius,
	bool exist) :
	AggressiveEntity(
		position,
		radius,
		angle,
		attack_dellay,
		attack_period,
		inactive_period,
		shoots_count,
		exist)
{
}

Turret::Turret(
	const Vec2F* position,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period, 
	GameTypes::tic_t inactive_period, 
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	float radius,
	bool exist) :
	AggressiveEntity(
		position,
		radius,
		angle,
		attack_dellay,
		attack_period,
		inactive_period,
		shoots_count,
		exist)
{
}

DynamicParticle Turret::CreateShards(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		Vec2F(),
		radius,
		angle,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		PARTICLE_TYPE_SHARDS_ASTEROID,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		PARTICLE_PERIOD_SHARDS_ASTEROID,
		PARTICLE_POSTPONE_SHARDS_ASTEROID,
		current_tic + PARTICLE_PERIOD_SHARDS_ASTEROID);

	std::cout << "Turret::CreateShards::Not realisated." << std::endl;
}

Bullet Turret::Shoot()
{
	Vec2F new_bullet_velocity = direction * BULLET_DEFAULT_VELOCITY;
	Vec2F new_bullet_position = position + direction * radius * 2.0f;
	return Bullet(&new_bullet_position, &new_bullet_velocity, AGGRESIVE_ENTITY_HOST_ID, AGGRESIVE_ENTITY_HOST_ID, false);
}

void Turret::Set(const Turret* turret)
{
	angle = turret->angle;
	attack_dellay = turret->attack_dellay;
	attack_period = turret->attack_period;
	direction = turret->direction;
	exist = turret->exist;
	inactive_period = turret->inactive_period;
	last_position = turret->last_position;
	position = turret->position;
	radius = turret->radius;
	shoots_count = turret->shoots_count;
}

void Turret::Set(
	Vec2F position,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	float radius, 
	bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
	this->shoots_count = shoots_count;
}

void Turret::Set(
	const Vec2F* position,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	float radius,
	bool exist)
{
	this->angle = angle;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	UpdateDirection();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
	this->shoots_count = shoots_count;
}

void Turret::operator=(Turret turret)
{
	angle = turret.angle;
	attack_dellay = turret.attack_dellay;
	attack_period = turret.attack_period;
	direction = turret.direction;
	exist = turret.exist;
	inactive_period = turret.inactive_period;
	last_position = turret.last_position;
	position = turret.position;
	radius = turret.radius;
	shoots_count = turret.shoots_count;
}

Turret::~Turret()
{
}