#version 330 core

out vec4 frag_color;

in vec2 v_pos;
in vec2 size;
uniform int properties;

#define BORDER				0.04f
#define UBBREAKABLE_COLOR	vec4(0.4f, 0.4f, 0.1f, 1.0f)
#define KILLER_COLLOR		vec4(0.0f, 0.5f, 0.1f, 1.0f)
#define AGGRESSIVE_COLOR	vec4(0.0f, 0.0f, 0.8f, 1.0f)

#define UNBREAKABLE		0x01
#define KILLER			0x02
#define AGGRESSIVE		0x04

#define PROPERTIES_COUNT_F	3.0f

void main()
{
	if(abs(v_pos.x) < (1.0f - BORDER / size.x) && abs(v_pos.y) < (1.0f - BORDER / size.y))
	{
		discard;
	}
	frag_color = vec4(vec3(0.1f), 1.0f);
	
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
}