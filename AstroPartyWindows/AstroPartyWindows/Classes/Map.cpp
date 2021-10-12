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
	points_array(nullptr),
	default_points_array(nullptr),
	points_array_length(0),
	closed(true)
{
}

Polygon::Polygon(const Polygon& polygon) :
	MapElement(polygon),
	closed(polygon.closed),
	default_points_array(new Vec2F[polygon.points_array_length]),
	points_array(new Vec2F[polygon.points_array_length]),
	points_array_length(polygon.points_array_length)
{
	for (uint32_t i = 0; i < points_array_length; i++)
	{
		points_array[i] = polygon.points_array[i];
		default_points_array[i] = polygon.default_points_array[i];
	}
}

Polygon::Polygon(Vec2F position, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	points_array_length(points_array_length),
	default_points_array(new Vec2F[points_array_length]),
	points_array(new Vec2F[points_array_length])
{
	for (EngineTypes::Polygon::points_array_length_t i = 0; i < points_array_length; i++)
	{
		default_points_array[i] = points_array[i];
		this->points_array[i] = default_points_array[i] + position;
	}
}

Polygon::Polygon(const Vec2F* position, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist) :
	MapElement(position, properties, exist),
	points_array_length(points_array_length),
	default_points_array(new Vec2F[points_array_length]),
	points_array(new Vec2F[points_array_length])
{
	for (EngineTypes::Polygon::points_array_length_t i = 0; i < points_array_length; i++)
	{
		default_points_array[i] = points_array[i];
		this->points_array[i] = default_points_array[i] + *position;
	}
}

bool Polygon::IsCollision(const Beam* beam)
{
	return false;
}

bool Polygon::IsCollision(const Line* line)
{
	return false;
}

bool Polygon::IsCollision(const Segment* segment)
{
	return false;
}

void Polygon::RotateGlobal(float angle, Vec2F global_rotating_point)
{
	Vec2F rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - global_rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = global_rotating_point + rot_vec;
	}
	rot_vec = position - global_rotating_point;
	rot_vec.RotateThis(angle);
	position = global_rotating_point + rot_vec;
}

void Polygon::RotateGlobal(float angle, const Vec2F* global_rotating_point)
{
	Vec2F rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - *global_rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = *global_rotating_point + rot_vec;
	}
	rot_vec = position - *global_rotating_point;
	rot_vec.RotateThis(angle);
	position = *global_rotating_point + rot_vec;
}

void Polygon::RotateLocal(float angle, Vec2F local_rotating_point)
{
	Vec2F global_rotating_point = local_rotating_point + position;
	Vec2F rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - global_rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = global_rotating_point + rot_vec;
	}
	rot_vec = position - global_rotating_point;
	rot_vec.RotateThis(angle);
	position = global_rotating_point + rot_vec;
}

void Polygon::RotateLocal(float angle, const Vec2F* local_rotating_point)
{
	Vec2F global_rotating_point = *local_rotating_point + position;
	Vec2F rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - global_rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = global_rotating_point + rot_vec;
	}
	rot_vec = position - global_rotating_point;
	rot_vec.RotateThis(angle);
	position = global_rotating_point + rot_vec;
}

void Polygon::Move(Vec2F move_vector)
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] += move_vector;
	}
	MapElement::Move(move_vector);
}

void Polygon::Move(const Vec2F* move_vector)
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] += *move_vector;
	}
	MapElement::Move(move_vector);
}

void Polygon::ToDefault()
{
	position.Set(0.0f, 0.0f);
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] = default_points_array[i];
	}
}

void Polygon::Set(const Polygon* parent)
{
	delete[] points_array;
	delete[] default_points_array;

	default_points_array = new Vec2F[points_array_length];
	exist = parent->exist;
	points_array = new Vec2F[points_array_length];
	points_array_length = parent->points_array_length;
	position = parent->position;
	properties = parent->properties;

	for (uint32_t i = 1; i < points_array_length; i++)
	{
		default_points_array[i] = parent->points_array[i];
		points_array[i] = default_points_array[i] + position;
	}
}

void Polygon::Set(Vec2F position, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist)
{
	delete[] this->points_array;
	delete[] default_points_array;

	default_points_array = new Vec2F[points_array_length];
	this->exist = exist;
	this->points_array = new Vec2F[points_array_length];
	this->points_array_length = points_array_length;
	this->position = position;
	this->properties = properties;

	for (uint32_t i = 1; i < points_array_length; i++)
	{
		default_points_array[i] = points_array[i];
		this->points_array[i] = default_points_array[i] + position;
	}
}

void Polygon::Set(const Vec2F* position, const Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, EngineTypes::Map::property_t properties, bool exist)
{
	delete[] this->points_array;
	delete[] default_points_array;

	default_points_array = new Vec2F[points_array_length];
	this->exist = exist;
	this->points_array = new Vec2F[points_array_length];
	this->points_array_length = points_array_length;
	this->position = *position;
	this->properties = properties;

	for (uint32_t i = 1; i < points_array_length; i++)
	{
		default_points_array[i] = points_array[i];
		this->points_array[i] = default_points_array[i] + *position;
	}
}

Polygon::~Polygon()
{
	delete[] points_array;
	delete[] default_points_array;
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

Rectangle* Map::GetRectanglePointer(EngineTypes::Map::array_length_t number)
{
	if (number >= rectangles_array_length)
	{
		return nullptr;
	}
	return &rectangles_array[number];
}

Cyrcle* Map::GetCyrclePointer(EngineTypes::Map::array_length_t number)
{
	if (number >= cyrcles_array_length)
	{
		return nullptr;
	}
	return &cyrcles_array[number];
}

Polygon* Map::GetPolygonPointer(EngineTypes::Map::array_length_t number)
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