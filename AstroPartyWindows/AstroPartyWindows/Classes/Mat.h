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
	Vec2D(double v);
	Vec2D(double x, double y);

	Vec2D operator+(const Vec2D& vector) const;
	void operator+=(const Vec2D& vector);
	Vec2D operator-() const;
	Vec2D operator-(const Vec2D& vector) const;
	void operator-=(const Vec2D& vector);
	Vec2D operator*(double value) const;
	double operator*(const Vec2D& vector) const;
	Vec2D operator*(const Mat2D& matrix) const;
	Vec2D operator*(const Mat3x2D& matrix) const;
	void operator*=(double value);
	void operator*=(const Mat2D& matrix);
	void operator*=(const Mat3x2D& matrix);
	Vec2D operator/(double value) const;
	void operator/=(double value);
	//vectors are parallel
	bool operator||(const Vec2D& vector) const;
	bool operator==(double value) const;
	bool operator==(const Vec2D& vector) const;
	bool operator!=(double value) const;
	bool operator!=(const Vec2D& vector) const;
	bool operator>(double value) const;
	bool operator>(const Vec2D& vector) const;
	bool operator>=(double value) const;
	bool operator>=(const Vec2D& vector) const;
	bool operator<(double value) const;
	bool operator<(const Vec2D& vector) const;
	bool operator<=(double value) const;
	bool operator<=(const Vec2D& vector) const;

	operator Vec2F();

	double AbsoluteAngle() const;
	double Distance(const Vec2D& target) const;
	double Distance(const Vec2D* target) const;
	double Length() const;
	Vec2D Normalize() const;
	void NormalizeThis();
	Vec2D Project(const Vec2D& projecting_vector) const;
	Vec2D Project(const Vec2D* projecting_vector) const;
	Vec2D Perpendicular() const;
	void PerpendicularThis();
	Vec2D Rotate(double angle) const;
	void RotateThis(double angle);
	Vec2D Scale(const Vec2D& scale) const;
	Vec2D Scale(const Vec2D* scale) const;
	void ScaleThis(const Vec2D& scale);
	void ScaleThis(const Vec2D* scale);

	void Set(const Vec2F& vector);
	void Set(const Vec2F* vector);
	void Set(const Vec2D& vector);
	void Set(const Vec2D* vector);
	void Set(float x, float y);
	void Set(double x, double y);

	void operator=(const Vec2F& vector);
	void operator=(const Vec2D& vector);

	~Vec2D();
};

class Vec2F
{
public:
	float x, y;
	Vec2F();
	Vec2F(const Vec2F& vector);
	Vec2F(float v);
	Vec2F(float x, float y);

	Vec2F operator+(const Vec2F& vector) const;
	Vec2F operator+(float value) const;
	void operator+=(const Vec2F& vector);
	Vec2F operator-() const;
	Vec2F operator-(const Vec2F& vector) const;
	Vec2F operator-(float value) const;
	void operator-=(const Vec2F& vector);
	Vec2F operator*(float value) const;
	float operator*(const Vec2F& vector) const;
	Vec2F operator*(const Mat2F& matrix) const;
	Vec2F operator*(const Mat3x2F& matrix) const;
	void operator*=(float value);
	void operator*=(const Mat2F& matrix);
	void operator*=(const Mat3x2F& matrix);
	Vec2F operator/(float value) const;
	void operator/=(float value);
	//vectors are parallel
	bool operator||(const Vec2F& vector) const;
	bool operator==(float value) const;
	bool operator==(const Vec2F& vector) const;
	bool operator!=(float value) const;
	bool operator!=(const Vec2F& vector) const;
	bool operator>(float value) const;
	bool operator>(const Vec2F& vector) const;
	bool operator>=(float value) const;
	bool operator>=(const Vec2F& vector) const;
	bool operator<(float value) const;
	bool operator<(const Vec2F& vector) const;
	bool operator<=(float value) const;
	bool operator<=(const Vec2F& vector) const;

	operator Vec2D();

	float AbsoluteAngle() const;
	float AbsoluteAngleClockwise() const;
	float Distance(const Vec2F& target) const;
	float Distance(const Vec2F* target) const;
	float Length() const;
	float LengthPow2() const;
	Vec2F Normalize() const;
	Vec2F NormalizePow2() const;
	void NormalizeThis();
	void NormalizeThisPow2();
	//Return project of "projection_vector" to this vector.
	Vec2F Project(Vec2F projection_vector) const;
	//Return project of "projection_vector" to this vector.
	Vec2F Project(const Vec2F* projection_vector) const;
	//Return project of "projection_vector" to this vector.
	//If angle between "projection_vector" and this vector more then 90 degrees the function return zero vector.
	Vec2F ProjectSign(Vec2F projection_vector) const;
	//Return project of "projection_vector" to this vector.
	//If angle between "projection_vector" and this vector more then 90 degrees the function return zero vector.
	Vec2F ProjectSign(const Vec2F* projection_vector) const;
	//The function return perpendiculat to posetive rotation.
	Vec2F Perpendicular() const;
	//The function rotate vector on 90 degrees.
	void PerpendicularThis();
	//The function return perpendiculat to clockwise.
	Vec2F PerpendicularClockwise() const;
	//The function rotate vector on -90 degrees.
	void PerpendicularClockwiseThis();
	//Clockwise is positive.
	Vec2F RotateClockwise(float angle) const;
	void RotateClockwiseThis(float angle);
	Vec2F Rotate(float angle) const;
	void RotateThis(float angle);
	Vec2F Scale(const Vec2F& scale) const;
	Vec2F Scale(const Vec2F* scale) const;
	void ScaleThis(const Vec2F& scale);
	void ScaleThis(const Vec2F* scale);

	void Set(const Vec2F& vector);
	void Set(const Vec2F* vector);
	void Set(const Vec2D& vector);
	void Set(const Vec2D* vector);
	void Set(float x, float y);
	void Set(double x, double y);

	void operator=(const Vec2F& vector);
	void operator=(const Vec2D& vector);

	~Vec2F();
};

std::ostream& operator<<(std::ostream& stream, const Vec2D& vector);
std::ostream& operator<<(std::ostream& stream, const Vec2F& vector);

class Mat2D
{
public:
	double
		a11, a12,
		a21, a22;

	Mat2D();
	Mat2D(double value);
	Mat2D(const Vec2D& abscissa,  const Vec2D& ordinate);
	Mat2D(const Vec2D* abscissa, const Vec2D* ordinate);
	Mat2D(double a11, double a12, double a21, double a22);

	Mat2D operator+(const Mat2D& matrix) const;
	void operator+=(const Mat2D& matrix);
	Mat2D operator-() const;
	Mat2D operator-(const Mat2D& matrix) const;
	void operator-=(const Mat2D& matrix);
	Mat2D operator*(double mult) const;
	Mat2D operator*(const Mat2D& matrix) const;
	void operator*=(double mult);
	void operator*=(const Mat2D& matrix);
	Mat2D operator/(double value) const;
	void operator/=(double value);
	bool operator==(const Mat2D& matrix) const;

	operator Mat2F();

	double Determinant() const;
	Mat2D Inverse() const;
	void InverseThis();
	Mat2D InverseNotNormalize() const;
	void InverseThisNotNormalize();

	void Set(double value);
	void Set(double a11, double a12, double a21, double a22);
	void Set(const Vec2D& abscisse,  const Vec2D& ardinate);
	void Set(const Vec2D* abscisse, const Vec2D* ardinate);

	void operator=(const Mat2D& matrix);

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
	Mat2F(const Vec2F& abscissa,  const Vec2F& ordinate);
	Mat2F(const Vec2F* abscissa, const Vec2F* ordinate);
	Mat2F(float a11, float a12, float a21, float a22);

	Mat2F operator+(const Mat2F& matrix) const;
	void operator+=(const Mat2F& matrix);
	Mat2F operator-() const;
	Mat2F operator-(const Mat2F& matrix) const;
	void operator-=(const Mat2F& matrix);
	Mat2F operator*(float mult) const;
	Mat2F operator*(const Mat2F& matrix) const;
	void operator*=(float mult);
	void operator*=(const Mat2F& matrix);
	Mat2F operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Mat2F& matrix) const;

	operator Mat2D();

	float Determinant() const;
	Mat2F Inverse() const;
	void InverseThis();
	Mat2F InverseNotNormalize() const;
	void InverseThisNotNormalize();

	void Set(float value);
	void Set(float a11, float a12, float a21, float a22);
	void Set(const Vec2F& abscisse,  const Vec2F& ardinate);
	void Set(const Vec2F* abscisse, const Vec2F* ardinate);

	void operator=(const Mat2F& matrix);

	~Mat2F();
};

std::ostream& operator<<(std::ostream& stream, Mat2D vector);
std::ostream& operator<<(std::ostream& stream, Mat2F vector);

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

	Mat3x2D operator+(const Mat3x2D& matrix) const;
	void operator+=(const Mat3x2D& matrix);
	Mat3x2D operator-() const;
	Mat3x2D operator-(const Mat3x2D& matrix) const;
	void operator-=(const Mat3x2D& matrix);
	Mat3x2D operator*(double mult) const;
	Mat3x2D operator*(const Mat3x2D& matrix) const;
	void operator*=(double mult);
	void operator*=(const Mat3x2D& matrix);
	Mat3x2D operator/(double value) const;
	void operator/=(double value);
	bool operator==(const Mat3x2D& matrix) const;

	operator Mat3x2F() const;
	operator Mat2D() const;
	operator Mat2F() const;

	double Determinant() const;
	Mat3x2D Rotate(double angle) const;
	void RotateThis(double angle);
	Mat3x2D Scale(const Vec2D& vector) const;
	Mat3x2D Scale(const Vec2D* vector) const;
	void ScaleThis(const Vec2D& vector);
	void ScaleThis(const Vec2D* vector);
	Mat3x2D Transport(const Vec2D& vector) const;
	Mat3x2D Transport(const Vec2D* vector) const;
	void TransportThis(const Vec2D& vector);
	void TransportThis(const Vec2D* vector);

	void Set(double value);
	void Set(
		double a11, double a12, double a13,
		double a21, double a22, double a23);
	void SetByAngle(double angle);
	void SetByDirection(const Vec2D& direction);
	void SetByDirection(const Vec2D* direction);
	void SetByPosition(const Vec2D& position);
	void SetByPosition(const Vec2D* position);
	void SetByScale(const Vec2D& scale);
	void SetByScale(const Vec2D* scale);

	void operator=(const Mat3x2D& matrix);

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

	Mat3x2F operator+(const Mat3x2F& matrix) const;
	void operator+=(const Mat3x2F& matrix);
	Mat3x2F operator-() const;
	Mat3x2F operator-(const Mat3x2F& matrix) const;
	void operator-=(const Mat3x2F& matrix);
	Mat3x2F operator*(float mult) const;
	Mat3x2F operator*(const Mat3x2F& matrix) const;
	void operator*=(float mult);
	void operator*=(const Mat3x2F& matrix);
	Mat3x2F operator/(float value) const;
	void operator/=(float value);
	bool operator==(const Mat3x2F& matrix) const;

	operator Mat3x2D() const;
	operator Mat2D() const;
	operator Mat2F() const;

	float Determinant() const;
	Mat3x2F Rotate(float angle) const;
	void RotateThis(float angle);
	Mat3x2F Scale(const Vec2F& vector) const;
	Mat3x2F Scale(const Vec2F* vector) const;
	void ScaleThis(const Vec2F& vector);
	void ScaleThis(const Vec2F* vector);
	Mat3x2F Transport(const Vec2F& vector) const;
	Mat3x2F Transport(const Vec2F* vector) const;
	void TransportThis(const Vec2F& vector);
	void TransportThis(const Vec2F* vector);

	void Set(float value);
	void Set(
		float a11, float a12, float a13,
		float a21, float a22, float a23);
	void SetByAngle(float angle);
	void SetByDirection(const Vec2F& direction);
	void SetByDirection(const Vec2F* direction);
	void SetByPosition(const Vec2F& position);
	void SetByPosition(const Vec2F* position);
	void SetByScale(const Vec2F& scale);
	void SetByScale(const Vec2F* scale);

	void operator=(const Mat3x2F& matrix);

	~Mat3x2F();
};

std::ostream& operator<<(std::ostream& stream, Mat3x2D vector);
std::ostream& operator<<(std::ostream& stream, Mat3x2F vector);
