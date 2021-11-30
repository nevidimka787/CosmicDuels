#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform int inventory;

uniform sampler2D bomb_texture;
uniform sampler2D knife_texture;
uniform sampler2D laser_texture;
uniform sampler2D loop_texture;
uniform sampler2D revers_texture;

float radius;
float radius_pow;

vec4 color_in;
vec4 color_out;

#define LOOP	0x0003 //0000 0000 0000 0011
#define LASER	0x000C //0000 0000 0000 1100
#define BOMB	0x0030 //0000 0000 0011 0000
#define KNIFE	0x00C0 //0000 0000 1100 0000
#define REVERS	0x4000 //0100 0000 0000 0000

void main()
{
	vec2 texel_position = (pixel_position + 1.0f) / 2.0f;

	if((radius = length(pixel_position)) > 1.0f)
	{
		//discard;
	}
	if((inventory & 0x00FF) != 0)
	{
		if((inventory & (LOOP | LASER)) != 0)
		{
			if((inventory & LOOP) != 0)
			{//LOOP
				fragment_color = texture(loop_texture, texel_position);
				return;
				color_in = vec4(0.8f, 0.2f, 0.0f, 1.0f);
				color_out = vec4(0.9f, 0.0f, 0.0f, 0.4f);
				radius_pow = 1.0f;
			}
			else
			{//LASER
				fragment_color = texture(laser_texture, texel_position);
				return;
				color_in = vec4(0.0f, 0.0f, 0.5f, 1.0f);
				color_out = vec4(1.0f, 1.0f, 1.0f, 0.2f);
				radius_pow = 3.0f;
			}
		}
		else if((inventory & BOMB) != 0)
		{//BOMB
			fragment_color = texture(bomb_texture, texel_position);
			return;
			color_in = vec4(1.0f, 0.2f, 0.2f, 1.0f);
			color_out = vec4(0.2f, 1.0f, 0.2f, 0.8f);
			radius_pow = 1.0f;
		}
		else
		{//KNIFE
			fragment_color = texture(knife_texture, texel_position);
			return;
			color_in = vec4(0.0f, 1.0f, 0.0f, 0.1f);
			color_out = vec4(0.0f, 1.0f, 0.0f, 1.0f);
			radius_pow = 2.0f;
		}
	}
	else
	{//REVERS
		fragment_color = texture(revers_texture, texel_position);
		return;
		color_in = vec4(1.0f, 0.0f, 1.0f, 1.0f);
		color_out = vec4(1.0f, 0.0f, 1.0f, 0.8f);
		radius_pow = 1.0f;
	}

	radius = pow(radius, radius_pow);
	fragment_color = color_in * (1.0f - radius) + color_out * radius;
}