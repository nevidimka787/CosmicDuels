#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform int connected;
uniform int glob_time;

#define ANIMATION_PERIOD	100

float animation = float(glob_time % ANIMATION_PERIOD) / ANIMATION_PERIOD;

#define CENTER_COLOR	vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define BORD_COLOR_0	vec4(float(0x81)/0xFF, float(0x00) / 0xFF, float(0x7F) / 0xFF, 0.5f)
#define BORD_COLOR_1	vec4(float(0x31)/0xFF, float(0x14) / 0xFF, float(0x32) / 0xFF, 0.4f)
#define BORD_RADIUS		0.9f

void main()
{
	float radius;
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}

	fragment_color = CENTER_COLOR * (1.0f - radius) + (BORD_COLOR_0 * (1.0f - animation) + BORD_COLOR_1 * animation) * radius;

	if(connected == 0)
	{
		fragment_color.w *= 0.5f;	
	}
}