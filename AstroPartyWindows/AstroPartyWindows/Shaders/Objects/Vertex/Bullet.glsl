#version 330 core
layout (location = 0) in vec2 aPos;
 
out vec2 position;

void main()
{
    position = vec2(aPos * 10.0f);
    gl_Position = vec4(10.0f * aPos.x, 10.0f * aPos.y, 0.0f, 1.0f);
}