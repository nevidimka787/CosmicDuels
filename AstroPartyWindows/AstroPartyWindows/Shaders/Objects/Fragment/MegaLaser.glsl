#version 330 core

out vec4 frag_color;

uniform int is_active;

void main()
{
	if(is_active > 0)
	{
		frag_color = vec4(vec3(0.9f, 0.7f, 0.3f), 0.9f);
	}
	else
	{
		frag_color = vec4(0.1f, 0.1f, 0.8f, 0.2f);
	}
}