#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
vec2 texel_position = (pixel_position + 2.0f) / 4.0f;

uniform sampler2D txtr;

void main()
{
	frag_color = texture(txtr, texel_position);
	return;
}