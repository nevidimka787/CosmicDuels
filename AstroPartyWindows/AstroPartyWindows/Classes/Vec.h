#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

class Vec2F;
class Vec2D;

class Vec2D
{
public:
	double x;
	double y;
	Vec2D();
	Vec2D(const Vec2D& vector);
	Vec2D(double x, double y);

	Vec2D operator+(Vec2D vector);
	void operator+=(Vec2D vector);
	Vec2D operator-();
	Vec2D operator-(Vec2D vector);
	void operator-=(Vec2D vector);
	Vec2D operator*(double value);
	double operator*(Vec2D vector);
	void operator*=(double value);
	Vec2D operator/(double value);
	void operator/=(double value);
	bool operator==(Vec2D vector);
	bool operator!=(Vec2D vector);

	operator Vec2F();

	double GetAbsoluteAngle();
	double GetDistance(Vec2D* target);
	double GetLength();
	Vec2D Normalize();
	void NormalizeThis();
	Vec2D Project(Vec2D* projecting_vector);
	Vec2D Perpendicular();
	void PerpendicularThis();
	Vec2D Rotate(double angle);
	void RotateThis(double angle);
	Vec2D Scale(Vec2D* scale);

	void Set(Vec2F* vector);
	void Set(Vec2D* vector);
	void Set(float x, float y);
	void Set(double x, double y);

	void operator=(Vec2F vector);
	void operator=(Vec2D vector);

	~Vec2D();
};



class Vec2F
{
public:
	float x;
	float y;
	Vec2F();
	Vec2F(const Vec2F& vector);
	Vec2F(float x, float y);

	Vec2F operator+(Vec2F vector);
	void operator+=(Vec2F vector);
	Vec2F operator-();
	Vec2F operator-(Vec2F vector);
	void operator-=(Vec2F vector);
	Vec2F operator*(float value);
	float operator*(Vec2F vector);
	void operator*=(float value);
	Vec2F operator/(float value);
	void operator/=(float value);
	bool operator==(Vec2F vector);
	bool operator!=(Vec2F vector);

	operator Vec2D();

	float GetAbsoluteAngle();
	float GetDistance(Vec2F* target);
	float GetLength();
	Vec2F Normalize();
	void NormalizeThis();
	Vec2F Project(Vec2F* projecting_vector);
	Vec2F Perpendicular();
	void PerpendicularThis();
	Vec2F Rotate(float angle);
	void RotateThis(float angle);
	Vec2F Scale(Vec2F* scale);

	void Set(Vec2F* vector);
	void Set(Vec2D* vector);
	void Set(float x, float y);
	void Set(double x, double y);

	void operator=(Vec2F vector);
	void operator=(Vec2D vector);

	~Vec2F();
};