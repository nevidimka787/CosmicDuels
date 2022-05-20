#include "../Entity.h"



DynamicParticle::DynamicParticle() :
	DynamicEntity(),
	active(false),
	animation(0.0f),
	animation_period(0),
	animation_postpone(0),
	finish_tic(0),
	properties(DYNAMIC_PARTICLE_PROPERTY_NULL),
	spawn_tic(0),
	type(0)
{
}

DynamicParticle::DynamicParticle(
	GameTypes::tic_t current_tic,
	Vec2F position,
	Vec2F velocisy,
	float radius,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient,
	EngineTypes::Particle::type_t type,
	EngineTypes::DynamicParticle::property_t properties,
	Color3F color,
	GameTypes::tic_t animation_period,
	GameTypes::tic_t animation_postpone,
	GameTypes::tic_t finish_tic,
	bool exist) 
	:
	DynamicEntity(
		position,
		velocisy,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient, 
		force_resistance_air_coefficient,
		exist),
	active(false),
	animation(0.0f),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(color),
	finish_tic(finish_tic),
	properties(properties),
	spawn_tic(current_tic),
	type(type)
{
}

DynamicParticle::DynamicParticle(
	GameTypes::tic_t current_tic,
	const Vec2F* position, 
	const Vec2F* velocisy, 
	float radius, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	EngineTypes::Particle::type_t type,
	EngineTypes::DynamicParticle::property_t properties,
	const Color3F* color,
	GameTypes::tic_t animation_period,
	GameTypes::tic_t animation_postpone,
	GameTypes::tic_t finish_tic,
	bool exist) 
	:
	DynamicEntity(
		position,
		velocisy, 
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	active(false),
	animation(0.0f),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(*color),
	finish_tic(finish_tic),
	properties(properties),
	spawn_tic(current_tic),
	type(type)
{
}

#pragma warning (disable : 6282)
bool DynamicParticle::Activate(GameTypes::tic_t current_tic)
{
	return (!active && current_tic == spawn_tic + animation_postpone) ? (active = true) : false;
}

bool DynamicParticle::CanRemove(GameTypes::tic_t current_tic) const
{
	return current_tic == finish_tic;
}

EngineTypes::Particle::type_t DynamicParticle::GetType() const
{
	return type;
}

bool DynamicParticle::IsActive() const
{
	return active;
}

void DynamicParticle::Update(GameTypes::tic_t current_tic)
{
	angle += angular_velocity;
	if (angle > (float)M_PI)
	{
		angle -= (float)M_PI * 2.0f;
	}
	else if (angle < -(float)M_PI)
	{
		angle += (float)M_PI * 2.0f;
	}
	UpdateDirection();

	velocity += force;
	if (properties & DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE)
	{
		velocity *= 1.0f - force_resistance_air_coefficient;
	}
	force.Set(0.0f, 0.0f);
	Move(&velocity);
	if (active)
	{
		animation = (float)((current_tic - spawn_tic - animation_postpone) % animation_period) / (float)animation_period;
	}
}

void DynamicParticle::operator=(DynamicParticle dynamical_entity)
{
	active = dynamical_entity.active;
	angle = dynamical_entity.angle;
	angular_velocity = dynamical_entity.angular_velocity;
	animation = dynamical_entity.animation;
	animation_period = dynamical_entity.animation_period;
	animation_postpone = dynamical_entity.animation_postpone;
	color = dynamical_entity.color;
	UpdateDirection();
	exist = dynamical_entity.exist;
	finish_tic = dynamical_entity.finish_tic;
	force = dynamical_entity.force;
	force_collision_coeffisient = dynamical_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = dynamical_entity.force_resistance_air_coefficient;
	position = dynamical_entity.position;
	properties = dynamical_entity.properties;
	radius = dynamical_entity.radius;
	spawn_tic = dynamical_entity.spawn_tic;
	type = dynamical_entity.type;
	velocity = dynamical_entity.velocity;
}

DynamicParticle::~DynamicParticle()
{
}