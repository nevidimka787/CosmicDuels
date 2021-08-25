#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;
uniform vec2 size;
uniform float scale;

void main()
{
	if(scale < 10.0f)
	{
		gl_Position = vec4(aPos * size * vec2(1.0f, scale) + position, 0.0f, 1.0f);
	}
	else
	{
		gl_Position = vec4(aPos * size * vec2(1.0f, scale) * (10.0f / scale) + position, 0.0f, 1.0f);
	}
}