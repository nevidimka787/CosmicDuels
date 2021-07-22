#include "Mat.h"

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

Mat2D Mat2D::operator+(Mat2D add_mat)
{
	return Mat2D(a11 + add_mat.a11, a12 + add_mat.a12, a21 + add_mat.a21, a22 + add_mat.a22);
}

Mat2D Mat2D::operator+=(Mat2D add_mat)
{
	return Mat2D(a11 + add_mat.a11, a12 + add_mat.a12, a21 + add_mat.a21, a22 + add_mat.a22);
}

Mat2D Mat2D::operator-()
{
	return Mat2D(-a11, -a12, -a21, -a22);
}

Mat2D Mat2D::operator-(Mat2D sub_mat)
{
	return Mat2D(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

Mat2D Mat2D::operator-=(Mat2D sub_mat)
{
	return Mat2D(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

Mat2D Mat2D::operator*(double mult)
{
	return Mat2D(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Vec2D Mat2D::operator*(Vec2D vec)
{
	return Vec2D(a11 * vec.x + a12 * vec.y, a21 * vec.x + a22 * vec.y);
}

Mat2D Mat2D::operator*(Mat2D mat)
{
	return Mat2D(a11 * mat.a11 + a12 * mat.a21, a11 * mat.a21 + a12 * mat.a22, a21 * mat.a11 + a22 * mat.a21, a21 * mat.a21 + a22 * mat.a22);
}

Mat2D Mat2D::operator*=(double mult)
{
	return Mat2D(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Vec2D Mat2D::operator*=(Vec2D vec)
{
	return Vec2D(a11 * vec.x + a12 * vec.y, a21 * vec.x + a22 * vec.y);
}

Mat2D Mat2D::operator*=(Mat2D mat)
{
	return Mat2D(a11 * mat.a11 + a12 * mat.a21, a11 * mat.a21 + a12 * mat.a22, a21 * mat.a11 + a22 * mat.a21, a21 * mat.a21 + a22 * mat.a22);
}

Mat2D Mat2D::operator/(double div)
{
	return Mat2D(a11 / div, a12 / div, a21 / div, a22 / div);
}

Mat2D Mat2D::operator/=(double div)
{
	return Mat2D(a11 / div, a12 / div, a21 / div, a22 / div);
}

bool Mat2D::operator==(Mat2D mat)
{
	return a11 == mat.a11 && a12 == mat.a12 && a21 == mat.a21 && a22 == mat.a22;
}

double Mat2D::Determinant()
{
	return a11 * a22 - a21 * a12;
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

Mat2F Mat2F::operator+=(Mat2F add_mat)
{
	return Mat2F(a11 + add_mat.a11, a12 + add_mat.a12, a21 + add_mat.a21, a22 + add_mat.a22);
}

Mat2F Mat2F::operator-()
{
	return Mat2F(-a11, -a12, -a21, -a22);
}

Mat2F Mat2F::operator-(Mat2F sub_mat)
{
	return Mat2F(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

Mat2F Mat2F::operator-=(Mat2F sub_mat)
{
	return Mat2F(a11 - sub_mat.a11, a12 - sub_mat.a12, a21 - sub_mat.a21, a22 - sub_mat.a22);
}

Mat2F Mat2F::operator*(float mult)
{
	return Mat2F(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Vec2F Mat2F::operator*(Vec2F vec)
{
	return Vec2F(a11 * vec.x + a12 * vec.y, a21 * vec.x + a22 * vec.y);
}

Mat2F Mat2F::operator*(Mat2F mat)
{
	return Mat2F(a11 * mat.a11 + a12 * mat.a21, a11 * mat.a21 + a12 * mat.a22, a21 * mat.a11 + a22 * mat.a21, a21 * mat.a21 + a22 * mat.a22);
}

Mat2F Mat2F::operator*=(float mult)
{
	return Mat2F(a11 * mult, a12 * mult, a21 * mult, a22 * mult);
}

Vec2F Mat2F::operator*=(Vec2F vec)
{
	return Vec2F(a11 * vec.x + a12 * vec.y, a21 * vec.x + a22 * vec.y);
}

Mat2F Mat2F::operator*=(Mat2F mat)
{
	return Mat2F(a11 * mat.a11 + a12 * mat.a21, a11 * mat.a21 + a12 * mat.a22, a21 * mat.a11 + a22 * mat.a21, a21 * mat.a21 + a22 * mat.a22);
}

Mat2F Mat2F::operator/(float div)
{
	return Mat2F(a11 / div, a12 / div, a21 / div, a22 / div);
}

Mat2F Mat2F::operator/=(float div)
{
	return Mat2F(a11 / div, a12 / div, a21 / div, a22 / div);
}

bool Mat2F::operator==(Mat2F mat)
{
	return a11 == mat.a11 && a12 == mat.a12 && a21 == mat.a21 && a22 == mat.a22;
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