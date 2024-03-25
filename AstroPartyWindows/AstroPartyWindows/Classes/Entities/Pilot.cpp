#include "../Entity.h"



Pilot::Pilot() :
	ControledEntity(),
	respawn_timer(0)
{
}

Pilot::Pilot(const Pilot& pilot) :
	ControledEntity(pilot),
	respawn_timer(pilot.respawn_timer)
{
}

Pilot::Pilot(
	const Vec2F& position,
	const Vec2F& velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags,
	GameTypes::tic_t respawn_timer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle, EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
	:
	ControledEntity(
		position,
		velocity,
		radius,
		player_number,
		player_team_number,
		controle_flags,
		heat_box_vertexes_array,
		angle, angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	respawn_timer(respawn_timer)
{
}

Pilot::Pilot(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags,
	GameTypes::tic_t respawn_timer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle, EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
	:
	ControledEntity(
		position,
		velocity,
		radius,
		player_number,
		player_team_number,
		controle_flags,
		heat_box_vertexes_array,
		angle, angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	respawn_timer(respawn_timer)
{
}

bool Pilot::CanRespawn() const
{
	return respawn_timer == 0;
}

DynamicParticle Pilot::CreateShards(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_SHARDS_PILOT,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_SHARDS_PILOT,
		PARTICLE_POSTPONE_SHARDS_PILOT,
		current_tic + PARTICLE_PERIOD_SHARDS_PILOT);
}

GameTypes::tic_t Pilot::GetRespawnDellay() const
{
	return respawn_timer;
}

Ship Pilot::Respawn()
{
	return Ship(
		position,
		velocity,
		GetPlayerNumber(),
		GetTeamNumber(),
		GetControleFlagsP(),
		std::vector<Vec2F>(),
		angle);
}

void Pilot::Set(const Pilot* pilot)
{
	ControledEntity::Set(pilot);
	this->respawn_timer = respawn_timer;
}

void Pilot::Set(
	const Vec2F& position,
	const Vec2F& velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags,
	GameTypes::tic_t respawn_timer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle,
	EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	ControledEntity::Set(
		position,
		velocity,
		radius,
		player_number,
		player_team_number,
		controle_flags,
		heat_box_vertexes_array,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient);

	this->respawn_timer = respawn_timer;
}

void Pilot::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const GameTypes::control_flags_t* controle_flags,
	GameTypes::tic_t respawn_timer,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle,
	EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	ControledEntity::Set(
		position,
		velocity,
		radius,
		player_number,
		player_team_number,
		controle_flags,
		heat_box_vertexes_array,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient);

	this->respawn_timer = respawn_timer;
}

void Pilot::Update()
{
	DynamicEntity::Update();
	if (respawn_timer > 0)
	{
		respawn_timer--;
	}
}

void Pilot::UpdateMatrix()
{
	Mat3x2F* mm = GetModelMatrixPointer();
	mm->SetByPosition(position);
	mm->RotateThis(angle);
	mm->ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}


void Pilot::operator=(const Pilot& pilot)
{
	ControledEntity::operator=(pilot);
	this->respawn_timer = pilot.respawn_timer;
}

Pilot::~Pilot()
{
}