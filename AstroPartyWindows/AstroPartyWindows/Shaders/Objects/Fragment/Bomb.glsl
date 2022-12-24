#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform float animation;
uniform sampler2D basic_txtr;
uniform sampler2D lighting_txtr;
uniform int status;

#define BLINK_LIGH_PART	0.15f

#define FRAME_COLOR		vec4(0.5f, 0.5f, 0.5f, 1.0f)
#define BLINK_COLOR_ON	vec4(1.0f, 0.0f, 0.0f, 1.0f)

#define ALMOST		0.99f
#define ALMOST_NOT	(1.0f - ALMOST)

#define STATUS_ACTIVE		1
#define STATUS_BOOM			2

vec2 texel_position = (pixel_position + 1.0f) / 2.0f;
float radius;

vec4 WaveGen(float animation, float radius);
vec3 ColorOfTemperature(float _temperature);

void main()
{

	if(status >= STATUS_BOOM)
	{
		if((radius = length(pixel_position)) > 1.0f)
		{
			discard;
		}
		//frag_color = vec4(vec3(1.0f), 1.0f - animation / 2.0f);
		frag_color = WaveGen(animation, radius);
	}
	else if(status == STATUS_ACTIVE)
	{
		float light = texture(lighting_txtr, texel_position).x;
		frag_color = texture(basic_txtr, texel_position);
		frag_color = frag_color * (1.0f - light) + BLINK_COLOR_ON * light;
	}
	else
	{
		frag_color = texture(basic_txtr, texel_position);
		if(animation < BLINK_LIGH_PART)
		{
			float light = texture(lighting_txtr, texel_position).z;
			frag_color = frag_color * (1.0f - light) + BLINK_COLOR_ON * light;
		}
	}
}

vec4 WaveGen(float _animation, float _radius)
{
#define EDGE_POWER		0.2f
#define SENTER_POWER	1.0f
	float phase = cos((_radius - _animation) * radians(180.0f) * 2.0f);
	float amplitude = (SENTER_POWER - sqrt(_radius)) * (SENTER_POWER - EDGE_POWER) * (1.0f - _animation) * (1.0f - _animation);
	return vec4(mix(ColorOfTemperature(1.0f - animation), vec3(1.0f), radius), phase * phase * (SENTER_POWER - EDGE_POWER) * amplitude + EDGE_POWER);
}

vec3 ColorOfTemperature(float _temperature)
{
#define COLOR_BROWN vec3(0.64453125, 0.1640625, 0.1640625)
	return vec3(mix(COLOR_BROWN, vec3(1.0f, 1.0f, 1.0f), _temperature));
}