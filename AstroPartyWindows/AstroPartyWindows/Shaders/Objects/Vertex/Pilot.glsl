#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 tPos;

uniform float scale;

uniform mat3 model;

uniform vec2 camera_position;
uniform float camera_size;

out vec2 pixel_position;
out vec2 texel_position;

mat3 Rotate(mat3 matrix, float angel);
mat3 Transport(mat3 matrix, vec2 vector);
mat3 Scale(mat3 matrix, vec2 vector);
mat3 Rotate(float angel);
mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);

mat3 matrix;

vec3 _position;
void main()
{
    pixel_position = aPos;
    texel_position = tPos;

    matrix = 
        Rotate(radians(-90.0f)) *
        model *
        Transport(-camera_position) *
        Scale(vec2(1.0f / camera_size)) *
        Scale(vec2(1.0f, scale));
 
    gl_Position = vec4(vec3(aPos, 1.0f) * matrix, 1.0f);
}

mat3 Transport(mat3 matrix, vec2 vector)
{
    return matrix * mat3(
        1.0f,   0.0f,   vector.x, 
        0.0f,   1.0f,   vector.y,
        0.0f,   0.0f,   1.0f);
}


mat3 Scale(mat3 matrix, vec2 vector)
{
    return matrix * mat3(
        vector.x,   0.0f,       0.0f,
        0.0f,       vector.y,   0.0f,
        0.0f,       0.0f,       1.0f);
}

mat3 Rotate(mat3 matrix, float angle)
{
    return matrix * mat3(
        cos(angle),    -sin(angle),     0.0f,
        sin(angle),     cos(angle),     0.0f,
        0.0f,           0.0f,           1.0f);
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

mat3 Rotate(float angle)
{
    return mat3(
        cos(angle),    -sin(angle),     0.0f,
        sin(angle),     cos(angle),     0.0f,
        0.0f,           0.0f,           1.0f);
}