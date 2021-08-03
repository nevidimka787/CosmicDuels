#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define INFO_LOG_LENGTH	1024
#define SUCCESS	1
#define FAILED	0

#pragma warning (disable : 26451)

Shader::Shader(const char* vertex_file_name, const char* fragment_file_name)
{
	char* vertex_code = nullptr;
	char* fragment_code = nullptr;

	FILE* vertex_file = nullptr;
	FILE* fragment_file = nullptr;

	if (fopen_s(&vertex_file, fragment_file_name, "r") != 0)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::VERTEX::File not found." << std::endl
			<< "File: " << vertex_file_name << std::endl << std::endl;
	}
	else
	{
		uint32_t file_length = 0;
		while (fgetc(vertex_file) >= 0)
		{
			file_length++;
		}
		fseek(vertex_file, SEEK_SET, 0);
		vertex_code = (char*)malloc(sizeof(char) * (file_length + 1));
		vertex_code[file_length] = '\0';
		for (uint32_t i = 0; i < file_length; i++)
		{
			vertex_code[i] = fgetc(vertex_file);
		}
		fclose(vertex_file);
	}

	if (fopen_s(&fragment_file, fragment_file_name, "r") != 0)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::FRAGMENT::File not found." << std::endl
			<< "File: " << fragment_file_name << std::endl << std::endl;
	}
	else
	{
		uint32_t file_length = 0;
		while (fgetc(fragment_file) >= 0)
		{
			file_length++;
		}
		fseek(fragment_file, SEEK_SET, 0);
		fragment_code = (char*)malloc(sizeof(char) * (file_length + 1));
		fragment_code[file_length] = '\0';
		for (uint32_t i = 0; i < file_length; i++)
		{
			fragment_code[i] = fgetc(fragment_file);
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
	if (status != SUCCESS)
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
	if (status != SUCCESS)
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
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status != SUCCESS)
	{
		glGetProgramInfoLog(id, INFO_LOG_LENGTH, NULL, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::Linking failed." << std::endl
			<< "Vertex file: " << vertex_file_name << std::endl
			<< "Fragment file: " << fragment_file_name << std::endl
			<< "Logs:\n" << info_log << std::endl
			<< "-----------------------------------------------------------------------" << std::endl << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	free(info_log);

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