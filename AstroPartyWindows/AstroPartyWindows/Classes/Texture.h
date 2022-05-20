#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../stb_image.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Texture2D
{
private:
	GLuint id;
public:
	Texture2D();

	void Delete();
	bool Initialisate(
		const char* texture_file_name,
		GLint format = GL_RGB,
		GLint internal_format = GL_RGB,
		GLint wrap_s_rule = GL_CLAMP_TO_BORDER,
		GLint wrap_t_rule = GL_CLAMP_TO_BORDER,
		GLint min_filter_parameter = GL_NEAREST,
		GLint mag_filter_parameter = GL_NEAREST);
	void Use();
	void Use(GLuint as_texture_number);

	~Texture2D();
};