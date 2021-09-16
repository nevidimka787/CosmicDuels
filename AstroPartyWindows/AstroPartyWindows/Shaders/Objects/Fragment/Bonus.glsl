#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform int inventory;

float radius;

vec3 color;

#define LOOP	0x03 //0000 0011
#define LASER	0x0C //0000 1100
#define BOMB	0x30 //0011 0000
#define KNIFE	0xC0 //1100 0000

void main()
{
	if((radius = length(pixel_position)) > 1.0f)
	{
		discard;
	}

	if((inventory & (LOOP | LASER)) != 0)
	{
		if((inventory & LOOP) != 0)
		{//LOOP
			color = vec3(0.8f, 0.2f, 0.0f);
		}
		else
		{//LASER
			color = vec3(0.0f, 0.0f, 0.5f);
		}
	}
	else if((inventory & BOMB) != 0)
	{//BOMB
		color = vec3(0.5f, 0.9f, 0.0f);
	}
	else
	{//KNIFE
		color = vec3(0.0f, 1.0f, 0.0f);
	}

	fragment_color = vec4(color * (1.0f - radius / 5.0f) + radius / 5.0f, 1.0f); 
}