#version 330 core

out vec4 frag_color;

in vec2 v_pos;
in vec2 size;

void main()
{
	if(v_pos.x > 0.02f / size.x && v_pos.y > 0.02f/ size.y && v_pos.x < 1.0f - 0.02f / size.x && v_pos.y < 1.0f - 0.02f / size.y)
	{
		discard;
	}
	else
	{
		frag_color = vec4(vec3(0.1f), 1.0f);
	}
}