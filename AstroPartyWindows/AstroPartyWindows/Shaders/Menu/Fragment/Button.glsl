#version 330 core

out vec4 frag_color;

in vec2 pixel_position;
in vec2 position_on_text_area;
in vec2 wrap_size;

uniform vec2 position;//position
uniform vec2 size;//size

uniform vec3 color;

uniform int text_size;
uniform int text_length;
uniform float scale;

uniform int text[500];
uniform float corners[256];
uniform int corners_count;

uniform sampler2D text_texture;

vec2 curent_texture_position;

bool IsPixelInTheBox()
{
	bool inside = false;
    int i, j = corners_count - 1;
	vec2 point = pixel_position;

    for (i = 0; i < corners_count; j = i++) {
		vec2 polygon_i = vec2(corners[i * 2], corners[i * 2 + 1]);
		vec2 polygon_j = vec2(corners[j * 2], corners[j * 2 + 1]);

        if (((polygon_i.y > point.y) != (polygon_j.y > point.y)) &&
            point.x < 
			(polygon_j.x - polygon_i.x) * (point.y - polygon_i.y) / 
			(polygon_j.y - polygon_i.y) + polygon_i.x)
		{
            inside = !inside;
        }
    }
    return inside;
}

vec2 GetCellPosition(int symbol)
{
	return vec2(
		(symbol % 16) / 16.0f,
		(symbol / 16) / 16.0f);
}

bool TexturePositionInRightCell(vec2 position, int cell_id)
{
	return 
		position.x > (cell_id % 16) / 16.0f &&
		position.x < (cell_id % 16 + 1) / 16.0f &&
		position.y > (cell_id / 16) / 16.0f &&
		position.y < (cell_id / 16 + 1) / 16.0f;
}

void main()
{
	if (!IsPixelInTheBox()) discard;

	bool white = false; 

	frag_color = vec4(length(pixel_position * size), 0.0f, 0.0f, 1.0f);

	if (
		position_on_text_area.x < 0.0 ||
		position_on_text_area.y < 0.0 ||
		position_on_text_area.x > text_length ||
		position_on_text_area.y > 1.0)
	{
		frag_color = vec4(color, 1.0);
		return;
	}

	frag_color = vec4(color * 0.7, 1.0);

	for(int letter = 0; letter < text_length; letter++)
	{
		int letter_number = int(position_on_text_area.x);
		if (letter_number > letter) continue;

		vec2 position_on_texture = position_on_text_area - vec2(letter_number, 0.0); // trunck position to [0;1]
		position_on_texture /= 16.0f; // scale position to [1; 1/16] because texture has 16 by 16 symbols
		position_on_texture += GetCellPosition(text[letter]); // select cell at the texture
		if(
			texture(text_texture, position_on_texture).z > 0.9f &&
			TexturePositionInRightCell(position_on_texture, text[letter]))
		{
			frag_color = vec4(0.95f, 0.95f, 0.95f, 1.0f);
		}
		return;
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