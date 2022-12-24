#version 330 core

out vec4 frag_color;

uniform int player;
uniform int team;
uniform int inventory;
uniform int bullets_count;
uniform int magazine_size;

uniform sampler2D txtr;

uniform int type;
#define TYPE_SHIP   0
#define TYPE_BULLET 1

in vec2 pixel_position;
in vec2 texel_position;

vec3 color;
vec3 pos_neg_alpha;

#define RED		1
#define GREEN	2
#define BLUE	3
#define PURPURE	4

#define PLAUER_COLOR	0.35f
#define TEAM_COLOR		0.65f

#define UNBRAKABLE	0xF000
#define SHIELD		0x0F00

vec4 DrawBullets(vec2 _pixel_position, int _inventory, int _bullets_count);
vec4 ColorSpread(vec4 color, float dist);

void main()
{
	pos_neg_alpha = texture(txtr, texel_position).xzw;

	if(type == TYPE_BULLET)
	{
		frag_color = DrawBullets(pixel_position, inventory, bullets_count);
		if(frag_color.w < 0.0f)
		{
			discard;
		}

		return;
	}
	if((player & SHIELD) != 0x0000)
	{
		frag_color = vec4(0.5f, 0.5f, 1.0f, 0.4f * pos_neg_alpha.z);
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
		frag_color = vec4((color * pos_neg_alpha.x + (1.0f - color) * pos_neg_alpha.y) * 0.9f + 0.1f, pos_neg_alpha.z * 0.4f);
		return;
	}
	
	frag_color = vec4((color * pos_neg_alpha.x + (1.0f - color) * pos_neg_alpha.y) * 0.9f + 0.1f, pos_neg_alpha.z);
}



vec4 DrawBullets(vec2 _pixel_position, int _inventory, int _bullets_count)
{
#define MAX_RADIUS		1.0f
#define MIN_RADIUS		0.80f
#define DELTA_RADIUS	(MAX_RADIUS - MIN_RADIUS)
#define ORBIT_RADIUS	(MIN_RADIUS + DELTA_RADIUS / 2.0f)

	if(length(_pixel_position) > MAX_RADIUS || length(_pixel_position) < MIN_RADIUS)
	{
		return vec4(-1.0f);
	}

	bool has_loop  = (inventory & 1) != 0;
	bool has_laser = (inventory & 2) != 0;
	bool has_bomb  = (inventory & 4) != 0;
	bool has_knife = (inventory & 8) != 0;

#define LOOP	0
#define LASER	1
#define BOMB	2
#define KNIFE	3

#define MAX_BULLETS_COUNT 10

#define BULLET_COLOR			vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define FULL_BULLETS_COLOR		vec4(0.0f, 1.0f, 1.0f, 1.0f)

#define LOOP_COLOR	vec4(1.0f, 0.1f, 0.3f, 1.0f)
#define LASER_COLOR	vec4(0.3f, 0.1f, 1.0f, 1.0f)
#define BOMB_COLOR	vec4(0.5f, 0.5f, 0.5f, 1.0f)
#define KNIFE_COLOR	vec4(0.1f, 1.0f, 0.1f, 1.0f)

	int bullets_on_orbit = int(has_loop) + int(has_laser) + int(has_bomb) + int(has_knife);
	
	if(bullets_on_orbit == 0)
	{
		for(int vertex = 0; vertex < _bullets_count; vertex++)
		{
			float angle = radians(float(vertex) / float(bullets_count) * 360.0f);
			vec2 r_vec = vec2(-sin(angle), cos(angle)) * ORBIT_RADIUS;

			float dist = length(r_vec - _pixel_position);

			if(dist < DELTA_RADIUS / 2.0f)
			{
				dist /= DELTA_RADIUS / 2.0f;
				if(_bullets_count < magazine_size)
				{
					return ColorSpread(BULLET_COLOR, dist);
				}
				return ColorSpread(FULL_BULLETS_COLOR, dist);
			}
		}

		return vec4(-1.0f);
	}


	for(int vertex = 0; vertex < bullets_on_orbit; vertex++)
	{
		float angle = radians(float(vertex) / float(bullets_on_orbit) * 360.0f);
		vec2 r_vec = vec2(-sin(angle), cos(angle)) * ORBIT_RADIUS;

		float dist = length(r_vec - _pixel_position);

		if(dist < DELTA_RADIUS / 2.0f)
		{
			dist /= DELTA_RADIUS / 2.0f;
			if(has_loop == true)
			{
				if(vertex <= 0)
				{
					return ColorSpread(LOOP_COLOR, dist);
				}
				vertex--;
			}
			if(has_laser == true)
			{
				if(vertex <= 0)
				{
					return ColorSpread(LASER_COLOR, dist);
				}
				vertex--;
			}
			if(has_bomb == true)
			{
				if(vertex <= 0)
				{
					return vec4(mix(vec3(1.0f, 0.7f, 0.7f), BOMB_COLOR.xyz, 0.5f + dist / 2.0f), mix(BOMB_COLOR.w, 0.0f, dist * dist));
				}
				vertex--;
			}
			if(has_knife == true)
			{
				if(vertex <= 0)
				{
					return ColorSpread(KNIFE_COLOR, dist);
				}
			}

			return vec4(0.5f);
		}
	}

	return vec4(-1.0f);
}

vec4 ColorSpread(vec4 color, float dist)
{
	return vec4(mix(vec3(1.0f), color.xyz, 0.5f + dist / 2.0f), mix(color.w, 0.0f, dist * dist));
}