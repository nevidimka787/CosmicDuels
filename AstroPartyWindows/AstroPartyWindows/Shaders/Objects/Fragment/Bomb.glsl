#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform float animation;

vec3 color_in_start = vec3(0.0f, 1.0f, 0.0f);
vec3 color_in_last = vec3(1.0f, 0.0f, 0.0f);
vec3 color_out_start = vec3(0.5f);
vec3 color_out_last = vec3(1.0f, 0.5f, 0.5f);
vec3 color_curent_in;
vec3 color_curent_out;

float radius;

void main()
{
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}
	color_curent_in = color_in_start * animation + color_in_last * (1.0f - animation);
	color_curent_out = color_out_start * animation + color_out_last * (1.0f - animation);
	frag_color = vec4(color_curent_out * radius + color_curent_in * (1.0f - radius) , 1.0f);
}