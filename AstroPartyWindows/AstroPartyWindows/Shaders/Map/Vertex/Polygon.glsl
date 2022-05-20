#version 330 core
layout (location = 0) in vec2 aPos;

uniform float scale;

uniform vec4 side;

uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float _angel);
mat3 Transport(vec2 _vector);
mat3 Scale(vec2 _vector);
mat3 SetBySegment(vec4 _segment);

mat3 matrix;

#define BORDER  0.015f

vec3 _position;
void main()
{
    matrix =
        Scale(vec2(1.0f, BORDER)) * 
        SetBySegment(side) * 
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

mat3 SetBySegment(vec4 _segment)
{
    if(_segment.z > 0.0f)
    {
        return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z)) * Transport(_segment.xy);
    }
    if(_segment.w > 0.0f)
    {
        return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z) + radians(180.0f)) * Transport(_segment.xy);
    }
    return Scale(vec2(length(_segment.zw), 1.0f)) * Rotate(atan(_segment.w / _segment.z) - radians(180.0f)) * Transport(_segment.xy);
}