#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

void main()
{
	if(length(pixel_position) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(0.7f, 0.2f, 0.0f, 1.0f);
}