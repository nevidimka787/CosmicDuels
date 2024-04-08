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
			const Vec2F& position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		Vec2F GetPosition() const;
		Vec2F GetVelocity() const;
		bool HasAllProrerties(EngineTypes::Map::property_t obtained_properties) const;
		bool HasNoProrerties(EngineTypes::Map::property_t obtained_properties) const;
		bool HasSomeProrerties(EngineTypes::Map::property_t obtained_properties) const;
		bool IsKiller() const;
		bool IsAggressive() const;
		bool IsCheckCollisionsInside() const;
		bool IsCheckCollisionsOutside() const;
		bool IsUnbreacable() const;
		void Move(const Vec2F& move_vector);
		EngineTypes::Map::property_t Prorerties() const;
		void RessetVelocity();
		void SetPosition(const Vec2F& position);
		void SetProperties(EngineTypes::Map::property_t properties);
		void Set(const MapElement* map_element);
		void Set(
			const Vec2F& position,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);
		void Update();

		void operator=(const MapElement& map_element);

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

	protected:
		Vec2F point2;//down left point

	public:
		Rectangle();
		Rectangle(const Rectangle& rectangle);
		Rectangle(
			const Segment& diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		Vec2F GetUpRightPoint() const;
		Vec2F GetDownRightPoint() const;
		Vec2F GetDownLeftPoint() const;
		Vec2F GetUpLeftPoint() const;
		Segment GetUpSide() const;
		Segment GetDownSide() const;
		Segment GetRightSide() const;
		Segment GetLeftSide() const;
		Vec2F GetSize() const;
		Vec2F GetPosition() const; // position of the rectangle's center
		bool IsCollision(const Beam& beam);
		bool IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position) const;
		bool IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position, Vec2F* perpendicular_vector) const;
		bool IsCollision(const Line& line);
		bool IsCollision(const Segment& segment);
		void Move(const Vec2F& move_vector);
		//set point2 as down left point and point1 as up right point
		Rectangle Normalise() const;
		//set point2 as down left point and point1 as up right point
		void NormaliseThis();
		// position of rectangle's center
		Vec2F Position() const;
		void Set(const Rectangle* patent);
		void Set(
			const Segment& diagonal,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		void SetPosition(const Vec2F& position);
		void SetSize(const Vec2F& size);

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
			const Vec2F& position,
			float radius = CYRCLE_DEFAULT_RADIUS,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		bool IsCollision(const Beam& beam) const;
		bool IsCollision(
			const Beam& beam,
			Vec2F* out_position,
			float* distance_to_out_position // distance from beam point to the nearest intersection position
		) const;
		bool IsCollision(
			const Beam& beam,
			Vec2F* out_position,
			float* distance_to_out_position, // distance from beam point to the nearest intersection position
			Vec2F* perpendicular_position
		) const;
		bool IsCollision(const Line& line) const;
		bool IsCollision(const Segment& segment) const;
		float GetRadius() const;
		void SetRadius(float radius);
		void Set(const Cyrcle* cyrcle);
		void Set(
			const Vec2F& position,
			float radius = CYRCLE_DEFAULT_RADIUS,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES,
			bool exist = true);

		void operator=(const Cyrcle& cyrcle);

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

		std::vector<Vec2F> local_points_array;

	public:
		std::vector<Vec2F> points_array;
		float angle;
		Vec2F size;

		Polygon();
		Polygon(const Polygon& polygon);
		Polygon(
			const Vec2F& position,
			float angle,
			const Vec2F& size,
			const std::vector<Vec2F>& points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES | MAP_PROPERTY_CLOSED,
			bool exist = true);

		bool IsClosed() const;
		bool IsCollision(const Beam& beam) const;
		bool IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position) const;
		bool IsCollision(const Beam& beam, Vec2F* out_position, float* distance_to_out_position, Vec2F* perpendicular_vector) const;
		bool IsCollision(const Line& line) const;
		bool IsCollision(const Segment& segment) const;
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
			const Vec2F& position,
			float angle,
			const Vec2F& size,
			const std::vector<Vec2F>& points_array,
			EngineTypes::Polygon::points_array_length_t points_array_length,
			EngineTypes::Map::property_t properties = MAP_DEFAULT_PROPERTIES | MAP_PROPERTY_CLOSED,
			bool exist = true);
		void SetAngle(float angle);
		void SetSize(const Vec2F& size);
		void ToDefault();
		//The function set local_points by global points, position and angle.
		void UpdatePoints();
		void Update();
		//The function return velocity of point in the polygon.
		Vec2F Velocity(const Vec2F& point) const;
		//The function return velocity of point in the polygon.
		Vec2F VelocityLocal(const Vec2F& point) const;

		void operator=(const Polygon& polygon);

		~Polygon();
	};

	class MapData
	{
	public:
		std::vector<Rectangle> rectangles_array;
		std::vector<Cyrcle> cyrcles_array;
		std::vector<Polygon> polygons_array;

		MapData(const MapData& map);
		MapData(
			const std::vector<Rectangle>& rectangles_array,
			const std::vector<Cyrcle>& cyrcles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		MapData(
			const std::vector<Cyrcle>& cyrcles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		MapData(
			const std::vector<Rectangle>& rectangles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		MapData(
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());

		void Set(const MapData* map);
		void Set(
			const std::vector<Rectangle>& rectangles_array,
			const std::vector<Cyrcle>& cyrcles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		void Set(
			const std::vector<Cyrcle>& cyrcles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		void Set(
			const std::vector<Rectangle>& rectangles_array,
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());
		void Set(
			const std::vector<Polygon>& polygons_array = std::vector<Polygon>());

		void operator=(const MapData& map);

		~MapData();
	};
}