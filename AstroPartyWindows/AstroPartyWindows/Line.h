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
	Vec2D* point;
	Vec2D* vector;

	Line();
	Line(Vec2D* point, Vec2D* point_vector, bool second_argument_is_point);

	bool Intersection(Line* intersection_line, Vec2D* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2D* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2D* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	double GetDistance(Vec2D* target);
	double GetDistance(Line* target);
	double GetDistance(Beam* target);
	double GetDistance(Segment* target);

	~Line();
};

class Beam
{
private:
public:
	Vec2D* point;
	Vec2D* vector;

	Beam();
	Beam(Vec2D* point, Vec2D* point_vector, bool second_argument_is_point);

	bool Intersection(Line* intersection_line, Vec2D* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2D* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2D* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	double GetDistance(Vec2D* target);
	double GetDistance(Line* target);
	double GetDistance(Beam* target);
	double GetDistance(Segment* target);

	~Beam();
};

class Segment
{
private:
public:
	Vec2D* point;
	Vec2D* vector;

	Segment();
	Segment(Vec2D* point, Vec2D* point_vector, bool second_argument_is_point);

	bool Intersection(Line* intersection_line, Vec2D* output_intersection_point);
	bool Intersection(Beam* intersection_beam, Vec2D* output_intersection_point);
	bool Intersection(Segment* intersection_segment, Vec2D* output_intersection_point);

	bool IsIntersection(Line* intersection_line);
	bool IsIntersection(Beam* intersection_beam);
	bool IsIntersection(Segment* intersection_segment);

	double GetDistance(Vec2D* target);
	double GetDistance(Line* target);
	double GetDistance(Beam* target);
	double GetDistance(Segment* target);

	Vec2D GetSecondPoint();

	~Segment();
};