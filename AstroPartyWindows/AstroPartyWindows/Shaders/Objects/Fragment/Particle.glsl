#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

#define STARS_COUNT	6

uniform int type;
uniform float animation;

uniform float angle;
uniform vec2 camera_position;
uniform float camera_size;
uniform float radius;
uniform float scale;

void Background();
void ExaustEngine();
void ExaustShoot();
void ExaustBombBoom();
void ExaustBurnous();
void ShardsAsteroid();
void ShardsAsteroidPowered();
void ShardsMapElement();
void ShardsPilot();
void ShardsShip();

#define BACKGROUND					0x01
#define EXAUST_ENGINE				0x02
#define EXAUST_SHOOT				0x03
#define EXAUST_BOMB_BOOM			0x04
#define EXAUST_BURNOUT				0x05
#define SHARDS_ASTEROID				0x06
#define SHARDS_ASTEROID_POWERED		0x07
#define SHARDS_MAP_ELEMENT			0x08
#define SHARDS_PILOT				0x09
#define SHARDS_SHIP					0x0A

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
#define SIZE	1.0f
	vec2 _star_pos;
	float _star_r;
	for(int _star = 0; _star < STARS_COUNT; _star++)
	{
		_star_pos = Rand(vec2(_star * (trunc(pixel_position) + 1.0f))) * 0.92f + 0.04f;
		_star_r = RandF(_star_pos) * 0.02f + 0.01f;
		float _temp = Rand(_star_r);

		if(length(fract(pixel_position / SIZE) - _star_pos) < _star_r / SIZE)
		{
			frag_color = vec4(vec3(1.0f), 1.0f - angle);
			return;
		}
	}
	if(radius < 100.0f)
	{
		discard;
	}
	
	frag_color = vec4(0.03f, 0.12f, 0.35f, 1.0f - angle);

}

void ExaustEngine()
{
	
}

void ExaustShoot()
{
	
}

void ExaustBombBoom()
{
	
}

void ExaustBurnous()
{
	
}

void ShardsAsteroid()
{
	
}

void ShardsAsteroidPowered()
{
	
}

void ShardsMapElement()
{
	
}

void ShardsPilot()
{
	
}

void ShardsShip()
{
	
}


