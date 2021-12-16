#version 330 core

out vec4 frag_color;

uniform int player;
uniform int team;
uniform float life;

uniform sampler2D txtr;

vec3 color;

#define RED		1
#define GREEN	2
#define BLUE	3
#define PURPURE	4

#define PLAUER_COLOR	0.35f
#define TEAM_COLOR		0.65f

vec3 pos_neg_alpha;

in vec2 pixel_position;
in vec2 texel_position;

void main()
{

	pos_neg_alpha = texture(txtr, texel_position).xzw;

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
	switch (player)
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

	frag_color = vec4((color * pos_neg_alpha.x + (1.0f - color) * pos_neg_alpha.y) * 0.9f + 0.1f, pos_neg_alpha.z);
}