#include "Mat.h"

#ifdef _DEBUG
#include <assert.h>
#endif

#pragma warning(disable : 26451)

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

Vec2D::Vec2D(double v) :
	x(v),
	y(v)
{
}

Vec2D::Vec2D(double x, double y) :
	x(x),
	y(y)
{
}

Vec2D Vec2D::operator+(const Vec2D& vector) const
{
	return Vec2D(x + vector.x, y + vector.y);
}

Vec2D Vec2D::operator-(const Vec2D& vector) const
{
	return Vec2D(x - vector.x, y - vector.y);
}

Vec2D Vec2D::operator-() const
{
	return Vec2D(-x, -y);
}

Vec2D Vec2D::operator*(double value) const
{
	return Vec2D(x * value, y * value);
}

double Vec2D::operator*(const Vec2D& vector) const
{
	return x * vector.x + y * vector.y;
}

Vec2D Vec2D::operator*(const Mat2D& matrix) const
{
	return Vec2D(x * matrix.a11 + y * matrix.a12, x * matrix.a21 + y * matrix.a22);
}

Vec2D Vec2D::operator*(const Mat3x2D& matrix) const
{
	return Vec2D(
		x * matrix.a11 + y * matrix.a12 + matrix.a13,
		x * matrix.a21 + y * matrix.a22 + matrix.a23);
}

Vec2D Vec2D::operator/(double value) const
{
	return Vec2D(x / value, y / value);
}

void Vec2D::operator+=(const Vec2D& vector)
{
	x += vector.x;
	y += vector.y;
}

void Vec2D::operator-=(const Vec2D& vector)
{
	x -= vector.x;
	y -= vector.y;
}

void Vec2D::operator*=(double value)
{
	x *= value;
	y *= value;
}

void Vec2D::operator*=(const Mat2D& matrix)
{
	double temp = x * matrix.a11 + y * matrix.a12;
	y = x * matrix.a21 + y * matrix.a22;
	x = temp;
}

void Vec2D::operator*=(const Mat3x2D& matrix)
{
	double temp = x * matrix.a11 + y * matrix.a12 + matrix.a13;
	y = x * matrix.a21 + y * matrix.a22 + matrix.a23;
	x = temp;
}

void Vec2D::operator/=(double value)
{
	x /= value;
	y /= value;
}

bool Vec2D::operator||(const Vec2D& vector) const
{
	return abs(Mat2D(*this, vector).Determinant()) < 1e-7;

	return x == 0.0 && vector.x == 0.0 ||
		y == 0.0 && vector.y == 0.0 ||
		x / y == vector.x / vector.y ||
		y / x == vector.y / vector.x;
}

bool Vec2D::operator==(double value) const
{
	return (x == value && y == value);
}

bool Vec2D::operator==(const Vec2D& vector) const
{
	return (x == vector.x && y == vector.y);
}

bool Vec2D::operator!=(double value) const
{
	return (x != value || y != value);
}

bool Vec2D::operator!=(const Vec2D& vector) const
{
	return (x != vector.x || y != vector.y);
}

bool Vec2D::operator>(double value) const
{
	return (x > value && y > value);
}

bool Vec2D::operator>(const Vec2D& vector) const
{
	return (x > vector.x && y > vector.y);
}

bool Vec2D::operator>=(double value) const
{
	return (x >= value && y >= value);
}

bool Vec2D::operator>=(const Vec2D& vector) const
{
	return (x >= vector.x && y >= vector.y);
}

bool Vec2D::operator<(double value) const
{
	return (x < value && y < value);
}

bool Vec2D::operator<(const Vec2D& vector) const
{
	return (x < vector.x && y < vector.y);
}

bool Vec2D::operator<=(double value) const
{
	return (x <= value && y <= value);
}

bool Vec2D::operator<=(const Vec2D& vector) const
{
	return (x <= vector.x && y <= vector.y);
}

Vec2D::operator Vec2F()
{
	return Vec2F((float)x, (float)y);
}

double Vec2D::Distance(const Vec2D& vector) const
{
	return (*this - vector).Length();
}

double Vec2D::GetAngle() const
{
	return (atan2(-y, x));
}

double Vec2D::GetAngle(const Vec2D& to_vector) const
{
	const auto& new_vector = to_vector.Rotate(-GetAngle());
	return new_vector.GetAngle();
}

double Vec2D::Length() const
{
	return sqrt(x * x + y * y);
}

Vec2D Vec2D::Normalize() const
{
	return *this / Length();
}

void Vec2D::NormalizeThis()
{
	*this /= Length();
}

Vec2D Vec2D::Project(const Vec2D& projecting_vector) const
{
	const double& l = Length();
#ifdef _DEBUG
	assert(l > 1e-7);
#endif

	const double& dot = operator*(projecting_vector);
	if (dot > 0.0)
	{
		return operator*(sqrt(dot / l));
	}
	if (dot < 0.0)
	{
		return operator*(-sqrt(-dot / l));
	}
	return Vec2D();
}

Vec2D Vec2D::Perpendicular() const
{
	return Vec2D(y, -x);
}

void Vec2D::PerpendicularThis()
{
	double temp = -x;
	x = y;
	y = temp;
}

Vec2D Vec2D::Rotate(double angle) const
{
	const double& sin_a = sin(angle);
	const double& cos_a = cos(angle);
	return Vec2D(x * cos_a + y * sin_a, y * cos_a - x * sin_a);
}

void Vec2D::RotateThis(double angle)
{
	const double& sin_a = sin(angle);
	const double& cos_a = cos(angle);
	const double& new_x = x * cos_a + y * sin_a;
	y = y * cos_a - x * sin_a;
	x = new_x;
}

Vec2D Vec2D::Scale(const Vec2D& scale) const
{
	return Vec2D(x * scale.x, y * scale.y);
}

void Vec2D::ScaleThis(const Vec2D& scale)
{
	x *= scale.x;
	y *= scale.y;
}

void Vec2D::Set(const Vec2D* vector)
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

void Vec2D::operator=(const Vec2D& vector)
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

Vec2F::Vec2F(float v) :
	x(v),
	y(v)
{
}

Vec2F::Vec2F(float x, float y) :
	x(x),
	y(y)
{
}

Vec2F Vec2F::operator+(const Vec2F& vector) const
{
	return Vec2F(x + vector.x, y + vector.y);
}

Vec2F Vec2F::operator+(float value) const
{
	return Vec2F(x + value, y + value);
}

Vec2F Vec2F::operator-(const Vec2F& vector) const
{
	return Vec2F(x - vector.x, y - vector.y);
}

Vec2F Vec2F::operator-(float value) const
{
	return Vec2F(x - value, y - value);
}

Vec2F Vec2F::operator-() const
{
	return Vec2F(-x, -y);
}

Vec2F Vec2F::operator*(float value) const
{
	return Vec2F(x * value, y * value);
}

float Vec2F::operator*(const Vec2F& vector) const
{
	return x * vector.x + y * vector.y;
}

Vec2F Vec2F::operator*(const Mat2F& matrix) const
{
	return Vec2F(x * matrix.a11 + y * matrix.a12, x * matrix.a21 + y * matrix.a22);
}

Vec2F Vec2F::operator*(const Mat3x2F& matrix) const
{
	return Vec2F(
		x * matrix.a11 + y * matrix.a12 + matrix.a13,
		x * matrix.a21 + y * matrix.a22 + matrix.a23);
}

Vec2F Vec2F::operator/(float value) const
{

	return Vec2F(x / value, y / value);
}

void Vec2F::operator+=(const Vec2F& vector)
{
	x += vector.x;
	y += vector.y;
}

void Vec2F::operator-=(const Vec2F& vector)
{
	x -= vector.x;
	y -= vector.y;
}

void Vec2F::operator*=(float value)
{
	x *= value;
	y *= value;
}

void Vec2F::operator*=(const Mat2F& matrix)
{
	const float& new_x = x * matrix.a11 + y * matrix.a12;
	y = x * matrix.a21 + y * matrix.a22;
	x = new_x;
}

void Vec2F::operator*=(const Mat3x2F& matrix)
{
	const float& new_x = x * matrix.a11 + y * matrix.a12 + matrix.a13;
	y = x * matrix.a21 + y * matrix.a22 + matrix.a23;
	x = new_x;
}

void Vec2F::operator/=(float value)
{
	x /= value;
	y /= value;
}

bool Vec2F::operator||(const Vec2F& vector) const
{
	return fabs(Mat2F(*this, vector).Determinant()) < 1e-7f;

	return 
		x == 0.0 && vector.x == 0.0 ||
		y == 0.0 && vector.y == 0.0 ||
		x / y == vector.x / vector.y ||
		y / x == vector.y / vector.x;
}

bool Vec2F::operator==(float value) const
{
	return x == value && y == value;
}

bool Vec2F::operator==(const Vec2F& vector) const
{
	return x == vector.x && y == vector.y;
}

bool Vec2F::operator!=(float value) const
{
	return (x != value || y != value);
}

bool Vec2F::operator!=(const Vec2F& vector) const
{
	return (x != vector.x || y != vector.y);
}

bool Vec2F::operator>(float value) const
{
	return (x > value && y > value);
}

bool Vec2F::operator>(const Vec2F& vector) const
{
	return (x > vector.x && y > vector.y);
}

bool Vec2F::operator>=(float value) const
{
	return (x >= value && y >= value);
}

bool Vec2F::operator>=(const Vec2F& vector) const
{
	return (x >= vector.x && y >= vector.y);
}

bool Vec2F::operator<(float value) const
{
	return (x < value && y < value);
}

bool Vec2F::operator<(const Vec2F& vector) const
{
	return (x < vector.x && y < vector.y);
}

bool Vec2F::operator<=(float value) const
{
	return (x <= value && y <= value);
}

bool Vec2F::operator<=(const Vec2F& vector) const
{
	return (x <= vector.x && y <= vector.y);
}

Vec2F::operator Vec2D()
{
	return Vec2D(x, y);
}

float Vec2F::Distance(const Vec2F& target_vector) const
{
	return (*this - target_vector).Length();
}

float Vec2F::DistancePow2(const Vec2F& target_vector) const
{
	return (*this - target_vector).LengthPow2();
}

float Vec2F::GetAngle() const
{
	return (atan2f(-y, x));
}

float Vec2F::GetAngle(const Vec2F& to_vector) const
{
	return to_vector.Rotate(-GetAngle()).GetAngle();
}

float Vec2F::GetAngleClockwise() const
{
	return (atan2f(y, x));
}

float Vec2F::Length() const
{
	return sqrtf(x * x + y * y);
}

Vec2F Vec2F::Length(float length) const
{
	return *this / Length() * length;
}

void Vec2F::LengthThis(float length)
{
	*this /= Length() * length;
}

float Vec2F::LengthPow2() const
{
	return x * x + y * y;
}

Vec2F Vec2F::LengthPow2(float length) const
{
	return *this / sqrtf(LengthPow2() * length);
}

void Vec2F::LengthPow2This(float length)
{
	*this /= sqrtf(LengthPow2() * length);
}

Vec2F Vec2F::Normalize() const
{
	return *this / Length();
}

Vec2F Vec2F::NormalizePow2() const
{
	return *this / LengthPow2();
}

void Vec2F::NormalizeThis()
{
	*this /= Length();
}

void Vec2F::NormalizeThisPow2()
{
	*this /= LengthPow2();
}

Vec2F Vec2F::Project(const Vec2F& projecting_vector) const
{
	Vec2F direction = Normalize();
	return direction * projecting_vector.Length() * (direction * projecting_vector.Normalize());
}

Vec2F Vec2F::ProjectSign(const Vec2F& projecting_vector) const
{
	if (*this * projecting_vector <= 0.0f)
	{
		return Vec2F();
	}
	Vec2F direction = Normalize();
	return direction * projecting_vector.Length() * (direction * projecting_vector.Normalize());
}

Vec2F Vec2F::Perpendicular() const
{
	return Vec2F(-y, x);
}

void Vec2F::PerpendicularThis()
{
	const float temp = x;
	x = -y;
	y = temp;
}

Vec2F Vec2F::PerpendicularClockwise() const
{
	return Vec2F(y, -x);
}

void Vec2F::PerpendicularClockwiseThis()
{
	const float temp = -x;
	x = y;
	y = temp;
}

Vec2F Vec2F::RotateClockwise(float angle) const
{
	const float& temp_cos = cosf(angle);
	const float& temp_sin = sinf(angle);
	return Vec2F(x * temp_cos + y * temp_sin, y * temp_cos - x * temp_sin);
}

void Vec2F::RotateClockwiseThis(float angle)
{
	const float& temp_cos = cosf(angle);
	const float& temp_sin = sinf(angle);

	const float& temp = x * temp_cos + y * temp_sin;
	y = x * temp_sin - y * temp_cos;
	x = temp;
}

Vec2F Vec2F::Rotate(float angle) const
{
	const float& temp_cos = cosf(angle);
	const float& temp_sin = sinf(angle);
	return Vec2F(x * temp_cos + y * temp_sin, x * temp_sin - y * temp_cos);
}

void Vec2F::RotateThis(float angle)
{
	const float& temp_cos = cosf(angle);
	const float& temp_sin = sinf(angle);

	const float& temp = x * temp_cos + y * temp_sin;
	y = y * temp_cos - x * temp_sin;
	x = temp;
}

Vec2F Vec2F::Scale(const Vec2F& scale) const
{
	return Vec2F(x * scale.x, y * scale.y);
}

void Vec2F::ScaleThis(const Vec2F& scale)
{
	x *= scale.x;
	y *= scale.y;
}

void Vec2F::Set(const Vec2F* vector)
{
	x = vector->x;
	y = vector->y;
}

void Vec2F::Set(float v)
{
	this->x = v;
	this->y = v;
}

void Vec2F::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vec2F::Set(double v)
{
	this->x = (float)v;
	this->y = (float)v;
}

void Vec2F::Set(double x, double y)
{
	this->x = (float)x;
	this->y = (float)y;
}

float Vec2F::VecMul(const Vec2F& vector) const
{
	return x * vector.y - y * vector.x;
}

void Vec2F::operator=(const Vec2F& vector)
{
	x = vector.x;
	y = vector.y;
}

Vec2F::~Vec2F()
{
}

Mat2D::Mat2D() : a11(0.0), a12(0.0), a21(0.0), a22(0.0)
{
}

Mat2D::Mat2D(double value) : a11(value), a12(0.0), a21(value), a22(0.0)
{
}

Mat2D::Mat2D(const Vec2D& abscissa, const Vec2D& ordinate) :
	a11(abscissa.x),
	a21(abscissa.y),
	a12(ordinate.x),
	a22(ordinate.y)
{
}

Mat2D::Mat2D(double a11, double a12, double a21, double a22) :
	a11(a11), 
	a12(a12), 
	a21(a21), 
	a22(a22)
{
}

Mat2D Mat2D::operator+(const Mat2D& matrix) const
{
	return Mat2D(a11 + matrix.a11, a12 + matrix.a12, a21 + matrix.a21, a22 + matrix.a22);
}

void Mat2D::operator+=(const Mat2D& matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a21 += matrix.a21;
	a22 += matrix.a22;
}

Mat2D Mat2D::operator-() const
{
	return Mat2D(-a11, -a12, -a21, -a22);
}

Mat2D Mat2D::operator-(const Mat2D& matrix) const
{
	return Mat2D(a11 - matrix.a11, a12 - matrix.a12, a21 - matrix.a21, a22 - matrix.a22);
}

void Mat2D::operator-=(const Mat2D& matrix)
{
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a21 -= matrix.a21;
	a22 -= matrix.a22;
}

Mat2D Mat2D::operator*(double value) const
{
	return Mat2D(a11 * value, a12 * value, a21 * value, a22 * value);
}

Mat2D Mat2D::operator*(const Mat2D& matrix) const
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

void Mat2D::operator*=(const Mat2D& matrix)
{
	double
		temp_a12 = a11 * matrix.a21 + a12 * matrix.a22,
		temp_a22 = a21 * matrix.a21 + a22 * matrix.a22;

	a11 = a11 * matrix.a11 + a12 * matrix.a21;
	a12 = temp_a12;
	a21 = a21 * matrix.a21 + a22 * matrix.a22;
	a22 = temp_a22;
}

Mat2D Mat2D::operator/(double value) const
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

bool Mat2D::operator==(const Mat2D& matrix) const
{
	return a11 == matrix.a11 && a12 == matrix.a12 && a21 == matrix.a21 && a22 == matrix.a22;
}

double Mat2D::Determinant() const
{
	return a11 * a22 - a21 * a12;
}

Mat2D::operator Mat2F()
{
	return Mat2F((float)a11, (float)a12, (float)a21, (float)a22);
}

Mat2D Mat2D::Inverse() const
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

Mat2D Mat2D::InverseNotNormalize() const
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

void Mat2D::Set(const Vec2D& abscissa, const Vec2D& ordinata)
{
	a11 = abscissa.x;
	a12 = abscissa.y;
	a21 = ordinata.x;
	a22 = ordinata.y;
}

void Mat2D::Set(const Mat2D* matrix)
{
	a11 = matrix->a11;
	a12 = matrix->a12;
	a21 = matrix->a21;
	a22 = matrix->a22;
}

void Mat2D::operator=(const Mat2D& matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a21 = matrix.a21;
	a22 = matrix.a22;
}

Mat2D::~Mat2D()
{
}



Mat2F::Mat2F() :
	a11(0.0),
	a12(0.0),
	a21(0.0),
	a22(0.0)
{
}

Mat2F::Mat2F(float value) :
	a11(value),
	a12(0.0),
	a21(value),
	a22(0.0)
{
}

Mat2F::Mat2F(const Vec2F& abscissa, const Vec2F& ordinate) :
	a11(abscissa.x),
	a21(abscissa.y),
	a12(ordinate.x),
	a22(ordinate.y)
{
}

Mat2F::Mat2F(float a11, float a12, float a21, float a22) :
	a11(a11),
	a12(a12),
	a21(a21),
	a22(a22)
{
}

Mat2F Mat2F::operator+(const Mat2F& add_mat) const
{
	return Mat2F(a11 + add_mat.a11, a12 + add_mat.a12, a21 + add_mat.a21, a22 + add_mat.a22);
}

void Mat2F::operator+=(const Mat2F& matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a21 += matrix.a21;
	a22 += matrix.a22;
}

Mat2F Mat2F::operator-() const
{
	return Mat2F(-a11, -a12, -a21, -a22);
}

Mat2F Mat2F::operator-(const Mat2F& sub_mat) const
{
	return Mat2F(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

void Mat2F::operator-=(const Mat2F& matrix)
{
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a21 -= matrix.a21;
	a22 -= matrix.a22;
}

Mat2F Mat2F::operator*(float mult) const
{
	return Mat2F(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Mat2F Mat2F::operator*(const Mat2F& mat) const
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

void Mat2F::operator*=(const Mat2F& matrix)
{
	float
		temp_a12 = a11 * matrix.a21 + a12 * matrix.a22,
		temp_a22 = a21 * matrix.a21 + a22 * matrix.a22;

	a11 = a11 * matrix.a11 + a12 * matrix.a21;
	a12 = temp_a12;
	a21 = a21 * matrix.a21 + a22 * matrix.a22;
	a22 = temp_a22;
}

Mat2F Mat2F::operator/(float div) const
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

bool Mat2F::operator==(const Mat2F& mat) const
{
	return a11 == mat.a11 && a12 == mat.a12 && a21 == mat.a21 && a22 == mat.a22;
}

Mat2F::operator Mat2D()
{
	return Mat2D((double)a11, (double)a12, (double)a21, (double)a22);
}

float Mat2F::Determinant() const
{
	return a11 * a22 - a21 * a12;
}

Mat2F Mat2F::Inverse() const
{
	const float& det = Determinant();
#ifdef _DEBUG
	assert(fabsf(det) > 1e-7);
#endif
	return Mat2F(a22, -a12, -a21, a11) / det;
}

void Mat2F::InverseThis()
{
	const float& det = Determinant();
#ifdef _DEBUG
	assert(fabsf(det) > 1e-7f);
#endif

	const float temp = a11;
	a11 = a22 / det;
	a22 = temp / det;
	a12 = -a12 / det;
	a21 = -a21 / det;
}

Mat2F Mat2F::InverseNotNormalize() const
{
	return Mat2F(a22, -a12, -a21, a11);
}

void Mat2F::InverseThisNotNormalize()
{
	float temp = a11;
	a11 = a22;
	a22 = temp;
	a12 = -a12;
	a21 = -a21;
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

void Mat2F::Set(const Vec2F& abscissa, const Vec2F& ordinata)
{
	a11 = abscissa.x;
	a21 = abscissa.y;
	a12 = ordinata.x;
	a22 = ordinata.y;
}

void Mat2F::Set(const Mat2F* matrix)
{
	a11 = matrix->a11;
	a12 = matrix->a12;
	a21 = matrix->a21;
	a22 = matrix->a22;
}

void Mat2F::operator=(const Mat2F& matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a21 = matrix.a21;
	a22 = matrix.a22;
}

Mat2F::~Mat2F()
{
}

Mat3x2D::Mat3x2D() :
	a11(0.0), a12(0.0), a13(0.0),
	a21(0.0), a22(0.0), a23(0.0)
{
}

Mat3x2D::Mat3x2D(double value) :
	a11(value), a12(value), a13(value),
	a21(value), a22(value), a23(value)
{
}

Mat3x2D::Mat3x2D(
	double a11, double a12, double a13,
	double a21, double a22, double a23)
	: 
	a11(a11), a12(a12), a13(a13),
	a21(a21), a22(a22), a23(a23)
{
}

Mat3x2D Mat3x2D::operator+(const Mat3x2D& matrix) const
{
	return Mat3x2D(
		a11 + matrix.a11, a12 + matrix.a12, a12 + matrix.a13,
		a21 + matrix.a21, a22 + matrix.a22, a23 + matrix.a23);
}

void Mat3x2D::operator+=(const Mat3x2D& matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a13 += matrix.a13;

	a21 += matrix.a21;
	a22 += matrix.a22;
	a23 += matrix.a23;
}

Mat3x2D Mat3x2D::operator-() const
{
	return Mat3x2D(
		-a11, -a12, -a13, 
		-a21, -a22, -a23);
}

Mat3x2D Mat3x2D::operator-(const Mat3x2D& matrix) const
{
	return Mat3x2D(
		a11 - matrix.a11, a12 - matrix.a12, a12 - matrix.a13,
		a21 - matrix.a21, a22 - matrix.a22, a23 - matrix.a23);
}

void Mat3x2D::operator-=(const Mat3x2D& matrix)
{

	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a13 -= matrix.a13;

	a21 -= matrix.a21;
	a22 -= matrix.a22;
	a23 -= matrix.a23;
}

Mat3x2D Mat3x2D::operator*(double value) const
{
	return Mat3x2D(
		a11 * value, a12 * value, a12 * value,
		a21 * value, a22 * value, a23 * value);
}

Mat3x2D Mat3x2D::operator*(const Mat3x2D& matrix) const
{
	return Mat3x2D(
		a11 * matrix.a11 + a12 * matrix.a21,
		a11 * matrix.a12 + a12 * matrix.a22,
		a11 * matrix.a13 + a12 * matrix.a23 + a13,

		a21 * matrix.a11 + a22 * matrix.a21,
		a21 * matrix.a12 + a22 * matrix.a22,
		a21 * matrix.a13 + a22 * matrix.a23 + a23);
}

void Mat3x2D::operator*=(double value)
{
	a11 *= value;
	a12 *= value;
	a13 *= value;

	a21 *= value;
	a22 *= value;
	a23 *= value;
}

void Mat3x2D::operator*=(const Mat3x2D& matrix)
{
	const double&
		temp1 = a11 * matrix.a11 + a12 * matrix.a21,
		temp2 = a11 * matrix.a12 + a12 * matrix.a22;
	a13 = a11 * matrix.a13 + a12 * matrix.a23 + a13;
	a11 = temp1;
	a12 = temp2;

	const double&
		temp3 = a21 * matrix.a11 + a22 * matrix.a21,
		temp4 = a21 * matrix.a12 + a22 * matrix.a22;
	a23 = a21 * matrix.a13 + a22 * matrix.a23 + a23;
	a21 = temp3;
	a22 = temp4;
}

Mat3x2D Mat3x2D::operator/(double value) const
{
	return Mat3x2D(
		a11 / value, a12 / value, a13 / value,
		a21 / value, a22 / value, a23 / value);
}

void Mat3x2D::operator/=(double value)
{
	a11 /= value;
	a12 /= value;
	a13 /= value;

	a21 /= value;
	a22 /= value;
	a23 /= value;
}

bool Mat3x2D::operator==(const Mat3x2D& matrix) const
{
	return 
		a11 == matrix.a11 && a12 == matrix.a12 && a13 == matrix.a13 &&
		a21 == matrix.a21 && a22 == matrix.a22 && a23 == matrix.a23;
}

double Mat3x2D::Determinant() const
{
	return a11 * a22 - a21 * a12;
}

Mat3x2D Mat3x2D::Rotate(double angle) const
{
	return *this * Mat3x2D(
		cos(angle), -sin(angle), 0.0f,
		sin(angle), cos(angle), 0.0f);
}

void Mat3x2D::RotateThis(double angle)
{
	*this *= Mat3x2D(
		cos(angle), -sin(angle), 0.0f,
		sin(angle), cos(angle), 0.0f);
}

Mat3x2D Mat3x2D::Scale(const Vec2D& vector) const
{
	return *this * Mat3x2D(
		vector.x, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f);
}

void Mat3x2D::ScaleThis(const Vec2D& vector)
{
	*this *= Mat3x2D(
		vector.x, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f);
}

Mat3x2D Mat3x2D::Transport(const Vec2D& vector) const
{
	return *this * Mat3x2D(
		1.0f, 0.0f, vector.x,
		0.0f, 1.0f, vector.y);
}

void Mat3x2D::TransportThis(const Vec2D& vector)
{
	*this *= Mat3x2D(
		1.0f, 0.0f, vector.x,
		0.0f, 1.0f, vector.y);
}

Mat3x2D::operator Mat3x2F() const
{
	return Mat3x2F(
		(float)a11, (float)a12, (float)a13,
		(float)a21, (float)a22, (float)a23);
}

Mat3x2D::operator Mat2D() const
{
	return Mat2D(
		a11, a12,
		a21, a22);
}

Mat3x2D::operator Mat2F() const
{
	return Mat2D(
		(float)a11, (float)a12,
		(float)a21, (float)a22);
}

void Mat3x2D::Set(double value)
{
	a11 = value;
	a12 = value;
	a13 = value;

	a21 = value;
	a22 = value;
	a23 = value;
}

void Mat3x2D::Set(
	double a11, double a12, double a13,
	double a21, double a22, double a23)
{
	this->a11 = a11;
	this->a12 = a12;
	this->a13 = a13;

	this->a21 = a21;
	this->a22 = a22;
	this->a23 = a23;
}

void Mat3x2D::Set(const Mat3x2D* matrix)
{
	a11 = matrix->a11;
	a12 = matrix->a12;
	a13 = matrix->a13;
	a21 = matrix->a21;
	a22 = matrix->a22;
	a23 = matrix->a23;
}

void Mat3x2D::SetByAngle(double angle)
{
	const double cos_a = cos(angle);
	const double sin_a = sin(angle);

	a11 = cos_a;
	a12 = -sin_a;
	a13 = 0.0f;
	a21 = sin_a;
	a22 = cos_a;
	a23 = 0.0f;
}

void Mat3x2D::SetByDirection(const Vec2D& direction)
{
	a11 = direction.x;
	a12 = direction.y;
	a13 = 0.0f;
	a21 = -direction.y;
	a22 = direction.x;
	a23 = 0.0f;
}

void Mat3x2D::SetByPosition(const Vec2D& position)
{
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = position.x;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = position.y;
}

void Mat3x2D::SetByScale(const Vec2D& scale)
{
	a11 = scale.x;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = scale.y;
	a23 = 0.0f;
}

void Mat3x2D::operator=(const Mat3x2D& matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a13 = matrix.a13;

	a21 = matrix.a21;
	a22 = matrix.a22;
	a23 = matrix.a23;
}

Mat3x2D::~Mat3x2D()
{
}



Mat3x2F::Mat3x2F() :
	a11(0.0), a12(0.0), a13(0.0),
	a21(0.0), a22(0.0), a23(0.0)
{
}

Mat3x2F::Mat3x2F(float value) :
	a11(value), a12(value), a13(value),
	a21(value), a22(value), a23(value)
{
}

Mat3x2F::Mat3x2F(
	float a11, float a12, float a13,
	float a21, float a22, float a23)
	:
	a11(a11), a12(a12), a13(a13),
	a21(a21), a22(a22), a23(a23)
{
}

Mat3x2F Mat3x2F::operator+(const Mat3x2F& matrix) const
{
	return Mat3x2F(
		a11 + matrix.a11, a12 + matrix.a12, a12 + matrix.a13,
		a21 + matrix.a21, a22 + matrix.a22, a23 + matrix.a23);
}

void Mat3x2F::operator+=(const Mat3x2F& matrix)
{
	a11 += matrix.a11;
	a12 += matrix.a12;
	a13 += matrix.a13;

	a21 += matrix.a21;
	a22 += matrix.a22;
	a23 += matrix.a23;
}

Mat3x2F Mat3x2F::operator-() const
{
	return Mat3x2F(
		-a11, -a12, -a13,
		-a21, -a22, -a23);
}

Mat3x2F Mat3x2F::operator-(const Mat3x2F& matrix) const
{
	return Mat3x2F(
		a11 - matrix.a11, a12 - matrix.a12, a12 - matrix.a13,
		a21 - matrix.a21, a22 - matrix.a22, a23 - matrix.a23);
}

void Mat3x2F::operator-=(const Mat3x2F& matrix)
{
	a11 -= matrix.a11;
	a12 -= matrix.a12;
	a13 -= matrix.a13;

	a21 -= matrix.a21;
	a22 -= matrix.a22;
	a23 -= matrix.a23;
}

Mat3x2F Mat3x2F::operator*(float value) const
{
	return Mat3x2F(
		a11 * value, a12 * value, a12 * value,
		a21 * value, a22 * value, a23 * value);
}

Mat3x2F Mat3x2F::operator*(const Mat3x2F& matrix) const
{
	return Mat3x2F(
		a11 * matrix.a11 + a12 * matrix.a21,
		a11 * matrix.a12 + a12 * matrix.a22,
		a11 * matrix.a13 + a12 * matrix.a23 + a13,

		a21 * matrix.a11 + a22 * matrix.a21,
		a21 * matrix.a12 + a22 * matrix.a22,
		a21 * matrix.a13 + a22 * matrix.a23 + a23);
}

void Mat3x2F::operator*=(float value)
{
	a11 *= value;
	a12 *= value;
	a13 *= value;

	a21 *= value;
	a22 *= value;
	a23 *= value;
}

void Mat3x2F::operator*=(const Mat3x2F& matrix)
{
	float temp1 = a11 * matrix.a11 + a12 * matrix.a21;
	float temp2 = a11 * matrix.a12 + a12 * matrix.a22;
	
	a13 = a11 * matrix.a13 + a12 * matrix.a23 + a13;
	a11 = temp1;
	a12 = temp2;

	temp1 = a21 * matrix.a11 + a22 * matrix.a21;
	temp2 = a21 * matrix.a12 + a22 * matrix.a22;
	a23 = a21 * matrix.a13 + a22 * matrix.a23 + a23;
	a21 = temp1;
	a22 = temp2;
}

Mat3x2F Mat3x2F::operator/(float value) const
{
	return Mat3x2F(
		a11 / value, a12 / value, a13 / value,
		a21 / value, a22 / value, a23 / value);
}

void Mat3x2F::operator/=(float value)
{
	a11 /= value;
	a12 /= value;
	a13 /= value;

	a21 /= value;
	a22 /= value;
	a23 /= value;
}

bool Mat3x2F::operator==(const Mat3x2F& matrix) const
{
	return
		a11 == matrix.a11 && a12 == matrix.a12 && a13 == matrix.a13 &&
		a21 == matrix.a21 && a22 == matrix.a22 && a23 == matrix.a23;
}

float Mat3x2F::Determinant() const
{
	return a11 * a22 - a21 * a12;
}

Mat3x2F Mat3x2F::Rotate(float angle) const
{
	const float sin_a = sinf(angle);
	const float cos_a = cosf(angle);
	return operator*(Mat3x2F(
		cos_a, -sin_a, 0.0f,
		sin_a, cos_a, 0.0f));
}

void Mat3x2F::RotateThis(float angle)
{
	const float sin_a = sinf(angle);
	const float cos_a = cosf(angle);

	operator*=(Mat3x2F(
		cos_a, -sin_a, 0.0f,
		sin_a, cos_a, 0.0f));
}

Mat3x2F Mat3x2F::Scale(const Vec2F& vector) const
{
	return *this * Mat3x2F(
		vector.x, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f);
}

void Mat3x2F::ScaleThis(const Vec2F& vector)
{
	*this *= Mat3x2F(
		vector.x, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f);
}

Mat3x2F Mat3x2F::Transport(const Vec2F& vector) const
{
	return *this * Mat3x2F(
		1.0f, 0.0f, vector.x,
		0.0f, 1.0f, vector.y);
}

void Mat3x2F::TransportThis(const Vec2F& vector)
{
	*this *= Mat3x2F(
		1.0f, 0.0f, vector.x,
		0.0f, 1.0f, vector.y);
}

Mat3x2F::operator Mat3x2D() const
{
	return Mat3x2D(
		(double)a11, (double)a12, (double)a13,
		(double)a21, (double)a22, (double)a23);
}

Mat3x2F::operator Mat2D() const
{
	return Mat2D(
		(double)a11, (double)a12,
		(double)a21, (double)a22);
}

Mat3x2F::operator Mat2F() const
{
	return Mat2D(
		a11, a12,
		a21, a22);
}

void Mat3x2F::Set(float value)
{
	a11 = value;
	a12 = value;
	a13 = value;

	a21 = value;
	a22 = value;
	a23 = value;
}

void Mat3x2F::Set(
	float a11, float a12, float a13,
	float a21, float a22, float a23)
{
	this->a11 = a11;
	this->a12 = a12;
	this->a13 = a13;

	this->a21 = a21;
	this->a22 = a22;
	this->a23 = a23;
}

void Mat3x2F::Set(const Mat3x2F* matrix)
{
	a11 = matrix->a11;
	a12 = matrix->a12;
	a13 = matrix->a13;
	a21 = matrix->a21;
	a22 = matrix->a22;
	a23 = matrix->a23;
}

void Mat3x2F::SetByAngle(float angle)
{
	a11 = cosf(angle);
	a12 = -sinf(angle);
	a13 = 0.0f;
	a21 = sinf(angle);
	a22 = cosf(angle);
	a23 = 0.0f;
}

void Mat3x2F::SetByDirection(const Vec2F& direction)
{
	a11 = direction.x;
	a12 = direction.y;
	a13 = 0.0f;
	a21 = -direction.y;
	a22 = direction.x;
	a23 = 0.0f;
}

void Mat3x2F::SetByPosition(const Vec2F& position)
{
	a11 = 1.0f;
	a12 = 0.0f;
	a13 = position.x;
	a21 = 0.0f;
	a22 = 1.0f;
	a23 = position.y;
}

void Mat3x2F::SetByScale(const Vec2F& scale)
{
	a11 = scale.x;
	a12 = 0.0f;
	a13 = 0.0f;
	a21 = 0.0f;
	a22 = scale.y;
	a23 = 0.0f;
}

void Mat3x2F::operator=(const Mat3x2F& matrix)
{
	a11 = matrix.a11;
	a12 = matrix.a12;
	a13 = matrix.a13;

	a21 = matrix.a21;
	a22 = matrix.a22;
	a23 = matrix.a23;
}

Mat3x2F::~Mat3x2F()
{
}



std::ostream& operator<<(std::ostream& stream, const Vec2D& vector)
{
	stream << "X: " << vector.x << " Y: " << vector.y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Vec2F& vector)
{
	stream << "X: " << vector.x << " Y: " << vector.y;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Mat2D& matrix)
{
	return stream << "a11: " << matrix.a11 << " a12: " << matrix.a12 << " a21: " << matrix.a21 << " a22: " << matrix.a22;
}

std::ostream& operator<<(std::ostream& stream, const Mat2F& matrix)
{
	return stream << "a11: " << matrix.a11 << " a12: " << matrix.a12 << " a21: " << matrix.a21 << " a22: " << matrix.a22;
}

std::ostream& operator<<(std::ostream& stream, const Mat3x2D& matrix)
{
	return stream << "a11: " << matrix.a11 << " a12: " << matrix.a12 << " a13: " << matrix.a13 << std::endl
		<< "a21: " << matrix.a21 << " a22: " << matrix.a22 << " a23: " << matrix.a23 << std::endl;
}

std::ostream& operator<<(std::ostream& stream, const Mat3x2F& matrix)
{
	return stream << "a11: " << matrix.a11 << " a12: " << matrix.a12 << " a13: " << matrix.a13 << std::endl
		<< "a21: " << matrix.a21 << " a22: " << matrix.a22 << " a23: " << matrix.a23 << std::endl;
}
