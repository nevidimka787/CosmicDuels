#pragma once

#include <iostream>
#include <glad/glad.h>
#include "Vec.h"
#include "Color.h"
#include "../Types/ClassesTypes.h"
class Shader
{
private:
	GLuint id;
public:
	Shader(const char* vertex_file_name, const char* fragment_file_name);

	void SetUniform(GLint uniform_id, ClassTypes::Button::button_text_t* text, uint16_t text_length);
	void SetUniform(GLint uniform_id, float value);
	void SetUniform(GLint uniform_id, int value);
	void SetUniform(GLint uniform_id, unsigned value);
	void SetUniform(GLint uniform_id, Color3F color);
	void SetUniform(GLint uniform_id, Color3F* color);
	void SetUniform(GLint uniform_id, Vec2F vector);
	void SetUniform(GLint uniform_id, Vec2F* vector);

	void Use();

	~Shader();

};