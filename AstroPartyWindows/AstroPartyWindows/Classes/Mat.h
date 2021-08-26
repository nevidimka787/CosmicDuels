#pragma once

#include <iostream>
#include <math.h>

#include "Vec.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Mat2F;
class Mat2D;

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
	Mat2D operator+=(Mat2D add_mat);
	Mat2D operator-();
	Mat2D operator-(Mat2D sub_mat);
	Mat2D operator-=(Mat2D sub_mat);
	Mat2D operator*(double mult);
	Vec2D operator*(Vec2D vec);
	Mat2D operator*(Mat2D mat);
	Mat2D operator*=(double mult);
	Vec2D operator*=(Vec2D vec);
	Mat2D operator*=(Mat2D mat);
	Mat2D operator/(double div);
	Mat2D operator/=(double div);
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
	Mat2F operator+=(Mat2F add_mat);
	Mat2F operator-();
	Mat2F operator-(Mat2F sub_mat);
	Mat2F operator-=(Mat2F sub_mat);
	Mat2F operator*(float mult);
	Vec2F operator*(Vec2F vec);
	Mat2F operator*(Mat2F mat);
	Mat2F operator*=(float mult);
	Vec2F operator*=(Vec2F vec);
	Mat2F operator*=(Mat2F mat);
	Mat2F operator/(float div);
	Mat2F operator/=(float div);
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