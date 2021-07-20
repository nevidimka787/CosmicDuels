#pragma once

class Parent
{
protected:
	int* some_value;
public:
	Parent(int* value);
	~Parent();
};

class Child : public Parent
{
protected:
	double* new_value;
public:
	Child(int* value, double* new_value);
	~Child();
};
