#version 330 core
layout (location = 0) in vec2 aPos;

out float pixel_position;

uniform float scale;

uniform vec4 beam;

uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float _angel);
mat3 Transport(vec2 _vector);
mat3 Scale(vec2 _vector);
mat3 SetByBeam(vec4 _beam);

mat3 matrix;

void main()
{
    pixel_position = aPos.y;
    matrix =
        Scale(vec2(1.1f, 0.5f)) * 
        SetByBeam(beam) * 
        Transport(-camera_position) *
        Scale(vec2(1.0f / camera_size)) *
        Scale(vec2(1.0f, scale));
 
    gl_Position = vec4(vec3(aPos, 1.0f) * matrix, 1.0f);
    //gl_Position = vec4(aPos, 0.0f, 1.0f);
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

mat3 SetByBeam(vec4 _beam)
{
    if(_beam.z > 0.0f)
    {
        return Scale(vec2(length(_beam.zw), 1.0f)) * Rotate(atan(_beam.w / _beam.z)) * Transport(_beam.xy);
    }
    if(_beam.w > 0.0f)
    {
        return Scale(vec2(length(_beam.zw), 1.0f)) * Rotate(atan(_beam.w / _beam.z) + radians(180.0f)) * Transport(_beam.xy);
    }
    return Scale(vec2(length(_beam.zw), 1.0f)) * Rotate(atan(_beam.w / _beam.z) - radians(180.0f)) * Transport(_beam.xy);
}