#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

float radius;

void main()
{
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}

	fragment_color = vec4(0.0f, 1.0f, 1.0f - radius, 1.0f); 
}