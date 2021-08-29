#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math.h"
#include "../Constants/AllConstants.h"
#include "../Types/AllTypes.h"

class StaticBuffer
{
private:
	GLuint id;
	EngineTypes::Buffer::array_length_t points_count;
public:
	StaticBuffer();

	void Draw();
	bool Initialisate(
		Vec2F* points_array = nullptr,
		EngineTypes::Buffer::array_length_t array_length = 0,
		bool delete_last_buffer = false);
	void Use();

	~StaticBuffer();
};