#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}
	fragment_color = vec4(1.0f, 0.2f, 0.2f, 0.3f); 
}