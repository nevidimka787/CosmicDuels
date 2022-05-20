#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
uniform int size;

uniform int inventory;
uniform sampler2D small_txtr;	//texture for small asteroid
uniform sampler2D medium_txtr;	//texture for medium asteroid
uniform sampler2D large_txtr;	//texture for large asteroid
uniform sampler2D sblmtn_txtr;	//texture that store information about colored pixels

//							vec4(0.4f, 0.4f, 0.4f, 1.0f); //last color
#define BASIC_COLOR			vec4(1.0f, 1.0f, 1.0f, 1.0f);
//							vec4(0.7f, 0.2f, 0.0f, 1.0f); //last color
#define POWEED_COLOR		vec4(7.0f / 4.0f, 0.5f, 0.0f, 1.0f)
//							vec4(0.0f, 0.9f, 0.9f, 1.0f); //last color
#define SUPER_POWERED_COLOR	vec4(0.0f, 7.0f / 4.0f, 7.0f / 4.0f, 1.0f)

#define SIZE_SMALL	0
#define SIZE_MEDIUM	1
#define SIZE_LARGE	2

vec2 texel_position = (pixel_position + 1.0f) / 2.0f;

void main()
{
	switch(size)
	{
	case SIZE_SMALL:
		frag_color = texture(small_txtr, texel_position);
		texel_position = (texel_position - 0.5f) / 4.0f + 0.5f;

		break;
	case SIZE_MEDIUM:
		frag_color = texture(medium_txtr, texel_position);
		texel_position = (texel_position - 0.5f) / 2.0f + 0.5f;
		break;
	case SIZE_LARGE:
		frag_color = texture(large_txtr, texel_position);
	default:
		break;
	}

	if((inventory & 0x3F00) != 0)//has any buff
	{
		if(texture(sblmtn_txtr, texel_position).z > 0.5f)//is blue
		{
			frag_color *= SUPER_POWERED_COLOR;
		}
	}
	if((inventory & 0xC0FF) != 0)//has any bonus
	{
		if(texture(sblmtn_txtr, texel_position).x > 0.5f)//is red
		{
			frag_color *= POWEED_COLOR;
		}
	}

	frag_color *= BASIC_COLOR;
	return;
}