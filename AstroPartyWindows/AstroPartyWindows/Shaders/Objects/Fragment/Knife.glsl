#version 330 core

out vec4 frag_color;

in float pixel_position;

void main()
{
	frag_color = vec4(0.1f, 1.0f, 0.4f,	abs(pixel_position * 50.0f));
}