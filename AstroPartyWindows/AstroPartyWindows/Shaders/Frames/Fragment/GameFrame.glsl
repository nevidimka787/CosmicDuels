#version 330 core

in vec2 pixel_position;

uniform int txtrs_count;
uniform sampler2D txtr_0;
uniform sampler2D txtr_1;
uniform sampler2D txtr_2;
uniform sampler2D txtr_3;

out vec4 frag_color;

vec2 texel_position = (pixel_position + 1.0f) / 2.0f;

void main()
{
	if(txtrs_count <= 1)
	{
		frag_color = texture(txtr_0, texel_position);
	}

}