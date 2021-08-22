#include "Color.h"

Color3F::Color3F() : red(0), green(0), blue(0)
{

}

Color3F::Color3F(float brite) : red(brite), green(brite), blue(brite)
{

}

Color3F::Color3F(float red, float green, float blue) : red(red), green(green), blue(blue)
{

}

float Color3F::GetLength()
{
	return sqrtf(red * red + green * green + blue * blue);
}

float Color3F::GetLengthSquare()
{
	return red * red + green * green + blue * blue;
}

float Color3F::GetMaxComponentValue()
{
	return fmaxf(fmaxf(red, green), blue);
}

float Color3F::GetMinComponentValue()
{
	return fminf(fminf(red, green), blue);
}

float Color3F::GetSumOfComponents()
{
	return red + green + blue;
}

Color3F Color3F::Log2()
{
	return Color3F((red > 0) ? log2f(red) : -log2f(-red), (green > 0) ? log2f(green) : -log2f(-green), (blue > 0) ? log2f(blue) : -log2f(-blue));
}

void Color3F::Log2This()
{
	if (red > 0)
	{
		red = log2f(red);
	}
	else
	{
		red = -log2f(-red);
	}
	if (green > 0)
	{
		green = log2f(green);
	}
	else
	{
		green = -log2f(-green);
	}
	if (blue > 0)
	{
		blue = log2f(blue);
		return;
	}
	blue = -log2f(-blue);
}

void Color3F::Set(float brite)
{
	red = brite;
	green = brite;
	blue = brite;
}

void Color3F::Set(float red, float green, float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

Color3F Color3F::NormalizeAsColor()
{
	Color3F return_color = Color3F(*this);
	float min = GetMinComponentValue();
	if (min < 0.0f)
	{
		return_color -= min;
	}
	min = return_color.GetMaxComponentValue();
	if (min > 1.0f)
	{
		return_color /= min;
	}
	return return_color;
}

void Color3F::NormalizeAsColorThis()
{
	float min = GetMinComponentValue();
	if (min < 0.0f)
	{
		*this -= min;
	}
	min = GetMaxComponentValue();
	if (min > 1.0f)
	{
		*this /= min;
	}
}

Color3F Color3F::NormalizeAsVector()
{
	return *this / GetLength();
}

void Color3F::NormalizeAsVectorThis()
{
	*this /= GetLength();
}

Color3F Color3F::operator+(Color3F color)
{
	return Color3F(red + color.red, green + color.green, blue + color.blue);
}

Color3F Color3F::operator-(Color3F color)
{
	return Color3F(red - color.red, green - color.green, blue - color.blue);
}

Color3F Color3F::operator+(float value)
{
	return Color3F(red + value, green + value, blue + value);
}

Color3F Color3F::operator-(float value)
{
	return Color3F(red - value, green - value, blue - value);
}

void Color3F::operator+=(Color3F color)
{
	red += color.red;
	green += color.green;
	blue += color.blue;
}

void Color3F::operator-=(Color3F color)
{
	red -= color.red;
	green -= color.green;
	blue -= color.blue;
}

void Color3F::operator+=(float value)
{
	red += value;
	green += value;
	blue += value;
}

void Color3F::operator-=(float value)
{
	red -= value;
	green -= value;
	blue -= value;
}

Color3F Color3F::operator*(Color3F color)
{
	return Color3F(red * color.red, green * color.green, blue * color.blue);
}

Color3F Color3F::operator/(Color3F color)
{
	return Color3F(red / color.red, green / color.green, blue / color.blue);
}

Color3F Color3F::operator*(float value)
{
	return Color3F(red * value, green * value, blue * value);
}

Color3F Color3F::operator/(float value)
{
	return Color3F(red / value, green / value, blue / value);
}

void Color3F::operator*=(Color3F color)
{
	red *= color.red;
	green *= color.green;
	blue *= color.blue;
}

void Color3F::operator/=(Color3F color)
{
	red /= color.red;
	green /= color.green;
	blue /= color.blue;
}

void Color3F::operator*=(float value)
{
	red *= value;
	green *= value;
	blue *= value;
}

void Color3F::operator/=(float value)
{
	red /= value;
	green /= value;
	blue /= value;
}

Color3F::~Color3F()
{

}
