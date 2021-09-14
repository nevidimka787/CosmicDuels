#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform float bonus;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}
	if(bonus > 0.5f)
	{
		frag_color = vec4(0.7f, 0.2f, 0.0f, 1.0f);
	}
	else
	{
		frag_color = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	}
}