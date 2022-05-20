#version 330 core

out vec4 frag_color;

uniform vec3 color;

in vec2 pixel_position;

#define DELTA	0.01f

void main()
{
	if(pixel_position.x > pixel_position.y - DELTA && pixel_position.x < pixel_position.y + DELTA)
	{
		discard;
	}
	frag_color = vec4(color, 0.2f);
}