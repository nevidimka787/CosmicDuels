#version 330 core
layout (location = 0) in vec2 aPos;

uniform float scale;
uniform vec2 position;

void main()
{
    gl_Position = vec4(aPos + position / 100.0f, 0.0f, 1.0f);
}