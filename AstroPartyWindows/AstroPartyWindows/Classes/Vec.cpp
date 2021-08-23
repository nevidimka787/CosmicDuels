#include "Vec.h"

Vec2D::Vec2D() : x(0.0), y(0.0)
{
}

Vec2D::Vec2D(double x, double y) : x(x), y(y)
{
}

Vec2D Vec2D::operator+(Vec2D vector)
{
	return Vec2D(this->x + vector.x, this->y + vector.y);
}

Vec2D Vec2D::operator-(Vec2D vector)
{
	return Vec2D(this->x - vector.x, this->y - vector.y);
}

Vec2D Vec2D::operator-()
{
	return Vec2D(-this->x, -this->y);
}

Vec2D Vec2D::operator*(double value)
{
	return Vec2D(this->x * value, this->y * value);
}

double Vec2D::operator*(Vec2D vector)
{
	return x * vector.x + y * vector.y;
}

Vec2D Vec2D::operator/(double value)
{
	return Vec2D(this->x / value, this->y / value);
}

void Vec2D::operator+=(Vec2D vector)
{
	x += vector.x;
	y += vector.y;
}

void Vec2D::operator-=(Vec2D vector)
{
	x -= vector.x;
	y -= vector.y;
}

void Vec2D::operator*=(double value)
{
	x *= value;
	y *= value;
}

void Vec2D::operator/=(double value)
{
	x /= value;
	y /= value;
}

bool Vec2D::operator==(Vec2D vector)
{
	return (this->x == vector.x && this->y == vector.y);
}

bool Vec2D::operator!=(Vec2D vector)
{
	return (this->x != vector.x && this->y != vector.y);
}

double Vec2D::GetAbsoluteAngle()
{
	return (atan2(-y, x));
}

double Vec2D::GetDistance(Vec2D* vector)
{
	return (*this - *vector).GetLength();
}

double Vec2D::GetLength()
{
	return sqrt(x * x + y * y);
}

Vec2D Vec2D::Normalize()
{
	return *this / GetLength();
}

void Vec2D::NormalizeThis()
{
	*this /= GetLength();
}

Vec2D Vec2D::Project(Vec2D* projecting_vector)
{
	double dot = *this * *projecting_vector;
	if (dot > 0.0)
	{
		return *this * sqrt(dot / GetLength());
	}
	if (dot < 0.0)
	{

		return *this * -sqrt(-dot / GetLength());
	}
	return Vec2D();
}

Vec2D Vec2D::Perpendicular()
{
	return Vec2D(y, -x);
}

void Vec2D::PerpendicularThis()
{
	double temp = -x;
	x = y;
	y = temp;
}

Vec2D Vec2D::Rotate(double angle)
{
	return Vec2D(x * cos(angle) + y * sin(angle), y * cos(angle) - x * sin(angle));
}

void Vec2D::RotateThis(double angle)
{
	double temp = x * cos(angle) + y * sin(angle);
	y = y * cos(angle) - x * sin(angle);
	x = temp;
}

Vec2D Vec2D::Scale(Vec2D* scale)
{
	return Vec2D(x * scale->x, y * scale->y);
}



Vec2F::Vec2F() : x(0.0), y(0.0)
{
}

Vec2F::Vec2F(float x, float y) : x(x), y(y)
{
}

Vec2F Vec2F::operator+(Vec2F vector)
{
	return Vec2F(this->x + vector.x, this->y + vector.y);
}

Vec2F Vec2F::operator-(Vec2F vector)
{
	return Vec2F(this->x - vector.x, this->y - vector.y);
}

Vec2F Vec2F::operator-()
{
	return Vec2F(-this->x, -this->y);
}

Vec2F Vec2F::operator*(float mult)
{
	return Vec2F(this->x * mult, this->y * mult);
}

float Vec2F::operator*(Vec2F vector)
{
	return x * vector.x + y * vector.y;
}

Vec2F Vec2F::operator/(float value)
{

	return Vec2F(this->x / value, this->y / value);
}

void Vec2F::operator+=(Vec2F vector)
{
	x += vector.x;
	y += vector.y;
}

void Vec2F::operator-=(Vec2F vector)
{
	x -= vector.x;
	y -= vector.y;
}

void Vec2F::operator*=(float value)
{
	x *= value;
	y *= value;
}

void Vec2F::operator/=(float value)
{
	x /= value;
	y /= value;
}

bool Vec2F::operator==(Vec2F vector)
{
	return x == vector.x && y == vector.y;
}

bool Vec2F::operator!=(Vec2F vector)
{
	return (this->x != vector.x && this->y != vector.y);
}

float Vec2F::GetAbsoluteAngle()
{
	return (atan2f(-y, x));
}

float Vec2F::GetDistance(Vec2F* target_vector)
{
	return (*this - *target_vector).GetLength();
}

float Vec2F::GetLength()
{
	return sqrtf(x * x + y * y);
}

Vec2F Vec2F::Normalize()
{
	return *this / GetLength();
}

void Vec2F::NormalizeThis()
{
	*this /= GetLength();
}

Vec2F Vec2F::Project(Vec2F* projecting_vector)
{
	float dot = *this * *projecting_vector;
	if (dot > 0.0)
	{
		return *this * sqrtf(dot / GetLength());
	}
	if (dot < 0.0)
	{

		return *this * -sqrtf(-dot / GetLength());
	}
	return Vec2F();
}

Vec2F Vec2F::Perpendicular()
{
	return Vec2F(y, -x);
}

void Vec2F::PerpendicularThis()
{
	float temp = -x;
	x = y;
	y = temp;
}

Vec2F Vec2F::Rotate(float angle)
{
	float temp_cos = cosf(angle);
	float temp_sin = sinf(angle);
	return Vec2F(x * temp_cos + y * temp_sin, y * temp_cos - x * temp_sin);
}

void Vec2F::RotateThis(float angle)
{
	float temp_cos = cosf(angle);
	float temp_sin = sinf(angle);

	float temp = x * temp_cos + y * temp_sin;
	y = y * temp_cos - x * temp_sin;
	x = temp;
}

Vec2F Vec2F::Scale(Vec2F* scale)
{
	return Vec2F(x * scale->x, y * scale->y);
}