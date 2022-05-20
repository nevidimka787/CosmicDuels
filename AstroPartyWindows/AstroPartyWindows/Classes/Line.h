#pragma once

#include <iostream>

#include "Mat.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Beam;
class Line;
class Segment;

class Line
{
private:
public:
	Vec2F point;
	Vec2F vector;

	Line();

	Line(const Line& line);
	
	Line(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);
	
	Line(
		const Vec2F* point, 
		const Vec2F* point_vector, 
		bool second_argument_is_point = false);
	
	void Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const;
	
	bool Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const;
	
	bool Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const;

	bool IsIntersection(const Beam* intersection_beam) const;
	
	bool IsIntersection(const Segment* intersection_segment) const;

	float Distance(Vec2F target) const;
	
	float Distance(const Vec2F* target) const;
	
	float Distance(Vec2F target, Vec2F* nearest_point) const;
	
	float Distance(const Vec2F* target, Vec2F* nearest_point) const;
	
	float Distance(const Line* target) const;
	
	float Distance(const Beam* target) const;

	float Distance(const Beam* target, Vec2F* nearest_point) const;
	
	float Distance(const Segment* target) const;

	float Distance(const Segment* target, Vec2F* nearest_point) const;

	void Set(const Line* line);
	
	void Set(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);
	
	void Set(
		const Vec2F* point,
		const Vec2F* point_vector,
		bool second_argument_is_point = false);

	
	void operator=(Line line);
	
	void operator=(Beam line);
	
	void operator=(Segment line);

	operator Beam();
	
	operator Segment();

	~Line();
};

class Beam
{
private:
public:

	Vec2F point;
	Vec2F vector;

	Beam();

	Beam(const Beam& beam);

	Beam(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);

	Beam(
		const Vec2F* point,
		const Vec2F* point_vector,
		bool second_argument_is_point = false);


	float Distance(Vec2F target) const;

	float Distance(const Beam* target) const;

	float Distance(const Beam* target, Vec2F* nearest_point) const;

	float Distance(const Line* target) const;

	float Distance(const Line* target, Vec2F* nearest_point) const;

	float Distance(const Segment* target) const;

	float Distance(const Segment* target, Vec2F* nearest_point) const;

	float Distance(const Vec2F* target) const;

	float Distance(Vec2F target, Vec2F* nearest_point) const;

	float Distance(const Vec2F* target, Vec2F* nearest_point) const;

	bool Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const;

	bool Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const;

	bool Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const;

	bool IsIntersection(const Beam* intersection_beam) const;

	bool IsIntersection(const Line* intersection_line) const;

	bool IsIntersection(const Segment* intersection_segment) const;
	
	void Set(const Beam* beam);
	
	void Set(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);
	
	void Set(
		const Vec2F* point,
		const Vec2F* point_vector,
		bool second_argument_is_point = false);

	
	void operator=(Line line);
	
	void operator=(Beam line);
	
	void operator=(Segment line);

	operator Line();
	
	operator Segment();

	~Beam();
};

class Segment
{
public:
	Vec2F point;
	Vec2F vector;

	Segment();
	
	Segment(const Segment& segment);
	
	Segment(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);
	
	Segment(
		const Vec2F* point,
		const Vec2F* point_vector,
		bool second_argument_is_point = false);


	float Distance(const Beam* target) const;

	float Distance(const Beam* target, Vec2F* nearest_point) const;

	float Distance(const Line* target) const;

	float Distance(const Line* target, Vec2F* nearest_point) const;

	float Distance(const Segment* target) const;

	float Distance(const Segment* target, Vec2F* nearest_point) const;

	float Distance(Vec2F target) const;

	float Distance(const Vec2F* target) const;

	float Distance(Vec2F target, Vec2F* nearest_point) const;

	float Distance(const Vec2F* target, Vec2F* nearest_point) const;

	bool Intersection(const Beam* intersection_beam, Vec2F* output_intersection_point) const;
	
	bool Intersection(const Line* intersection_line, Vec2F* output_intersection_point) const;
	
	bool Intersection(const Segment* intersection_segment, Vec2F* output_intersection_point) const;

	bool IsIntersection(const Beam* intersection_beam) const;

	bool IsIntersection(const Line* intersection_line) const;
	
	bool IsIntersection(const Segment* intersection_segment) const;

	Vec2F SecondPoint() const;

	void Set(const Segment* segment);
	
	void Set(
		Vec2F point,
		Vec2F point_vector,
		bool second_argument_is_point = false);
	
	void Set(
		const Vec2F* point,
		const Vec2F* point_vector,
		bool second_argument_is_point = false);

	
	void operator=(Line line);
	
	void operator=(Beam line);
	
	void operator=(Segment line);

	operator Line();
	
	operator Beam();

	~Segment();
};