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

float Wave(float animation);

void main()
{

	if(status >= STATUS_BOOM)
	{
		if((radius = length(pixel_position)) > 1.0f)
		{
			discard;
		}
		frag_color = vec4(vec3(Wave(animation)), 1.0f - animation / 2.0f);
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

float Wave(float animation)
{
	return 1.0f;
}