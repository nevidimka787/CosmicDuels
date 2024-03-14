#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform float decel_param;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}
	if (decel_param > 0.0f)
	{
		fragment_color = vec4(1.0f, 0.2f, 0.2f, 0.3f);
	}
	else
	{
		fragment_color = vec4(0.2f, 1.0f, 0.2f, 0.3f); 
	}
}