#include "Buffer.h"

#pragma warning (disable : 6011)
#pragma warning (disable : 6386)
#pragma warning (disable : 26451)

StaticBuffer::StaticBuffer() :
	id(0),
	points_count(0)
{
}

void StaticBuffer::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, points_count);
}

bool StaticBuffer::Initialisate(Vec2F* points_array, EngineTypes::Buffer::array_length_t array_lenght, bool delete_last_buffer)
{
	if (delete_last_buffer)
	{
		glDeleteBuffers(0, &id);
	}
	if (points_array == nullptr && array_lenght <= 0)
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

	glEnableVertexAttribArray(0);       //disable vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind buffer
	glBindVertexArray(0);               //unbind array

	return true;
}

void StaticBuffer::Use()
{
	glBindVertexArray(id);
}

StaticBuffer::~StaticBuffer()
{
	glDeleteBuffers(0, &id);
}