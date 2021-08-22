#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
in vec2 texture_position;

uniform vec2 position;//position
uniform vec2 size;//size

uniform vec3 color;

uniform int text_size;
uniform int text_length;

uniform int text[500];

uniform sampler2D text_texture;

vec2 curent_texture_position;

vec2 GetCellPosition(int cell_id, int index)
{
	return vec2(
		(cell_id % 16 - index) / 16.0f,
		(cell_id / 16) / 16.0f);
}

bool TexturePositionInNeedCell(vec2 position, int cell_id)
{
	return position.x > (cell_id % 16) / 16.0f &&
		position.x < (cell_id % 16 + 1) / 16.0f &&
		position.y > (cell_id / 16) / 16.0f &&
		position.y < (cell_id / 16 + 1) / 16.0f;
}

void main()
{
	bool white = false; 
	vec2 current_texture_position;
	if(pixel_position.x < 0.5f + text_size / 200.0f / abs(size.x) * text_length &&
	pixel_position.x > 0.5f - text_size / 200.0f / abs(size.x) * text_length &&
	pixel_position.y < 0.5f + text_size / 200.0f / abs(size.y) &&
	pixel_position.y > 0.5f - text_size / 200.0f / abs(size.y))
	{
		for(int letter = 0; letter < text_length; letter++)
		{
			current_texture_position = texture_position + GetCellPosition(text[letter], letter);
			if(texture(text_texture, current_texture_position).z > 0.9f &&
				TexturePositionInNeedCell(current_texture_position, text[letter]))
			{
				frag_color = vec4(1.0f);
				white = true;
			}
		}
		if(!white)
		{
			frag_color = vec4(color, 1.0f);
		}
	}
	else
	{
		frag_color = vec4(color, 1.0f);
	}
}

/*
SPASE = 32
0 = 48
A = 65	LAT
a = 97
À = 128	KIR
ð = 224
¨ = 240
*/