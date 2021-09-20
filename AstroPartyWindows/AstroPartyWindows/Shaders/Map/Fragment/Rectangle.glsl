#version 330 core

out vec4 frag_color;

in vec2 v_pos;
in vec2 size;

#define BORDER	0.04f

void main()
{
	if(abs(v_pos.x) < (1.0f - BORDER / size.x) && abs(v_pos.y) < (1.0f - BORDER / size.y))
	{
		discard;
	}

	frag_color = vec4(vec3(0.1f), 1.0f);
}