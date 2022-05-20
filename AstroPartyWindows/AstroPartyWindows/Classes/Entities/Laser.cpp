#include "../Entity.h"



Laser::Laser() :
	SupportEntity(),
	properties(false),
	shoot_time(0),
	width(0.0f)
{
}

Laser::Laser(const Laser& laser) :
	SupportEntity(laser),
	properties(laser.properties),
	shoot_time(laser.shoot_time),
	width(laser.width)
{
	Update();
}

Laser::Laser(
	const ControledEntity* host,
	const Beam* local_beam,
	float width,
	GameTypes::tic_t shoot_time,
	EngineTypes::Laser::property_t properties,
	bool exist) 
	:
	SupportEntity(
		host,
		&local_beam->point,
		0.0f,
		local_beam->vector.AbsoluteAngle(), 
		exist),
	properties(properties),
	shoot_time(shoot_time),
	width(width)
{
	Update();
}

bool Laser::Collision(Map::MapData* map)
{
	Beam beam = GetBeam();
	bool collision = false;
	void* element_p;
	EngineTypes::Map::array_length_t element;

	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Map::Cyrcle*)element_p)->exist &&
			!((Map::Cyrcle*)element_p)->IsUnbreacable() &&
			((Map::Cyrcle*)element_p)->IsCollision(&beam))
		{
			((Map::Cyrcle*)element_p)->exist = false;
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Map::Polygon*)element_p)->exist &&
			!((Map::Polygon*)element_p)->IsUnbreacable() &&
			((Map::Polygon*)element_p)->IsCollision(&beam))
		{
			((Map::Polygon*)element_p)->exist = false;
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Map::Rectangle*)element_p)->exist &&
			!((Map::Rectangle*)element_p)->IsUnbreacable() &&
			((Map::Rectangle*)element_p)->IsCollision(&beam))
		{
			((Map::Rectangle*)element_p)->exist = false;
			collision = true;
		}
	}
	return collision;
}

bool Laser::IsActive() const
{
	return shoot_time > 0;
}

bool Laser::CreatedBy(ControledEntity* controled_entity)
{
	return host_number == controled_entity->GetPlayerNumber();
}

Beam Laser::GetBeam() const
{
	return Beam(position, direction);
}

GameTypes::tic_t Laser::GetLifeTime() const
{
	return shoot_time;
}

GameTypes::players_count_t Laser::GetPlayerMasterNumber() const
{
	return host_number;
}

GameTypes::players_count_t Laser::GetPlayerMasterTeamNumber() const
{
	return host_team;
}

bool Laser::GetProperty(EngineTypes::Laser::property_t property) const
{
	return (properties & property) != LASER_PROPERTY_NOTHING;
}

bool Laser::IsCollision(const Beam* beam) const
{
	return GetBeam().Distance(beam) < width;
}

bool Laser::IsCollision(const Line* line) const
{
	return GetBeam().Distance(line) < width;
}

bool Laser::IsCollision(const Segment* segment) const
{
	return GetBeam().Distance(segment) < width;
}

void Laser::Set(const Laser* laser)
{
	angle = laser->angle;
	direction = laser->direction;
	exist = laser->exist;
	host_matrix_p = laser->host_matrix_p;
	host_number = laser->host_number;
	host_p = laser->host_p;
	host_team = laser->host_team;
	last_position = laser->last_position;
	local_angle = laser->local_angle;
	local_direction = laser->local_direction;
	local_position = laser->local_position;
	position = laser->position;
	properties = laser->properties;
	shoot_time = laser->shoot_time;
	width = laser->width;
}

void Laser::Set(
	const ControledEntity* host,
	const Beam* local_beam,
	float width,
	GameTypes::tic_t shoot_time,
	bool can_create_loops,
	bool exist)
{
	UpdateDirection();
	this->properties = can_create_loops;
	this->exist = exist;
	this->host_number = host_number;
	this->host_team = host_team;
	local_angle = local_beam->vector.AbsoluteAngle();
	local_position = local_beam->point;
	shoot_time = shoot_time;
	this->width = width;
	SupportEntity::Update();
}

void Laser::Update()
{
	if (host_p == nullptr)
	{
		return;
	}
	direction = local_direction.Rotate(angle);
	position = local_position * *host_matrix_p;
	StaticEntity::Update();

	if (shoot_time > 0)
	{
		shoot_time--;
	}
}

void Laser::operator=(Laser laser)
{
	angle = laser.angle;
	direction = laser.direction;
	exist = laser.exist;
	host_matrix_p = laser.host_matrix_p;
	host_number = laser.host_number;
	host_p = laser.host_p;
	host_team = laser.host_team;
	last_position = laser.last_position;
	local_angle = laser.local_angle;
	local_direction = laser.local_direction;
	local_position = laser.local_position;
	position = laser.position;
	properties = laser.properties;
	shoot_time = laser.shoot_time;
	width = laser.width;
}

Laser::~Laser()
{
}