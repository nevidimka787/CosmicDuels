#version 330 core

out vec4 frag_color;

in vec2 v_pos;
in vec2 size;
uniform int properties;
uniform int tik; // increment 100 times per second

#define BORDER			  0.04f
#define GLOW_FACTOR       1.f

#define BASIC_COLOR		 vec4(0.3f, 0.3f, 0.3f, 1.0f)

#define AGGRESSIVE_COLOR	vec4(0.0f, 0.0f, 0.5f, 0.0f)
#define KILLER_COLOR		vec4(0.0f, 0.5f, 0.0f, 0.0f)
#define UNBREAKABLE_COLOR   vec4(0.5f, 0.0f, 0.0f, 0.0f)

#define UNBREAKABLE		0x01
#define KILLER			0x02
#define AGGRESSIVE		0x04

/**
 * @param pos[IN|VAR]    position of the pixel
 * @param anim[IN|VAR]   animation value [0;1]. 0 - start, 1 - end
 * @param V[IN|PARAM]    volume of the particle. Impact total brightness.
 * @param pos0[IN|PARAM] start position of the particle
 * @param vel[IN|PARAM]  velocity of the particle.
 *
 * @return brightness at the position `pos` and the time `anim`
 */
float particle_wave(vec2 pos, float anim, float V, vec2 pos0, vec2 vel);

void main()
{
	float glow = 1;
	float glow_x = (abs(v_pos.y * size.y) > (size.y + BORDER)) ? 0.f : max(1.f - abs(abs(v_pos.x * size.x) - (size.x)) / BORDER / GLOW_FACTOR, 0.f);
	float glow_y = (abs(v_pos.x * size.x) > (size.x + BORDER)) ? 0.f : max(1.f - abs(abs(v_pos.y * size.y) - (size.y)) / BORDER / GLOW_FACTOR, 0.f);
	glow = glow_x + glow_y;

	frag_color = BASIC_COLOR;

	if ((properties & (UNBREAKABLE | KILLER | AGGRESSIVE)) == 0)
	{
		frag_color += vec4(0.3f, 0.3f, 0.3f, 0.0f) / 2.0f;
	}

	if((properties & KILLER) != 0)
	{
		frag_color += KILLER_COLOR;
	}

	if((properties & AGGRESSIVE) != 0)
	{
		frag_color += AGGRESSIVE_COLOR;
	}

	if((properties & UNBREAKABLE) != 0)
	{
		frag_color += UNBREAKABLE_COLOR;
	}

	frag_color.w *= glow;

	return;
}
