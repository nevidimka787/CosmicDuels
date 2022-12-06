#include "../Entity.h"



AggressiveEntity::AggressiveEntity() :
	StaticEntity(),
	attack_dellay(0),
	attack_period(AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD),
	inactive_period(AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD),
	shoots_count(AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT)
{
}

AggressiveEntity::AggressiveEntity(const AggressiveEntity& aggressive_entity) 
	:
	StaticEntity(aggressive_entity),
	attack_dellay(aggressive_entity.attack_dellay),
	attack_period(aggressive_entity.attack_period),
	inactive_period(aggressive_entity.inactive_period),
	shoots_count(aggressive_entity.shoots_count)
{
}

AggressiveEntity::AggressiveEntity(
	Vec2F position,
	float radius,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	bool exist) 
	:
	StaticEntity(
		position,
		radius,
		angle,
		exist),
	attack_dellay(attack_dellay),
	attack_period(attack_period),
	inactive_period(inactive_period),
	shoots_count(shoots_count)
{
}

AggressiveEntity::AggressiveEntity(
	const Vec2F* position, 
	float radius,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	bool exist) 
	:
	StaticEntity(position, radius, angle, exist),
	attack_dellay(attack_dellay),
	attack_period(attack_period),
	inactive_period(inactive_period),
	shoots_count(shoots_count)
{
}

bool AggressiveEntity::CanShoot(GameTypes::tic_t current_tic) const
{
	if (current_tic <= attack_dellay)
	{
		return false;
	}
	GameTypes::tic_t local_tic = (current_tic - attack_dellay) % (attack_period);
	GameTypes::tic_t shoot_session_period = attack_period - inactive_period;
	if (local_tic >= shoot_session_period)
	{
		return false;
	}

	GameTypes::tic_t time_between_shoots = shoot_session_period / shoots_count;
	if (time_between_shoots <= 1)
	{
		return true;
	}

	shoot_session_period = time_between_shoots * shoots_count;
	if (local_tic >= shoot_session_period)
	{
		return false;
	}
	return local_tic % time_between_shoots == 0;
}

void AggressiveEntity::PostponeAttack(GameTypes::tic_t dellay)
{
	attack_dellay += dellay;
}

void AggressiveEntity::Set(const AggressiveEntity* aggressive_entity)
{
	angle = aggressive_entity->angle;
	attack_dellay = aggressive_entity->attack_dellay;
	attack_period = aggressive_entity->attack_period;
	direction = aggressive_entity->direction;
	exist = aggressive_entity->exist;
	inactive_period = aggressive_entity->inactive_period;
	last_position = aggressive_entity->last_position;
	position = aggressive_entity->position;
	shoots_count = aggressive_entity->shoots_count;
}

void AggressiveEntity::Set(
	Vec2F position, 
	float radius,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
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
	this->shoots_count = shoots_count;
}

void AggressiveEntity::Set(
	const Vec2F* position, 
	float radius,
	float angle,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count, 
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
	this->shoots_count = shoots_count;
}

void AggressiveEntity::operator=(AggressiveEntity aggressive_entity)
{
	angle = aggressive_entity.angle;
	attack_dellay = aggressive_entity.attack_dellay;
	attack_period = aggressive_entity.attack_period;
	direction = aggressive_entity.direction;
	exist = aggressive_entity.exist;
	inactive_period = aggressive_entity.inactive_period;
	last_position = aggressive_entity.last_position;
	position = aggressive_entity.position;
	shoots_count = aggressive_entity.shoots_count;
}

AggressiveEntity::~AggressiveEntity()
{

}