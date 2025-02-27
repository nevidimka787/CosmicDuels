#include "Map.h"

#pragma warning(disable : 6384)
#pragma warning(disable : 6385)

using namespace Map;

MapElement::MapElement() :
	exist(true),
	position(),
	last_position(),
	properties(MAP_DEFAULT_PROPERTIES)
{
}

MapElement::MapElement(const MapElement& map_element) :
	exist(map_element.exist),
	position(map_element.position),
	last_position(map_element.last_position),
	properties(map_element.properties)
{
}

MapElement::MapElement(const Vec2F& position, EngineTypes::Map::property_t properties, bool exist) :
	position(position),
	exist(exist),
	last_position(position),
	properties(properties)
{
}

Vec2F MapElement::GetPosition() const
{
	return position;
}

Vec2F MapElement::GetVelocity() const
{
	return position - last_position;
}

bool MapElement::HasAllProrerties(EngineTypes::Map::property_t obtained_properties) const
{
	return (properties & obtained_properties) == obtained_properties;
}

bool MapElement::HasNoProrerties(EngineTypes::Map::property_t obtained_properties) const
{
	return (~properties & obtained_properties) == obtained_properties;
}

bool MapElement::HasSomeProrerties(EngineTypes::Map::property_t obtained_properties) const
{
	return properties & obtained_properties;
}

bool MapElement::IsAggressive() const
{
	return properties & MAP_PROPERTY_AGRESSIVE;
}

bool MapElement::IsKiller() const
{
	return properties & MAP_PROPERTY_KILLER;
}

bool MapElement::IsCheckCollisionsInside() const
{
	return properties & MAP_PROPERTY_COLLIDE_INSIDE;
}

bool MapElement::IsCheckCollisionsOutside() const
{
	return properties & MAP_PROPERTY_COLLIDE_OUTSIDE;
}

bool MapElement::IsUnbreacable() const
{
	return properties & MAP_PROPERTY_UNBREACABLE;
}

void MapElement::Move(const Vec2F& move_vector)
{
	position += move_vector;
}

EngineTypes::Map::property_t MapElement::Prorerties() const
{
	return properties;
}

void MapElement::Update()
{
	last_position = position;
}

void MapElement::Set(const MapElement* map_element)
{
	exist = map_element->exist;
	position = map_element->position;
	last_position = map_element->last_position;
	properties = map_element->properties;
}

void MapElement::Set(const Vec2F& position, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->position = position;
	this->properties = properties;
}

void MapElement::RessetVelocity()
{
	last_position = position;
}

void MapElement::SetPosition(const Vec2F& position)
{
	this->position = position;
}

void MapElement::SetProperties(EngineTypes::Map::property_t properties)
{
	this->properties = properties;
}

void MapElement::operator=(const MapElement& map_element)
{
	exist = map_element.exist;
	position = map_element.position;
	last_position = map_element.last_position;
	properties = map_element.properties;
}

MapElement::~MapElement()
{
}



Rectangle::Rectangle() :
	MapElement()
{
}

Rectangle::Rectangle(const Rectangle& rectangle) :
	MapElement(rectangle),
	point2(rectangle.point2)
{
}

Rectangle::Rectangle(const Segment& diagonal, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(diagonal.point, properties, exist),
	point2(diagonal.point + diagonal.vector)
{
	NormaliseThis();
}

Rectangle::Rectangle(Vec2F point1, Vec2F point2, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(point1, properties, exist),
	point2(point2)
{
	NormaliseThis();
}

Vec2F Rectangle::GetUpRightPoint() const
{
	return position;
}

Vec2F Rectangle::GetDownRightPoint() const
{
	return Vec2F(position.x, point2.y);
}

Vec2F Rectangle::GetDownLeftPoint() const
{
	return point2;
}

Vec2F Rectangle::GetUpLeftPoint() const
{
	return Vec2F(point2.x, position.y);
}

Segment Rectangle::GetUpSide() const
{
	return Segment(position, Vec2F(point2.x, position.y), true);
}

Segment Rectangle::GetDownSide() const
{
	return Segment(Vec2F(position.x, point2.y), point2, true);
}

Segment Rectangle::GetRightSide() const
{
	return Segment(position, Vec2F(position.x, point2.y), true);
}

Segment Rectangle::GetLeftSide() const
{
	return Segment(Vec2F(point2.x, position.y), point2, true);
}

Vec2F Rectangle::GetSize() const
{
	return point2 - position;
}

Vec2F Rectangle::GetPosition() const
{
	return (point2 + position) / 2.0f;
}

bool Rectangle::IsCollision(const Beam& beam)
{
	if (GetUpSide().IsIntersection(beam))
	{
		return true;
	}
	if (GetRightSide().IsIntersection(beam))
	{
		return true;
	}
	if (GetDownSide().IsIntersection(beam))
	{
		return true;
	}
	if (GetLeftSide().IsIntersection(beam))
	{
		return true;
	}
	return false;
}

bool Rectangle::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position) const
{
	Vec2F output_intersection_point;
	float min_distance = INFINITY;
	if (GetUpSide().Intersection(beam, out_position))
	{
		min_distance = (*out_position - beam.point).Length();
	}
	if (GetRightSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
		}
	}
	if (GetDownSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
		}
	}
	if (GetLeftSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
		}
	}

	if (min_distance != INFINITY)
	{
		*distance_to_out_position = min_distance;
		return true;
	}

	return false;
}

bool Rectangle::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position, Vec2F* perpendicular_position) const
{
	Vec2F output_intersection_point;
	float min_distance = INFINITY;
	if (GetUpSide().Intersection(beam, out_position))
	{
		min_distance = (*out_position - beam.point).Length();
		const Vec2F& side_vec = GetUpSide().vector;
		*perpendicular_position = side_vec.VecMul(beam.vector) > 0.0f ? side_vec.Perpendicular() : side_vec.PerpendicularClockwise();
	}
	if (GetRightSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
			const Vec2F& side_vec = GetRightSide().vector;
			*perpendicular_position = side_vec.VecMul(beam.vector) > 0.0f ? side_vec.Perpendicular() : side_vec.PerpendicularClockwise();
		}
	}
	if (GetDownSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
			const Vec2F& side_vec = GetDownSide().vector;
			*perpendicular_position = side_vec.VecMul(beam.vector) > 0.0f ? side_vec.Perpendicular() : side_vec.PerpendicularClockwise();
		}
	}
	if (GetLeftSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam.point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
			const Vec2F& side_vec = GetLeftSide().vector;
			*perpendicular_position = side_vec.VecMul(beam.vector) > 0.0f ? side_vec.Perpendicular() : side_vec.PerpendicularClockwise();
		}
	}

	if (min_distance != INFINITY)
	{
		*distance_to_out_position = min_distance;
		return true;
	}

	return false;
}

bool Rectangle::IsCollision(const Line& line)
{
	if (GetUpSide().IsIntersection(line))
	{
		return true;
	}
	if (GetRightSide().IsIntersection(line))
	{
		return true;
	}
	if (GetDownSide().IsIntersection(line))
	{
		return true;
	}
	if (GetLeftSide().IsIntersection(line))
	{
		return true;
	}
	return false;
}

bool Rectangle::IsCollision(const Segment& segment)
{
	if (GetUpSide().IsIntersection(segment))
	{
		return true;
	}
	if (GetRightSide().IsIntersection(segment))
	{
		return true;
	}
	if (GetDownSide().IsIntersection(segment))
	{
		return true;
	}
	if (GetLeftSide().IsIntersection(segment))
	{
		return true;
	}
	return false;
}

void Rectangle::Move(const Vec2F& move_vector)
{
	MapElement::Move(move_vector);
	point2 += move_vector;
}

Rectangle Rectangle::Normalise() const
{
	//up right point
	Vec2F temp_point1;


	if (position.x > point2.x)
	{
		if (position.y > point2.y)
		{
			return Rectangle(
				Vec2F(position.x, position.y),
				Vec2F(point2.x, point2.y));
		}
		return Rectangle(
			Vec2F(position.x, point2.y),
			Vec2F(point2.x, position.y));
	}
	if (position.y > point2.y)
	{
		return Rectangle(
			Vec2F(point2.x, position.y),
			Vec2F(position.x, point2.y));
	}
	return Rectangle(
		Vec2F(point2.x, point2.y),
		Vec2F(position.x, position.y));
}

void Rectangle::NormaliseThis()
{
	//up right point
	Vec2F temp_point1;

	if (position.x > point2.x)
	{
		if (position.y > point2.y)
		{
			temp_point1.Set(position.x, position.y);
			point2.Set(point2.x, point2.y);
		}
		else
		{
			temp_point1.Set(position.x, point2.y);
			point2.Set(point2.x, position.y);
		}
	}
	else if (position.y > point2.y)
	{
		temp_point1.Set(point2.x, position.y);
		point2.Set(position.x, point2.y);
	}
	else
	{
		temp_point1.Set(point2.x, point2.y);
		point2.Set(position.x, position.y);
	}
	position = temp_point1;
}

Vec2F Rectangle::Position() const
{
	return (position + point2) / 2.0f;
}

void Rectangle::Set(const Rectangle* rectangle)
{
	MapElement::Set(rectangle);
	point2 = rectangle->point2;
}

void Rectangle::Set(const Segment& diagonal, EngineTypes::Map::property_t properties, bool exist)
{
	MapElement::Set(diagonal.point, properties, exist);
	this->point2 = diagonal.SecondPoint();

	NormaliseThis();
}

void Rectangle::SetPosition(const Vec2F& position)
{
	Vec2F point_to_position = (this->position - point2) / 2.0f;
	this->position = position + point_to_position;
	point2 = position - point_to_position;
}

void Rectangle::SetSize(const Vec2F& size)
{
	Vec2F center = (this->position + point2) / 2.0f;
	position = center - size;
	point2 = center + size;
}

void Rectangle::operator=(const Rectangle& rectangle)
{
	MapElement::operator=(rectangle);
	point2 = rectangle.point2;
}

Rectangle::~Rectangle()
{
}

Cyrcle::Cyrcle() :
	MapElement(),
	radius(CYRCLE_DEFAULT_RADIUS)
{

}

Cyrcle::Cyrcle(const Cyrcle& cyrcle) :
	MapElement(cyrcle),
	radius(cyrcle.radius)
{
}

Cyrcle::Cyrcle(const Vec2F& position, float radius, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	radius(radius)
{
}

bool Cyrcle::IsCollision(const Beam& beam) const
{
	return beam.Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position) const
{
	Vec2F neares_point_on_line;

	float distance = Line(beam.point, beam.vector).Distance(position, &neares_point_on_line);

	if (distance > radius)
	{
		return false;
	}

	float sub_vector_length = sqrtf(radius * radius - (neares_point_on_line - position).LengthPow2());

	*out_position =
		(beam.point.Distance(position) > radius) ?
		(neares_point_on_line - beam.vector.Normalize() * sub_vector_length) :
		(neares_point_on_line + beam.vector.Normalize() * sub_vector_length);

	*distance_to_out_position = beam.point.Distance(*out_position);

	return true;
}

bool Cyrcle::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position, Vec2F* perpendicular_direction) const
{
	Vec2F neares_point_on_line;

	float distance = Line(beam.point, beam.vector).Distance(position, &neares_point_on_line);

	if (distance > radius)
	{
		return false;
	}

	float sub_vector_length = sqrtf(radius * radius - (neares_point_on_line - position).LengthPow2());

	*out_position =
		(beam.point.Distance(position) > radius) ?
		(neares_point_on_line - beam.vector.Normalize() * sub_vector_length) :
		(neares_point_on_line + beam.vector.Normalize() * sub_vector_length);

	*distance_to_out_position = beam.point.Distance(*out_position);

	*perpendicular_direction = *out_position - position;

	return true;
}

bool Cyrcle::IsCollision(const Line& line) const
{
	return line.Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Segment& segment) const
{
	return segment.Distance(position) < radius;
}

float Cyrcle::GetRadius() const
{
	return radius;
}

void Cyrcle::SetRadius(float radius)
{
	this->radius = radius;
}

void Cyrcle::Set(const Cyrcle* cyrcle)
{
	MapElement::Set(cyrcle);
	radius = cyrcle->radius;
}

void Cyrcle::Set(const Vec2F& position, float radius, EngineTypes::Map::property_t properties, bool exist)
{
	MapElement::Set(position, properties, exist);
	this->radius = radius;
}

void Cyrcle::operator=(const Cyrcle& cyrcle)
{
	exist = cyrcle.exist;
	last_position = cyrcle.last_position;
	position = cyrcle.position;
	radius = cyrcle.radius;
	properties = cyrcle.properties;
}

Cyrcle::~Cyrcle()
{
}



Polygon::Polygon() :
	MapElement(),
	angle(0.0f),
	points_array(),
	last_angle(0.0f),
	local_points_array(),
	need_update(false)
{
}

Polygon::Polygon(const Polygon& polygon) :
	MapElement(polygon),
	angle(polygon.angle),
	need_update(true),
	last_angle(polygon.last_angle),
	last_size(polygon.size),
	size(polygon.size),
	local_points_array(polygon.local_points_array),
	points_array(polygon.points_array)
{
}

Polygon::Polygon(
	const Vec2F& position,
	float angle,
	const Vec2F& size,
	const std::vector<Vec2F>& points_array,
	EngineTypes::Map::property_t properties,
	bool exist) :
	MapElement(position, properties, exist),
	angle(angle),
	last_angle(angle),
	last_size(size),
	need_update(true),
	points_array(points_array),
	size(size)
{
	UpdatePoints();
}

bool Polygon::IsClosed() const
{
	return properties & MAP_PROPERTY_CLOSED;
}

bool Polygon::IsCollision(const Beam& beam) const
{
	if (points_array.size() <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(beam))
	{
		return true;
	}
	if (points_array.size() > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array.back(), true);
		if (side.IsIntersection(beam))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_array.size(); p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.IsIntersection(beam))
		{
			return true;
		}
	}
	return false;
}

bool Polygon::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position) const
{
	if (points_array.size() <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);

	float min_distance = INFINITY;
	Vec2F output_intersection_point;

	if (side.Intersection(beam, out_position))
	{
		min_distance = beam.point.Distance(*out_position);
	}
	if (points_array.size() > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array.back(), true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam.point.Distance(output_intersection_point);
			if (min_distance > distance)
			{
				min_distance = distance;
				*out_position = output_intersection_point;
			}
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_array.size(); p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam.point.Distance(output_intersection_point);
			if (min_distance > distance)
			{
				min_distance = distance;
				*out_position = output_intersection_point;
			}
		}
	}

	if (min_distance != INFINITY)
	{
		*distance_to_out_position = min_distance;
		return true;
	}

	return false;
}

bool Polygon::IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position, Vec2F* perpendicular_direction) const
{
	if (points_array.size() <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);

	float min_distance = INFINITY;
	Vec2F output_intersection_point;

	if (side.Intersection(beam, out_position))
	{
		min_distance = beam.point.Distance(*out_position);
		*perpendicular_direction = side.vector.VecMul(beam.vector) > 0.0f ? side.vector.Perpendicular() : side.vector.PerpendicularClockwise();
	}
	if (points_array.size() > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array.back(), true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam.point.Distance(output_intersection_point);
			if (min_distance > distance)
			{
				min_distance = distance;
				*out_position = output_intersection_point;
				*perpendicular_direction = side.vector.VecMul(beam.vector) > 0.0f ? side.vector.Perpendicular() : side.vector.PerpendicularClockwise();
			}
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_array.size(); p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam.point.Distance(output_intersection_point);
			if (min_distance > distance)
			{
				min_distance = distance;
				*out_position = output_intersection_point;
				*perpendicular_direction = side.vector.VecMul(beam.vector) > 0.0f ? side.vector.Perpendicular() : side.vector.PerpendicularClockwise();
			}
		}
	}

	if (min_distance != INFINITY)
	{
		*distance_to_out_position = min_distance;
		return true;
	}

	return false;
}

bool Polygon::IsCollision(const Line& line) const
{
	if (points_array.size() <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(line))
	{
		return true;
	}
	if (points_array.size() > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array.back(), true);
		if (side.IsIntersection(line))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_array.size(); p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.IsIntersection(line))
		{
			return true;
		}
	}
	return false;
}

bool Polygon::IsCollision(const Segment& segment) const
{
	if (points_array.size() <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(segment))
	{
		return true;
	}
	if (points_array.size() > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array.back(), true);
		if (side.IsIntersection(segment))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_array.size(); p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.IsIntersection(segment))
		{
			return true;
		}
	}
	return false;
}

bool Polygon::IsShouldUpdate() const
{
	return need_update;
}

void Polygon::ShouldUpdate()
{
	need_update = true;
}

Vec2F Polygon::DynamicalProperties() const
{
	return size - last_size;
}

void Polygon::DynamicalProperties(float* angular_velocity) const
{
	*angular_velocity = angle - last_angle;
}

void Polygon::DynamicalProperties(Vec2F* velocity) const
{
	*velocity = position - last_position;
}

void Polygon::DynamicalProperties(float* angular_velocity, Vec2F* resize_velocity) const
{
	*angular_velocity = angle - last_angle;
	*resize_velocity = size - last_size;
}

void Polygon::DynamicalProperties(Vec2F* velocity, float* angular_velocity) const
{
	*angular_velocity = angle - last_angle;
	*velocity = position - last_position;
}

void Polygon::DynamicalProperties(Vec2F* velocity, Vec2F* resize_velocity) const
{
	*velocity = position - last_position;
	*resize_velocity = size - last_size;
}

void Polygon::DynamicalProperties(Vec2F* velocity, float* angular_velocity, Vec2F* resize_velocity) const
{
	*angular_velocity = angle - last_angle;
	*velocity = position - last_position;
	*resize_velocity = size - last_size;
}

void Polygon::ToDefault()
{
	position.Set(0.0f, 0.0f);
	angle = 0.0f;
	size.Set(1.0f, 1.0f);
	for (EngineTypes::Map::array_length_t i = 0; i < points_array.size() - 1; i++)
	{
		points_array[i] = local_points_array[i];
	}
}

EngineTypes::Polygon::points_array_length_t Polygon::PointsCount() const
{
	return points_array.size();
}

void Polygon::Set(const Polygon* parent)
{
	MapElement::Set(parent);

	angle = parent->angle;
	last_angle = parent->last_angle;
	last_size = parent->last_size;
	size = parent->last_size;
	points_array = parent->points_array;
	local_points_array = parent->local_points_array;

	need_update = parent->need_update;
}

void Polygon::Set(
	const Vec2F& position,
	float angle,
	const Vec2F& size,
	const std::vector<Vec2F>& points_array,
	EngineTypes::Map::property_t properties,
	bool exist)
{
	MapElement::Set(position, properties, exist);
	this->angle = angle;
	this->last_angle = angle;
	this->last_size = size;
	this->need_update = true;
	this->local_points_array = points_array;
	this->size = size;

	UpdatePoints();
}

void Polygon::SetAngle(float angle)
{
	this->last_angle = this->angle;
	this->angle = angle;

	need_update = true;
}

void Polygon::SetSize(const Vec2F& size)
{
	this->last_size = this->size;
	this->size = size;

	need_update = true;
}

void Polygon::Update()
{
	if (need_update)
	{
		UpdatePoints();
		need_update = false;
	}

	MapElement::Update();
	last_angle = angle;
	last_size = size;
}

void Polygon::UpdatePoints()
{
	const size_t& count = local_points_array.size();

	if (count <= 0) return;

	if (local_points_array.size() != points_array.size())
	{
		points_array = local_points_array;
	}

	for (EngineTypes::Map::array_length_t p = 0; p < count; p++)
	{
		points_array[p] = local_points_array[p].Scale(size).Rotate(angle) + position;
	}
}

Vec2F Polygon::Velocity(const Vec2F& point) const
{
	Vec2F local = (point - position).Rotate(-angle).Scale(Vec2F(1.0f / size.x, 1.0f / size.y));
	return
		Vec2F((size.x - last_size.x) * local.x, (size.y - last_size.y) * local.y) +
		local.Perpendicular() * (angle - last_angle) * local.Length() +
		position - last_position;
}

Vec2F Polygon::VelocityLocal(const Vec2F& point) const
{
	return
		Vec2F((size.x - last_size.x) * point.x, (size.y - last_size.y) * point.y) +
		point.Perpendicular() * (angle - last_angle) * point.Length() +
		position - last_position;
}

void Polygon::operator=(const Polygon& polygon)
{
	MapElement::operator=(polygon);

	angle = polygon.angle;
	last_angle = polygon.last_angle;
	last_size = polygon.last_size;
	local_points_array = polygon.local_points_array;
	need_update = polygon.need_update;
	points_array = polygon.points_array;
	size = polygon.size;
}

Polygon::~Polygon()
{
}

MapData::MapData(const MapData& map) :
	cyrcles_array(map.cyrcles_array),
	polygons_array(map.polygons_array),
	rectangles_array(map.rectangles_array)
{
}

MapData::MapData(
	const std::vector<Rectangle>& rectangles_array,
	const std::vector<Cyrcle>& cyrcles_array,
	const std::vector<Polygon>& polygons_array) :
	cyrcles_array(cyrcles_array),
	polygons_array(polygons_array),
	rectangles_array(rectangles_array)
{
}

MapData::MapData(
	const std::vector<Cyrcle>& cyrcles_array,
	const std::vector<Polygon>& polygons_array) :
	cyrcles_array(cyrcles_array),
	polygons_array(polygons_array),
	rectangles_array()
{
}

MapData::MapData(
	const std::vector<Rectangle>& rectangles_array,
	const std::vector<Polygon>& polygons_array) :
	cyrcles_array(),
	polygons_array(polygons_array),
	rectangles_array(rectangles_array)
{
}

MapData::MapData(
	const std::vector<Polygon>& polygons_array) :
	cyrcles_array(),
	polygons_array(polygons_array),
	rectangles_array()
{
}

void MapData::Set(const MapData* map)
{
	cyrcles_array = map->cyrcles_array;
	polygons_array = map->polygons_array;
	rectangles_array = map->rectangles_array;
}

void MapData::Set(
	const std::vector<Rectangle>& rectangles_array,
	const std::vector<Cyrcle>& cyrcles_array,
	const std::vector<Polygon>& polygons_array)
{
	this->rectangles_array = rectangles_array;
	this->cyrcles_array = cyrcles_array;
	this->polygons_array = polygons_array;
}

void MapData::Set(
	const std::vector<Rectangle>& rectangles_array,
	const std::vector<Polygon>& polygons_array)
{
	this->rectangles_array = rectangles_array;
	this->cyrcles_array.clear();
	this->polygons_array = polygons_array;
}

void MapData::Set(
	const std::vector<Cyrcle>& cyrcles_array,
	const std::vector<Polygon>& polygons_array)
{
	this->rectangles_array.clear();
	this->cyrcles_array = cyrcles_array;
	this->polygons_array = polygons_array;
}

void MapData::Set(
	const std::vector<Polygon>& polygons_array)
{
	this->rectangles_array.clear();
	this->cyrcles_array.clear();
	this->polygons_array = polygons_array;
}

void MapData::operator=(const MapData& map)
{
	cyrcles_array = map.cyrcles_array;
	polygons_array = map.polygons_array;
	rectangles_array = map.rectangles_array;
}

MapData::~MapData()
{
}