#include "../Entity.h"



Laser::Laser() :
	SupportEntity(),
	end_point(position),
	properties(false),
	shoot_time(0),
	width(0.0f),
	reflections_count(0)
{
}

Laser::Laser(const Laser& laser) :
	SupportEntity(laser),
	end_point(laser.end_point),
	properties(laser.properties),
	shoot_time(laser.shoot_time),
	width(laser.width),
	reflections_count(laser.reflections_count)
{
	Update();
}

Laser::Laser(
	const ControledEntity* host,
	const Beam& local_beam,
	float width,
	GameTypes::tic_t shoot_time,
	EngineTypes::Laser::property_t properties,
	size_t reflections_count,
	bool exist)
	:
	SupportEntity(
		host,
		local_beam.point,
		0.0f,
		local_beam.vector.GetAngleClockwise(),
		exist),
	end_point(local_beam.point),
	properties(properties),
	shoot_time(shoot_time),
	width(width),
	reflections_count(reflections_count)
{
	Update();
}

bool Laser::Collision(Map::MapData& map, Vec2F& nearest_intersect_position, Vec2F& nearest_perpendicular_direction)
{
#define MAP_ELEMENT_TYPE__NONE_TYPE	0
#define MAP_ELEMENT_TYPE__CYRCLE	1
#define MAP_ELEMENT_TYPE__POLYGON	2
#define MAP_ELEMENT_TYPE__RECTANGLE	3

	Beam beam(position, direction);
	bool collision = false;
	EngineTypes::Map::array_length_t element;

	float min_distance = INFINITY;
	void* collision_element_p = nullptr;
	uint8_t element_type = MAP_ELEMENT_TYPE__NONE_TYPE;

	for (element = 0; element < map.cyrcles_array_length; element++)
	{
		const auto element_p = map.CyrclePointer(element);
		Vec2F intersect_position;
		float distance_to_intersection;
		Vec2F perpendicular_position;
		if (element_p->exist &&
			element_p->IsCollision(&beam, &intersect_position, &distance_to_intersection, &perpendicular_position))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				nearest_perpendicular_direction = perpendicular_position;
				collision_element_p = element_p;
				element_type = MAP_ELEMENT_TYPE__CYRCLE;
			}
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map.polygons_array_length; element++)
	{
		const auto element_p = map.PolygonPointer(element);
		Vec2F intersect_position;
		Vec2F perpendicular_position;
		float distance_to_intersection;
		if (element_p->exist &&
			element_p->IsCollision(beam, &intersect_position, &distance_to_intersection, &perpendicular_position))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				nearest_perpendicular_direction = perpendicular_position;
				collision_element_p = element_p;
				element_type = MAP_ELEMENT_TYPE__POLYGON;
			}
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map.rectangles_array_length; element++)
	{
		const auto element_p = map.RectanglePointer(element);
		Vec2F intersect_position;
		Vec2F perpendicular_position;
		float distance_to_intersection;
		if (element_p->exist &&
			element_p->IsCollision(&beam, &intersect_position, &distance_to_intersection, &perpendicular_position))
		{
			collision = true;
			if (distance_to_intersection < min_distance)
			{
				min_distance = distance_to_intersection;
				nearest_intersect_position = intersect_position;
				nearest_perpendicular_direction = perpendicular_position;
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

bool Laser::CreatedBy(const ControledEntity& controled_entity) const
{
	return host_number == controled_entity.GetPlayerNumber();
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

GameTypes::tic_t Laser::GetShootTime() const
{
	return shoot_time;
}

bool Laser::IsCollision(const Beam* beam) const
{
	return GetSegment().Distance(beam) < width;
}

bool Laser::IsCollision(const Line* line) const
{
	return GetSegment().Distance(*line) < width;
}

bool Laser::IsCollision(const Segment* segment) const
{
	return GetSegment().Distance(segment) < width;
}

void Laser::Set(const Laser* laser)
{
	SupportEntity::Set(laser);

	end_point = laser->end_point;
	properties = laser->properties;
	reflections_count = laser->reflections_count;
	shoot_time = laser->shoot_time;
	width = laser->width;
}

void Laser::Set(
	const ControledEntity* host_p,
	const Beam& local_beam,
	float width,
	GameTypes::tic_t shoot_time,
	bool can_create_loops,
	size_t reflections_count,
	bool exist)
{
	UpdateDirection();
	this->properties = can_create_loops;
	this->end_point = local_beam.point;
	this->exist = exist;
	this->host_number = host_p->GetPlayerNumber();
	this->host_team = host_p->GetTeamNumber();
	this->local_angle = local_beam.vector.GetAngle();
	this->local_position = local_beam.point;
	this->shoot_time = shoot_time;
	this->width = width;
	this->reflections_count = reflections_count;
	SupportEntity::Update();
}

void Laser::Update()
{
	if (host_p == nullptr)
	{
		return;
	}
	if (!(properties & LASER_PROPERTY_FREE_FROM_HOST))
	{
		position = local_position * *host_matrix_p;
	}
	else
	{
		direction.Set(local_direction.x, local_direction.y);
		position = local_position;
	}
	StaticEntity::Update();

	if (shoot_time > 0)
	{
		shoot_time--;
	}
}

void Laser::operator=(const Laser& laser)
{
	SupportEntity::operator=(laser);

	end_point = laser.end_point;
	properties = laser.properties;
	shoot_time = laser.shoot_time;
	width = laser.width;
	reflections_count = laser.reflections_count;
}

Laser::~Laser()
{
}