#version 330 core
layout (location = 0) in vec2 aPos;

uniform float scale;

uniform vec2 position;
uniform vec2 vector;
uniform float angle;

uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float _angel);
mat3 Transport(vec2 _vector);
mat3 Scale(vec2 _vector);
float GetAngle(vec2 _vector);

mat3 matrix;

vec3 _position;
void main()
{
    matrix = 
        Scale(vec2(length(100.0f), 1.0f)) * 
        Rotate(-angle) * 
        Transport(position) * 
        Transport(-camera_position) *
        Scale(vec2(1.0f / camera_size)) *
        Scale(vec2(1.0f, scale));
 
    gl_Position = vec4(vec3(aPos, 1.0f) * matrix, 1.0f);
}

mat3 Transport(vec2 _vector)
{
    return mat3(
        1.0f,   0.0f,   _vector.x, 
        0.0f,   1.0f,   _vector.y,
        0.0f,   0.0f,   1.0f);
}


mat3 Scale(vec2 _vector)
{
    return mat3(
        _vector.x,   0.0f,       0.0f,
        0.0f,       _vector.y,   0.0f,
        0.0f,       0.0f,       1.0f);
}

mat3 Rotate(float _angle)
{
    return mat3(
        cos(_angle),   -sin(_angle),    0.0f,
        sin(_angle),    cos(_angle),    0.0f,
        0.0f,           0.0f,           1.0f);
}