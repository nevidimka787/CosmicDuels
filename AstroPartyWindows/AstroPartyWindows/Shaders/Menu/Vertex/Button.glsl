#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 p;//position
uniform vec2 s;//size

void main()
{
    gl_Position = vec4(aPos * s + p, 0.0f, 1.0f);
    //gl_Position = vec4(aPos, 0.0f, 1.0f);
}