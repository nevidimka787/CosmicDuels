#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform float gravity;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}

	fragment_color = vec4(gravity / 2.0f + 0.5f, -gravity / 2.0f + 0.5f, 0.0f, 1.0f); 
}