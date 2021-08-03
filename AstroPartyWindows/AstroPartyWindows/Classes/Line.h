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
	Line(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point);

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

	~Line();
};

class Beam
{
private:
public:
	Vec2F* point;
	Vec2F* vector;

	Beam();
	Beam(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point);

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

	~Beam();
};

class Segment
{
private:
public:
	Vec2F* point;
	Vec2F* vector;

	Segment();
	Segment(Vec2F* point, Vec2F* point_vector, bool second_argument_is_point);

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

	~Segment();
};