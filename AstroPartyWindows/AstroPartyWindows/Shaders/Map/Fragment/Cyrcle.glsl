#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
uniform float size;
float radius;
void main()
{
	if((radius = length(pixel_position)) < (1.0f - 0.02f / size) || radius > 1.0f)
	{
		discard;
	}
	frag_color = vec4(vec3(0.1f), 1.0f);
}