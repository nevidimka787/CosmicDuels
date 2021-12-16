#include "Buffer.h"

#pragma warning (disable : 6011)
#pragma warning (disable : 6386)
#pragma warning (disable : 26451)

StaticBuffer::StaticBuffer() :
	id(0),
	points_count(0)
{
}

void StaticBuffer::Draw() const
{
	glDrawArrays(GL_TRIANGLES, 0, points_count);
}

bool StaticBuffer::Initialisate(const Vec2F* points_array, EngineTypes::Buffer::array_length_t array_lenght, bool delete_last_buffer)
{
	if (delete_last_buffer)
	{
		glDeleteBuffers(0, &id);
	}
	if (points_array == nullptr || array_lenght <= 0)
	{
		std::cout << "ERROR::BUFFER::INITIALISATE::Incorrect input data." << std::endl;
		id = 0;
		points_count = 0;
		return false;
	}

	float* vertexes = new float[array_lenght * 2];
	points_count = array_lenght;
	for (EngineTypes::Buffer::array_length_t i = 0; i < array_lenght; i++)
	{
		vertexes[i * 2] = points_array[i].x;
		vertexes[i * 2 + 1] = points_array[i].y;
	}

	GLuint buffer_id;

	glGenVertexArrays(1, &id);
	glGenBuffers(1, &buffer_id);

	Use();

	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_lenght * 2, vertexes, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
	glBindVertexArray(0);               //unbind array

	delete[] vertexes;

	return true;
}

bool StaticBuffer::Initialisate(const Vec2F* points_array, const Vec2F* points_second_array, EngineTypes::Buffer::array_length_t array_lenght, bool delete_last_buffer)
{
	if (delete_last_buffer)
	{
		glDeleteBuffers(0, &id);
	}
	if (points_array == nullptr || points_second_array == nullptr || array_lenght <= 0)
	{
		std::cout << "ERROR::BUFFER::INITIALISATE::Incorrect input data." << std::endl;
		id = 0;
		points_count = 0;
		return false;
	}

	float* vertexes = new float[array_lenght * 4];
	points_count = array_lenght;
	for (EngineTypes::Buffer::array_length_t i = 0; i < array_lenght; i++)
	{
		vertexes[i * 4] = points_array[i].x;
		vertexes[i * 4 + 1] = points_array[i].y;
		vertexes[i * 4 + 2] = points_second_array[i].x;
		vertexes[i * 4 + 3] = points_second_array[i].y;
	}

	GLuint buffer_id;

	glGenVertexArrays(1, &id);
	glGenBuffers(1, &buffer_id);

	Use();

	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * array_lenght * 4, vertexes, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
	glBindVertexArray(0);               //unbind array

	delete[] vertexes;

	return true;
}

void StaticBuffer::Use() const
{
	glBindVertexArray(id);
}

StaticBuffer::~StaticBuffer()
{
	glDeleteBuffers(0, &id);
}



FrameBuffer::FrameBuffer()
	:
	id(0),
	buffers_count(0),
	draw_buffers(nullptr),
	height(0),
	width(0)
{
	Vec2F points[6] = {Vec2F(-1.0f), Vec2F(-1.0f, 1.0f), Vec2F(1.0f, -1.0f), Vec2F(1.0f), Vec2F(-1.0f, 1.0f), Vec2F(1.0f, -1.0f)};
	frame.Initialisate(points, 6);
}

void FrameBuffer::Draw()
{
	glDrawBuffers(buffers_count, draw_buffers);
	frame.Draw();
}

void FrameBuffer::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glViewport(0, 0, height, width);
}

bool FrameBuffer::Initialisate(GLuint width, GLuint height, GLuint buffers_count)
{
	if (buffers_count == 0)
	{
		return false;
	}

	this->buffers_count = buffers_count;
	this->height = height;
	this->width = width;

	glGenFramebuffers(buffers_count, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	draw_buffers = new GLenum[buffers_count];
	for (GLuint i = 0; i < buffers_count; i++)
	{
		draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	return true;
}

void FrameBuffer::Resize(GLuint width, GLuint height)
{
	this->height = height;
	this->width = width;

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

}

void FrameBuffer::Use()
{
	frame.Use();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);
}

FrameBuffer::~FrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDeleteBuffers(buffers_count, draw_buffers);
	delete[] draw_buffers;
}