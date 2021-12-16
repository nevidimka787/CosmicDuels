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
	GLuint buffer_id;
public:
	StaticBuffer();

	void Draw() const;
	bool Initialisate(
		const Vec2F* points_array = nullptr,
		EngineTypes::Buffer::array_length_t array_length = 0,
		bool delete_last_buffer = false);
	bool Initialisate(
		const Vec2F* points_array = nullptr,
		const Vec2F* points_second_array = nullptr,
		EngineTypes::Buffer::array_length_t array_length = 0,
		bool delete_last_buffer = false);
	void Use() const;

	~StaticBuffer();
};

class FrameBuffer
{
private:
	StaticBuffer frame;

	GLuint id;
	GLuint buffers_count;
	GLenum* draw_buffers;

	GLuint height;
	GLuint width;
public:

	FrameBuffer();

	//draw buffer to window
	void Draw();
	bool Initialisate(
		GLuint width, 
		GLuint height,
		GLuint buffers_count = 1);
	//The function renders this buffer.
	void Render();
	//The function change size of this buffer.
	void Resize(GLuint width, GLuint height);
	//OpenGL will work with this buffer.
	void Use();

	~FrameBuffer();
};