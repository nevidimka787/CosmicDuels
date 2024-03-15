#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

namespace Map
{
	class MapElement;
	class Rectangle;
	class Cyrcle;
	class Polygon;
	class MapData;

	class MapElement
	{
	protected:
		Vec2F position;
		Vec2F last_position;
		EngineTypes::Map::property_t properties;
	public:
		bool exist;
		MapElement();
		MapElement(const MapElement& map_element);
		MapElement(
			Vec2F position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		MapElement(
			const Vec2F* position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		Vec2F GetPosition() const;
		EngineTypes::Map::property_t Prorerties() const;
		Vec2F GetVelocity() const;
		bool IsKiller() const;
		bool IsAggressive() const;
		bool IsUnbreacable() const;
		void Move(const Vec2F& move_vector);
		void Move(const Vec2F* move_vector);
		void SetPosition(const Vec2F& position);
		void SetPosition(const Vec2F* position);
		void SetProperties(EngineTypes::Map::property_t properties);
		void Set(const MapElement* map_element);
		void Set(
			const Vec2F& position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Set(
			const Vec2F* position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Update();

		void operator=(MapElement map_element);

		~MapElement();
	};

	class Rectangle : public MapElement
	{
	private:
		Rectangle(
			Vec2F point1,
			Vec2F point2,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		Rectangle(
			const Vec2F* point1,
			const Vec2F* point2,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
	protected:
		Vec2F point2;//down left point
	public:
		Rectangle();
		Rectangle(const Rectangle& rectangle);
		Rectangle(
			const Segment& diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		Rectangle(
			const Segment* diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
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
		bool IsCollision(const Beam* beam, Vec2F* out_position, float* distance_to_out_position) const;
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
			const Segment& diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Set(
			const Segment* diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void SetPosition(Vec2F position);
		void SetPosition(const Vec2F* position);
		void SetSize(Vec2F size);
		void SetSize(const Vec2F* size);

		void operator=(const Rectangle& rectangle);

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
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		Cyrcle(
			const Vec2F* position,
			float radius = CYRCLE_DEFAULT_RADIUS,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		bool IsCollision(const Beam* beam) const;
		bool IsCollision(
			const Beam* beam, 
			Vec2F* out_position,
			float* distance_to_out_position // distance from beam point to the nearest intersection position
		) const;
		bool IsCollision(const Line* line) const;
		bool IsCollision(const Segment* segment) const;
		float GetRadius() const;
		void SetRadius(float radius);
		void Set(const Cyrcle* cyrcle);
		void Set(
			Vec2F position,
			float radius = CYRCLE_DEFAULT_RADIUS,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Set(
			const Vec2F* position,
			float radius = CYRCLE_DEFAULT_RADIUS,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
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
		bool need_update;

		Vec2F last_position;
		Vec2F last_size;
		float last_angle;

		Vec2F* local_points_array;
		EngineTypes::Polygon::points_array_length_t points_count;

	public:
		Vec2F* points_array;
		float angle;
		Vec2F size;

		Polygon();
		Polygon(const Polygon& polygon);
		Polygon(
			Vec2F position,
			float angle,
			Vec2F size,
			const Vec2F* points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES | MAP_PROPERTY_CLOSED,
			bool exist = true);
		Polygon(
			const Vec2F* position,
			float angle,
			const Vec2F* size,
			const Vec2F* points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		bool IsClosed() const;
		bool IsCollision(const Beam* beam) const;
		bool IsCollision(const Beam* beam, Vec2F* out_position, float* distance_to_out_position) const;
		bool IsCollision(const Line* line) const;
		bool IsCollision(const Segment* segment) const;
		//If polygon should update enges then function returns true.
		bool IsShouldUpdate() const;
		//The function set flag that indicated that should update edges of the polygon.
		void ShouldUpdate();
		//The function return resize velocity.
		Vec2F DynamicalProperties() const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(float* angular_velocty) const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(Vec2F* velocity) const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(float* angular_velocty, Vec2F* resize_velocity) const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(Vec2F* velocity, Vec2F* resize_velocity) const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(Vec2F* velocity, float* angular_velocty) const;
		//The function returns dynamic properties of the polygon.
		void DynamicalProperties(Vec2F* velocity, float* angular_velocty, Vec2F* resize_velocity) const;
		EngineTypes::Polygon::points_array_length_t PointsCount() const;
		void Set(const Polygon* parent);
		void Set(
			Vec2F position,
			float angle,
			Vec2F size,
			const Vec2F* points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Set(
			const Vec2F* position,
			float angle,
			const Vec2F* size,
			const Vec2F* points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void SetAngle(float angle);
		void SetSize(const Vec2F& size);
		void ToDefault();
		//The function set local_points by global points, position and angle.
		void UpdatePoints();
		void Update();
		//The function return velocity of point in the polygon.
		Vec2F Velocity(Vec2F point) const;
		//The function return velocity of point in the polygon.
		Vec2F Velocity(const Vec2F* point) const;
		//The function return velocity of point in the polygon.
		Vec2F VelocityLocal(Vec2F point) const;
		//The function return velocity of point in the polygon.
		Vec2F VelocityLocal(const Vec2F* point) const;

		void operator=(Polygon polygon);

		~Polygon();
	};

	class MapData
	{
	protected:
		Rectangle* rectangles_array;
		Cyrcle* cyrcles_array;
		Polygon* polygons_array;

	public:
		EngineTypes::Map::array_length_t rectangles_array_length;
		EngineTypes::Map::array_length_t cyrcles_array_length;
		EngineTypes::Map::array_length_t polygons_array_length;

		MapData(const MapData& map);
		MapData(
			const Rectangle* rectangles_array = nullptr,
			EngineTypes::Map::array_length_t rectangles_array_length = 0,
			const Cyrcle* cyrcles_array = nullptr,
			EngineTypes::Map::array_length_t cyrcles_array_length = 0,
			const Polygon* polygons_array = nullptr,
			EngineTypes::Map::array_length_t polygons_array_length = 0);

		Rectangle GetRectangle(EngineTypes::Map::array_length_t number) const;
		Cyrcle GetCyrcle(EngineTypes::Map::array_length_t number) const;
		Polygon GetPolygon(EngineTypes::Map::array_length_t number) const;
		Rectangle* RectanglePointer(EngineTypes::Map::array_length_t number) const;
		Cyrcle* CyrclePointer(EngineTypes::Map::array_length_t number) const;
		Polygon* PolygonPointer(EngineTypes::Map::array_length_t number) const;
		void Set(const MapData* map);
		void Set(
			const Rectangle* rectangles_array = nullptr,
			EngineTypes::Map::array_length_t rectangles_array_length = 0,
			const Cyrcle* cyrcles_array = nullptr,
			EngineTypes::Map::array_length_t cyrcles_array_length = 0,
			const Polygon* polygons_array = nullptr,
			EngineTypes::Map::array_length_t polygons_array_length = 0);

		~MapData();
	};
}