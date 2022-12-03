#include "../Entity.h"



Entity::Entity() :
	angle(0.0),
	direction(Vec2F(1.0, 0.0)),
	exist(false),
	radius(0.0)
{
}

Entity::Entity(
	Vec2F position,
	float radius,
	float angle,
	bool exist) 
	:
	angle(angle),
	direction(Vec2F(cosf(angle), -sinf(angle))),
	exist(exist),
	position(position),
	radius(radius)
{
}

Entity::Entity(
	const Vec2F* position,
	float radius, 
	float angle,
	bool exist) 
	:
	angle(angle),
	direction(Vec2F(cosf(angle), -sinf(angle))),
	exist(exist),
	position(*position),
	radius(radius)
{
}

Entity::Entity(const Entity& entity) :
	angle(entity.angle),
	direction(Vec2F(cosf(entity.angle), -sinf(entity.angle))),
	exist(entity.exist),
	position(entity.position),
	radius(entity.radius)
{

}

float Entity::GetAngle() const
{
	return angle;
}

float Entity::GetDistance(const Beam* beam) const
{
	return beam->Distance(&position) - radius, 0.0f;
}

float Entity::GetDistance(const Map::Cyrcle* cyrcle, bool* is_inside) const
{
	Vec2F temp = cyrcle->GetPosition();
	float dist = GetDistance(&temp) - cyrcle->GetRadius();
	if (dist < -2.0f * radius)
	{
		if (is_inside != nullptr)
		{
			*is_inside = true;
		}
		dist = cyrcle->GetRadius() - temp.Distance(&position) - 3.0f * radius;
	}
	else if (is_inside != nullptr)
	{
		*is_inside = false;
	}
	return dist;
}

float Entity::GetDistance(const DecelerationArea* deceler_area) const
{
	return deceler_area->GetDistance(position) - radius;
}

float Entity::GetDistance(const DynamicEntity* entity) const
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(const Entity* entity) const
{
	return entity->position.Distance(&position) - entity->radius - radius;
}

float Entity::GetDistance(const Line* line) const
{
	return line->Distance(&position) - radius;
}

float Entity::GetDistance(const Segment* segment) const
{
	return segment->Distance(&position) - radius;
}

float Entity::GetDistance(const StaticEntity* entity) const
{
	return entity->GetDistance(&position) - radius;
}

float Entity::GetDistance(const Map::Polygon* polygon) const
{
	return 0.0;
}

float Entity::GetDistance(const Map::Rectangle* rectangle) const
{
	Segment temp = rectangle->UpSide();
	float dist1 = GetDistance(&temp);
	if (dist1 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->DownSide();
	float dist2 = GetDistance(&temp);
	if (dist2 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->LeftSide();
	float dist3 = GetDistance(&temp);
	if (dist3 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->RightSide();
	float dist4 = GetDistance(&temp);
	if (dist4 == 0.0)
	{
		return 0.0;
	}
	if (dist1 < dist2)
	{
		if (dist3 < dist4)
		{
			if (dist1 < dist3)
			{
				return dist1;
			}
			return dist3;
		}
		if (dist1 < dist4)
		{
			return dist1;
		}
		return dist4;
	}
	if (dist3 < dist4)
	{
		if (dist2 < dist3)
		{
			return dist2;
		}
		return dist3;
	}
	if (dist2 < dist4)
	{
		return dist2;
	}
	return dist4;
}

float Entity::GetDistance(Vec2F point) const
{
	return point.Distance(&position) - radius;
}

float Entity::GetDistance(const Vec2F* point) const
{
	return point->Distance(&position) - radius;
}

Vec2F Entity::GetDirectionNotNormalize() const
{
	return direction;
}

float Entity::GetFrameSize(Entity* entity, float scale) const
{
	return fmaxf(fabs((entity->position - position).x), fabs((entity->position - position).y * scale)) + radius + entity->radius;
}

Vec2F Entity::GetDirection() const
{
	return direction.Normalize();
}

Vec2F Entity::GetPosition() const
{
	return position;
}

const Vec2F* Entity::GetPositionPointer() const
{
	return &position;
}

bool Entity::IsCollision(Beam* beam) const
{
	return beam->Distance(position) <= radius;
}

bool Entity::IsCollision(Map::Cyrcle* cyrcle) const
{
	if (cyrcle->exist == false)
	{
		return false;
	}
	Vec2F temp = cyrcle->GetPosition();
	return GetDistance(&temp) < radius + cyrcle->GetRadius();
}

bool Entity::IsCollision(DecelerationArea* deceler_area) const
{
	return GetDistance(deceler_area) <= 0.0f;
}

bool Entity::IsCollision(DynamicEntity* dynamic_entity) const
{
	return GetDistance(&dynamic_entity->position) <= dynamic_entity->radius;
}

bool Entity::IsCollision(Entity* entity) const
{
	return GetDistance(&entity->position) <= entity->radius;
}

bool Entity::IsCollision(Line* line) const
{
	return line->Distance(&position) <= radius;
}

bool Entity::IsCollision(Laser* laser) const
{
	return laser->GetSegment().Distance(position) < radius + laser->width;
}

bool Entity::IsCollision(MegaLaser* mega_laser) const
{
	Line perpendicular = Line(position, mega_laser->GetDirection().Perpendicular());
	Segment segment = mega_laser->GetSegment();
	Segment start = Segment(
		segment.point + mega_laser->GetDirection().PerpendicularClockwise() * mega_laser->width,
		mega_laser->GetDirection().Perpendicular() * mega_laser->width * 2.0f);
	Vec2F intersect;
	return
		segment.Intersection(&perpendicular, &intersect) && intersect.Distance(position) < radius + mega_laser->width ||
		start.Distance(position) < radius ||
		Segment(start.point + segment.vector, start.vector).Distance(position) < radius;
}

bool Entity::IsCollision(Map::MapData* map) const
{
	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		if (IsCollision(map->RectanglePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		if (IsCollision(map->CyrclePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		if (IsCollision(map->PolygonPointer(i)))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Map::Polygon* polygon) const
{
	if (!polygon->exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon->PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment side = Segment(polygon->points_array[p_count - 1], polygon->points_array[0], true);
	if (p_count > 2 && polygon->IsClosed())
	{
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		side.Set(polygon->points_array[p - 1], polygon->points_array[p], true);
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Map::Rectangle* rectangle) const
{
	if (rectangle->exist == false)
	{
		return false;
	}
	Segment side;
	side = rectangle->UpSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->DownSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->RightSide();
	if (IsCollision(&side))
	{
		return true;
	}
	side = rectangle->LeftSide();
	if (IsCollision(&side))
	{
		return true;
	}
	return false;
}

bool Entity::IsCollision(Segment* segment) const
{
	return segment->Distance(&position) <= radius;
}

bool Entity::IsCollision(StaticEntity* static_entity) const
{
	return GetDistance(static_entity->GetPosition()) <= static_entity->radius;
}

bool Entity::IsCollision(Vec2F point) const
{
	return GetDistance(point) <= 0.0f;
}

bool Entity::IsCollision(Vec2F* point) const
{
	return GetDistance(point) <= 0.0f;
}

void Entity::Rotate(float angle)
{
	this->angle += angle;
	UpdateDirection();
}

void Entity::Set(const Entity* entity)
{
	angle = entity->angle;
	direction = entity->direction;
	exist = entity->exist;
	position = entity->position;
	radius = entity->radius;
}

void Entity::Set(
	Vec2F position,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = position;
	this->radius = radius;
}

void Entity::Set(
	const Vec2F* position,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
}

void Entity::SetAngle(float angle)
{
	this->angle = angle;
	UpdateDirection();
}

void Entity::SetDirection(Vec2F direction)
{
	this->direction = direction.Normalize();
}

void Entity::SetDirection(Vec2F* direction)
{
	this->direction = direction->Normalize();
}

void Entity::SetDirectionNotNormalize(Vec2F direction)
{
	if (direction.x != 0 || direction.y != 0)
	{
		this->direction = direction;
	}
}

void Entity::SetDirectionNotNormalize(Vec2F* direction)
{
	if (direction->x != 0 || direction->y != 0)
	{
		this->direction = *direction;
	}
}

void Entity::SetPosition(Vec2F position)
{
	this->position = position;
}

void Entity::SetPosition(const Vec2F* position)
{
	this->position.Set(position);
}

void Entity::Move(Vec2F delta)
{
	position += delta;
}

void Entity::Move(Vec2F* delta)
{
	position += *delta;
}

void Entity::UpdateAngle()
{
	angle = -atan2f(direction.y, direction.x);
}

void Entity::UpdateDirection()
{
	direction = Vec2F(1.0f, 0.0f).Rotate(angle);
}

void Entity::operator=(Entity entity)
{
	angle = entity.angle;
	direction = entity.direction;
	exist = entity.exist;
	position = entity.position;
	radius = entity.radius;
}

Entity::~Entity()
{
}