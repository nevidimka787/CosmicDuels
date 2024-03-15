#include "../Entity.h"



Knife::Knife() :
	SupportEntity(),
	health(KNIFE_DEFAULT_HEALTH)
{
	Update();
}

Knife::Knife(const Knife& knife) :
	SupportEntity(knife),
	health(knife.health)
{
	Update();
}

Knife::Knife(
	const ControledEntity* host,
	const Segment* local_segment,
	EngineTypes::Knife::knife_health_t health,
	bool exist) 
	:
	SupportEntity(
		host,
		&local_segment->point,
		local_segment->vector.Length(),
		local_segment->vector.GetAngleClockwise(),
		exist),
	health(health)
{
	Update();
}

bool Knife::Collision(Map::MapData* map)
{
	if (health == 0)
	{
		exist = false;
		return false;
	}
	Segment segment = GetSegment();
	bool collision = false;
	void* element_p;
	EngineTypes::Map::array_length_t element;

	for (element = 0; element < map->cyrcles_array_length; element++)
	{
		element_p = (void*)map->CyrclePointer(element);
		if (((Map::Cyrcle*)element_p)->exist &&
			!((Map::Cyrcle*)element_p)->IsUnbreacable() &&
			((Map::Cyrcle*)element_p)->IsCollision(&segment))
		{
			((Map::Cyrcle*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->polygons_array_length; element++)
	{
		element_p = (void*)map->PolygonPointer(element);
		if (((Map::Polygon*)element_p)->exist &&
			!((Map::Polygon*)element_p)->IsUnbreacable() &&
			((Map::Polygon*)element_p)->IsCollision(&segment))
		{
			((Map::Polygon*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t element = 0; element < map->rectangles_array_length; element++)
	{
		element_p = (void*)map->RectanglePointer(element);
		if (((Map::Rectangle*)element_p)->exist &&
			!((Map::Rectangle*)element_p)->IsUnbreacable() &&
			((Map::Rectangle*)element_p)->IsCollision(&segment))
		{
			((Map::Rectangle*)element_p)->exist = false;
			health--;
			if (health == 0)
			{
				exist = false;
				return true;
			}
			collision = true;
		}
	}
	return collision;
}

Segment Knife::GetSegment() const
{
	return Segment(position, direction * radius);
}

void Knife::Set(const Knife* knife)
{
	angle = knife->angle;
	direction = knife->direction;
	exist = knife->exist;
	health = knife->health;
	host_matrix_p = knife->host_matrix_p;
	host_number = knife->host_number;
	host_p = knife->host_p;
	host_team = knife->host_team;
	last_position = knife->last_position;
	local_angle = knife->local_angle;
	local_direction = knife->local_direction;
	local_position = knife->local_position;
	position = knife->position;
	radius = knife->radius;
}

void Knife::Set(
	const ControledEntity* host,
	const Segment* local_segment,
	EngineTypes::Knife::knife_health_t health,
	bool exist)
{
	this->exist = exist;
	this->health = health;
	host_matrix_p = host->GetModelMatrixPointer();
	host_p = host;
	host_number = host->GetPlayerNumber();
	host_team = host->GetTeamNumber();
	local_direction = local_segment->vector;
	local_angle = local_direction.GetAngle();
	local_position = local_segment->point;
	radius = local_segment->vector.Length();
	SupportEntity::Update();
}

bool Knife::LoseHealth()
{
	if (health > 1)
	{
		health--;
		return true;
	}
	if (health == 1)
	{
		health--;
		return false;
	}
	return false;
}

void Knife::operator=(Knife knife)
{
	angle = knife.angle;
	direction = knife.direction;
	exist = knife.exist;
	health = knife.health;
	host_matrix_p = knife.host_matrix_p;
	host_number = knife.host_number;
	host_p = knife.host_p;
	host_team = knife.host_team;
	last_position = knife.last_position;
	local_angle = knife.local_angle;
	local_direction = knife.local_direction;
	local_position = knife.local_position;
	position = knife.position;
	radius = knife.radius;
}

Knife::~Knife()
{
}