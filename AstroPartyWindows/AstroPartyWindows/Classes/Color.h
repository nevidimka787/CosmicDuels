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

	float GetLength() const;
	float GetLengthSquare() const;
	float GetMaxComponentValue() const;
	float GetMinComponentValue() const;
	float GetSumOfComponents() const;
	Color3F Log2() const;
	void Log2This();
	void Set(float brite);
	void Set(float red, float green, float blue);
	Color3F NormalizeAsColor() const;
	Color3F NormalizeAsVector() const;
	void NormalizeAsColorThis();
	void NormalizeAsVectorThis();

	Color3F operator+(Color3F color) const;
	Color3F operator-(Color3F color) const;
	Color3F operator+(float brite) const;
	Color3F operator-(float brite) const;
	void operator+=(Color3F color);
	void operator-=(Color3F color);
	void operator+=(float brite);
	void operator-=(float brite);
	Color3F operator*(Color3F color) const;
	Color3F operator/(Color3F color) const;
	Color3F operator*(float value) const;
	Color3F operator/(float value) const;
	void operator*=(Color3F color);
	void operator/=(Color3F color);
	void operator*=(float value);
	void operator/=(float value);

	~Color3F();
};