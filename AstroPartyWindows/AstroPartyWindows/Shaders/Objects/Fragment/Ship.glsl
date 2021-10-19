#version 330 core

out vec4 frag_color;

uniform int player;
uniform int team;

in vec2 pixel_position;

vec3 color;

#define RED		1
#define GREEN	2
#define BLUE	3
#define PURPURE	4

#define PLAUER_COLOR	0.3f
#define TEAM_COLOR		0.7f

#define UNBRAKABLE	0xF000
#define SHIELD		0x0F00

void main()
{
	if(length(pixel_position) > 0.6f)
	{
		discard;
	}
	if((player & SHIELD) != 0x0000)
	{
		frag_color = vec4(0.5f, 0.5f, 1.0f, 0.6f);
		return;
	}

	switch(team)
	{
	case RED:
		color = vec3(TEAM_COLOR, 0.0f, 0.0f);
		break;
	case GREEN:
		color = vec3(0.0f, TEAM_COLOR, 0.0f);
		break;
	case BLUE:
		color = vec3(0.0f, 0.0f, TEAM_COLOR);
		break;
	case PURPURE:
		color = vec3(TEAM_COLOR, 0.0f, TEAM_COLOR);
		break;
	default:
		color = vec3(0.0f, 0.0f, 0.0f);
		break;
	}
	switch (player & 0x00FF)
	{
	case 0:
		color += vec3(PLAUER_COLOR, 0.0f, 0.0f);
		break;
	case 1:
		color += vec3(0.0f, PLAUER_COLOR, 0.0f);
		break;
	case 2:
		color += vec3(0.0f, 0.0f, PLAUER_COLOR);
		break;
	case 3:
		color += vec3(PLAUER_COLOR, 0.0f, PLAUER_COLOR);
		break;
	default:
		color = vec3(1.0f);
		break;
	}
	if((player & UNBRAKABLE) != 0x0000)
	{
		frag_color = vec4(color * 0.7 + 0.3, 0.7f);
		return;
	}
	frag_color = vec4(color, 1.0f);
}