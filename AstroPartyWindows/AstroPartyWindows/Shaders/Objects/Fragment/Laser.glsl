 #version 330 core

out vec4 frag_color;

uniform float life;	//from 1 to 0
uniform int properties;	//laser can create loops or/and bombs

#define CAN_CREATE_LOOP	0x01
#define CAN_CREATE_BOMB	0x02

in float pixel_position;

#define BOMB_COLOR_CENTER		vec4(0.3f, 0.4f, 0.4f, 0.8f)
#define LOOP_COLOR_CENTER		vec4(1.0f, 0.3f, 0.2f, 0.9f)
#define COLOR_CENTER			vec4(0.8f, 0.4f, 1.0f, 0.9f)

#define COLOR_BORD				vec4(0.1f, 0.1f, 0.5f, 0.2f)

vec4 color_center = COLOR_CENTER;
int colors_count = 1;

void main()
{

	if((properties & CAN_CREATE_BOMB) == CAN_CREATE_BOMB)
	{
		colors_count++;
		color_center = (float(colors_count - 1) * color_center + BOMB_COLOR_CENTER) / float(colors_count);
	}

	if((properties & CAN_CREATE_LOOP) == CAN_CREATE_LOOP)
	{
		colors_count++;
		color_center = (float(colors_count - 1) * color_center + LOOP_COLOR_CENTER) / float(colors_count);
	}

	
	frag_color = (abs(pixel_position) * (1.0f - life)) * COLOR_BORD + (1.0f - abs(pixel_position) * (1.0f - life)) * color_center;
}