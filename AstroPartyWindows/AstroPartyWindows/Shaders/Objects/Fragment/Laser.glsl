#version 330 core

out vec4 frag_color;

uniform float life;	//from 1 to 0

in float pixel_position;

#define COLOR_CENTER	vec4(0.8f, 0.4f, 1.0f, 0.9f)
#define COLOR_BORD		vec4(0.1f, 0.1f, 0.5f, 0.2f)

void main()
{
	frag_color = (abs(pixel_position) * (1.0f - life)) * COLOR_BORD + (1.0f - abs(pixel_position) * (1.0f - life)) * COLOR_CENTER;
}