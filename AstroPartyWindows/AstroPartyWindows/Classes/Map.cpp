#include "Map.h"

#pragma warning(disable : 6384)
#pragma warning(disable : 6385)

MapElement::MapElement() :
	unbreakable(true),
	exist(true)
{
}

MapElement::MapElement(const MapElement& map_element) :
	exist(map_element.exist),
	position(map_element.position),
	last_position(map_element.last_position),
	unbreakable(map_element.unbreakable)
{
}

MapElement::MapElement(Vec2F* position, bool unbreakable, bool exist) :
	position(*position),
	exist(exist),
	last_position(*position),
	unbreakable(unbreakable)
{
}

Vec2F MapElement::GetPosition()
{
	return position;
}

Vec2F MapElement::GetVelocity()
{
	return position - last_position;
}

bool MapElement::IsUnbreacable()
{
	return unbreakable;
}

void MapElement::Move(Vec2F* move_vector)
{
	position += *move_vector;
}

void MapElement::Set(MapElement* map_element)
{
	exist = map_element->exist;
	position = map_element->position;
	last_position = map_element->last_position;
	unbreakable = map_element->unbreakable;
}

void MapElement::Set(Vec2F* position, bool unbreakable, bool exist)
{
	this->exist = exist;
	this->position = *position;
	this->unbreakable = unbreakable;
}

void MapElement::SetPosition(Vec2F* position)
{
	this->position = *position;
}

void MapElement::operator=(MapElement map_element)
{
	exist = map_element.exist;
	position = map_element.position;
	last_position = map_element.last_position;
	unbreakable = map_element.unbreakable;
}

MapElement::~MapElement()
{
}



Rectangle::Rectangle() :
	MapElement(),
	collision_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE),
	show_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE)
{
}

Rectangle::Rectangle(const Rectangle& rectangle) :
	MapElement(rectangle),
	collision_sides(rectangle.collision_sides),
	point2(rectangle.point2),
	show_sides(rectangle.show_sides)
{
}

Rectangle::Rectangle(Segment* diagonal, EngineTypes::Rectangle::sides_t show_sides, EngineTypes::Rectangle::sides_t collision_sides, bool unbreakable, bool exist) :
	MapElement(&diagonal->point, unbreakable, exist),
	collision_sides(collision_sides),
	point2(diagonal->point + diagonal->vector),
	show_sides(show_sides)
{
}

Vec2F Rectangle::GetUpRightPoint()
{
	return Vec2F(point2.x, position.y);
}

Vec2F Rectangle::GetDownRightPoint()
{
	return point2;
}


Vec2F Rectangle::GetDownLeftPoint()
{
	return Vec2F(position.x, point2.y);;
}


Vec2F Rectangle::GetUpLeftPoint()
{
	return position;
}

Segment Rectangle::GetUpSide()
{
	Vec2F temp = Vec2F(point2.x, position.y);
	return Segment(&position, &temp, true);
}

Segment Rectangle::GetDownSide()
{
	Vec2F temp = Vec2F(position.x, point2.y);
	return Segment(&temp, &point2, true);
}

Segment Rectangle::GetRightSide()
{
	Vec2F temp = Vec2F(point2.x, position.y);
	return Segment(&temp, &point2, true);
}

Segment Rectangle::GetLeftSide()
{
	Vec2F temp = Vec2F(position.x, point2.y);
	return Segment(&position, &temp, true);
}

void Rectangle::Move(Vec2F* move_vector)
{
	MapElement::Move(move_vector);
	point2 += *move_vector;
}

void Rectangle::Set(Rectangle* rectangle)
{
	collision_sides = rectangle->collision_sides;
	exist = rectangle->exist;
	last_position = rectangle->last_position;
	point2 = rectangle->point2;
	position = rectangle->position;
	show_sides = rectangle->show_sides;
	unbreakable = rectangle->unbreakable;
}

void Rectangle::Set(Segment* diagonal, EngineTypes::Rectangle::sides_t show_sides, EngineTypes::Rectangle::sides_t collision_sides, bool unbreakable, bool exist)
{
	this->collision_sides = collision_sides;
	this->exist = exist;
	this->point2 = diagonal->point + diagonal->vector;
	this->position = diagonal->point;
	this->show_sides = show_sides;
	this->unbreakable = unbreakable;
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

Cyrcle::Cyrcle(Vec2F* position, float radius, bool unbreakable, bool exist) :
	MapElement(position, unbreakable, exist),
	radius(radius)
{
}

float Cyrcle::GetRadius()
{
	return radius;
}

void Cyrcle::SetRadius(float radius)
{
	this->radius = radius;
}

void Cyrcle::Set(Cyrcle* cyrcle)
{
	exist = cyrcle->exist;
	last_position = cyrcle->last_position;
	position = cyrcle->position;
	radius = cyrcle->radius;
	unbreakable = cyrcle->unbreakable;
}

void Cyrcle::Set(Vec2F* position, float radius, bool unbreakable, bool exist)
{
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
	this->unbreakable = unbreakable;
}

void Cyrcle::operator=(Cyrcle cyrcle)
{
	exist = cyrcle.exist;
	last_position = cyrcle.last_position;
	position = cyrcle.position;
	radius = cyrcle.radius;
	unbreakable = cyrcle.unbreakable;
}

Cyrcle::~Cyrcle()
{

}



Polygon::Polygon() : points_array(nullptr), default_points_array(nullptr), points_array_length(0)
{
}

Polygon::Polygon(const Polygon& polygon) :
	MapElement(polygon),
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

Polygon::Polygon(Vec2F* position, Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, bool unbreakable, bool exist) :
	MapElement(position, unbreakable, exist),
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

void Polygon::RotateGlobal(float angle, Vec2F* global_rotating_point)
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


void Polygon::RotateLocal(float angle, Vec2F* local_rotating_point)
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

void Polygon::Move(Vec2F* move_vector)
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

void Polygon::Set(Polygon* parent)
{
	delete[] points_array;
	delete[] default_points_array;

	default_points_array = new Vec2F[points_array_length];
	exist = parent->exist;
	points_array = new Vec2F[points_array_length];
	points_array_length = parent->points_array_length;
	position = parent->position;
	unbreakable = parent->unbreakable;

	for (uint32_t i = 1; i < points_array_length; i++)
	{
		default_points_array[i] = parent->points_array[i];
		points_array[i] = default_points_array[i] + position;
	}
}

void Polygon::Set(Vec2F* position, Vec2F* points_array, EngineTypes::Polygon::points_array_length_t points_array_length, bool unbreakable, bool exist)
{
	delete[] this->points_array;
	delete[] default_points_array;

	default_points_array = new Vec2F[points_array_length];
	this->exist = exist;
	this->points_array = new Vec2F[points_array_length];
	this->points_array_length = points_array_length;
	this->position = *position;
	this->unbreakable = unbreakable;

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
	if (cyrcles_array_length == 0 && polygons_array_length == 0 && rectangles_array_length == 0)
	{
		rectangles_array = new Rectangle[1];
		rectangles_array_length = 1;
		Vec2F new_point1;
		new_point1.Set(1.0f, 1.0f);
		Vec2F new_point2;
		new_point2.Set(-1.0f, -1.0f);
		Segment new_segment;
		new_segment.Set(&new_point1, &new_point2, true);
		rectangles_array[0].Set(&new_segment);
		cyrcles_array = nullptr;
		polygons_array = nullptr;
		return;
	}
	if (cyrcles_array_length > 0)
	{
		cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (EngineTypes::Map::elements_array_length_t i = 0; i < cyrcles_array_length; i++)
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
		for (EngineTypes::Map::elements_array_length_t i = 0; i < polygons_array_length; i++)
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
		for (EngineTypes::Map::elements_array_length_t i = 0; i < rectangles_array_length; i++)
		{
			rectangles_array[i] = map.rectangles_array[i];
		}
	}
	else
	{
		rectangles_array = nullptr;
	}
}

Map::Map(Rectangle* rectangles_array, EngineTypes::Map::elements_array_length_t rectangles_array_length, Cyrcle* cyrcles_array, EngineTypes::Map::elements_array_length_t cyrcles_array_length, Polygon* polygons_array, EngineTypes::Map::elements_array_length_t polygons_array_length) :
	rectangles_array_length(rectangles_array_length),
	cyrcles_array_length(cyrcles_array_length),
	polygons_array_length(polygons_array_length)
{
	if (cyrcles_array_length == 0 && polygons_array_length == 0 && rectangles_array_length == 0)
	{
		this->rectangles_array = new Rectangle[1];
		this->rectangles_array_length = 1;
		Vec2F new_point1;
		new_point1.Set(1.0f, 1.0f);
		Vec2F new_point2;
		new_point2.Set(-1.0f, -1.0f);
		Segment new_segment;
		new_segment.Set(&new_point1, &new_point2, true);
		this->rectangles_array[0].Set(&new_segment);
		this->cyrcles_array = nullptr;
		this->polygons_array = nullptr;
		return;
	}
	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}
	else
	{
		this->rectangles_array = nullptr;
	}

	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (uint8_t i = 0; i < cyrcles_array_length; i++)
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
		for (uint8_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}
	else
	{
		this->polygons_array = nullptr;
	}
}

Rectangle Map::GetRectangle(uint8_t number)
{
	if (number >= rectangles_array_length)
	{
		return Rectangle(rectangles_array[number % rectangles_array_length]);
	}
	return Rectangle(rectangles_array[number]);
}

Cyrcle Map::GetCyrcle(uint8_t number)
{
	if (number >= cyrcles_array_length)
	{
		return cyrcles_array[number % cyrcles_array_length];
	}
	return cyrcles_array[number];
}

Polygon Map::GetPolygon(uint8_t number)
{
	if (number >= polygons_array_length)
	{
		return polygons_array[number % polygons_array_length];
	}
	return polygons_array[number];
}

Rectangle* Map::GetRectanglePointer(uint8_t number)
{
	if (number >= rectangles_array_length)
	{
		return &rectangles_array[number % rectangles_array_length];
	}
	return &rectangles_array[number];
}

Cyrcle* Map::GetCyrclePointer(uint8_t number)
{
	if (number >= cyrcles_array_length)
	{
		return &cyrcles_array[number % cyrcles_array_length];
	}
	return &cyrcles_array[number];
}

Polygon* Map::GetPolygonPointer(uint8_t number)
{
	if (number >= polygons_array_length)
	{
		return &polygons_array[number % polygons_array_length];
	}
	return &polygons_array[number];
}

void Map::Set(Map* map)
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
	if (cyrcles_array_length == 0 && polygons_array_length == 0 && rectangles_array_length == 0)
	{
		rectangles_array = new Rectangle[1];
		rectangles_array_length = 1;
		Vec2F new_point1;
		new_point1.Set(1.0f, 1.0f);
		Vec2F new_point2;
		new_point2.Set(-1.0f, -1.0f);
		Segment new_segment;
		new_segment.Set(&new_point1, &new_point2, true);
		rectangles_array[0].Set(&new_segment);
	}
	
	if (rectangles_array_length > 0)
	{
		rectangles_array = new Rectangle[rectangles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			rectangles_array[i].Set(&map->rectangles_array[i]);
		}
	}
	else
	{
		this->rectangles_array = nullptr;
	}

	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (uint8_t i = 0; i < cyrcles_array_length; i++)
		{
			cyrcles_array[i].Set(&map->cyrcles_array[i]);
		}
	}
	else
	{
		this->cyrcles_array = nullptr;
	}

	if (polygons_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (uint8_t i = 0; i < polygons_array_length; i++)
		{
			polygons_array[i].Set(&map->polygons_array[i]);
		}
	}
	else
	{
		this->polygons_array = nullptr;
	}
}

void Map::Set(Rectangle* rectangles_array, EngineTypes::Map::elements_array_length_t rectangles_array_length, Cyrcle* cyrcles_array, EngineTypes::Map::elements_array_length_t cyrcles_array_length, Polygon* polygons_array, EngineTypes::Map::elements_array_length_t polygons_array_length)
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
	if (cyrcles_array_length == 0 && polygons_array_length == 0 && rectangles_array_length == 0)
	{
		this->rectangles_array = new Rectangle[1];
		this->rectangles_array_length = 1;
		Vec2F new_point1;
		new_point1.Set(1.0f, 1.0f);
		Vec2F new_point2;
		new_point2.Set(-1.0f, -1.0f);
		Segment new_segment;
		new_segment.Set(&new_point1, &new_point2, true);
		this->rectangles_array[0].Set(&new_segment);
		return;
	}

	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}

	if (cyrcles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (uint8_t i = 0; i < cyrcles_array_length; i++)
		{
			this->cyrcles_array[i].Set(&cyrcles_array[i]);
		}
	}

	if (polygons_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (uint8_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}
}

Map::~Map()
{
	delete[] rectangles_array;
	delete[] cyrcles_array;
	delete[] polygons_array;
}