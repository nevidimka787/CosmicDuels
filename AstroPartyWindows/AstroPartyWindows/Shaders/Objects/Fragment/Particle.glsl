#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

#define STARS_COUNT	5

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
	case BACKGROUND:
		Background();
		break;
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
	case PORTAL_IN:
		PortalIn();
		break;
	case PORTAL_OUT:
		PortalOut();
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

float Rand(float _value)
{
	return fract((sin(6438593.3489 * _value) + 0.234) * 3297.2352);
}

float RandF(vec2 _value)
{
	return fract(sin(dot(_value, vec2(45.4622, 78.90342))) * 70053.2345);
}

vec2 Rand(vec2 _value)
{
	return fract(vec2(sin(dot(_value, vec2(98.4544, 12.1298))), sin(dot(_value, vec2(23.23222, 79.7543)))) * 73146.3433);
}

vec3 Rand(vec3 _value)
{
	return fract(vec3(sin(dot(_value.xy, vec2(98.4544, 12.1298))) * 43983.3453, sin(dot(_value.yz, vec2(23.23222, 79.7543))) * 37462.457, sin(dot(_value.xz, vec2(30.4833, 87.2433))) * 28733.534));
}

void Background()
{
#define SIZE		1.0f
#define STAR_ALPHA	0.4f
	vec2 _star_pos;
	float _star_r;
	for(int _star = 0; _star < STARS_COUNT; _star++)
	{
		_star_pos = Rand(vec2((_star + 1) * (trunc(pixel_position) + 1.0f))) * 0.92f + 0.04f;
		_star_r = RandF(_star_pos) * 0.02f + 0.015f;
		float _temp = Rand(_star_r);

		if(PositionInTriangle(fract(pixel_position / SIZE) - _star_pos, _star_r / SIZE * sqrt(3.0f), Rand(_temp) * radians(60.0f)))
		{
			if(radius < 100.0f)
			{
				frag_color = vec4(
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f + 2.0f / 3.0f)), 1.0f), 0.0f),
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f)), 1.0f), 0.0f),
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f - 2.0f / 3.0f)), 1.0f), 0.0f),
					(1.0f - angle) * STAR_ALPHA);
				return;
			}
			frag_color = vec4(
				vec3(
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f + 2.0f / 3.0f)), 1.0f), 0.0f),
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f)), 1.0f), 0.0f),
					max(min(3.0f * (1.0f - abs((_temp * 2.0f - 1.0f) * 1.5f - 2.0f / 3.0f)), 1.0f), 0.0f)
					) * STAR_ALPHA + color * (1.0f - STAR_ALPHA), 1.0f - angle);
			return;
		}
	}
	if(radius < 100.0f)
	{
		discard;
	}
	
	frag_color = vec4(color, 1.0f - angle);

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

bool PositionInTriangle(vec2 _position, float _triangle_side, float _triangle_angle)
{
	vec2 _l_pos = _position / _triangle_side;
	_l_pos = vec2(
		_l_pos.x * cos(_triangle_angle) - _l_pos.y * sin(_triangle_angle),
		_l_pos.x * sin(_triangle_angle) + _l_pos.y * cos(_triangle_angle));
	return
		_l_pos.y < sqrt(3.0f) * _l_pos.x + 1.0f / sqrt(3.0f) &&
		_l_pos.y < -sqrt(3.0f) * _l_pos.x + 1.0f / sqrt(3.0f) &&
		_l_pos.y > -0.5f / sqrt(3.0f);
}

void ExaustEngine()
{
#define POS0 (vec2(0.0f,		0.79f) * 2.0f - 1.0f)
#define POS1 (vec2(1.0f / 3.0f, 0.84f) * 2.0f - 1.0f)
#define POS2 (vec2(2.0f / 3.0f, 0.74f) * 2.0f - 1.0f)

#define POS3 (vec2(0.0f,		0.21f) * 2.0f - 1.0f)
#define POS4 (vec2(2.0f / 3.0f, 0.26f) * 2.0f - 1.0f)
#define POS5 (vec2(1.0f / 3.0f, 0.16f) * 2.0f - 1.0f)
#define RADIUS	0.1f

	if(
		length(pixel_position - POS0) > RADIUS &&
		length(pixel_position - POS1) > RADIUS &&
		length(pixel_position - POS2) > RADIUS &&
		length(pixel_position - POS3) > RADIUS &&
		length(pixel_position - POS4) > RADIUS &&
		length(pixel_position - POS5) > RADIUS)
	{
		discard;
	}

	frag_color = vec4(color, 1.1f * (1.0f - pow(animation, 4.0f)));
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

void PortalIn()
{
	if(length(pixel_position) < 0.9f || length(pixel_position) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(color, 1.0f);
}

void PortalOut()
{
	#define WAVE_SPEED	9.8f
	#define WAVE_LENGHT	0.033f
	#define LN__0_5	-0.69314718f
	
	float x;
	if((x = length(pixel_position)) > 1.0f)
	{
		discard;
	}

	if(x < animation)
	{
		frag_color = vec4(color,
			(sin(WAVE_SPEED * animation * 2.0f * radians(180.0f) - radians(90.0f) - x / WAVE_LENGHT) * 
			(exp(x * LN__0_5) - 0.5f) - 0.5f + exp(x * LN__0_5)) *
			(1.0f - animation));
		return;
	}

	discard;
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


