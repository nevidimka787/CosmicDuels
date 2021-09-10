#version 330 core

out vec4 frag_color;

uniform float life;

void main()
{
	frag_color = vec4(0.6f, 0.1f, life * 0.2f, 1.0f);
}