#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform float animation;

#define color_in_start	vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_IN_LAST	vec4(1.0f, 0.0f, 0.0f, 0.1f)
#define COLOR_OUT_START	vec4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_OUT_LAST	vec4(1.0f, 0.5f, 0.5f, 0.1f)
vec4 color_curent_in;
vec4 color_curent_out;

float radius;

void main()
{
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}

	color_curent_in = color_in_start * animation + COLOR_IN_LAST * (1.0f - animation);
	color_curent_out = COLOR_OUT_START * animation + COLOR_OUT_LAST * (1.0f - animation);
	frag_color = color_curent_out * radius + color_curent_in * (1.0f - radius);
}