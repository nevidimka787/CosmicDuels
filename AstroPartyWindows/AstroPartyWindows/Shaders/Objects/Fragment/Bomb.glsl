#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

uniform float animation;

float len;

void main()
{
	if((len = length(pixel_position)) > 1.0f)
	{
		discard;
	}
	frag_color = vec4(vec3(0.9f * (1.0f - animation), 0.9f * animation, 0.0f) * (1.0f - len) + 0.4f * len, 1.0f);
}