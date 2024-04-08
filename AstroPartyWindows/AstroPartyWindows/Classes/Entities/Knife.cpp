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
	const Segment& local_segment,
	EngineTypes::Knife::knife_health_t health,
	bool exist) 
	:
	SupportEntity(
		host,
		local_segment.point,
		local_segment.vector.Length(),
		local_segment.vector.GetAngleClockwise(),
		exist),
	health(health)
{
	Update();
}

bool Knife::Collision(Map::MapData& map)
{
	if (health == 0)
	{
		exist = false;
		return false;
	}
	const Segment& segment = GetSegment();
	bool collision = false;

	for (auto& element : map.cyrcles_array)
	{
		collision |= CollissionWithElement(element, segment);
		if (health <= 0)
		{
			exist = false;
			return true;
		}
	}
	for (auto& element : map.polygons_array)
	{
		collision |= CollissionWithElement(element, segment);
		if (health <= 0)
		{
			exist = false;
			return true;
		}
	}
	for (auto& element : map.rectangles_array)
	{
		collision |= CollissionWithElement(element, segment);
		if (health <= 0)
		{
			exist = false;
			return true;
		}
	}
	return collision;
}

template <typename ElemetT>
bool Knife::CollissionWithElement(ElemetT& element, const Segment& segment)
{
	if (element.exist &&
		!element.IsUnbreacable() &&
		element.IsCollision(segment))
	{
		element.exist = false;
		health--;
		return true;
	}

	return false;
}

Segment Knife::GetSegment() const
{
	return Segment(position, direction * radius);
}

bool Knife::IsCollision(const Line& line) const
{
	return GetSegment().IsIntersection(line);
}

bool Knife::IsCollision(const Beam& beam) const
{
	return GetSegment().IsIntersection(beam);
}

bool Knife::IsCollision(const Segment& segment) const
{
	return GetSegment().IsIntersection(segment);
}

void Knife::Set(const Knife* knife)
{
	SupportEntity::Set(knife);

	health = knife->health;
}

void Knife::Set(
	const ControledEntity* host_p,
	const Segment& local_segment,
	EngineTypes::Knife::knife_health_t health,
	bool exist)
{
	this->exist = exist;
	this->health = health;
	this->host_matrix_p = host_p->GetModelMatrixPointerConst();
	this->host_p = host_p;
	this->host_number = host_p->GetPlayerNumber();
	this->host_team = host_p->GetTeamNumber();
	this->local_direction = local_segment.vector;
	this->local_angle = local_direction.GetAngle();
	this->local_position = local_segment.point;
	this->radius = local_segment.vector.Length();
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

void Knife::operator=(const Knife& knife)
{
	SupportEntity::operator=(knife);

	health = knife.health;
}

Knife::~Knife()
{
}