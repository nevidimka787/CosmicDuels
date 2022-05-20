#version 330 core
layout (location = 0) in vec2 a_pos;

uniform float scale;

uniform vec2 position;
uniform vec2 point2;

uniform vec2 camera_position;
uniform float camera_size;

mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);

mat3 matrix;
vec3 _position;

out vec2 v_pos;
out vec2 size;

#define BORDER  0.02f

void main()
{
    size = abs(position - point2);
    v_pos = a_pos * (1.0f + BORDER / size);

    matrix = 
        Scale((point2 - position) / 2.0f) * 
        Transport((point2 + position) / 2.0f) * 
        Transport(-camera_position) *
        Scale(vec2(1.0f / camera_size)) *
        Scale(vec2(1.0f, scale));

    gl_Position = vec4(vec3(v_pos, 1.0f) * matrix, 1.0f);
}

mat3 Transport(vec2 vector)
{
    return mat3(
        1.0f,   0.0f,   vector.x, 
        0.0f,   1.0f,   vector.y,
        0.0f,   0.0f,   1.0f);
}


mat3 Scale(vec2 vector)
{
    return mat3(
        vector.x,   0.0f,       0.0f,
        0.0f,       vector.y,   0.0f,
        0.0f,       0.0f,       1.0f);
}