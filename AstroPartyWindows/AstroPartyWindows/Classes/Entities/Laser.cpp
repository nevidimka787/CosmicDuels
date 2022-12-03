#include "../Entity.h"



Laser::Laser() :
	SupportEntity(),
	end_point(position),
	properties(false),
	shoot_time(0),
	width(0.0f)
{
}

Laser::Laser(const Laser& laser) :
	SupportEntity(laser),
	end_point(laser.end_point),
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
	end_point(local_beam->point),
	properties(properties),
	shoot_time(shoot_time),
	width(width)
{
	Update();
}

bool Laser::Collision(Map::MapData* map)
{
#define MAP_ELEMENT_TYPE__NONE_TYPE	0
#define MAP_ELEMENT_TYPE__CYRCLE	1
#define MAP_ELEMENT_TYPE__POLYGON	2
#define MAP_ELEMENT_TYPE__RECTANGLE	3

	Beam beam = GetBeamForMapData();
	bool collision = false;
	void* element_p;
	EngineTypes::Map::array_length_t element;

	float min_distance = INFINITY;
	Vec2F nearest_intersect_position = position;
	void* collision_element_p = nullptr;
	uint8_t element_type = MAP_ELEMENT_TYPE__NONE_TYPE;

	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		Vec2F intersect_position;
		float distance_to_intersection;
		if (((Map::Cyrcle*)element_p)->exist &&
			((Map::Cyrcle*)element_p)->IsCollision(&beam, &intersect_position, &distance_to_intersection))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				collision_element_p = element_p;
				element_type = MAP_ELEMENT_TYPE__CYRCLE;
			}
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		Vec2F intersect_position;
		float distance_to_intersection;
		if (((Map::Polygon*)element_p)->exist &&
			((Map::Polygon*)element_p)->IsCollision(&beam, &intersect_position, &distance_to_intersection))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				collision_element_p = element_p;
				element_type = MAP_ELEMENT_TYPE__POLYGON;
			}
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		Vec2F intersect_position;
		float distance_to_intersection;
		if (((Map::Rectangle*)element_p)->exist &&
			((Map::Rectangle*)element_p)->IsCollision(&beam, &intersect_position, &distance_to_intersection))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				collision_element_p = element_p;
				element_type = MAP_ELEMENT_TYPE__RECTANGLE;
			}
		}
	}

	this->end_point = nearest_intersect_position; // If min_distance is INFINITY, end_point set to position.

	switch (element_type)
	{
	case MAP_ELEMENT_TYPE__CYRCLE:
		if (!((Map::Cyrcle*)collision_element_p)->IsUnbreacable())
		{
			((Map::Cyrcle*)collision_element_p)->exist = false;
		}
		break;
	case MAP_ELEMENT_TYPE__POLYGON:
		if (!((Map::Polygon*)collision_element_p)->IsUnbreacable())
		{
			((Map::Polygon*)collision_element_p)->exist = false;
		}
		break;
	case MAP_ELEMENT_TYPE__RECTANGLE:
		if (!((Map::Rectangle*)collision_element_p)->IsUnbreacable())
		{
			((Map::Rectangle*)collision_element_p)->exist = false;
		}
		break;
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
	std::cout << "WARNING::Laser::GetBeam::Function outdated. Use Laser::GetSegment." << std::endl;
	return Beam(position, direction);
}

Beam Laser::GetBeamForMapData() const
{
	return Beam(position, direction);
}

Segment Laser::GetSegment() const
{
	if (position == end_point)
	{
		return (Segment(position, direction * 10000.0f));
	}
	return Segment(position, end_point, true);
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
	return GetSegment().Distance(beam) < width;
}

bool Laser::IsCollision(const Line* line) const
{
	return GetSegment().Distance(line) < width;
}

bool Laser::IsCollision(const Segment* segment) const
{
	return GetSegment().Distance(segment) < width;
}

void Laser::Set(const Laser* laser)
{
	angle = laser->angle;
	direction = laser->direction;
	end_point = laser->end_point;
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
	this->end_point = local_beam->point;
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
	end_point = laser.end_point;
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