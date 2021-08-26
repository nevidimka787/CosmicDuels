#pragma once

#include <iostream>

#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Color3F
{
public:
	float red;
	float green;
	float blue;

	Color3F();
	Color3F(float brite);
	Color3F(float red, float green, float blue);

	float GetLength();
	float GetLengthSquare();
	float GetMaxComponentValue();
	float GetMinComponentValue();
	float GetSumOfComponents();
	Color3F Log2();
	void Log2This();
	void Set(float brite);
	void Set(float red, float green, float blue);
	Color3F NormalizeAsColor();
	Color3F NormalizeAsVector();
	void NormalizeAsColorThis();
	void NormalizeAsVectorThis();

	Color3F operator+(Color3F color);
	Color3F operator-(Color3F color);
	Color3F operator+(float brite);
	Color3F operator-(float brite);
	void operator+=(Color3F color);
	void operator-=(Color3F color);
	void operator+=(float brite);
	void operator-=(float brite);
	Color3F operator*(Color3F color);
	Color3F operator/(Color3F color);
	Color3F operator*(float value);
	Color3F operator/(float value);
	void operator*=(Color3F color);
	void operator/=(Color3F color);
	void operator*=(float value);
	void operator/=(float value);

	~Color3F();
};