#include "Map.h"

#pragma warning(disable : 6384)
#pragma warning(disable : 6385)

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

Vec2F MapElement::Position()
{
	return position;
}

EngineTypes::Map::property_t MapElement::Prorerties()
{
	return properties;
}

Vec2F MapElement::Velocity()
{
	return position - last_position;
}

bool MapElement::IsAggressive()
{
	return properties & MAP_PROPERTY_AGRESSIVE;
}

bool MapElement::IsKiller()
{
	return properties & MAP_PROPERTY_KILLER;
}

bool MapElement::IsUnbreacable()
{
	return properties & MAP_PROPERTY_UNBREACABLE;
}

void MapElement::Move(Vec2F move_vector)
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

void MapElement::Set(Vec2F position, EngineTypes::Map::property_t properties, bool exist)
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

void MapElement::SetPosition(Vec2F position)
{
	this->position = position;
}

void MapElement::SetPosition(const Vec2F* position)
{
	this->position = *position;
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

Rectangle::Rectangle(const Segment* diagonal, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(&diagonal->point, properties, exist),
	point2(diagonal->point + diagonal->vector)
{
}

Rectangle::Rectangle(Vec2F point1, Vec2F point2, EngineTypes::Map::property_t properties, bool exist):
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

bool Cyrcle::IsCollision(const Beam* beam)
{
	return beam->Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Line* line)
{
	return line->Distance(position) < radius;
}

bool Cyrcle::IsCollision(const Segment* segment)
{
	return segment->Distance(position) < radius;
}

float Cyrcle::Radius()
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

bool Polygon::IsClosed()
{
	return properties & MAP_PROPERTY_CLOSED;
}

bool Polygon::IsCollision(const Beam* beam)
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

bool Polygon::IsCollision(const Line* line)
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

bool Polygon::IsCollision(const Segment* segment)
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

bool Polygon::IsNeedUpdate()
{
	return need_update;
}

Vec2F Polygon::DynamicalProperties()
{
	return size - last_size;
}

void Polygon::DynamicalProperties(float* angular_velocity)
{
	*angular_velocity = angle - last_angle;
}

void Polygon::DynamicalProperties(Vec2F* velocity)
{
	*velocity = position - last_position;
}

void Polygon::DynamicalProperties(float* angular_velocity, Vec2F* resize_velocity)
{
	*angular_velocity = angle - last_angle;
	*resize_velocity = size - last_size;
}

void Polygon::DynamicalProperties(Vec2F* velocity, float* angular_velocity)
{
	*angular_velocity = angle - last_angle;
	*velocity = position - last_position;
}

void Polygon::DynamicalProperties(Vec2F* velocity, Vec2F* resize_velocity)
{
	*velocity = position - last_position;
	*resize_velocity = size - last_size;
}

void Polygon::DynamicalProperties(Vec2F* velocity, float* angular_velocity, Vec2F* resize_velocity)
{
	*angular_velocity = angle - last_angle;
	*velocity = position - last_position;
	*resize_velocity = size - last_size;
}

void Polygon::ToDefault()
{
	position.Set(0.0f, 0.0f);
	angle = 0.0f;
	for (EngineTypes::Map::array_length_t i = 0; i < points_count - 1; i++)
	{
		points_array[i] = local_points_array[i];
	}
}

EngineTypes::Polygon::points_array_length_t Polygon::PointsCount()
{
	return points_count;
}

void Polygon::Set(const Polygon* parent)
{
	delete[] points_array;
	delete[] local_points_array;

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
		for (EngineTypes::Map::array_length_t i = 1; i < points_count; i++)
		{
			local_points_array[i] = parent->local_points_array[i];
		}
	}
}

void Polygon::Set(Vec2F position, float angle, Vec2F size, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist)
{
	delete[] this->points_array;
	delete[] local_points_array;

	this->angle = angle;
	this->exist = exist;
	this->last_angle = angle;
	this->last_position = position;
	this->last_size = size;
	this->points_count = points_array_length;
	this->position = position;
	this->properties = properties;
	this->size = size;

	need_update = true;

	if (points_array_length > 0)
	{
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
	delete[] this->points_array;
	delete[] local_points_array;

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
		for (EngineTypes::Map::array_length_t i = 1; i < points_array_length; i++)
		{
			local_points_array[i] = points_array[i];
		}
	}
}

void Polygon::Update()
{
	MapElement::Update();
	last_angle = angle;
	last_size = size;

	if (need_update)
	{
		UpdatePoints();
	}
}

void Polygon::UpdatePoints()
{
	if (points_count == 0)
	{
		return;
	}

	for (EngineTypes::Map::array_length_t p = 0; p < points_count; p++)
	{
		points_array[p] = local_points_array[p].Scale(size).Rotate(angle) + position;
	}

	need_update = false;
}

Vec2F Polygon::Velocity(Vec2F point)
{
	Vec2F local = (point - position).Rotate(-angle).Scale(Vec2F(1.0f / size.x, 1.0f / size.y));
	return
		Vec2F((size.x - last_size.x) * local.x, (size.y - last_size.y) * local.y) +
		local.Perpendicular() * (angle - last_angle) * local.Length() +
		position - last_position;
}

Vec2F Polygon::Velocity(const Vec2F* point)
{
	Vec2F local = (*point - position).Rotate(-angle).Scale(Vec2F(1.0f / size.x, 1.0f / size.y));
	return
		Vec2F((size.x - last_size.x) * local.x, (size.y - last_size.y) * local.y) +
		local.Perpendicular() * (angle - last_angle) * local.Length() +
		position - last_position;
}

Vec2F Polygon::VelocityLocal(Vec2F point)
{
	return
		Vec2F((size.x - last_size.x) * point.x, (size.y - last_size.y) * point.y) +
		point.Perpendicular() * (angle - last_angle) * point.Length() +
		position - last_position;
}

Vec2F Polygon::VelocityLocal(const Vec2F* point)
{
	return
		Vec2F((size.x - last_size.x) * point->x, (size.y - last_size.y) * point->y) +
		point->Perpendicular() * (angle - last_angle) * point->Length() +
		position - last_position;
}

void Polygon::operator=(Polygon polygon)
{
	delete[] this->points_array;
	delete[] local_points_array;

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



Map::Map(const Map& map) :
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

Map::Map(const Rectangle* rectangles_array, EngineTypes::Map::array_length_t rectangles_array_length, const Cyrcle* cyrcles_array, EngineTypes::Map::array_length_t cyrcles_array_length, const Polygon* polygons_array, EngineTypes::Map::array_length_t polygons_array_length) :
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

Rectangle Map::GetRectangle(EngineTypes::Map::array_length_t number)
{
	if (number >= rectangles_array_length)
	{
		return Rectangle();
	}
	return rectangles_array[number];
}

Cyrcle Map::GetCyrcle(EngineTypes::Map::array_length_t number)
{
	if (number >= cyrcles_array_length)
	{
		return Cyrcle();
	}
	return cyrcles_array[number];
}

Polygon Map::GetPolygon(EngineTypes::Map::array_length_t number)
{
	if (number >= polygons_array_length)
	{
		return Polygon();
	}
	return polygons_array[number];
}

Rectangle* Map::RectanglePointer(EngineTypes::Map::array_length_t number)
{
	if (number >= rectangles_array_length)
	{
		return nullptr;
	}
	return &rectangles_array[number];
}

Cyrcle* Map::CyrclePointer(EngineTypes::Map::array_length_t number)
{
	if (number >= cyrcles_array_length)
	{
		return nullptr;
	}
	return &cyrcles_array[number];
}

Polygon* Map::PolygonPointer(EngineTypes::Map::array_length_t number)
{
	if (number >= polygons_array_length)
	{
		return nullptr;
	}
	return &polygons_array[number];
}

void Map::Set(const Map* map)
{
	cyrcles_array_length = map->cyrcles_array_length;
	polygons_array_length = map->polygons_array_length;
	rectangles_array_length = map->rectangles_array_length;

	if (cyrcles_array != nullptr)
	{
		delete[] cyrcles_array;
		cyrcles_array = nullptr;
	}
	if (polygons_array != nullptr)
	{
		delete[] polygons_array;
		polygons_array = nullptr;
	}
	if (rectangles_array != nullptr)
	{
		delete[] rectangles_array;
		rectangles_array = nullptr;
	}

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

void Map::Set(const Rectangle* rectangles_array, EngineTypes::Map::array_length_t rectangles_array_length, const Cyrcle* cyrcles_array, EngineTypes::Map::array_length_t cyrcles_array_length, const Polygon* polygons_array, EngineTypes::Map::array_length_t polygons_array_length)
{
	this->cyrcles_array_length = cyrcles_array_length;
	this->polygons_array_length = polygons_array_length;
	this->rectangles_array_length = rectangles_array_length;

	if (this->cyrcles_array != nullptr)
	{
		delete[] this->cyrcles_array;
		this->cyrcles_array = nullptr;
	}
	if (this->polygons_array != nullptr)
	{
		delete[] this->polygons_array;
		this->polygons_array = nullptr;
	}
	if (this->rectangles_array != nullptr)
	{
		delete[] this->rectangles_array;
		this->rectangles_array = nullptr;
	}

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

Map::~Map()
{
	delete[] rectangles_array;
	delete[] cyrcles_array;
	delete[] polygons_array;
}