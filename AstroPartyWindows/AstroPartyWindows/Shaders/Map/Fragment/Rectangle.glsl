#version 330 core

out vec4 frag_color;

in vec2 v_pos;
in vec2 size;
uniform int properties;
uniform int tik; // increment 100 times per second

#define M_PI 3.14159265358979323846f

#define BORDER			  0.04f
#define GLOW_FACTOR       1.25f

#define BASIC_COLOR		 	vec4(0.3f, 0.3f, 0.3f, 1.0f)

#define AGGRESSIVE_COLOR	vec4(0.0f, 0.0f, 0.7f, 0.0f)
#define KILLER_COLOR		vec4(0.0f, 0.7f, 0.0f, 0.0f)
#define UNBREAKABLE_COLOR   vec4(0.7f, 0.0f, 0.0f, 0.0f)

#define UNBREAKABLE		0x01
#define KILLER			0x02
#define AGGRESSIVE		0x04

#define KILLER_GLOW_PERIOD 75
#define AGGRESSIVE_GLOW_PERIOD 100

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
	frag_color = BASIC_COLOR;

	vec4 effect_color = ((properties & UNBREAKABLE) == UNBREAKABLE) ? UNBREAKABLE_COLOR : BASIC_COLOR; // base effect color

	vec2 scale_pos = v_pos / size;
	float dist = abs(scale_pos.x) + abs(scale_pos.y);

	float killer_factor = 0.f;
	if((properties & KILLER) != 0)
	{
		float anim = sin(-2.f * M_PI / KILLER_GLOW_PERIOD * (tik % KILLER_GLOW_PERIOD) + dist * 0.5f) / 3.f + 0.5f;
		killer_factor = min(1.f, max(anim, 0.f));
		effect_color = KILLER_COLOR * killer_factor + effect_color * (1.f - killer_factor);
	}

	float aggressive_factor = 0.f;
	if((properties & AGGRESSIVE) != 0)
	{
		float anim = sin(-2.f * M_PI / AGGRESSIVE_GLOW_PERIOD * (tik % AGGRESSIVE_GLOW_PERIOD) + dist * 0.3f) / 3.f + 0.5f;
		aggressive_factor = min(1.f, max(anim, 0.f));
		effect_color = AGGRESSIVE_COLOR * aggressive_factor + effect_color * (1.f - aggressive_factor);
	}

	frag_color += effect_color;

	float glow_factor = ((properties & UNBREAKABLE) == UNBREAKABLE) ? (GLOW_FACTOR + killer_factor / 3.f + aggressive_factor / 3.f) : (GLOW_FACTOR / 2.f);
	float glow_x = (abs(v_pos.y * size.y) > (size.y + BORDER)) ? 0.f : max(1.f - abs(abs(v_pos.x * size.x) - (size.x)) / BORDER / glow_factor, 0.f);
	float glow_y = (abs(v_pos.x * size.x) > (size.x + BORDER)) ? 0.f : max(1.f - abs(abs(v_pos.y * size.y) - (size.y)) / BORDER / glow_factor, 0.f);
	float glow = glow_x + glow_y;

	frag_color.w *= glow;

	return;
}
