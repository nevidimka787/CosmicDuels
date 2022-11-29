#include "Shader.h"

#pragma warning(disable : 4996)

#define FRAGMENT 1
#define VERTEX 2
#define PROGRAM 3

#define INFO_LOG_LENGTH	1024
#define SUCCESS	1
#define FAILED	0

Shader::Shader():
    id(0)
{
}

void Shader::Delete()
{
	if (id != 0)
	{
		glDeleteProgram(id);
	}
}

bool Shader::Initialisate(const char* vertex_file_name, const char* fragment_file_name)
{
    bool succses_flag = true;

	char* vertex_code = nullptr;
	char* fragment_code = nullptr;

	FILE* vertex_file = nullptr;
	FILE* fragment_file = nullptr;

	if ((vertex_file = fopen(vertex_file_name, "r")) == nullptr)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::VERTEX::File not found." << std::endl
			<< "File: " << vertex_file_name << std::endl << std::endl;
		return false;
	}
	else
	{
		uint32_t file_length = 0;
		while (fgetc(vertex_file) >= 0)
		{
			file_length++;
		}
		fseek(vertex_file, SEEK_SET, 0);
		vertex_code = (char*)malloc((uint64_t)sizeof(char) * ((uint64_t)file_length + 1));
		vertex_code[file_length] = '\0';
		for (uint32_t i = 0; i < file_length; i++)
		{
			vertex_code[i] = fgetc(vertex_file);
		}
		fclose(vertex_file);
	}

	if ((fragment_file = fopen(fragment_file_name, "r")) == nullptr)
	{
		std::cout << "ERROR::SHADER_CONSTRUCTOR::FRAGMENT::File not found." << std::endl
			<< "File: " << fragment_file_name << std::endl << std::endl;
		free(vertex_code);
		return false;
	}
	else
	{
		uint32_t file_length = 0;
		while (fgetc(fragment_file) >= 0)
		{
			file_length++;
		}
		fseek(fragment_file, SEEK_SET, 0);
		fragment_code = (char*)malloc((uint64_t)sizeof(char) * ((uint64_t)file_length + 1));
		fragment_code[file_length] = '\0';
		for (uint32_t i = 0; i < file_length; i++)
		{
			fragment_code[i] = fgetc(fragment_file);
		}
		fclose(fragment_file);
	}

	GLuint vertex_shader;
	GLuint fragment_shader;

	GLint status;
	GLchar* info_log = (GLchar*)malloc(sizeof(GLchar) * INFO_LOG_LENGTH);

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
		succses_flag = false;
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
		succses_flag = false;
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
		succses_flag = false;
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

	return succses_flag;
}
void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetUniform(const std::string& name, const EngineTypes::Button::text_t* text, uint16_t text_length) const
{
    glUniform1iv(glGetUniformLocation(id, name.c_str()), text_length, (GLint*)text);
}
void Shader::SetUniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::SetUniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetUniform(const std::string& name, Beam beam) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), beam.point.x, beam.point.y, beam.vector.x, beam.vector.y);
}

void Shader::SetUniform(const std::string& name, const Beam* beam) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), beam->point.x, beam->point.y, beam->vector.x, beam->vector.y);
}

void Shader::SetUniform(const std::string& name, Color3F color) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), color.red, color.green, color.blue);
}

void Shader::SetUniform(const std::string& name, const Color3F* color) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), color->red, color->green, color->blue);
}

void Shader::SetUniform(const std::string& name, Line line) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), line.point.x, line.point.y, line.vector.x, line.vector.y);
}

void Shader::SetUniform(const std::string& name, const Line* line) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), line->point.x, line->point.y, line->vector.x, line->vector.y);
}

void Shader::SetUniform(const std::string& name, Mat2F matrix) const
{
	float f_matrix[] = 
	{
		matrix.a11, matrix.a12,
		matrix.a21, matrix.a22 
	};
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, false, f_matrix);
}

void Shader::SetUniform(const std::string& name, const Mat2F* matrix) const
{
	float f_matrix[] =
	{
		matrix->a11, matrix->a12,
		matrix->a21, matrix->a22
	};
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, false, f_matrix);
}

void Shader::SetUniform(const std::string& name, Mat3x2F matrix) const
{
	float f_matrix[] =
	{
		matrix.a11, matrix.a12, matrix.a13,
		matrix.a21, matrix.a22, matrix.a23,
		0.0f, 0.0f, 1.0f
	};
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, false, f_matrix);
}

void Shader::SetUniform(const std::string& name, const Mat3x2F* matrix) const
{
	float f_matrix[] =
	{
		matrix->a11, matrix->a12, matrix->a13,
		matrix->a21, matrix->a22, matrix->a23,
		0.0f, 0.0f, 1.0f
	};
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, false, f_matrix);
}


void Shader::SetUniform(const std::string& name, Vec2F vector) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
}

void Shader::SetUniform(const std::string& name, const Vec2F* vector) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector->x, vector->y);
}

void Shader::SetUniform(const std::string& name, Segment segment) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), segment.point.x, segment.point.y, segment.vector.x, segment.vector.y);
}

void Shader::SetUniform(const std::string& name, const Segment* segment) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), segment->point.x, segment->point.y, segment->vector.x, segment->vector.y);
}

Shader::~Shader()
{
}