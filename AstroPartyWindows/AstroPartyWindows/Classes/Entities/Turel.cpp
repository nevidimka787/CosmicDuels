#include "../Entity.h"



Turel::Turel() :
	AggressiveEntity()
{
}

Turel::Turel(const Turel& turel)
	:
	AggressiveEntity(turel)
{
}

Turel::Turel(
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

Turel::Turel(
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

DynamicParticle Turel::CreateShards(GameTypes::tic_t current_tic)
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

	std::cout << "Turel::CreateShards::Not realisated." << std::endl;
}

Bullet Turel::Shoot()
{
	Vec2F new_bullet_velocity = direction * BULLET_DEFAULT_VELOCITY;
	Vec2F new_bullet_position = position + direction * radius * 2.0f;
	return Bullet(&new_bullet_position, &new_bullet_velocity, AGGRESIVE_ENTITY_HOST_ID, AGGRESIVE_ENTITY_HOST_ID, false);
}

void Turel::Set(const Turel* turel)
{
	angle = turel->angle;
	attack_dellay = turel->attack_dellay;
	attack_period = turel->attack_period;
	direction = turel->direction;
	exist = turel->exist;
	inactive_period = turel->inactive_period;
	last_position = turel->last_position;
	position = turel->position;
	radius = turel->radius;
	shoots_count = turel->shoots_count;
}

void Turel::Set(
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

void Turel::Set(
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

void Turel::operator=(Turel turel)
{
	angle = turel.angle;
	attack_dellay = turel.attack_dellay;
	attack_period = turel.attack_period;
	direction = turel.direction;
	exist = turel.exist;
	inactive_period = turel.inactive_period;
	last_position = turel.last_position;
	position = turel.position;
	radius = turel.radius;
	shoots_count = turel.shoots_count;
}

Turel::~Turel()
{
}