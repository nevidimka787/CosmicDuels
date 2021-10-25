#version 330 core
layout (location = 0) in vec2 aPos;

uniform float radius;
uniform vec2 position;
uniform float angle;
uniform int type;

uniform float scale;
uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float angel);
mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);

mat3 matrix;

#define BACKGROUND					0x01
#define EXAUST_ENGINE				0x02
#define EXAUST_SHOOT				0x03
#define EXAUST_BOMB_BOOM			0x04
#define EXAUST_BUTNOUT				0x05
#define SHARDS_ASTEROID				0x06
#define SHARDS_ASTEROID_POWERED		0x07
#define SHARDS_MAP_ELEMENT			0x08
#define SHARDS_PILOT				0x09
#define SHARDS_SHIP					0x0A

out vec2 pixel_position;

void main()
{
    switch(type)
    {
    case BACKGROUND:
        pixel_position = vec2(aPos.x, aPos.y / scale) * camera_size + camera_position / radius + position + radius;
        matrix = mat3(1.0f);
        break;
    case EXAUST_ENGINE:
        pixel_position = aPos;
        matrix =
            Transport(vec2(-2.0f, 0.0f)) *
            Rotate(angle) *
            Scale(vec2(radius)) * 
            Transport(position) *
            Transport(-camera_position) *
            Scale(vec2(1.0f / camera_size)) *
            Scale(vec2(1.0f, scale));
        break;
    default:
        pixel_position = aPos;
        matrix =
            Rotate(angle) *
            Scale(vec2(radius)) * 
            Transport(position) *
            Transport(-camera_position) *
            Scale(vec2(1.0f / camera_size)) *
            Scale(vec2(1.0f, scale));
        break;
    }
 
    gl_Position = vec4(vec3(aPos, 1.0f) * matrix, 1.0f);
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