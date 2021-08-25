#pragma once
#include "Vec.h"
#include "Mat.h"
#include <iostream>

class Line;
class Beam;
class Segment;

class Line
{
private:
public:
	Vec2F* point;
	Vec2F* vector;

	Line();
	Line(const Line& line);
	Line(
		Vec2F* point, 
		Vec2F* point_vector, 
		bool second_argument_is_point = false);

	bool Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	float GetDistance(Vec2F* target);
	float GetDistance(Line* target);
	float GetDistance(Beam* target);
	float GetDistance(Segment* target);

	void Set(Line* line);
	void Set(
		Vec2F* point,
		Vec2F* point_vector,
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
	Vec2F* point;
	Vec2F* vector;

	Beam();
	Beam(const Beam& beam);
	Beam(
		Vec2F* point,
		Vec2F* point_vector,
		bool second_argument_is_point = false);

	bool Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	float GetDistance(Vec2F* target);
	float GetDistance(Line* target);
	float GetDistance(Beam* target);
	float GetDistance(Segment* target);
	
	void Set(Beam* beam);
	void Set(
		Vec2F* point,
		Vec2F* point_vector,
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
private:
public:
	Vec2F* point;
	Vec2F* vector;

	Segment();
	Segment(const Segment& segment);
	Segment(
		Vec2F* point,
		Vec2F* point_vector,
		bool second_argument_is_point = false);

	bool Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	float GetDistance(Vec2F* target);
	float GetDistance(Line* target);
	float GetDistance(Beam* target);
	float GetDistance(Segment* target);

	Vec2F GetSecondPoint();

	void Set(Segment* segment);
	void Set(
		Vec2F* point,
		Vec2F* point_vector,
		bool second_argument_is_point = false);

	void operator=(Line line);
	void operator=(Beam line);
	void operator=(Segment line);

	operator Line();
	operator Beam();

	~Segment();
};