#ifndef MAT_H
#define MAT_H
#include <math.h>
#include "Vec.h"

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

	double Determinant();
	Mat2D Inverse();
	void InverseThis();
	Mat2D InverseNotNormalize();
	void InverseThisNotNormalize();
};
#endif