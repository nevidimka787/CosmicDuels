#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform sampler2D small_txtr;
uniform sampler2D medium_txtr;
uniform sampler2D large_txtr;

uniform float radius;

#define DEFAULT_RADIUS	0.02f

void main()
{
	if(radius < DEFAULT_RADIUS)
	{
		fragment_color = texture(small_txtr, (pixel_position + 1.0f) / 2.0f);
	}
	else if(radius <= DEFAULT_RADIUS * 2.0f)
	{
		fragment_color = texture(medium_txtr, (pixel_position + 1.0f) / 2.0f);
	}
	else
	{
		fragment_color = texture(large_txtr, (pixel_position + 1.0f) / 2.0f);
	}
}