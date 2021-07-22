#ifndef VEC_H
#define VEC_H

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



class Vec2F
{
public:
	float x;
	float y;
	Vec2F();
	Vec2F(float x, float y);

	Vec2F operator+(Vec2F add_vector);
	Vec2F operator+=(Vec2F add_vector);
	Vec2F operator-();
	Vec2F operator-(Vec2F sub_vector);
	Vec2F operator-=(Vec2F sub_vector);
	Vec2F operator*(float mult);
	float operator*(Vec2F vec);
	Vec2F operator*=(float mult);
	float operator*=(Vec2F vec);
	Vec2F operator/(float div);
	Vec2F operator/=(float div);
	bool operator==(Vec2F vec);
	bool operator!=(Vec2F vec);

	float GetDistance(Vec2F* target);
	float GetLength();
	Vec2F Normalize();
	void NormalizeThis();
	Vec2F Project(Vec2F* projecting_vector);
	Vec2F Perpendicular();
	void PerpendicularThis();
	Vec2F Rotate(float angle);
	void RotateThis(float angle);
};


#endif // !VEC_H