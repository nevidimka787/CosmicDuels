#pragma once

#include <iostream>
#include "Vec.h"
class Shader
{
private:
	uint32_t id;
public:
	Shader(const char* vertex_file_name, const char* fragment_file_name);

	void SetValue(char* name, float value);
	void SetValue(char* name, int value);
	void SetValue(char* name, unsigned value);
	void SetValue(char* name, Vec2F* vector);

	void Use();

	~Shader();

};