#version 330 core
layout (location = 0) in vec2 aPos;

uniform vec2 position;
uniform vec2 size;
uniform float scale;

out vec2 pixel_position;

void main()
{
	pixel_position = aPos;
	gl_Position = vec4(aPos * vec2(1.0f, scale) / max(scale, 1.0f) * size + position, 0.0f, 1.0f);
}