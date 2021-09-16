#version 330 core

out vec4 frag_color;

uniform float life;

vec3 color_start = vec3(0.5f, 0.5f, 1.0f);
vec3 color_last = vec3(0.0f, 0.0f, 0.5f);

void main()
{
	frag_color = vec4(color_start * life + color_last * (1.0f - life), 1.0f);
}