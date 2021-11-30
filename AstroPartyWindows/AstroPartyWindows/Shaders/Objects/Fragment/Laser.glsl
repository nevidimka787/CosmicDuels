 #version 330 core

out vec4 frag_color;

uniform float life;	//from 1 to 0
uniform int power;	//laser can create loops

in float pixel_position;

#define POWERED_COLOR_CENTER	vec4(1.0f, 0.3f, 0.2f, 0.9f)
#define COLOR_CENTER			vec4(0.8f, 0.4f, 1.0f, 0.9f)
#define COLOR_BORD				vec4(0.1f, 0.1f, 0.5f, 0.2f)

void main()
{
	if(power == 0)
	{
		frag_color = (abs(pixel_position) * (1.0f - life)) * COLOR_BORD + (1.0f - abs(pixel_position) * (1.0f - life)) * COLOR_CENTER;
	}
	else
	{
		frag_color = (abs(pixel_position) * (1.0f - life)) * COLOR_BORD + (1.0f - abs(pixel_position) * (1.0f - life)) * POWERED_COLOR_CENTER;
	}
}