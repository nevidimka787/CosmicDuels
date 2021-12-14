#version 330 core

out vec4 fragment_color;

in vec2 pixel_position;

uniform int inventory;

uniform sampler2D bomb_texture;
uniform sampler2D knife_texture;
uniform sampler2D laser_texture;
uniform sampler2D loop_texture;
uniform sampler2D revers_texture;
uniform sampler2D shield_texture;
uniform sampler2D stream_texture;
uniform sampler2D triple_texture;

#define BONUS	0x00FF //0000 0000 1111 1111
#define BUFF	0x3F00 //0011 1111 0000 0000
#define RULE	0xC000 //1100 0000 0000 0000

#define LOOP	0x0003 //0000 0000 0000 0011
#define LASER	0x000C //0000 0000 0000 1100
#define BOMB	0x0030 //0000 0000 0011 0000
#define KNIFE	0x00C0 //0000 0000 1100 0000

#define TRIPLE	0x0300 //0000 0011 0000 0000
#define SHIELD	0x0C00 //0000 1100 0000 0000
#define STREAM	0x3000 //0011 0000 0000 0000

#define REVERS	0xC000 //1100 0000 0000 0000

void main()
{
	vec2 texel_position = (pixel_position + 1.0f) / 2.0f;

	if((inventory & BONUS) != 0)
	{
		//LOOP | LASER | BOMB | KNIFE
		if((inventory & (LOOP | LASER)) != 0)
		{
			//LOOP | LASER
			if((inventory & LOOP) != 0)
			{
				//LOOP
				fragment_color = texture(loop_texture, texel_position);
				return;
			}
			//LASER
			fragment_color = texture(laser_texture, texel_position);
			return;
		}
		//BOMB | KNIFE
		if((inventory & BOMB) != 0)
		{
			//BOMB
			fragment_color = texture(bomb_texture, texel_position);
			return;
		}
		//KNIFE
		fragment_color = texture(knife_texture, texel_position);
		return;
	}
	//TRIPLE | SHIELD | STREAM | REVERS
	if((inventory & (TRIPLE | SHIELD)) != 0)
	{
		//TRIPLE | SHIELD
		if((inventory & TRIPLE) != 0)
		{
			//TRIPLE
			fragment_color = texture(triple_texture, texel_position);
			return;
		}
		//SHIELD
		fragment_color = texture(shield_texture, texel_position);
		return;
	}
	//STREAM | REVERS
	if((inventory & STREAM) != 0)
	{
		//STREAM
		fragment_color = texture(stream_texture, texel_position);
		return;
	}
	//REVERS
	fragment_color = texture(revers_texture, texel_position);
	return;
}