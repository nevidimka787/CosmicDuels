#pragma once

#include <iostream>
#include <math.h>

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Vec2F;
class Vec2D;
class Mat2F;
class Mat2D;
class Mat3x2F;
class Mat3x2D;

class Vec2D
{
public:
	double x, y;
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
	Vec2D operator*(Mat3x2D matrix);
	void operator*=(double value);
	void operator*=(Mat2D matrix);
	void operator*=(Mat3x2D matrix);
	Vec2D operator/(double value);
	void operator/=(double value);
	//vectors are parallel
	bool operator||(Vec2D vector);
	bool operator==(double value);
	bool operator==(Vec2D vector);
	bool operator!=(double value);
	bool operator!=(Vec2D vector);
	bool operator>(double value);
	bool operator>(Vec2D vector);
	bool operator>=(double value);
	bool operator>=(Vec2D vector);
	bool operator<(double value);
	bool operator<(Vec2D vector);
	bool operator<=(double value);
	bool operator<=(Vec2D vector);

	operator Vec2F();

	double GetAbsoluteAngle();
	double GetDistance(Vec2D target);
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
	float x, y;
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
	Vec2F operator*(Mat3x2F matrix);
	void operator*=(float value);
	void operator*=(Mat2F matrix);
	void operator*=(Mat3x2F matrix);
	Vec2F operator/(float value);
	void operator/=(float value);
	//vectors are parallel
	bool operator||(Vec2F vector);
	bool operator==(float value);
	bool operator==(Vec2F vector);
	bool operator!=(float value);
	bool operator!=(Vec2F vector);
	bool operator>(float value);
	bool operator>(Vec2F vector);
	bool operator>=(float value);
	bool operator>=(Vec2F vector);
	bool operator<(float value);
	bool operator<(Vec2F vector);
	bool operator<=(float value);
	bool operator<=(Vec2F vector);

	operator Vec2D();

	float GetAbsoluteAngle();
	float GetDistance(Vec2F target);
	float GetDistance(Vec2F* target);
	float GetLength();
	Vec2F Normalize();
	void NormalizeThis();
	//Return project of "projection_vector" to this vector.
	Vec2F Project(Vec2F* projection_vector);
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
	float
		a11, a12,
		a21, a22;

	Mat2D();
	Mat2D(double value);
	Mat2D(Vec2D* abscissa, Vec2D* ordinate);
	Mat2D(double a11, double a12, double a21, double a22);

	Mat2D operator+(Mat2D matrix);
	void operator+=(Mat2D matrix);
	Mat2D operator-();
	Mat2D operator-(Mat2D matrix);
	void operator-=(Mat2D matrix);
	Mat2D operator*(double mult);
	Mat2D operator*(Mat2D matrix);
	void operator*=(double mult);
	void operator*=(Mat2D matrix);
	Mat2D operator/(double value);
	void operator/=(double value);
	bool operator==(Mat2D vector);

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
	float 
		a11, a12,
		a21, a22;

	Mat2F();
	Mat2F(float value);
	Mat2F(Vec2F* abscissa, Vec2F* ordinate);
	Mat2F(float a11, float a12, float a21, float a22);

	Mat2F operator+(Mat2F matrix);
	void operator+=(Mat2F matrix);
	Mat2F operator-();
	Mat2F operator-(Mat2F matrix);
	void operator-=(Mat2F matrix);
	Mat2F operator*(float mult);
	Mat2F operator*(Mat2F matrix);
	void operator*=(float mult);
	void operator*=(Mat2F matrix);
	Mat2F operator/(float value);
	void operator/=(float value);
	bool operator==(Mat2F vector);

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



class Mat3x2D
{
public:
	double
		a11, a12, a13,
		a21, a22, a23;

	Mat3x2D();
	Mat3x2D(double value);
	Mat3x2D(
		double a11, double a12, double a13,
		double a21, double a22, double a23);

	Mat3x2D operator+(Mat3x2D matrix);
	void operator+=(Mat3x2D matrix);
	Mat3x2D operator-();
	Mat3x2D operator-(Mat3x2D matrix);
	void operator-=(Mat3x2D matrix);
	Mat3x2D operator*(double mult);
	Mat3x2D operator*(Mat3x2D matrix);
	void operator*=(double mult);
	void operator*=(Mat3x2D matrix);
	Mat3x2D operator/(double value);
	void operator/=(double value);
	bool operator==(Mat3x2D vector);

	operator Mat3x2F();

	double Determinant();
	Mat3x2D Rotate(double angle);
	void RotateThis(double angle);
	Mat3x2D Scale(Vec2D vector);
	Mat3x2D Scale(Vec2D* vector);
	void ScaleThis(Vec2D vector);
	void ScaleThis(Vec2D* vector);
	Mat3x2D Transport(Vec2D vector);
	Mat3x2D Transport(Vec2D* vector);
	void TransportThis(Vec2D vector);
	void TransportThis(Vec2D* vector);

	void Set(double value);
	void Set(
		double a11, double a12, double a13,
		double a21, double a22, double a23);
	void SetByAngle(double angle);
	void SetByDirection(Vec2D direction);
	void SetByDirection(Vec2D* direction);
	void SetByPosition(Vec2D position);
	void SetByPosition(Vec2D* position);
	void SetByScale(Vec2D scale);
	void SetByScale(Vec2D* scale);

	void operator=(Mat3x2D matrix);

	~Mat3x2D();
};


class Mat3x2F
{
public:
	float
		a11, a12, a13,
		a21, a22, a23;

	Mat3x2F();
	Mat3x2F(float value);
	Mat3x2F(
		float a11, float a12, float a13,
		float a21, float a22, float a23);

	Mat3x2F operator+(Mat3x2F matrix);
	void operator+=(Mat3x2F matrix);
	Mat3x2F operator-();
	Mat3x2F operator-(Mat3x2F matrix);
	void operator-=(Mat3x2F matrix);
	Mat3x2F operator*(float mult);
	Mat3x2F operator*(Mat3x2F matrix);
	void operator*=(float mult);
	void operator*=(Mat3x2F matrix);
	Mat3x2F operator/(float value);
	void operator/=(float value);
	bool operator==(Mat3x2F vector);

	operator Mat3x2D();

	float Determinant();
	Mat3x2F Rotate(double angle);
	void RotateThis(double angle);
	Mat3x2F Scale(Vec2F vector);
	Mat3x2F Scale(Vec2F* vector);
	void ScaleThis(Vec2F vector);
	void ScaleThis(Vec2F* vector);
	Mat3x2F Transport(Vec2F vector);
	Mat3x2F Transport(Vec2F* vector);
	void TransportThis(Vec2F vector);
	void TransportThis(Vec2F* vector);

	void Set(float value);
	void Set(
		float a11, float a12, float a13,
		float a21, float a22, float a23);
	void SetByAngle(float angle);
	void SetByDirection(Vec2F direction);
	void SetByDirection(Vec2F* direction);
	void SetByPosition(Vec2F position);
	void SetByPosition(Vec2F* position);
	void SetByScale(Vec2F scale);
	void SetByScale(Vec2F* scale);

	void operator=(Mat3x2F matrix);

	~Mat3x2F();
};