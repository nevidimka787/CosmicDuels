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
	Vec2F position,
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

GravGen::GravGen(
	const Vec2F* position,
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
	Vec2F position,
	float gravity, 
	float radius, 
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->gravity = gravity;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
}

void GravGen::Set(
	const Vec2F* position,
	float gravity, 
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->gravity = gravity;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
}

void GravGen::operator=(GravGen grav_gen)
{
	angle = grav_gen.angle;
	direction = grav_gen.direction;
	exist = grav_gen.exist;
	gravity = grav_gen.gravity;
	last_position = grav_gen.last_position;
	position = grav_gen.position;
	radius = grav_gen.radius;
}

GravGen::~GravGen()
{
}