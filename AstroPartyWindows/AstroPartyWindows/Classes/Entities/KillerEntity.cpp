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
	const Vec2F& position,
	const Vec2F& velocity,
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

bool KillerEntity::CreatedBy(const ControledEntity& controled_entity) const
{
	return host_number == controled_entity.GetPlayerNumber();
}

bool KillerEntity::CreatedByTeam(const ControledEntity & controled_entity) const
{
	return host_team_number == controled_entity.GetTeamNumber();
}

GameTypes::players_count_t KillerEntity::GetHostNumber() const
{
	return host_number;
}

GameTypes::players_count_t KillerEntity::GetHostTeamNumber() const
{
	return host_team_number;
}

bool KillerEntity::SameTeam(const KillerEntity& killer_entity) const
{
	return host_team_number == killer_entity.host_team_number;
}

void KillerEntity::Set(const KillerEntity* killer_entity)
{
	DynamicEntity::Set(killer_entity);

	host_number = killer_entity->host_number;
	host_team_number = killer_entity->host_team_number;
}

void KillerEntity::Set(
	const Vec2F& position,
	const Vec2F& velocity,
	float radius,
	GameTypes::players_count_t player_master_number, 
	GameTypes::players_count_t player_master_team_number,
	float angle,
	float angular_velocity, 
	float force_collision_coeffisient,
	float force_resistance_air_coefficient, 
	bool exist)
{
	DynamicEntity::Set(
		position,
		velocity,
		radius, angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist);

	this->host_number = player_master_number;
	this->host_team_number = player_master_team_number;
}

void KillerEntity::operator=(const KillerEntity& killer_entity)
{
	DynamicEntity::operator=(killer_entity);

	host_number = killer_entity.host_number;
	host_team_number = killer_entity.host_team_number;
}

KillerEntity::~KillerEntity()
{
}