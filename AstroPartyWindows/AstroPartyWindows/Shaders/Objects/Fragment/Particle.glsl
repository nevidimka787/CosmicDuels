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
	case BACKGROUND:
		Background();
		break;
	case PORTAL_IN:
		PortalIn();
		break;
	case PORTAL_OUT:
		PortalOut();
		break;
	default:
		frag_color = vec4(float(type / 3) / 3.0f, float(type % 3) / 3.0f, 0.0f, 1.0f);
		break;
	}
}

// The function generate random number that vary from 0 to 1
float Rand(float _value)
{
	return fract((sin(6438593.3489 * _value) + 0.234) * 3297.2352);
}

// The function generate random number that vary from 0 to 1
float RandF(vec2 _value)
{
	return fract(sin(dot(_value, vec2(45.4622, 78.90342))) * 70053.2345);
}

// The function generate random vector. Values of components of the vector vary from 0 to 1.
vec2 Rand(vec2 _value)
{
	return fract(vec2(sin(dot(_value, vec2(98.4544, 12.1298))), sin(dot(_value, vec2(23.23222, 79.7543)))) * 73146.3433);
}

// The function generate random vector. Values of components of the vector vary from 0 to 1.
vec3 Rand(vec3 _value)
{
	return fract(vec3(sin(dot(_value.xy, vec2(98.4544, 12.1298))) * 43983.3453, sin(dot(_value.yz, vec2(23.23222, 79.7543))) * 37462.457, sin(dot(_value.xz, vec2(30.4833, 87.2433))) * 28733.534));
}

void Background()
{
#define STARS_COUNT	5
#define stun angle
#define SIZE		1.0f
#define STAR_ALPHA	1.0f

#define STAR_MIN_RADIUS		0.005f
#define START_DELTA_RADIUS	0.01f
#define STAR_MIN_POSITION	0.04f
#define STAR_DELTA_POSITION	0.92f
#define STAR_ENDGE_POWER	0.4f

#define LAST_LAYER_RADIUS	100.0f
#define BACKGROUND_VAWE_LENGTH	5.0f
#define NOIZE_VAWE_ANPLITUDE	0.2f

	vec2 _star_pos;
	float _star_r;
	for(int _star = 0; _star < STARS_COUNT; _star++)
	{
		// Generate random vector by star_number and area of pixels position (by trunc).
		_star_pos = Rand(
			vec2((_star + 1) * (trunc(pixel_position / SIZE) + 1.0f)))
			* STAR_DELTA_POSITION + STAR_MIN_POSITION; // Edges of positio is 0.04 and 0.96.
		_star_r = RandF(_star_pos) * START_DELTA_RADIUS + STAR_MIN_RADIUS; // Generate star's radious that vary from 0.02 to 0.035.
		// Star temperature vary from 0 to 1.
		float _temp = Rand(_star_r);

		// Position of pixel relatively the star.
		vec2 _local_position = fract(pixel_position / SIZE) - _star_pos;

		float _dist_to_star_edge = (_star_r - length(_local_position)) / _star_r;

		if (_dist_to_star_edge > 0) // the pixel inside star chec
		{
			if(radius < LAST_LAYER_RADIUS)
			{
				frag_color = vec4(
					ColorOfTemperature(_temp), // main color
					(1.0f - stun) * STAR_ALPHA * pow(_dist_to_star_edge, STAR_ENDGE_POWER)); // alpha chanel
				return;
			}
			frag_color = vec4(
				mix (color, ColorOfTemperature(_temp), STAR_ALPHA * pow(_dist_to_star_edge, STAR_ENDGE_POWER)), //last layer is calculated by mixing
				1.0f - stun); // and has alpha chanel vary by stun only.
			return;
		}
	}
	if(radius < LAST_LAYER_RADIUS)
	{
		discard;
	}
	
	frag_color = vec4(
		mix(
			color,
			vec3(0.0f),
			(sin((pixel_position.x + pixel_position.y) + sin(pixel_position.x - pixel_position.y) * NOIZE_VAWE_ANPLITUDE) / BACKGROUND_VAWE_LENGTH) * 0.5f + 0.5f),
		1.0f - stun);
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

vec3 ColorOfTemperature(float _temperature)
{
	return vec3(1.0f - _temperature * 0.3f, 0.7f + 0.15f, 0.7f + _temperature * 0.3f) / min (1.0f - _temperature * 0.3f, 1.0f + _temperature * 0.3f);
}
