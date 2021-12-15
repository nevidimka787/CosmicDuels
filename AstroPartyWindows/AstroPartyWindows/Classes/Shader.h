#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Color.h"
#include "Math.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Shader
{
private:
    GLuint id;
public:
    //GLuint* uniform_ids_array;

    Shader();
    bool Initialisate(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetUniform(const std::string& name, const EngineTypes::Button::text_t* text, uint16_t text_length) const;
    void SetUniform(const std::string& name, int value) const;
    void SetUniform(const std::string& name, float value) const;
    void SetUniform(const std::string& name, Beam beam) const;
    void SetUniform(const std::string& name, const Beam* beam) const;
    void SetUniform(const std::string& name, Color3F color) const;
    void SetUniform(const std::string& name, const Color3F* color) const;
    void SetUniform(const std::string& name, Line line) const;
    void SetUniform(const std::string& name, const Line* line) const;
    void SetUniform(const std::string& name, Mat2F matrix) const;
    void SetUniform(const std::string& name, const Mat2F* matrix) const;
    void SetUniform(const std::string& name, Mat3x2F matrix) const;
    void SetUniform(const std::string& name, const Mat3x2F* matrix) const;
    void SetUniform(const std::string& name, Vec2F vector) const;
    void SetUniform(const std::string& name, const Vec2F* vector) const;
    void SetUniform(const std::string& name, Segment segment) const;
    void SetUniform(const std::string& name, const Segment* segment) const;
    ~Shader();
};