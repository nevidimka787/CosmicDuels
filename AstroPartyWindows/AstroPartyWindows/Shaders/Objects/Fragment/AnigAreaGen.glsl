#version 330 core

out vec4 frag_color;

uniform int shield;
#define SHIELD_COLOR vec4(0.5f, 0.5f, 1.0f, 0.4f)

void main()
{	
	if(shield != 0)
	{
		frag_color = SHIELD_COLOR;
		return;
	}
	frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	return;
}