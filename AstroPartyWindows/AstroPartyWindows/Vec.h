#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

class Vec2D
{
public:
	double x;
	double y;
	Vec2D();
	Vec2D(double x, double y);

	Vec2D operator+(Vec2D add_vector);
	Vec2D operator+=(Vec2D add_vector);
	Vec2D operator-();
	Vec2D operator-(Vec2D sub_vector);
	Vec2D operator-=(Vec2D sub_vector);
	Vec2D operator*(double mult);
	double operator*(Vec2D vec);
	Vec2D operator*=(double mult);
	double operator*=(Vec2D vec);
	Vec2D operator/(double div);
	Vec2D operator/=(double div);
	bool operator==(Vec2D vec);
	bool operator!=(Vec2D vec);

	double GetDistance(Vec2D* target);
	double GetLength();
	Vec2D Normalize();
	void NormalizeThis();
	Vec2D Project(Vec2D* projecting_vector);
	Vec2D Perpendicular();
	void PerpendicularThis();
	Vec2D Rotate(double angle);
	void RotateThis(double angle);
};
