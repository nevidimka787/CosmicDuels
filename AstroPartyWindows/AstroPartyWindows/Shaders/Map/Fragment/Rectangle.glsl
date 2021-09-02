#version 330 core

out vec4 frag_color;

in vec2 v_pos;

void main()
{
	if(v_pos.x > 0.01f && v_pos.y > 0.01f && v_pos.x < 0.99f && v_pos.y < 0.99f)
	{
		discard;
	}
	else
	{
		frag_color = vec4(vec3(0.1f), 1.0f);
	}
}