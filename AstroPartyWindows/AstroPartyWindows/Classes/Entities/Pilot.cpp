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
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	GameTypes::tic_t respawn_timer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle, EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	GameTypes::tic_t unbrakable,
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
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		heat_box_vertexes_array,
		heat_box_vertexes_array_length,
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
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	GameTypes::tic_t respawn_timer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle, EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	GameTypes::tic_t unbrakable,
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
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		heat_box_vertexes_array,
		heat_box_vertexes_array_length,
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
		player_number,
		player_team_number,
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		nullptr, 
		0,
		angle);
}

void Pilot::Set(const Pilot* pilot)
{
	angle = pilot->angle;
	angular_velocity = pilot->angular_velocity;
	direction = pilot->direction;
	exist = pilot->exist;
	force = pilot->force;
	heat_box_vertexes_array_length = pilot->heat_box_vertexes_array_length;
	player_number = pilot->player_number;
	player_team_number = pilot->player_team_number;
	position = pilot->position;
	radius = pilot->radius;
	respawn_timer = pilot->respawn_timer;
	burnout_input_value_pointer = pilot->burnout_input_value_pointer;
	rotate_input_value_pointer = pilot->rotate_input_value_pointer;
	shoot_input_value_pointer = pilot->shoot_input_value_pointer;
	velocity = pilot->velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = pilot->heat_box_vertexes_array[vertex];
	}
}

void Pilot::Set(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	GameTypes::tic_t respawn_timer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	GameTypes::tic_t unbrakable, float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force = force;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = position;
	this->radius = radius;
	this->respawn_timer = respawn_timer;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->velocity = velocity;

	delete[] this->heat_box_vertexes_array;
	this->heat_box_vertexes_array = new Vec2F[this->heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < this->heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
}

void Pilot::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	GameTypes::tic_t respawn_timer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
	float angle,
	EngineTypes::Bonus::inventory_t buffs_bonuses,
	EngineTypes::Bonus::inventory_t active_baffs,
	GameTypes::tic_t unbrakable,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	UpdateDirection();
	this->exist = exist;
	this->force = force;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = *position;
	this->radius = radius;
	this->respawn_timer = respawn_timer;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->velocity = *velocity;

	delete[] this->heat_box_vertexes_array;
	this->heat_box_vertexes_array = new Vec2F[this->heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < this->heat_box_vertexes_array_length; vertex++)
	{
		this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
	}
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
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}


void Pilot::operator=(Pilot pilot)
{
	angle = pilot.angle;
	angular_velocity = pilot.angular_velocity;
	direction = pilot.direction;
	exist = pilot.exist;
	force = pilot.force;
	heat_box_vertexes_array_length = pilot.heat_box_vertexes_array_length;
	player_number = pilot.player_number;
	player_team_number = pilot.player_team_number;
	position = pilot.position;
	radius = pilot.radius;
	respawn_timer = pilot.respawn_timer;
	rotate_input_value_pointer = pilot.rotate_input_value_pointer;
	shoot_input_value_pointer = pilot.shoot_input_value_pointer;
	velocity = pilot.velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = pilot.heat_box_vertexes_array[vertex];
	}
}

Pilot::~Pilot()
{
}