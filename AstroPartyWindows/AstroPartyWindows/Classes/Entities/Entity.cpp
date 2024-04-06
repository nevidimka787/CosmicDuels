#include "../Entity.h"



Entity::Entity() :
	angle(0.0),
	direction(Vec2F(1.0, 0.0)),
	exist(false),
	radius(0.0)
{
}

Entity::Entity(
	const Vec2F& position,
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

Entity::Entity(const Entity& entity) :
	angle(entity.angle),
	direction(Vec2F(cosf(entity.angle), sinf(entity.angle))),
	exist(entity.exist),
	position(entity.position),
	radius(entity.radius)
{

}

float Entity::GetAngle() const
{
	return angle;
}

float Entity::GetDistance(const Beam& beam) const
{
	return beam.Distance(position) - radius, 0.0f;
}

float Entity::GetDistance(const Map::Cyrcle& cyrcle, bool* is_inside) const
{
	const Vec2F& cyrcle_position(cyrcle.GetPosition());

	float distance = position.Distance(cyrcle_position) - cyrcle.GetRadius();
	if (distance < 0.0f)
	{
		if (is_inside != nullptr)
		{
			*is_inside = true;
		}
		return -distance - radius;
	}
	
	if (is_inside != nullptr)
	{
		*is_inside = false;
	}
	return distance - radius;
}

float Entity::GetDistance(const DecelerationArea& deceler_area) const
{
	return deceler_area.GetDistance(position) - radius;
}

float Entity::GetDistance(const DynamicEntity& entity) const
{
	return entity.GetDistance(position) - radius;
}

float Entity::GetDistance(const Entity& entity) const
{
	return entity.position.Distance(position) - entity.radius - radius;
}

float Entity::GetDistance(const Line& line) const
{
	return line.Distance(position) - radius;
}

float Entity::GetDistance(const Segment& segment) const
{
	return segment.Distance(position) - radius;
}

float Entity::GetDistance(const StaticEntity& entity) const
{
	return entity.GetDistance(position) - radius;
}

float Entity::GetDistance(const Map::Polygon& polygon) const
{
	return 0.0;
}

float Entity::GetDistance(const Map::Rectangle& rectangle) const
{
	Segment temp = rectangle.GetUpSide();
	float dist1 = GetDistance(temp);
	if (dist1 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle.GetDownSide();
	float dist2 = GetDistance(temp);
	if (dist2 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle.GetLeftSide();
	float dist3 = GetDistance(temp);
	if (dist3 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle.GetRightSide();
	float dist4 = GetDistance(temp);
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

float Entity::GetDistance(const Vec2F& point) const
{
	return point.Distance(position) - radius;
}

Vec2F Entity::GetDirectionNotNormalize() const
{
	return direction;
}

float Entity::GetFrameSize(const Entity& entity, float scale) const
{
	return 
		fmaxf(
			fabs((entity.position - position).x), 
			fabs((entity.position - position).y * scale))
		+ radius + entity.radius;
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

bool Entity::IsCollision(const Beam& beam) const
{
	return beam.Distance(position) <= radius;
}

bool Entity::IsCollision(const Map::Cyrcle& cyrcle) const
{
	if (cyrcle.exist == false)
	{
		return false;
	}
	Vec2F temp = cyrcle.GetPosition();
	return GetDistance(temp) < radius + cyrcle.GetRadius();
}

bool Entity::IsCollision(const DecelerationArea& deceler_area) const
{
	return GetDistance(deceler_area) <= 0.0f;
}

bool Entity::IsCollision(const DynamicEntity& dynamic_entity) const
{
	return GetDistance(dynamic_entity.position) <= dynamic_entity.radius;
}

bool Entity::IsCollision(const Entity& entity) const
{
	return GetDistance(entity.position) <= entity.radius;
}

bool Entity::IsCollision(const Line& line) const
{
	return line.Distance(position) <= radius;
}

bool Entity::IsCollision(const Laser& laser) const
{
	return laser.GetSegment().Distance(position) < radius + laser.width;
}

bool Entity::IsCollision(const Map::MapData& map) const
{
	for (uint8_t i = 0; i < map.rectangles_array_length; i++)
	{
		if (IsCollision(map.RectanglePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map.cyrcles_array_length; i++)
	{
		if (IsCollision(*map.CyrclePointer(i)))
		{
			return true;
		}
	}
	for (uint8_t i = 0; i < map.polygons_array_length; i++)
	{
		if (IsCollision(*map.PolygonPointer(i)))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(const Map::Polygon& polygon) const
{
	if (!polygon.exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon.PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment side = Segment(polygon.points_array[p_count - 1], polygon.points_array[0], true);
	if (p_count > 2 && polygon.IsClosed())
	{
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		side.Set(polygon.points_array[p - 1], polygon.points_array[p], true);
		if (side.Distance(position) < radius)
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(const Map::Rectangle& rectangle) const
{
	if (rectangle.exist == false)
	{
		return false;
	}
	Segment side;
	side = rectangle.GetUpSide();
	if (IsCollision(side))
	{
		return true;
	}
	side = rectangle.GetDownSide();
	if (IsCollision(side))
	{
		return true;
	}
	side = rectangle.GetRightSide();
	if (IsCollision(side))
	{
		return true;
	}
	side = rectangle.GetLeftSide();
	if (IsCollision(side))
	{
		return true;
	}
	return false;
}

bool Entity::IsCollision(const Segment& segment) const
{
	return segment.Distance(position) <= radius;
}

bool Entity::IsCollision(const StaticEntity& static_entity) const
{
	return GetDistance(static_entity.GetPosition()) <= static_entity.radius;
}

bool Entity::IsCollision(const Vec2F& point) const
{
	return GetDistance(point) <= 0.0f;
}

bool Entity::IsInside(const Map::Rectangle& rectangle) const
{
	const auto& p1 = rectangle.GetDownLeftPoint();
	const auto& p2 = rectangle.GetUpRightPoint();
	return
		position > rectangle.GetDownLeftPoint() &&
		position < rectangle.GetUpRightPoint();
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
	const Vec2F& position,
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

void Entity::SetAngle(float angle)
{
	this->angle = angle;
	UpdateDirection();
}

void Entity::SetDirection(const Vec2F& direction)
{
	this->direction = direction.Normalize();
}

void Entity::SetDirectionNotNormalize(const Vec2F& direction)
{
	if (direction.x != 0 || direction.y != 0)
	{
		this->direction = direction;
	}
}

void Entity::SetPosition(const Vec2F& position)
{
	this->position = position;
}

void Entity::Move(const Vec2F& delta)
{
	position += delta;
}

void Entity::UpdateAngle()
{
	angle = -atan2f(direction.y, direction.x);
}

void Entity::UpdateDirection()
{
	direction = Vec2F(cosf(angle), sinf(angle));
}

void Entity::operator=(const Entity& entity)
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