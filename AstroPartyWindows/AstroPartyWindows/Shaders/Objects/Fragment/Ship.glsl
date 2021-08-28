#version 330 core

out vec4 frag_color;

uniform int player;
uniform int team;

vec3 color;

#define RED		1
#define GREEN	2
#define BLUE	3
#define PURPURE	4

void main()
{
	switch(team)
	{
	case RED:
		color = vec3(1.0f, 0.0f, 0.0f);
		break;
	case GREEN:
		color = vec3(0.0f, 1.0f, 0.0f);
		break;
	case BLUE:
		color = vec3(0.0f, 0.0f, 1.0f);
		break;
	case PURPURE:
		color = vec3(1.0f, 0.0f, 1.0f);
		break;
	default:
		color = vec3(0.0f, 0.0f, 0.0f);
		break;
	}
	switch (player)
	{
	case 0:
		color *= 0.9f;
		color += vec3(0.05f);
		break;
	case 1:
		color *= 0.85f;
		color += vec3(0.1f);
		break;
	case 2:
		color *= 0.8f;
		color += vec3(0.15f);
		break;
	case 3:
		color *= 0.75f;
		color += vec3(0.2f);
		break;
	default:
		break;
	}
	frag_color = vec4(color, 1.0f);
}