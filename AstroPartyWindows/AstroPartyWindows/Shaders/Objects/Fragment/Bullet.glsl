#version 330 core

out vec4 FragColor;
 
in vec2 pixel_position;

struct Grav
{
    vec2 pos;
    float mas;
};

Grav sun = {vec2(0.0f, 0.0f), 1.0f};
Grav earth = {vec2(0.6f, 0.0f), 0.1f};

vec2 GetForce(Grav obj, vec2 pos)
{
    vec2 target = pos - obj.pos;
    return normalize(target) / (target.x * target.x + target.y * target.y) * obj.mas;
}

vec2 GetMasPos(Grav obj1, Grav obj2)
{
    return obj2.pos + (obj1.pos - obj2.pos) * obj1.mas / (obj1.mas + obj2.mas);
}

float GetSqrAngularVel(Grav obj1, Grav obj2)
{
    float l = length(obj1.pos - obj2.pos);
    return obj1.mas / l / l / l * (1.0f + obj2.mas / obj1.mas);
}

float GetAngularVel(Grav obj1, Grav obj2)
{
    float l = length(obj1.pos - obj2.pos);
    return sqrt(obj1.mas / l / l / l * (1.0f + obj2.mas / obj1.mas));
}

float SqrLength(vec2 vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

void main()
{
    vec2 f1 = GetForce(sun, pixel_position);
    vec2 f2 = GetForce(earth, pixel_position);
    vec2 fc = -(pixel_position - GetMasPos(sun, earth)) * GetSqrAngularVel(sun, earth);
    vec2 res = f1 + f2 + fc;
    float blue = 0.00001f / SqrLength(res) / SqrLength(res);
    FragColor = vec4(length(res) / 50 * sign(dot(res, sun.pos - pixel_position)), length(res) / 50 * -sign(dot(res, sun.pos - pixel_position)), blue, 1.0f);
}