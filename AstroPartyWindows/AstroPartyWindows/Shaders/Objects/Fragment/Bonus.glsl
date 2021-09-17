#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform int inventory;

float radius;

vec3 color;

#define LOOP	0x0003 //0000 0000 0000 0011
#define LASER	0x000C //0000 0000 0000 1100
#define BOMB	0x0030 //0000 0000 0011 0000
#define KNIFE	0x00C0 //0000 0000 1100 0000
#define REVERS	0x4000 //0100 0000 0000 0000

void main()
{
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}
	if((inventory & 0x00FF) != 0)
	{
		if((inventory & (LOOP | LASER)) != 0)
		{
			if((inventory & LOOP) != 0)
			{//LOOP
				color = vec3(0.8f, 0.2f, 0.0f);//red
			}
			else
			{//LASER
				color = vec3(0.0f, 0.0f, 0.5f);//dark blue
			}
		}
		else if((inventory & BOMB) != 0)
		{//BOMB
			color = vec3(0.5f, 0.9f, 0.0f);//yellow - green
		}
		else
		{//KNIFE
			color = vec3(0.0f, 1.0f, 0.0f);//green
		}
	}
	else
	{//REVERS
		color = vec3(1.0f, 0.0f, 1.0f);//purpure
	}

	fragment_color = vec4(color * (1.0f - radius / 5.0f) + radius / 5.0f, 1.0f); 
}