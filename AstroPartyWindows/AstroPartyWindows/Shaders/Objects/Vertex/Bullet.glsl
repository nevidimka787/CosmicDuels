#version 330 core
layout (location = 0) in vec2 aPos;
 
out vec2 position;

void main()
{
    position = vec2(aPos * 0.9f);
    gl_Position = vec4(0.9f * aPos.x, 0.9f * aPos.y, 0.0f, 1.0f);
}