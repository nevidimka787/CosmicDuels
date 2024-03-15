#include "Map.h"

#pragma warning(disable : 6384)
#pragma warning(disable : 6385)

using namespace Map;

MapElement::MapElement() :
	exist(true),
	properties(MAP_DEFAULT_PROPERTIES)
{
}

MapElement::MapElement(const MapElement& map_element) :
	exist(map_element.exist),
	position(map_element.position),
	last_position(map_element.last_position),
	properties(MAP_DEFAULT_PROPERTIES)
{
}

MapElement::MapElement(Vec2F position, EngineTypes::Map::property_t properties, bool exist) :
	position(position),
	exist(exist),
	last_position(position),
	properties(properties)
{
}

MapElement::MapElement(const Vec2F* position, EngineTypes::Map::property_t properties, bool exist) :
	position(*position),
	exist(exist),
	last_position(*position),
	properties(properties)
{
}

Vec2F MapElement::GetPosition() const
{
	return position;
}

EngineTypes::Map::property_t MapElement::Prorerties() const
{
	return properties;
}

Vec2F MapElement::GetVelocity() const
{
	return position - last_position;
}

bool MapElement::IsAggressive() const
{
	return properties & MAP_PROPERTY_AGRESSIVE;
}

bool MapElement::IsKiller() const
{
	return properties & MAP_PROPERTY_KILLER;
}

bool MapElement::IsUnbreacable() const
{
	return properties & MAP_PROPERTY_UNBREACABLE;
}

void MapElement::Move(const Vec2F& move_vector)
{
	position += move_vector;
}

void MapElement::Move(const Vec2F* move_vector)
{
	position += *move_vector;
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

void MapElement::Set(const Vec2F* position, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->position = *position;
	this->properties = properties;
}

void MapElement::SetPosition(const Vec2F& position)
{
	this->position = position;
}

void MapElement::SetPosition(const Vec2F* position)
{
	this->position = *position;
}

void MapElement::SetProperties(EngineTypes::Map::property_t properties)
{
	this->properties = properties;
}

void MapElement::operator=(MapElement map_element)
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
	MapElement(&diagonal.point, properties, exist),
	point2(diagonal.point + diagonal.vector)
{
}

Rectangle::Rectangle(const Segment* diagonal, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(&diagonal->point, properties, exist),
	point2(diagonal->point + diagonal->vector)
{
}

Rectangle::Rectangle(Vec2F point1, Vec2F point2, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(point1, properties, exist),
	point2(point2)
{
}

Rectangle::Rectangle(const Vec2F* point1, const Vec2F* point2, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(point1, properties, exist),
	point2(*point2)
{
}

Vec2F Rectangle::UpRightPoint() const
{
	return position;
}

Vec2F Rectangle::DownRightPoint() const
{
	return Vec2F(position.x, point2.y);
}

Vec2F Rectangle::DownLeftPoint() const
{
	return point2;
}

Vec2F Rectangle::UpLeftPoint() const
{
	return Vec2F(point2.x, position.y);
}

Segment Rectangle::UpSide() const
{
	Vec2F temp;
	temp.Set(point2.x, position.y);
	return Segment(&position, &temp, true);
}

Segment Rectangle::DownSide() const
{
	Vec2F temp;
	temp.Set(position.x, point2.y);
	return Segment(&temp, &point2, true);
}

Segment Rectangle::RightSide() const
{
	Vec2F temp;
	temp.Set(position.x, point2.y);
	return Segment(&position, &temp, true);
}

Segment Rectangle::LeftSide() const
{
	Vec2F temp;
	temp.Set(point2.x, position.y);
	return Segment(&temp, &point2, true);
}

bool Rectangle::IsCollision(const Beam* beam)
{
	if (UpSide().IsIntersection(beam))
	{
		return true;
	}
	if (RightSide().IsIntersection(beam))
	{
		return true;
	}
	if (DownSide().IsIntersection(beam))
	{
		return true;
	}
	if (LeftSide().IsIntersection(beam))
	{
		return true;
	}
	return false;
}

bool Rectangle::IsCollision(const Beam* beam, Vec2F* out_position, float* distance_to_out_position) const
{
	Vec2F output_intersection_point;
	float min_distance = INFINITY;
	if (UpSide().Intersection(beam, out_position))
	{
		min_distance = (*out_position - beam->point).Length();
	}
	if (RightSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam->point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
		}
	}
	if (DownSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam->point).Length();
		if (min_distance > distance)
		{
			min_distance = distance;
			*out_position = output_intersection_point;
		}
	}
	if (LeftSide().Intersection(beam, &output_intersection_point))
	{
		float distance = (output_intersection_point - beam->point).Length();
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

bool Rectangle::IsCollision(const Line* line)
{
	if (UpSide().IsIntersection(line))
	{
		return true;
	}
	if (RightSide().IsIntersection(line))
	{
		return true;
	}
	if (DownSide().IsIntersection(line))
	{
		return true;
	}
	if (LeftSide().IsIntersection(line))
	{
		return true;
	}
	return false;
}

bool Rectangle::IsCollision(const Segment* segment)
{
	if (UpSide().IsIntersection(segment))
	{
		return true;
	}
	if (RightSide().IsIntersection(segment))
	{
		return true;
	}
	if (DownSide().IsIntersection(segment))
	{
		return true;
	}
	if (LeftSide().IsIntersection(segment))
	{
		return true;
	}
	return false;
}

void Rectangle::Move(Vec2F move_vector)
{
	MapElement::Move(move_vector);
	point2 += move_vector;
}

void Rectangle::Move(const Vec2F* move_vector)
{
	MapElement::Move(move_vector);
	point2 += *move_vector;
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
	exist = rectangle->exist;
	last_position = rectangle->last_position;
	point2 = rectangle->point2;
	position = rectangle->position;
	properties = rectangle->properties;
}

void Rectangle::Set(const Segment& diagonal, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->point2 = diagonal.point + diagonal.vector;
	this->position = diagonal.point;
	this->properties = properties;
}

void Rectangle::Set(const Segment* diagonal, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->point2 = diagonal->point + diagonal->vector;
	this->position = diagonal->point;
	this->properties = properties;
}

void Rectangle::SetPosition(Vec2F position)
{
	Vec2F point_to_position = (this->position - point2) / 2.0f;
	this->position = position + point_to_position;
	point2 = position - point_to_position;
}

void Rectangle::SetPosition(const Vec2F* position)
{
	Vec2F point_to_position = (this->position - point2) / 2.0f;
	this->position = *position + point_to_position;
	point2 = *position - point_to_position;
}

void Rectangle::SetSize(Vec2F size)
{
	Vec2F center = (this->position + point2) / 2.0f;
	position = center - size;
	point2 = center + size;
}

void Rectangle::SetSize(const Vec2F* size)
{
	Vec2F center = (this->position + point2) / 2.0f;
	position = center - *size;
	point2 = center + *size;
}

void Rectangle::operator=(const Rectangle& rectangle)
{
	exist = rectangle.exist;
	last_position = rectangle.last_position;
	point2 = rectangle.point2;
	position = rectangle.position;
	properties = rectangle.properties;
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

Cyrcle::Cyrcle(Vec2F position, float radius, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	radius(radius)
{
}

Cyrcle::Cyrcle(const Vec2F* position, float radius, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	radius(radius)
{
}

bool Cyrcle::IsCollision(const Beam* beam) const
{
	return beam->Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Beam* beam, Vec2F* out_position, float* distance_to_out_position) const
{
	Vec2F neares_point_on_line;

	float distance = Line(beam).Distance(position, &neares_point_on_line);

	if (distance > radius)
	{
		return false;
	}

	float sub_vector_length = sqrtf(radius * radius - (neares_point_on_line - position).LengthPow2());

	*out_position =
		(beam->point.Distance(position) > radius) ?
		(neares_point_on_line - beam->vector.Normalize() * sub_vector_length) :
		(neares_point_on_line + beam->vector.Normalize() * sub_vector_length);

	*distance_to_out_position = beam->point.Distance(out_position);

	return true;
}

bool Cyrcle::IsCollision(const Line* line) const
{
	return line->Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Segment* segment) const
{
	return segment->Distance(position) < radius;
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
	exist = cyrcle->exist;
	last_position = cyrcle->last_position;
	position = cyrcle->position;
	radius = cyrcle->radius;
	properties = cyrcle->properties;
}

void Cyrcle::Set(Vec2F position, float radius, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->position = position;
	this->radius = radius;
	this->properties = properties;
}

void Cyrcle::Set(const Vec2F* position, float radius, EngineTypes::Map::property_t properties, bool exist)
{
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
	this->properties = properties;
}

void Cyrcle::operator=(Cyrcle cyrcle)
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
	points_array(nullptr),
	last_angle(0.0f),
	local_points_array(nullptr),
	need_update(false),
	points_count(0)
{
}

Polygon::Polygon(const Polygon& polygon) :
	MapElement(polygon),
	angle(polygon.angle),
	need_update(true),
	last_angle(polygon.last_angle),
	last_position(polygon.last_position),
	last_size(polygon.size),
	points_count(polygon.points_count),
	size(polygon.size)
{
	if (points_count > 0)
	{
		this->points_array = new Vec2F[points_count];
		local_points_array = new Vec2F[points_count];
		for (EngineTypes::Polygon::points_array_length_t i = 0; i < points_count; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
	else
	{
		this->points_array = nullptr;
		local_points_array = nullptr;
	}
}

Polygon::Polygon(Vec2F position, float angle, Vec2F size, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	angle(angle),
	last_angle(angle),
	last_size(size),
	need_update(true),
	points_count(points_array_length),
	size(size)
{
	if (points_array_length > 0)
	{
		this->points_array = new Vec2F[points_array_length];
		local_points_array = new Vec2F[points_array_length];
		for (EngineTypes::Polygon::points_array_length_t i = 0; i < points_array_length; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
	else
	{
		this->points_array = nullptr;
		local_points_array = nullptr;
	}

	UpdatePoints();
}

Polygon::Polygon(const Vec2F* position, float angle, const Vec2F* size, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	angle(angle),
	last_angle(angle),
	last_size(*size),
	need_update(true),
	points_count(points_array_length),
	size(*size)
{
	if (points_array_length > 0)
	{
		this->points_array = new Vec2F[points_array_length];
		local_points_array = new Vec2F[points_array_length];
		for (EngineTypes::Polygon::points_array_length_t i = 0; i < points_array_length; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
	else
	{
		this->points_array = nullptr;
		local_points_array = nullptr;
	}
}

bool Polygon::IsClosed() const
{
	return properties & MAP_PROPERTY_CLOSED;
}

bool Polygon::IsCollision(const Beam* beam) const
{
	if (points_count <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(beam))
	{
		return true;
	}
	if (points_count > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array[points_count - 1], true);
		if (side.IsIntersection(beam))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_count; p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.IsIntersection(beam))
		{
			return true;
		}
	}
	return false;
}

bool Polygon::IsCollision(const Beam* beam, Vec2F* out_position, float* distance_to_out_position) const
{
	if (points_count <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);

	float min_distance = INFINITY;
	Vec2F output_intersection_point;

	if (side.Intersection(beam, out_position))
	{
		min_distance = beam->point.Distance(out_position);
	}
	if (points_count > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array[points_count - 1], true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam->point.Distance(output_intersection_point);
			if (min_distance > distance)
			{
				min_distance = distance;
				*out_position = output_intersection_point;
			}
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_count; p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.Intersection(beam, &output_intersection_point))
		{
			float distance = beam->point.Distance(output_intersection_point);
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

bool Polygon::IsCollision(const Line* line) const
{
	if (points_count <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(line))
	{
		return true;
	}
	if (points_count > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array[points_count - 1], true);
		if (side.IsIntersection(line))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_count; p++)
	{
		side.Set(points_array[p - 1], points_array[p], true);
		if (side.IsIntersection(line))
		{
			return true;
		}
	}
	return false;
}

bool Polygon::IsCollision(const Segment* segment) const
{
	if (points_count <= 1)
	{
		return false;
	}
	Segment side = Segment(points_array[0], points_array[1], true);
	if (side.IsIntersection(segment))
	{
		return true;
	}
	if (points_count > 2 && properties & MAP_PROPERTY_CLOSED)
	{
		side.Set(points_array[0], points_array[points_count - 1], true);
		if (side.IsIntersection(segment))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 2; p < points_count; p++)
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
	for (EngineTypes::Map::array_length_t i = 0; i < points_count - 1; i++)
	{
		points_array[i] = local_points_array[i];
	}
}

EngineTypes::Polygon::points_array_length_t Polygon::PointsCount() const
{
	return points_count;
}

void Polygon::Set(const Polygon* parent)
{
	if (points_count > 0)
	{
		delete[] points_array;
		delete[] local_points_array;
	}

	angle = parent->angle;
	exist = parent->exist;
	last_angle = parent->last_angle;
	last_position = parent->last_position;
	last_size = parent->last_size;
	points_count = parent->points_count;
	position = parent->position;
	properties = parent->properties;
	size = parent->last_size;

	need_update = true;

	if (points_count > 0)
	{
		local_points_array = new Vec2F[points_count];
		points_array = new Vec2F[points_count];
		for (EngineTypes::Map::array_length_t i = 0; i < points_count; i++)
		{
			local_points_array[i] = parent->local_points_array[i];
		}
	}
}

void Polygon::Set(Vec2F position, float angle, Vec2F size, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist)
{
	if (this->points_count > 0)
	{
		delete[] this->points_array;
		delete[] local_points_array;
	}

	this->angle = angle;
	this->exist = exist;
	this->last_angle = angle;
	this->last_position = position;
	this->last_size = size;
	this->points_count = points_array_length;
	this->position = position;
	this->properties = properties;
	this->size = size;

	if (points_array_length > 0)
	{
		need_update = true;

		local_points_array = new Vec2F[points_array_length];
		this->points_array = new Vec2F[points_array_length];

		for (EngineTypes::Map::array_length_t i = 0; i < points_array_length; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
}

void Polygon::Set(const Vec2F* position, float angle, const Vec2F* size, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist)
{
	if (this->points_count > 0)
	{
		delete[] this->points_array;
		delete[] local_points_array;
	}

	this->angle = angle;
	this->exist = exist;
	this->last_angle = angle;
	this->last_position = *position;
	this->last_size = *size;
	this->points_count = points_array_length;
	this->position = *position;
	this->properties = properties;
	this->size = *size;

	need_update = true;

	if (points_array_length > 0)
	{
		this->points_array = new Vec2F[points_array_length];
		this->local_points_array = new Vec2F[points_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < points_array_length; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
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
	if (points_count == 0)
	{
		return;
	}

	//std::cout << "AngV: " << angle - last_angle << " NorV: " << position - last_position << " ResV: " << size - last_size << std::endl;

	for (EngineTypes::Map::array_length_t p = 0; p < points_count; p++)
	{
		points_array[p] = local_points_array[p].Scale(size).Rotate(angle) + position;
	}
}

Vec2F Polygon::Velocity(Vec2F point) const
{
	Vec2F local = (point - position).Rotate(-angle).Scale(Vec2F(1.0f / size.x, 1.0f / size.y));
	return
		Vec2F((size.x - last_size.x) * local.x, (size.y - last_size.y) * local.y) +
		local.Perpendicular() * (angle - last_angle) * local.Length() +
		position - last_position;
}

Vec2F Polygon::Velocity(const Vec2F* point) const
{
	Vec2F local = (*point - position).Rotate(-angle).Scale(Vec2F(1.0f / size.x, 1.0f / size.y));
	return
		Vec2F((size.x - last_size.x) * local.x, (size.y - last_size.y) * local.y) +
		local.Perpendicular() * (angle - last_angle) * local.Length() +
		position - last_position;
}

Vec2F Polygon::VelocityLocal(Vec2F point) const
{
	return
		Vec2F((size.x - last_size.x) * point.x, (size.y - last_size.y) * point.y) +
		point.Perpendicular() * (angle - last_angle) * point.Length() +
		position - last_position;
}

Vec2F Polygon::VelocityLocal(const Vec2F* point) const
{
	return
		Vec2F((size.x - last_size.x) * point->x, (size.y - last_size.y) * point->y) +
		point->Perpendicular() * (angle - last_angle) * point->Length() +
		position - last_position;
}

void Polygon::operator=(Polygon polygon)
{
	if (points_count > 0)
	{
		delete[] points_array;
		delete[] local_points_array;
	}

	angle = polygon.angle;
	exist = polygon.exist;
	points_count = polygon.points_count;
	position = polygon.position;
	properties = polygon.properties;

	need_update = true;

	if (points_count > 0)
	{
		points_array = new Vec2F[points_count];
		local_points_array = new Vec2F[points_count];
		for (EngineTypes::Map::array_length_t i = 1; i < points_count; i++)
		{
			local_points_array[i] = polygon.local_points_array[i];
		}
	}
}

Polygon::~Polygon()
{
	if (points_count > 0)
	{
		delete[] points_array;
		delete[] local_points_array;
	}
}



MapData::MapData(const MapData& map) :
	cyrcles_array_length(map.cyrcles_array_length),
	polygons_array_length(map.polygons_array_length),
	rectangles_array_length(map.rectangles_array_length)
{
	if (cyrcles_array_length > 0)
	{
		cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < cyrcles_array_length; i++)
		{
			cyrcles_array[i] = map.cyrcles_array[i];
		}
	}
	else
	{
		cyrcles_array = nullptr;
	}

	if (polygons_array_length > 0)
	{
		polygons_array = new Polygon[polygons_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < polygons_array_length; i++)
		{
			polygons_array[i] = map.polygons_array[i];
		}
	}
	else
	{
		polygons_array = nullptr;
	}

	if (rectangles_array_length > 0)
	{
		rectangles_array = new Rectangle[rectangles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < rectangles_array_length; i++)
		{
			rectangles_array[i] = map.rectangles_array[i];
		}
	}
	else
	{
		rectangles_array = nullptr;
	}

}

MapData::MapData(const Rectangle* rectangles_array, EngineTypes::Map::array_length_t rectangles_array_length, const Cyrcle* cyrcles_array, EngineTypes::Map::array_length_t cyrcles_array_length, const Polygon* polygons_array, EngineTypes::Map::array_length_t polygons_array_length) :
	cyrcles_array_length(cyrcles_array_length),
	polygons_array_length(polygons_array_length),
	rectangles_array_length(rectangles_array_length)
{
	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < cyrcles_array_length; i++)
		{
			this->cyrcles_array[i].Set(&cyrcles_array[i]);
		}
	}
	else
	{
		this->cyrcles_array = nullptr;
	}

	if (polygons_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}
	else
	{
		this->polygons_array = nullptr;
	}	
	
	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}
	else
	{
		this->rectangles_array = nullptr;
	}
}

Rectangle MapData::GetRectangle(EngineTypes::Map::array_length_t number) const
{
	if (number >= rectangles_array_length)
	{
		return Rectangle();
	}
	return rectangles_array[number];
}

Cyrcle MapData::GetCyrcle(EngineTypes::Map::array_length_t number) const
{
	if (number >= cyrcles_array_length)
	{
		return Cyrcle();
	}
	return cyrcles_array[number];
}

Polygon MapData::GetPolygon(EngineTypes::Map::array_length_t number) const
{
	if (number >= polygons_array_length)
	{
		return Polygon();
	}
	return polygons_array[number];
}

Rectangle* MapData::RectanglePointer(EngineTypes::Map::array_length_t number) const
{
	if (number >= rectangles_array_length)
	{
		return nullptr;
	}
	return &rectangles_array[number];
}

Cyrcle* MapData::CyrclePointer(EngineTypes::Map::array_length_t number) const
{
	if (number >= cyrcles_array_length)
	{
		return nullptr;
	}
	return &cyrcles_array[number];
}

Polygon* MapData::PolygonPointer(EngineTypes::Map::array_length_t number) const
{
	if (number >= polygons_array_length)
	{
		return nullptr;
	}
	return &polygons_array[number];
}

void MapData::Set(const MapData* map)
{
	if (cyrcles_array_length > 0)
	{
		delete[] cyrcles_array;
	}
	if (polygons_array_length > 0)
	{
		delete[] polygons_array;
	}
	if (rectangles_array_length > 0)
	{
		delete[] rectangles_array;
	}

	cyrcles_array_length = map->cyrcles_array_length;
	polygons_array_length = map->polygons_array_length;
	rectangles_array_length = map->rectangles_array_length;

	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < cyrcles_array_length; i++)
		{
			cyrcles_array[i].Set(&map->cyrcles_array[i]);
		}
	}

	if (polygons_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < polygons_array_length; i++)
		{
			polygons_array[i].Set(&map->polygons_array[i]);
		}
	}

	if (rectangles_array_length > 0)
	{
		rectangles_array = new Rectangle[rectangles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < rectangles_array_length; i++)
		{
			rectangles_array[i].Set(&map->rectangles_array[i]);
		}
	}
}

void MapData::Set(const Rectangle* rectangles_array, EngineTypes::Map::array_length_t rectangles_array_length, const Cyrcle* cyrcles_array, EngineTypes::Map::array_length_t cyrcles_array_length, const Polygon* polygons_array, EngineTypes::Map::array_length_t polygons_array_length)
{
	if (this->cyrcles_array_length > 0)
	{
		delete[] this->cyrcles_array;
	}
	if (this->polygons_array_length > 0)
	{
		delete[] this->polygons_array;
	}
	if (this->rectangles_array_length > 0)
	{
		delete[] this->rectangles_array;
	}

	this->cyrcles_array_length = cyrcles_array_length;
	this->polygons_array_length = polygons_array_length;
	this->rectangles_array_length = rectangles_array_length;

	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < cyrcles_array_length; i++)
		{
			this->cyrcles_array[i].Set(&cyrcles_array[i]);
		}
	}

	if (polygons_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}

	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (EngineTypes::Map::array_length_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}
}

MapData::~MapData()
{
	if (cyrcles_array_length > 0)
	{
		delete[] cyrcles_array;
	}
	if (polygons_array_length > 0)
	{
		delete[] polygons_array;
	}
	if (rectangles_array_length > 0)
	{
		delete[] rectangles_array;
	}
}