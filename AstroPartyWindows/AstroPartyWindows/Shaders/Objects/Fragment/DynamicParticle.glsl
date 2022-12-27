#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform int type;
uniform float animation;

uniform float angle;
uniform vec2 camera_position;
uniform float camera_size;
uniform float radius;
uniform float scale;
uniform vec3 color;

void Background();
void ExaustEngine();
void ExaustShoot();
void ExaustBombBoom();
void ExaustBurnous();
void PortalIn();
void PortalOut();
void ShardsAsteroid();
void ShardsAsteroidPowered();
void ShardsMapElement();
void ShardsPilot();
void ShardsShip();

bool PositionInCyrcle(vec2 _position, float _radius);
bool PositionInQuare(vec2 _position, float _quare_side);
bool PositionInTriangle(vec2 _position, float _triangle_side, float _triangle_angle);

float ElipsLength(vec2 pos1, vec2 pos2, vec2 scale);
vec3 ColorOfTemperature(float _temperature);

#define NULL					0
#define BACKGROUND				1
#define EXAUST_ENGINE			2
#define EXAUST_BURNOUT			3
#define EXAUST_BOMB_BOOM		4
#define EXAUST_SHOOT			5
#define PORTAL_IN				6
#define PORTAL_OUT				7
#define SHARDS_ASTEROID			8
#define SHARDS_ASTEROID_POWERED	9
#define SHARDS_MAP_ELEMENT		10
#define SHARDS_PILOT			11
#define SHARDS_SHIP				12

void main()
{
	switch(type)
	{
	case EXAUST_ENGINE:
		ExaustEngine();
		break;
	case EXAUST_SHOOT:
		ExaustShoot();
		break;
	case EXAUST_BOMB_BOOM:
		ExaustBombBoom();
		break;
	case EXAUST_BURNOUT:
		ExaustBurnous();
		break;
	case SHARDS_ASTEROID:
		ShardsAsteroid();
		break;
	case SHARDS_ASTEROID_POWERED:
		ShardsAsteroidPowered();
		break;
	case SHARDS_MAP_ELEMENT:
		ShardsPilot();
		break;
	case SHARDS_PILOT:
		ShardsPilot();
		break;
	case SHARDS_SHIP:
		ShardsShip();
		break;
	default:
		frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	}
}

bool PositionInCyrcle(vec2 _position, float _radius)
{
	return length(_position) < _radius;
}

bool PositionInQuare(vec2 _position, float _quare_side)
{
	return 
		_position.x > -_quare_side &&
		_position.y > -_quare_side &&
		_position.x < _quare_side &&
		_position.y < _quare_side;
}

void ExaustEngine()
{
#define POS0 (vec2(0.0f,		0.79f) * 2.0f - 1.0f)
#define POS1 (vec2(1.0f / 3.0f, 0.84f) * 2.0f - 1.0f)
#define POS2 (vec2(2.0f / 3.0f, 0.74f) * 2.0f - 1.0f)

#define POS3 (vec2(0.0f,		0.21f) * 2.0f - 1.0f)
#define POS4 (vec2(2.0f / 3.0f, 0.26f) * 2.0f - 1.0f)
#define POS5 (vec2(1.0f / 3.0f, 0.16f) * 2.0f - 1.0f)
#define RADIUS	(0.4f - animation * 0.4f)

	float disance = min(
		min(min(ElipsLength(pixel_position, POS0 * (1.0f - animation), vec2(0.5f, 1.0f)),
				ElipsLength(pixel_position, POS1 * (1.0f - animation), vec2(0.5f, 1.0f))),
		min(	ElipsLength(pixel_position, POS2 * (1.0f - animation), vec2(0.5f, 1.0f)),
				ElipsLength(pixel_position, POS3 * (1.0f - animation), vec2(0.5f, 1.0f)))),
		min(	ElipsLength(pixel_position, POS4 * (1.0f - animation), vec2(0.5f, 1.0f)),
				ElipsLength(pixel_position, POS5 * (1.0f - animation), vec2(0.5f, 1.0f))));
	if(
		disance > RADIUS)
	{
		discard;
	}

	float power = 1.0f / max(disance, 1.0f);

	frag_color = mix(vec4(mix(color, vec3(1.0f), 1.0f - disance / RADIUS), 1.0f - disance / RADIUS), vec4(color, 0.0f), animation);
	//frag_color = vec4(color, 1.1f * (1.0f - pow(animation, 4.0f)));
}

void ExaustShoot()
{
	frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ExaustBombBoom()
{
	frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ExaustBurnous()
{
	frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ShardsAsteroid()
{
	if(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

void ShardsAsteroidPowered()
{
	if(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

void ShardsMapElement()
{
	if(
		(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f) && 
		(length(pixel_position) < 0.7f || length(pixel_position) > 0.8f) &&
		(length(pixel_position) < 0.5f || length(pixel_position) > 0.6f))
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

void ShardsPilot()
{
	if(
		(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f) && 
		(length(pixel_position) < 0.7f || length(pixel_position) > 0.8f))
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

void ShardsShip()
{
	if(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

float ElipsLength(vec2 pos1, vec2 pos2, vec2 scale)
{
	return sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) * scale.x + (pos1.y - pos2.y) * (pos1.y - pos2.y) * scale.y);
}


