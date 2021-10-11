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
		Vec2F position,
		bool unbreakable = true,
		bool exist = true);
	MapElement(
		const Vec2F* position,
		bool unbreakable = true,
		bool exist = true);

	Vec2F Position();
	Vec2F Velocity();
	bool IsUnbreacable();
	void Move(Vec2F move_vector);
	void Move(const Vec2F* move_vector);
	void Update();
	void SetPosition(Vec2F position);
	void SetPosition(const Vec2F* position);
	void Set(const MapElement* map_element);
	void Set(
		Vec2F position,
		bool unbreakable = true,
		bool exist = true);
	void Set(
		const Vec2F* position,
		bool unbreakable = true,
		bool exist = true);
	void operator=(MapElement map_element);

	~MapElement();
};

class Rectangle : public MapElement
{
private:
	Rectangle(
		Vec2F point1, 
		Vec2F point2,
		bool unbreakable = true,
		bool exist = true);
	Rectangle(
		const Vec2F* point1,
		const Vec2F* point2,
		bool unbreakable = true,
		bool exist = true);
protected:
	Vec2F point2;//down left point
public:
	Rectangle();
	Rectangle(const Rectangle& rectangle);
	Rectangle(
		const Segment* diagonal,
		bool unbreakable = true,
		bool exist = true);

	Vec2F UpRightPoint() const;
	Vec2F DownRightPoint() const;
	Vec2F DownLeftPoint() const;
	Vec2F UpLeftPoint() const;
	Segment UpSide() const;
	Segment DownSide() const;
	Segment RightSide() const;
	Segment LeftSide() const;
	bool IsCollision(const Beam* beam);
	bool IsCollision(const Line* line);
	bool IsCollision(const Segment* segment);
	void Move(Vec2F move_vector);
	void Move(const Vec2F* move_vector);
	//set point2 as down left point and point1 as up right point
	Rectangle Normalise() const;
	//set point2 as down left point and point1 as up right point
	void NormaliseThis();
	Vec2F Position() const;
	void Set(const Rectangle* patent);
	void Set(
		const Segment* diagonal,
		bool unbreakable = true,
		bool exist = true);
	void SetPosition(Vec2F position);
	void SetPosition(const Vec2F* position);
	void SetSize(Vec2F size);
	void SetSize(const Vec2F* size);

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
		Vec2F position,
		float radius = CYRCLE_DEFAULT_RADIUS,
		bool unbreakable = true,
		bool exist = true);
	Cyrcle(
		const Vec2F* position,
		float radius = CYRCLE_DEFAULT_RADIUS,
		bool unbreakable = true,
		bool exist = true);

	bool IsCollision(const Beam* beam);
	bool IsCollision(const Line* line);
	bool IsCollision(const Segment* segment);
	float Radius();
	void SetRadius(float radius);
	void Set(const Cyrcle* cyrcle);
	void Set(
		Vec2F position,
		float radius = CYRCLE_DEFAULT_RADIUS,
		bool unbreakable = true,
		bool exist = true);
	void Set(
		const Vec2F* position,
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
		Vec2F position,
		const Vec2F* points_array,
		EngineTypes::Polygon::points_array_length_t points_array_length,
		bool unbreakable = true,
		bool exist = true);
	Polygon(
		const Vec2F* position,
		const Vec2F* points_array,
		EngineTypes::Polygon::points_array_length_t points_array_length,
		bool unbreakable = true,
		bool exist = true);

	bool IsCollision(const Beam* beam);
	bool IsCollision(const Line* line);
	bool IsCollision(const Segment* segment);
	void RotateGlobal(float angle, Vec2F global_rotating_point);
	void RotateGlobal(float angle, const Vec2F* global_rotating_point);
	void RotateLocal(float angle, Vec2F local_rotating_point);
	void RotateLocal(float angle, const Vec2F* local_rotating_point);
	void Move(Vec2F move_vector);
	void Move(const Vec2F* move_vector);
	void ToDefault();
	void Set(const Polygon* parent);
	void Set(
		Vec2F position,
		const Vec2F* points_array,
		EngineTypes::Polygon::points_array_length_t points_array_length,
		bool unbreakable = true,
		bool exist = true);
	void Set(
		const Vec2F* position,
		const Vec2F* points_array,
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
		const Rectangle* rectangles_array = nullptr,
		EngineTypes::Map::elements_array_length_t rectangles_array_length = 0,
		const Cyrcle* cyrcles_array = nullptr,
		EngineTypes::Map::elements_array_length_t cyrcles_array_length = 0,
		const Polygon* polygons_array = nullptr,
		EngineTypes::Map::elements_array_length_t polygons_array_length = 0);

	Rectangle GetRectangle(EngineTypes::Map::elements_array_length_t number);
	Cyrcle GetCyrcle(EngineTypes::Map::elements_array_length_t number);
	Polygon GetPolygon(EngineTypes::Map::elements_array_length_t number);
	Rectangle* GetRectanglePointer(EngineTypes::Map::elements_array_length_t number);
	Cyrcle* GetCyrclePointer(EngineTypes::Map::elements_array_length_t number);
	Polygon* GetPolygonPointer(EngineTypes::Map::elements_array_length_t number);
	void Set(const Map* map);
	void Set(
		const Rectangle* rectangles_array = nullptr,
		EngineTypes::Map::elements_array_length_t rectangles_array_length = 0,
		const Cyrcle* cyrcles_array = nullptr,
		EngineTypes::Map::elements_array_length_t cyrcles_array_length = 0,
		const Polygon* polygons_array = nullptr,
		EngineTypes::Map::elements_array_length_t polygons_array_length = 0);

	~Map();
};