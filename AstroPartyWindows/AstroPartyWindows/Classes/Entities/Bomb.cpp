#include "../Entity.h"



Bomb::Bomb() 
	:
	KillerEntity(),
	animation_tic(0),
	activation_period(0),
	blinking_period(0),
	status(BOMB_STATUS_INACTIVE)
{
}

Bomb::Bomb(const Bomb& bomb) 
	:
	KillerEntity(bomb),
	activation_period(bomb.activation_period),
	animation_tic(bomb.animation_tic),
	blinking_period(bomb.blinking_period),
	status(bomb.status)
{
}

Bomb::Bomb(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t master1_team_number,
	GameTypes::players_count_t master2_team_number,
	GameTypes::tic_t animation_tic,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius,
	EngineTypes::Bomb::status_t status,
	GameTypes::tic_t activation_period,
	GameTypes::tic_t blinking_period, 
	bool exist)
	:
	KillerEntity(
		position,
		velocity,
		radius,
		master1_team_number, 
		master2_team_number,
		angle, angular_velocity, 
		force_collision_coeffisient, 
		force_resistance_air_coefficient,
		exist),
	activation_period(activation_period),
	animation_tic(animation_tic),
	blinking_period(blinking_period),
	status(status)
{
}

Bomb::Bomb(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t master1_team_number,
	GameTypes::players_count_t master2_team_number,
	GameTypes::tic_t animation_tic,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius, 
	EngineTypes::Bomb::status_t status,
	GameTypes::tic_t activation_period,
	GameTypes::tic_t blinking_period,
	bool exist) 
	:
	KillerEntity(
		position,
		velocity, 
		radius,
		master1_team_number,
		master2_team_number,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	activation_period(activation_period),
	animation_tic(animation_tic),
	blinking_period(blinking_period),
	status(status)
{
}

Bomb::Bomb(const Bullet& bullet1, const Bullet& bullet2) :
	KillerEntity(
		(bullet1.GetPosition() + bullet2.GetPosition()) / 2.0f,
		(bullet1.GetVelocity() + bullet2.GetVelocity()) / 2.0f,
		BOMB_DEFAULT_RADIUS* BOMB_BOOM_RADIUS_COEF / 3.0f,
		bullet1.GetHostTeamNumber(),
		bullet2.GetHostTeamNumber(),
		0.0f,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT),
	activation_period(75),
	animation_tic(0),
	blinking_period(75),
	status(BOMB_STATUS_BOOM)
{
}

void Bomb::Activate()
{
	if (status & BOMB_STATUS_ACTIVE)
	{
		return;
	}
	status |= BOMB_STATUS_ACTIVE;
	animation_tic = activation_period;
	blinking_period = activation_period;
}

void Bomb::Boom(GameTypes::tic_t period)
{
	if (status & BOMB_STATUS_BOOM)
	{
		return;
	}
	animation_tic = period;
	blinking_period = period;
	status &= BOMB_STATUS_ALL - BOMB_STATUS_ACTIVE;
	radius *= BOMB_BOOM_RADIUS_COEF;
	status |= BOMB_STATUS_BOOM;
}

bool Bomb::CanRemove() const
{
	return status & BOMB_STATUS_CAN_REMOVE;
}

bool Bomb::Collision(const Map::MapData& map)
{
	void* map_element;
	bool collision = false;

	if (status & BOMB_STATUS_BOOM)
	{
		for (EngineTypes::Map::array_length_t i = 0; i < map.cyrcles_array_length; i++)
		{
			map_element = (void*)map.CyrclePointer(i);
			if (((Map::Cyrcle*)map_element)->exist && !(((Map::Cyrcle*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Map::Cyrcle*)map_element))
			{
				((Map::Cyrcle*)map_element)->exist = false;
				collision = true;
			}
		}
		for (EngineTypes::Map::array_length_t i = 0; i < map.polygons_array_length; i++)
		{
			map_element = (void*)map.PolygonPointer(i);
			if (((Map::Polygon*)map_element)->exist && !(((Map::Polygon*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Map::Polygon*)map_element))
			{
				((Map::Polygon*)map_element)->exist = false;
				collision = true;
			}
		}
		for (EngineTypes::Map::array_length_t i = 0; i < map.rectangles_array_length; i++)
		{
			map_element = (void*)map.RectanglePointer(i);
			if (((Map::Rectangle*)map_element)->exist && !(((Map::Rectangle*)map_element)->Prorerties() & MAP_PROPERTY_UNBREACABLE) && DynamicEntity::IsCollision((Map::Rectangle*)map_element))
			{
				((Map::Rectangle*)map_element)->exist = false;
				collision = true;
			}
		}
		return collision;
	}

	for (EngineTypes::Map::array_length_t i = 0; i < map.cyrcles_array_length; i++)
	{
		map_element = (void*)map.CyrclePointer(i);
		if (((Map::Cyrcle*)map_element)->exist)
		{
			bool current_collision = DynamicEntity::Collision((Map::Cyrcle*)map_element);
			if (current_collision && ((Map::Cyrcle*)map_element)->IsKiller() && ((Map::Cyrcle*)map_element)->IsAggressive())
			{
				Boom();
			}
			else if (current_collision && ((Map::Cyrcle*)map_element)->IsAggressive())
			{
				Activate();
			}

			collision |= current_collision;
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map.polygons_array_length; i++)
	{
		map_element = (void*)map.PolygonPointer(i);
		if (((Map::Polygon*)map_element)->exist)
		{
			bool current_collision = DynamicEntity::Collision((Map::Polygon*)map_element);
			if (current_collision && ((Map::Polygon*)map_element)->IsKiller() && ((Map::Polygon*)map_element)->IsAggressive())
			{
				Boom();
			}
			else if (current_collision && ((Map::Polygon*)map_element)->IsAggressive())
			{
				Activate();
			}

			collision |= current_collision;
		}
	}
	for (EngineTypes::Map::array_length_t i = 0; i < map.rectangles_array_length; i++)
	{
		map_element = (void*)map.RectanglePointer(i);
		if (((Map::Rectangle*)map_element)->exist)
		{
			bool current_collision = DynamicEntity::Collision((Map::Rectangle*)map_element);
			if (current_collision && ((Map::Rectangle*)map_element)->IsKiller() && ((Map::Rectangle*)map_element)->IsAggressive())
			{
				Boom();
			}
			else if (current_collision && ((Map::Rectangle*)map_element)->IsAggressive())
			{
				Activate();
			}

			collision |= current_collision;
		}
	}
	return collision;
}

GameTypes::tic_t Bomb::GetAnimationTic() const
{
	return animation_tic;
}

bool Bomb::IsActive() const
{
	return status & BOMB_STATUS_ACTIVE;
}

bool Bomb::IsBoom() const
{
	return status & BOMB_STATUS_BOOM;
}

bool Bomb::IsCreatedByAggressiveTeam() const
{
	return host_number == AGGRESIVE_ENTITY_HOST_ID || host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByAggressiveTeamOnly() const
{
	return host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByAggressiveTeamNotOnly() const
{
	return
		host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number != AGGRESIVE_ENTITY_HOST_ID ||
		host_number != AGGRESIVE_ENTITY_HOST_ID && host_team_number == AGGRESIVE_ENTITY_HOST_ID;
}

bool Bomb::IsCreatedByTeam(const ControledEntity* host) const
{
	GameTypes::players_count_t host_team = host->GetTeamNumber();
	return host_team_number == host_team;
}

GameTypes::players_count_t Bomb::GetTeamNumber(const ControledEntity& not_host) const
{
	if (host_number == not_host.GetTeamNumber() || host_number == AGGRESIVE_ENTITY_HOST_ID)
	{
		return host_team_number;
	}
	return host_number;
}

bool Bomb::IsAggressiveFor(const ControledEntity& host) const
{
	GameTypes::players_count_t host_team = host.GetTeamNumber();
	return
		host_number == host_team && host_team_number == host_team ||
		host_number != host_team && host_team_number != host_team && host_number != host_team_number ||
		host_number == host_team && host_team_number == AGGRESIVE_ENTITY_HOST_ID ||
		host_number == AGGRESIVE_ENTITY_HOST_ID && host_team_number == host_team ||
		host_team_number == AGGRESIVE_ENTITY_HOST_ID && host_number == AGGRESIVE_ENTITY_HOST_ID;
}

void Bomb::Set(const Bomb* bomb)
{
	activation_period = bomb->activation_period;
	angle = bomb->angle;
	angular_velocity = bomb->angular_velocity;
	animation_tic = bomb->animation_tic;
	blinking_period = bomb->blinking_period;
	direction = bomb->direction;
	exist = bomb->exist;
	force_collision_coeffisient = bomb->force_collision_coeffisient;
	force_resistance_air_coefficient = bomb->force_resistance_air_coefficient;
	host_number = bomb->host_number;
	host_team_number = bomb->host_team_number;
	position = bomb->position;
	radius = bomb->radius;
	status = bomb->status;
	velocity = bomb->velocity;
}

void Bomb::Set(
	Vec2F position, 
	Vec2F velocity, 
	GameTypes::players_count_t payer_master_number,
	GameTypes::players_count_t player_master_team_number,
	GameTypes::tic_t animation_tic,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	float radius,
	EngineTypes::Bomb::status_t status,
	GameTypes::tic_t activation_period,
	GameTypes::tic_t blinking_period,
	bool exist)
{
	this->activation_period = activation_period;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	this->blinking_period = blinking_period;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = host_number;
	this->host_team_number = player_master_team_number;
	this->position = position;
	this->radius = radius;
	this->status = status;
	this->velocity = velocity;
}

void Bomb::Set(
	const Vec2F* position,
	const Vec2F* velocity, 
	GameTypes::players_count_t payer_master_number,
	GameTypes::players_count_t player_master_team_number,
	GameTypes::tic_t animation_tic,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient,
	float radius,
	EngineTypes::Bomb::status_t status,
	GameTypes::tic_t activation_period,
	GameTypes::tic_t blinking_period,
	bool exist)
{
	this->activation_period = activation_period;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->animation_tic = animation_tic;
	this->blinking_period = blinking_period;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->host_number = host_number;
	this->host_team_number = player_master_team_number;
	this->position = *position;
	this->radius = radius;
	this->status = status;
	this->velocity = *velocity;
}

void Bomb::Update()
{
	if (status & BOMB_STATUS_CAN_REMOVE)
	{
		return;
	}
	DynamicEntity::Update();
	if (status & (BOMB_STATUS_ACTIVE | BOMB_STATUS_BOOM))
	{
		if (status & BOMB_STATUS_ACTIVE)
		{
			angular_velocity += BOMB_DEFAULT_ANGULAR_VELOCITY_INCREMENTATION / (float)activation_period;
		}
		if (animation_tic == 0)
		{
			if (status & BOMB_STATUS_BOOM)
			{
				status |= BOMB_STATUS_CAN_REMOVE;
				return;
			}
			Boom();
			return;
		}
		animation_tic--;
		return;
	}
	else
	{
		if (animation_tic == 0)
		{
			animation_tic = blinking_period;
			return;
		}
		animation_tic--;
		return;
	}
}

void Bomb::operator=(Bomb bomb)
{
	activation_period = bomb.activation_period;
	angle = bomb.angle;
	angular_velocity = bomb.angular_velocity;
	animation_tic = bomb.animation_tic;
	blinking_period = bomb.blinking_period;
	direction = bomb.direction;
	exist = bomb.exist;
	force = bomb.force;
	force_collision_coeffisient = bomb.force_collision_coeffisient;
	force_resistance_air_coefficient = bomb.force_resistance_air_coefficient;
	host_number = bomb.host_number;
	host_team_number = bomb.host_team_number;
	position = bomb.position;
	radius = bomb.radius;
	status = bomb.status;
	velocity = bomb.velocity;
}

Bomb::~Bomb()
{
}