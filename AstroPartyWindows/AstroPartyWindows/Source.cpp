#include "Header.h"

Parent::Parent(int* value) : some_value(new int(*value))
{

}

Parent::~Parent()
{
	delete some_value;
}

Child::Child(int* value, double* new_value) : new_value(new double(*new_value))
{
	Parent::Parent(value);
}

Child::~Child()
{

}