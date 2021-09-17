#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class MapElement;
class Rectangle;
class Cyrcle;
class Polygon;
class Map;

class MapElement
{
protected:
	Vec2F position;
	Vec2F last_position;
	bool unbreakable;
public:
	bool exist;
	MapElement();
	MapElement(const MapElement& map_element);
	MapElement(
		Vec2F* position,
		bool unbreakable = true,
		bool exist = true);

	Vec2F GetPosition();
	Vec2F GetVelocity();
	bool IsUnbreacable();
	void Move(Vec2F* move_vector);
	void Update();
	void SetPosition(Vec2F* position);
	void Set(MapElement* map_element);
	void Set(
		Vec2F* position,
		bool unbreakable = true,
		bool exist = true);
	void operator=(MapElement map_element);

	~MapElement();
};

class Rectangle : public MapElement
{
protected:
	Vec2F point2;//down left point
public:
	Rectangle();
	Rectangle(const Rectangle& rectangle);
	Rectangle(
		Segment* diagonal,
		bool unbreakable = true,
		bool exist = true);

	Vec2F GetUpRightPoint();
	Vec2F GetDownRightPoint();
	Vec2F GetDownLeftPoint();
	Vec2F GetUpLeftPoint();
	Segment GetUpSide();
	Segment GetDownSide();
	Segment GetRightSide();
	Segment GetLeftSide();
	void Move(Vec2F* move_vector);
	void Set(Rectangle* patent);
	void Set(
		Segment* diagonal,
		bool unbreakable = true,
		bool exist = true);

	~Rectangle();
};

class Cyrcle : public MapElement
{
protected:
	float radius;
public:
	Cyrcle();
	Cyrcle(const Cyrcle& cyrcle);
	Cyrcle(
		Vec2F* position,
		float radius = CYRCLE_DEFAULT_RADIUS,
		bool unbreakable = true,
		bool exist = true);

	float GetRadius();
	void SetRadius(float radius);
	void Set(Cyrcle* cyrcle);
	void Set(
		Vec2F* position,
		float radius = CYRCLE_DEFAULT_RADIUS,
		bool unbreakable = true,
		bool exist = true);

	void operator=(Cyrcle cyrcle);

	~Cyrcle();
};

/*
* This class not recommended use in default maps.
*/
class Polygon : public MapElement
{
protected:
	Vec2F* points_array;
	Vec2F* default_points_array;
	EngineTypes::Polygon::points_array_length_t points_array_length;

public:
	bool closed;
	Polygon();
	Polygon(const Polygon& polygon);
	Polygon(
		Vec2F* position,
		Vec2F* points_array,
		EngineTypes::Polygon::points_array_length_t points_array_length,
		bool unbreakable = true,
		bool exist = true);

	void RotateGlobal(float angle, Vec2F* global_rotating_point);
	void RotateLocal(float angle, Vec2F* local_rotating_point);
	void Move(Vec2F* move_vector);
	void ToDefault();
	void Set(Polygon* parent);
	void Set(Vec2F* position,
		Vec2F* points_array,
		EngineTypes::Polygon::points_array_length_t points_array_length,
		bool unbreakable = true,
		bool exist = true);

	void operator=(Polygon polygon)
	{

	}

	~Polygon();
};

class Map
{
protected:
	Rectangle* rectangles_array;
	Cyrcle* cyrcles_array;
	Polygon* polygons_array;

public:
	EngineTypes::Map::elements_array_length_t rectangles_array_length;
	EngineTypes::Map::elements_array_length_t cyrcles_array_length;
	EngineTypes::Map::elements_array_length_t polygons_array_length;

	Map(const Map& map);
	Map(
		Rectangle* rectangles_array = nullptr,
		EngineTypes::Map::elements_array_length_t rectangles_array_length = 0,
		Cyrcle* cyrcles_array = nullptr,
		EngineTypes::Map::elements_array_length_t cyrcles_array_length = 0,
		Polygon* polygons_array = nullptr,
		EngineTypes::Map::elements_array_length_t polygons_array_length = 0);

	Rectangle GetRectangle(EngineTypes::Map::elements_array_length_t number);
	Cyrcle GetCyrcle(EngineTypes::Map::elements_array_length_t number);
	Polygon GetPolygon(EngineTypes::Map::elements_array_length_t number);
	Rectangle* GetRectanglePointer(EngineTypes::Map::elements_array_length_t number);
	Cyrcle* GetCyrclePointer(EngineTypes::Map::elements_array_length_t number);
	Polygon* GetPolygonPointer(EngineTypes::Map::elements_array_length_t number);
	void Set(Map* map);
	void Set(
		Rectangle* rectangles_array = nullptr,
		EngineTypes::Map::elements_array_length_t rectangles_array_length = 0,
		Cyrcle* cyrcles_array = nullptr,
		EngineTypes::Map::elements_array_length_t cyrcles_array_length = 0,
		Polygon* polygons_array = nullptr,
		EngineTypes::Map::elements_array_length_t polygons_array_length = 0);

	~Map();
};