#include "OpenGLRealisation.h"
#include <stdio.h>
#include <stdlib.h>

#define INFO_LOG_LENGTH	512

Shader::Shader(const char* vertex_file_name, const char* fragment_file_name)
{
	char* vertex_code = nullptr;
	char* fragment_code = nullptr;

	FILE* vertex_file = nullptr;
	FILE* fragment_file = nullptr;

	vertex_file = fopen(vertex_file_name, "r");
	fragment_file = fopen(fragment_file_name, "r");

	if (vertex_file == nullptr)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::VERTEX::File not found." << std::endl
			<< "File: " << vertex_file_name << std::endl << std::endl;
	}
	else 
	{
		uint32_t file_length = 0;
		while (fgetc(vertex_file) != '\0')
		{
			file_length++;
		}
		vertex_code = (char*)malloc(sizeof(char) * file_length);
		if (fscanf(vertex_file, "%s", vertex_code) != 1)
		{
			std::cout << "ERROR::SHADER_CONSTRUCTOR::VERTEX::File is empty." << std::endl
				<< "File: " << vertex_file_name << std::endl << std::endl;
		}
		fclose(vertex_file);
	}

	if (fragment_file == nullptr)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::FRAGMENT::File not found." << std::endl
			<< "File: " << fragment_file_name << std::endl << std::endl;
	}
	else 
	{
		uint32_t file_length = 0;
		while (fgetc(fragment_file) != '\0')
		{
			file_length++;
		}
		fragment_code = (char*)malloc(sizeof(char) * file_length);
		if (fscanf(fragment_file, "%s", fragment_code) != 1)
		{
			std::cout << "ERROR::SHADER_CONSTRUCTOR::FRAGMENT::File is empty." << std::endl
				<< "File: " << fragment_file_name << std::endl << std::endl;
		}
		fclose(fragment_file);
	}

	uint32_t vertex_shader;
	uint32_t fragment_shader;

	int32_t status;
	char* info_log = (char*)malloc(sizeof(char) * INFO_LOG_LENGTH);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_code, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != 0)
	{
		glGetShaderInfoLog(vertex_shader, INFO_LOG_LENGTH, NULL, info_log);
		std::cout << "ERROR::SHADER::VERTEX::Shader compilation failed." << std::endl
			<< "File: " << fragment_file_name << std::endl
			<< "Logs:\n" << info_log << std::endl
			<< "-----------------------------------------------------------------------" << std::endl << std::endl;
	}
	
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_code, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != 0)
	{
		glGetShaderInfoLog(fragment_shader, INFO_LOG_LENGTH, NULL, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::Shader compilation failed." << std::endl
			<< "File: " << fragment_file_name << std::endl
			<< "Logs:\n" << info_log << std::endl 
			<< "-----------------------------------------------------------------------" << std::endl << std::endl;
	}

	id = glCreateProgram();
	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);

	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status != 0)
	{
		glGetShaderInfoLog(id, INFO_LOG_LENGTH, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::Linking failed." << std::endl
			<< "Vertex file: " << vertex_file_name << std::endl
			<< "Fragment file: " << fragment_file_name << std::endl
			<< "Logs:\n" << info_log << std::endl
			<< "-----------------------------------------------------------------------" << std::endl << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	free((void*)info_log);
	
	if (vertex_code != nullptr)
	{
		free((void*)vertex_code);
	}
	if (fragment_code != nullptr)
	{
		free((void*)fragment_code);
	}
}

void Shader::SetValue(char* name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::SetValue(char* name, int value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::SetValue(char* name, unsigned value)
{
	glUniform1ui(glGetUniformLocation(id, name), value);
}

void Shader::SetValue(char* name, Vec2F* vector)
{
	glUniform2f(glGetUniformLocation(id, name), vector->x, vector->y);
}

void Shader::Use()
{
	glUseProgram(id);
}

Shader::~Shader()
{
	glDeleteProgram(id);
}