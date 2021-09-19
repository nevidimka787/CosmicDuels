#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}
	fragment_color = vec4(0.0f, 0.8f, 0.2f, 1.0f); 
}