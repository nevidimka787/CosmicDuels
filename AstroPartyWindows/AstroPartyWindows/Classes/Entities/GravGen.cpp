#include "../Entity.h"



GravGen::GravGen() :
	gravity(GRAVITY_GENERATOR_DEFAULT_GRAVITY)
{
}

GravGen::GravGen(const GravGen& grav_gen) :
	StaticEntity(grav_gen),
	gravity(grav_gen.gravity)
{
}

GravGen::GravGen(
	const Vec2F& position,
	float gravity,
	float radius,
	float angle,
	bool exist) 
	:
	StaticEntity(
		position, 
		radius,
		angle,
		exist),
	gravity(gravity)
{
}

void GravGen::Set(const GravGen* grav_gen)
{
	angle = grav_gen->angle;
	direction = grav_gen->direction;
	exist = grav_gen->exist;
	gravity = grav_gen->gravity;
	last_position = grav_gen->last_position;
	position = grav_gen->position;
}

void GravGen::Set(
	const Vec2F& position,
	float gravity, 
	float radius, 
	float angle,
	bool exist)
{
	StaticEntity::Set(position, radius, angle, exist);

	this->gravity = gravity;
}

void GravGen::operator=(const GravGen& grav_gen)
{
	StaticEntity::operator=(grav_gen);

	gravity = grav_gen.gravity;
}

GravGen::~GravGen()
{
}