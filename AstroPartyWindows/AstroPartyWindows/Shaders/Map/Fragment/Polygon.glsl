#version 330 core

out vec4 frag_color;

uniform int properties;

#define BASIC_COLOR			vec4(0.6f, 0.1f, 0.2f, 1.0f)
#define UBBREAKABLE_COLOR	vec4(0.4f, 0.4f, 0.1f, 1.0f)
#define KILLER_COLLOR		vec4(0.0f, 0.5f, 0.1f, 1.0f)
#define AGGRESSIVE_COLOR	vec4(0.6f, 0.1f, 0.8f, 1.0f)

#define UNBREAKABLE		0x01
#define KILLER			0x02
#define AGGRESSIVE		0x04

void main()
{
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