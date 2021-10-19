#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform int type;
uniform float animation;

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

void Background()
{
	vec2 _position = vec2(animation * 0.01f, 0.0f);
	float _radius = 0.001f;
	if(length(_position - pixel_position) > _radius)
	{
		discard;
	}
	frag_color = vec4(1.0f);
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


