#include "Mat.h"

Vec2D::Vec2D() :
	x(0.0),
	y(0.0)
{
}

Vec2D::Vec2D(const Vec2D& vector) :
	x(vector.x),
	y(vector.y)
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

Vec2D Vec2D::operator*(Mat2D matrix)
{
	return Vec2D(x * matrix.a11 + y * matrix.a12, x * matrix.a21 + y * matrix.a22);
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

void Vec2D::operator*=(Mat2D matrix)
{
	double temp = x * matrix.a11 + y * matrix.a12;
	y = x * matrix.a21 + y * matrix.a22;
	x = temp;
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

Vec2D::operator Vec2F()
{
	return Vec2F(x, y);
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

void Vec2D::Set(Vec2F* vector)
{
	x = vector->x;
	y = vector->y;
}

void Vec2D::Set(Vec2D* vector)
{
	x = vector->x;
	y = vector->y;
}

void Vec2D::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vec2D::Set(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Vec2D::operator=(Vec2F vector)
{
	x = vector.x;
	y = vector.y;
}

void Vec2D::operator=(Vec2D vector)
{
	x = vector.x;
	y = vector.y;
}

Vec2D::~Vec2D()
{
}



Vec2F::Vec2F() : x(0.0), y(0.0)
{
}

Vec2F::Vec2F(const Vec2F& vector) :
	x(vector.x),
	y(vector.y)
{
}

Vec2F::Vec2F(float x, float y) :
	x(x),
	y(y)
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

Vec2F Vec2F::operator*(Mat2F matrix)
{
	return Vec2F(x * matrix.a11 + y * matrix.a12, x * matrix.a21 + y * matrix.a22);
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

void Vec2F::operator*=(Mat2F matrix)
{
	double temp = x * matrix.a11 + y * matrix.a12;
	y = x * matrix.a21 + y * matrix.a22;
	x = temp;
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

Vec2F::operator Vec2D()
{
	return Vec2D(x, y);
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
	Vec2F direction = Normalize();
	return direction * projecting_vector->GetLength() * (direction * projecting_vector->Normalize());
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
	return Vec2F(x * temp_cos + y * temp_sin, x * temp_sin - y * temp_cos);
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

void Vec2F::ScaleThis(Vec2F* scale)
{
	x *= scale->x;
	y *= scale->y;
}

void Vec2F::Set(Vec2F* vector)
{
	x = vector->x;
	y = vector->y;
}

void Vec2F::Set(Vec2D* vector)
{
	x = vector->x;
	y = vector->y;
}

void Vec2F::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vec2F::Set(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Vec2F::operator=(Vec2F vector)
{
	x = vector.x;
	y = vector.y;
}

void Vec2F::operator=(Vec2D vector)
{
	x = vector.x;
	y = vector.y;
}

Vec2F::~Vec2F()
{
}



std::ostream& operator<<(std::ostream& stream, Vec2F vector)
{
	stream << "X: " << vector.x << ' ' << "Y: " << vector.y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Vec2D vector)
{
	stream << "X: " << vector.x << ' ' << "Y: " << vector.y;
	return stream;
}

Mat2D::Mat2D() : a11(0.0), a12(0.0), a21(0.0), a22(0.0)
{
}

Mat2D::Mat2D(double value) : a11(value), a12(0.0), a21(value), a22(0.0)
{
}

Mat2D::Mat2D(Vec2D* abscissa, Vec2D* ordinate) : a11(abscissa->x), a12(abscissa->y), a21(ordinate->x), a22(ordinate->y)
{
}

Mat2D::Mat2D(double a11, double a12, double a21, double a22) : a11(a11), a12(a12), a21(a21), a22(a22)
{
}

Mat2D Mat2D::operator+(Mat2D matrix)
{
	return Mat2D(a11 + matrix.a11, a12 + matrix.a12, a21 + matrix.a21, a22 + matrix.a22);
}

void Mat2D::operator+=(Mat2D matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a21 += matrix.a21;
	a22 += matrix.a22;
}

Mat2D Mat2D::operator-()
{
	return Mat2D(-a11, -a12, -a21, -a22);
}

Mat2D Mat2D::operator-(Mat2D matrix)
{
	return Mat2D(a11 - matrix.a11, a12 - matrix.a12, a21 - matrix.a21, a22 - matrix.a22);
}

void Mat2D::operator-=(Mat2D matrix)
{
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a21 -= matrix.a21;
	a22 -= matrix.a22;
}

Mat2D Mat2D::operator*(double value)
{
	return Mat2D(a11 * value, a12 * value, a21 * value, a22 * value);
}

Mat2D Mat2D::operator*(Mat2D matrix)
{
	return Mat2D(a11 * matrix.a11 + a12 * matrix.a21, a11 * matrix.a21 + a12 * matrix.a22, a21 * matrix.a11 + a22 * matrix.a21, a21 * matrix.a21 + a22 * matrix.a22);
}

void Mat2D::operator*=(double value)
{
	a11 *= value;
	a12 *= value;
	a21 *= value;
	a22 *= value;
}

void Mat2D::operator*=(Mat2D matrix)
{
	double
		temp_a12 = a11 * matrix.a21 + a12 * matrix.a22,
		temp_a22 = a21 * matrix.a21 + a22 * matrix.a22;

	a11 = a11 * matrix.a11 + a12 * matrix.a21;
	a12 = temp_a12;
	a21 = a21 * matrix.a21 + a22 * matrix.a22;
	a22 = temp_a22;
}

Mat2D Mat2D::operator/(double value)
{
	return Mat2D(a11 / value, a12 / value, a21 / value, a22 / value);
}

void Mat2D::operator/=(double value)
{
	a11 /= value;
	a12 /= value; 
	a21 /= value; 
	a22 /= value;
}

bool Mat2D::operator==(Mat2D matrix)
{
	return a11 == matrix.a11 && a12 == matrix.a12 && a21 == matrix.a21 && a22 == matrix.a22;
}

double Mat2D::Determinant()
{
	return a11 * a22 - a21 * a12;
}

Mat2D::operator Mat2F()
{
	return Mat2F((float)a11, (float)a12, (float)a21, (float)a22);
}

Mat2D Mat2D::Inverse()
{
	return Mat2D(a22, -a12, -a21, a11) / Determinant();
}

void Mat2D::InverseThis()
{
	double temp = a11;
	double det = Determinant();
	a11 = a22 / det;
	a22 = a11 / det;
	a12 = -a12 / det;
	a21 = -a12 / det;
}

Mat2D Mat2D::InverseNotNormalize()
{
	return Mat2D(a22, -a12, -a21, a11);
}

void Mat2D::InverseThisNotNormalize()
{
	double temp = a11;
	a11 = a22;
	a22 = a11;
	a12 = -a12;
	a21 = -a12;
}

void Mat2D::Set(double value)
{
	a11 = value;
	a12 = value;
	a21 = value;
	a22 = value;
}

void Mat2D::Set(double a11, double a12, double a21, double a22)
{
	this->a11 = a11;
	this->a12 = a12;
	this->a21 = a21;
	this->a22 = a22;
}

void Mat2D::Set(Vec2D* abscissa, Vec2D* ordinata)
{
	a11 = abscissa->x;
	a12 = abscissa->y;
	a21 = ordinata->x;
	a22 = ordinata->y;
}

void Mat2D::operator=(Mat2D matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a21 = matrix.a21;
	a22 = matrix.a22;
}

Mat2D::~Mat2D()
{
}



Mat2F::Mat2F() : a11(0.0), a12(0.0), a21(0.0), a22(0.0)
{
}

Mat2F::Mat2F(float value) : a11(value), a12(0.0), a21(value), a22(0.0)
{
}

Mat2F::Mat2F(Vec2F* abscissa, Vec2F* ordinate) : a11(abscissa->x), a12(abscissa->y), a21(ordinate->x), a22(ordinate->y)
{
}

Mat2F::Mat2F(float a11, float a12, float a21, float a22) : a11(a11), a12(a12), a21(a21), a22(a22)
{
}

Mat2F Mat2F::operator+(Mat2F add_mat)
{
	return Mat2F(a11 + add_mat.a11, a12 + add_mat.a12, a21 + add_mat.a21, a22 + add_mat.a22);
}

void Mat2F::operator+=(Mat2F matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a21 += matrix.a21;
	a22 += matrix.a22;
}

Mat2F Mat2F::operator-()
{
	return Mat2F(-a11, -a12, -a21, -a22);
}

Mat2F Mat2F::operator-(Mat2F sub_mat)
{
	return Mat2F(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

void Mat2F::operator-=(Mat2F matrix)
{
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a21 -= matrix.a21;
	a22 -= matrix.a22;
}

Mat2F Mat2F::operator*(float mult)
{
	return Mat2F(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Mat2F Mat2F::operator*(Mat2F mat)
{
	return Mat2F(a11 * mat.a11 + a12 * mat.a21, a11 * mat.a21 + a12 * mat.a22, a21 * mat.a11 + a22 * mat.a21, a21 * mat.a21 + a22 * mat.a22);
}

void Mat2F::operator*=(float value)
{
	a11 *= value;
	a12 *= value;
	a21 *= value;
	a22 *= value;
}

void Mat2F::operator*=(Mat2F matrix)
{
	double
		temp_a12 = a11 * matrix.a21 + a12 * matrix.a22,
		temp_a22 = a21 * matrix.a21 + a22 * matrix.a22;

	a11 = a11 * matrix.a11 + a12 * matrix.a21;
	a12 = temp_a12;
	a21 = a21 * matrix.a21 + a22 * matrix.a22;
	a22 = temp_a22;
}

Mat2F Mat2F::operator/(float div)
{
	return Mat2F(a11 / div, a12 / div, a21 / div, a22 / div);
}

void Mat2F::operator/=(float value)
{
	a11 /= value;
	a12 /= value;
	a21 /= value;
	a22 /= value;
}

bool Mat2F::operator==(Mat2F mat)
{
	return a11 == mat.a11 && a12 == mat.a12 && a21 == mat.a21 && a22 == mat.a22;
}

Mat2F::operator Mat2D()
{
	return Mat2D((double)a11, (double)a12, (double)a21, (double)a22);
}

float Mat2F::Determinant()
{
	return a11 * a22 - a21 * a12;
}

Mat2F Mat2F::Inverse()
{
	return Mat2F(a22, -a12, -a21, a11) / Determinant();
}

void Mat2F::InverseThis()
{
	float temp = a11;
	float det = Determinant();
	a11 = a22 / det;
	a22 = a11 / det;
	a12 = -a12 / det;
	a21 = -a12 / det;
}

Mat2F Mat2F::InverseNotNormalize()
{
	return Mat2F(a22, -a12, -a21, a11);
}

void Mat2F::InverseThisNotNormalize()
{
	float temp = a11;
	a11 = a22;
	a22 = a11;
	a12 = -a12;
	a21 = -a12;
}

void Mat2F::Set(float value)
{
	a11 = value;
	a12 = value;
	a21 = value;
	a22 = value;
}

void Mat2F::Set(float a11, float a12, float a21, float a22)
{
	this->a11 = a11;
	this->a12 = a12;
	this->a21 = a21;
	this->a22 = a22;
}

void Mat2F::Set(Vec2F* abscissa, Vec2F* ordinata)
{
	a11 = abscissa->x;
	a21 = abscissa->y;
	a12 = ordinata->x;
	a22 = ordinata->y;
}

void Mat2F::operator=(Mat2F matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a21 = matrix.a21;
	a22 = matrix.a22;
}

Mat2F::~Mat2F()
{
}