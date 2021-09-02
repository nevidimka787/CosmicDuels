#pragma once

#include <iostream>
#include <math.h>

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Vec2F;
class Vec2D;
class Mat2F;
class Mat2D;

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
	Vec2D operator*(Mat2D matrix);
	void operator*=(double value);
	void operator*=(Mat2D matrix);
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
	Vec2F operator*(Mat2F matrix);
	void operator*=(float value);
	void operator*=(Mat2F matrix);
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
	void ScaleThis(Vec2F* scale);

	void Set(Vec2F* vector);
	void Set(Vec2D* vector);
	void Set(float x, float y);
	void Set(double x, double y);

	void operator=(Vec2F vector);
	void operator=(Vec2D vector);

	~Vec2F();
};

std::ostream& operator<<(std::ostream& stream, Vec2D vector);
std::ostream& operator<<(std::ostream& stream, Vec2F vector);


class Mat2D
{
public:
	double a11, a12;
	double a21, a22;

	Mat2D();
	Mat2D(double value);
	Mat2D(Vec2D* abscissa, Vec2D* ordinate);
	Mat2D(double a11, double a12, double a21, double a22);

	Mat2D operator+(Mat2D add_mat);
	void operator+=(Mat2D add_mat);
	Mat2D operator-();
	Mat2D operator-(Mat2D sub_mat);
	void operator-=(Mat2D sub_mat);
	Mat2D operator*(double mult);
	Mat2D operator*(Mat2D mat);
	void operator*=(double mult);
	void operator*=(Mat2D mat);
	Mat2D operator/(double div);
	void operator/=(double div);
	bool operator==(Mat2D vec);

	operator Mat2F();

	double Determinant();
	Mat2D Inverse();
	void InverseThis();
	Mat2D InverseNotNormalize();
	void InverseThisNotNormalize();

	void Set(double value);
	void Set(double a11, double a12, double a21, double a22);
	void Set(Vec2D* abscisse, Vec2D* ardinate);

	void operator=(Mat2D matrix);

	~Mat2D();
};


class Mat2F
{
public:
	float a11, a12;
	float a21, a22;

	Mat2F();
	Mat2F(float value);
	Mat2F(Vec2F* abscissa, Vec2F* ordinate);
	Mat2F(float a11, float a12, float a21, float a22);

	Mat2F operator+(Mat2F add_mat);
	void operator+=(Mat2F add_mat);
	Mat2F operator-();
	Mat2F operator-(Mat2F sub_mat);
	void operator-=(Mat2F sub_mat);
	Mat2F operator*(float mult);
	Mat2F operator*(Mat2F mat);
	void operator*=(float mult);
	void operator*=(Mat2F mat);
	Mat2F operator/(float div);
	void operator/=(float div);
	bool operator==(Mat2F vec);

	operator Mat2D();

	float Determinant();
	Mat2F Inverse();
	void InverseThis();
	Mat2F InverseNotNormalize();
	void InverseThisNotNormalize();

	void Set(float value);
	void Set(float a11, float a12, float a21, float a22);
	void Set(Vec2F* abscisse, Vec2F* ardinate);

	void operator=(Mat2F matrix);

	~Mat2F();
};