#version 330 core
layout (location = 0) in vec2 aPos;

#define fopen fopen_s

uniform float radius;
uniform vec2 position;
uniform float angle;
uniform int type;
uniform float animation;

uniform float scale;
uniform vec2 camera_position;
uniform float camera_size;

mat3 Rotate(float angel);
mat3 Transport(vec2 vector);
mat3 Scale(vec2 vector);

mat3 matrix;

#define NULL					0
#define BACKGROUND				1
#define EXAUST_ENGINE			2
#define EXAUST_BURNOUT			3
#define EXAUST_BOMB_BOOM		4
#define EXAUST_SHOOT			5
#define PORTAL_IN				6
#define PORTAL_OUT				7
#define SHARDS_ASTEROID			8
#define SHARDS_ASTEROID_POWERED	9
#define SHARDS_MAP_ELEMENT		10
#define SHARDS_PILOT			11
#define SHARDS_SHIP				12

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
            Transport(vec2(-0.8f, 0.0f)) *
            Rotate(angle) *
            Scale(vec2(radius)) * 
            Transport(position) *
            Transport(-camera_position) *
            Scale(vec2(1.0f / camera_size)) *
            Scale(vec2(1.0f, scale));
        break;
    case PORTAL_OUT:
        pixel_position = aPos;
        matrix =
            Rotate(angle) *
            Scale(vec2(radius)) * 
            Transport(position) *
            Transport(-camera_position) *
            Scale(vec2(1.0f / camera_size)) *
            Scale(vec2(1.0f, scale));
        break;
    case PORTAL_IN:
    case SHARDS_ASTEROID:
    case SHARDS_ASTEROID_POWERED:
    case SHARDS_PILOT:
    case SHARDS_SHIP:
        pixel_position = aPos;
        matrix =
            Scale(vec2(1.0f + animation)) *
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