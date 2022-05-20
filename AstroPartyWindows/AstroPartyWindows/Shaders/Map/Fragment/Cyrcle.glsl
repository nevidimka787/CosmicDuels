#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
uniform float size;
uniform int properties;
float radius;

#define BORDER			0.02f

#define BASIC_COLOR			vec4(0.6f, 0.1f, 0.2f, 1.0f)
#define UBBREAKABLE_COLOR	vec4(0.4f, 0.4f, 0.1f, 1.0f)
#define KILLER_COLLOR		vec4(0.0f, 0.5f, 0.1f, 1.0f)
#define AGGRESSIVE_COLOR	vec4(0.6f, 0.1f, 0.8f, 1.0f)

#define UNBREAKABLE		0x01
#define KILLER			0x02
#define AGGRESSIVE		0x04

void main()
{
	if((radius = length(pixel_position)) < (1.0f - BORDER / size) || radius > 1.0f)
	{
		discard;
	}

	if((properties & (UNBREAKABLE | KILLER | AGGRESSIVE)) == UNBREAKABLE)
	{
		frag_color = BASIC_COLOR;
		return;
	}
	else
	{
		frag_color = vec4(0.0f);
	}
	
	if((properties & UNBREAKABLE) == 0)
	{
		frag_color += UBBREAKABLE_COLOR;
	}
	if((properties & KILLER) != 0)
	{
		frag_color += KILLER_COLLOR;
	}
	if((properties & AGGRESSIVE) != 0)
	{
		frag_color += AGGRESSIVE_COLOR;
	}

	return;
}