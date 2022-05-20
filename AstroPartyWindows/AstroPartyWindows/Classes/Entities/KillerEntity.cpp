#include "../Entity.h"



KillerEntity::KillerEntity() :
	DynamicEntity(),
	host_number(0),
	host_team_number(0)
{
}

KillerEntity::KillerEntity(const KillerEntity& killer_entity) :
	DynamicEntity(killer_entity),
	host_number(killer_entity.host_number),
	host_team_number(killer_entity.host_team_number)
{
}

KillerEntity::KillerEntity(
	Vec2F position,
	Vec2F velocity,
	float radius,
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	float angle,
	float angular_velocity,
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
	host_number(player_master_number),
	host_team_number(player_master_team_number)
{
}

KillerEntity::KillerEntity(
	const Vec2F* position,
	const Vec2F* velocity,
	float radius,
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	float angle,
	float angular_velocity,
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
	host_number(player_master_number),
	host_team_number(player_master_team_number)
{
}

bool KillerEntity::CreatedBy(ControledEntity* controled_entity)
{
	return host_number == controled_entity->GetPlayerNumber();
}

bool KillerEntity::CreatedByTeam(ControledEntity* controled_entity)
{
	return host_team_number == controled_entity->GetTeamNumber();
}

GameTypes::players_count_t KillerEntity::GetHostNumber()
{
	return host_number;
}

GameTypes::players_count_t KillerEntity::GetHostTeamNumber()
{
	return host_team_number;
}

bool KillerEntity::SameTeam(KillerEntity* killer_entity)
{
	return host_team_number == killer_entity->host_team_number;
}

void KillerEntity::Set(KillerEntity* killer_entity)
{
	angle = killer_entity->angle;
	angular_velocity = killer_entity->angular_velocity;
	direction = killer_entity->direction;
	exist = killer_entity->exist;
	force_collision_coeffisient = killer_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = killer_entity->force_resistance_air_coefficient;
	host_number = killer_entity->host_number;
	host_team_number = killer_entity->host_team_number;
	position = killer_entity->position;
	radius = killer_entity->radius;
	velocity = killer_entity->velocity;
}

void KillerEntity::Set(
	Vec2F position,
	Vec2F velocity,
	float radius,
	GameTypes::players_count_t player_master_number, 
	GameTypes::players_count_t player_master_team_number,
	float angle,
	float angular_velocity, 
	float force_collision_coeffisient,
	float force_resistance_air_coefficient, 
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
	this->position = position;
	this->radius = radius;
	this->velocity = velocity;
}

void KillerEntity::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	float radius,
	GameTypes::players_count_t player_master_number,
	GameTypes::players_count_t player_master_team_number,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->velocity = *velocity;
}

void KillerEntity::operator=(KillerEntity killer_entity)
{
	angle = killer_entity.angle;
	angular_velocity = killer_entity.angular_velocity;
	direction = killer_entity.direction;
	exist = killer_entity.exist;
	force = killer_entity.force;
	force_collision_coeffisient = killer_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = killer_entity.force_resistance_air_coefficient;
	host_number = killer_entity.host_number;
	host_team_number = killer_entity.host_team_number;
	position = killer_entity.position;
	radius = killer_entity.radius;
	velocity = killer_entity.velocity;
}

KillerEntity::~KillerEntity()
{
}