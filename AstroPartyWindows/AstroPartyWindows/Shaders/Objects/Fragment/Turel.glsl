#version 330 core

out vec4 frag_color;

in vec2 pixel_position;

void main()
{
	if(abs(pixel_position.y) < 0.2f && pixel_position.x > 0.0f)
	{
		frag_color = vec4(0.8f, 0.3f, 0.0f, 1.0f);
		return;
	}
	if(length(pixel_position) < 1.0f)
	{
		frag_color = vec4(vec3(0.5f), 1.0f);
		return;
	}
	discard;
}