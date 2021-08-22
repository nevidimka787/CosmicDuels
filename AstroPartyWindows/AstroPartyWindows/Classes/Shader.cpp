#include "Shader.h"
#define FRAGMENT 1
#define VERTEX 2
#define PROGRAM 3
Shader::Shader(const char* vertex_file_name, const char* fragment_file_name)
{
    std::string vertex_source_code;
    std::string fragment_source_code;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertex_shader_file.open(vertex_file_name);
        fragment_shader_file.open(fragment_file_name);
        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_source_code = vertex_shader_stream.str();
        fragment_source_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::File not succesfully read." << std::endl
            << "Vertex file: " << vertex_file_name << std::endl
            << "Fragment file: " << fragment_file_name << std::endl
            << "--------------------------------------------------------------" << std::endl << std::endl;
    }
    const char* vertex_shader_code = vertex_source_code.c_str();
    const char* fragment_shader_code = fragment_source_code.c_str();

    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
    int success;
    char logs[1024];

    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader_id);
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 1024, NULL, logs);
        std::cout << "ERROR::SHADER::VERTEX::Compilation failed." << std::endl
            << "File: " << vertex_file_name << std::endl
            << "Logs: " << logs << std::endl
            << "--------------------------------------------------------------" << std::endl << std::endl;
    }

    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader_id);
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 1024, NULL, logs);
        std::cout << "ERROR::SHADER::FRAGMENT::Compilation failed." << std::endl
            << "File: " << fragment_file_name << std::endl
            << "Logs: " << logs << std::endl
            << "--------------------------------------------------------------" << std::endl << std::endl;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex_shader_id);
    glAttachShader(id, fragment_shader_id);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader_id, 1024, NULL, logs);
        std::cout << "ERROR::SHADER::PROGRAM::Linking failed." << std::endl
            << "Vertex file: " << fragment_file_name << std::endl
            << "Fragment file: " << fragment_file_name << std::endl
            << "Logs: " << logs << std::endl
            << "--------------------------------------------------------------" << std::endl << std::endl;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    /*GLint active_uniforms_count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &active_uniforms_count);
    if (active_uniforms_count > 0)
    {
        uniform_ids_array = new GLuint[active_uniforms_count];
    }*/
}
void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetUniform(const std::string& name, ClassTypes::Button::button_text_t* text, uint16_t text_length) const
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

void Shader::SetUniform(const std::string& name, Color3F color) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), color.red, color.green, color.blue);
}

void Shader::SetUniform(const std::string& name, Color3F* color) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), color->red, color->green, color->blue);
}

void Shader::SetUniform(const std::string& name, Vec2F vector) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
}

void Shader::SetUniform(const std::string& name, Vec2F* vector) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector->x, vector->y);
}