#pragma once

#include <iostream>

#include "Mat.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Line;
class Beam;
class Segment;

class LightLine
{
public:
	Vec2F point;
	Vec2F vector;
	LightLine();
	LightLine(const LightLine& line);

	void Set(Vec2F* point, Vec2F* vector);

	~LightLine();
};

class Line
{
private:
public:
	Vec2F temp_vector1;
	Vec2F temp_vector2;
	Mat2F temp_matrix1;
	Mat2F temp_matrix2;
	LightLine temp_line1;

	Vec2F point;
	Vec2F vector;

	Line();
	Line(const Line& line);
	Line(
		Vec2F* point, 
		Vec2F* point_vector, 
		bool second_argument_is_point = false);
	//use v1
	//use m1 m2
	//alwaus true
	void Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	//alwaus true
	void Intersection(LightLine* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	//use v1
	//use m1 m2
	bool IsIntersection(Beam* intersection_beam);
	//use v1
	//use m1 m2
	bool IsIntersection(Segment* intersection_segment);

	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Vec2F* target);
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Vec2F* target, Vec2F* nearest_point);
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Line* target);
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Beam* target);
	//use v1 v2 v3
	//use m1 m2 m3
	//use l1
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
	bool temp_bool1;
	float temp_float1;
	float temp_float2;
	Vec2F temp_vector1;
	Vec2F temp_vector2;
	Mat2F temp_matrix1;
	Mat2F temp_matrix2;
	LightLine temp_line1;

	Vec2F point;
	Vec2F vector;

	Beam();
	Beam(const Beam& beam);
	Beam(
		Vec2F* point,
		Vec2F* point_vector,
		bool second_argument_is_point = false);

	//use v1
	//use m1 m2
	bool Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(LightLine* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	//use v1
	//use m1 m2
	bool IsIntersection(Line* intersection_line);
	//use v1
	//use m1 m2
	bool IsIntersection(Beam* intersection_beam);
	//use v1
	//use m1 m2
	bool IsIntersection(Segment* intersection_segment);

	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Vec2F* target);
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Vec2F* target, Vec2F* nearest_point);
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Line* target);
	//use b1
	//use f1 f2 f3
	//use v1 v2
	//use m1 m2
	//use l1
	float GetDistance(Beam* target);
	//use b1
	//use f1 f2
	//use v1 v2 v3 v4
	//use m1 m2
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
public:
	bool temp_bool1;
	float temp_float1;
	float temp_float2;
	Vec2F temp_vector1;
	Vec2F temp_vector2;
	Mat2F temp_matrix1;
	Mat2F temp_matrix2;
	LightLine temp_line1;

	Vec2F point;
	Vec2F vector;

	Segment();
	Segment(const Segment& segment);
	Segment(
		Vec2F* point,
		Vec2F* point_vector,
		bool second_argument_is_point = false);

	//use v1
	//use m1 m2
	bool Intersection(Line* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(LightLine* intersection_line, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Beam* intersection_beam, Vec2F* output_intersection_point);
	//use v1
	//use m1 m2
	bool Intersection(Segment* intersection_segment, Vec2F* output_intersection_point);

	//use v1
	//use m1 m2
	bool IsIntersection(Line* intersection_line);
	//use v1
	//use m1 m2
	bool IsIntersection(Beam* intersection_beam);
	//use v1
	//use m1 m2
	bool IsIntersection(Segment* intersection_segment);

	//use f1 f2
	//use v1 v2
	//use m1 m2
	float GetDistance(Vec2F* target);
	//use f1 f2
	//use v1 v2
	//use m1 m2
	float GetDistance(Vec2F* target, Vec2F* nearest_point);
	//use v1 v2 v3 v4
	//use m1 m2 m3
	//use l1
	float GetDistance(Line* target);
	//use b1
	//use f1 f2
	//use v1 v2 v3 v4
	//use m1 m2
	float GetDistance(Beam* target);
	//use f1 f2 f3 f4
	//use v1 v2 v3
	//use m1 m2
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