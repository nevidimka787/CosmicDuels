#include "../Entity.h"



Particle::Particle() :
	Entity(),
	type(0),
	animation_period(PARTICLE_PROPERTY_AUTO),
	animation_postpone(PARTICLE_PROPERTY_AUTO),
	finish_tic(PARTICLE_PROPERTY_AUTO),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(0)
{
}

Particle::Particle(
	GameTypes::tic_t current_tic,
	const Entity* pointer_to_host,
	EngineTypes::Particle::type_t type,
	Color3F color,
	GameTypes::tic_t animation_period,
	GameTypes::tic_t animation_postpone,
	GameTypes::tic_t finish_tic,
	bool exist) :
	Entity(*pointer_to_host),
	color(color),
	finish_tic(finish_tic),
	linked(true),
	pointer_to_entity(pointer_to_host),
	spawn_tic(current_tic),
	type(type)
{
	if (animation_period == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPeriod(type);
	}
	else
	{
		this->animation_period = animation_period;
	}

	if (animation_postpone == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPostpone(type);
	}
	else
	{
		this->animation_postpone = animation_postpone;
	}
}

Particle::Particle(
	GameTypes::tic_t current_tic,
	const Entity* pointer_to_host,
	EngineTypes::Particle::type_t type,
	const Color3F* color,
	GameTypes::tic_t animation_period, 
	GameTypes::tic_t animation_postpone, 
	GameTypes::tic_t finish_tic,
	bool exist) :
	Entity(*pointer_to_host),
	color(*color),
	finish_tic(finish_tic),
	linked(true),
	pointer_to_entity(pointer_to_host),
	spawn_tic(current_tic),
	type(type)
{
	if (animation_period == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPeriod(type);
	}
	else
	{
		this->animation_period = animation_period;
	}

	if (animation_postpone == PARTICLE_PROPERTY_AUTO)
	{
		SetAutoPostpone(type);
	}
	else
	{
		this->animation_postpone = animation_postpone;
	}
}

Particle::Particle(
	GameTypes::tic_t current_tic,
	Vec2F position,
	float angle,
	float radius,
	EngineTypes::Particle::type_t type,
	Color3F color,
	GameTypes::tic_t animation_period,
	GameTypes::tic_t animation_postpone,
	GameTypes::tic_t finish_tic,
	bool exist) 
	:
	Entity(
		position, 
		radius,
		angle, 
		exist),
	type(type),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(color),
	finish_tic(finish_tic),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(current_tic)
{
}

Particle::Particle(
	GameTypes::tic_t current_tic, 
	const Vec2F* position,
	float angle,
	float radius,
	EngineTypes::Particle::type_t type,
	const Color3F* color,
	GameTypes::tic_t animation_period,
	GameTypes::tic_t animation_postpone, 
	GameTypes::tic_t finish_tic, 
	bool exist) 
	:
	Entity(
		position, 
		radius, 
		angle, 
		exist),
	type(type),
	animation_period(animation_period),
	animation_postpone(animation_postpone),
	color(*color),
	finish_tic(finish_tic),
	linked(false),
	pointer_to_entity(nullptr),
	spawn_tic(current_tic)
{
}

void Particle::Activate(GameTypes::tic_t current_tic)
{
	if (!active && current_tic == spawn_tic + animation_postpone)
	{
		active = true;
	}
}

bool Particle::CanRemove(GameTypes::tic_t current_tic) const
{
	if (linked)
	{
		return !pointer_to_entity->exist || finish_tic != PARTICLE_ANIMATION_NOT_FINISH && current_tic == finish_tic;
	}
	return finish_tic != PARTICLE_ANIMATION_NOT_FINISH && current_tic == finish_tic;
}

EngineTypes::Particle::type_t Particle::GetType() const
{
	return type;
}

bool Particle::IsActive() const
{
	return active;
}

void Particle::Link(Entity* new_pointer)
{
	pointer_to_entity = new_pointer;
	linked = true;
}

void Particle::SetAutoPeriod(EngineTypes::Particle::type_t type)
{
	switch (type)
	{
	case PARTICLE_TYPE_BACKGROUND:
		this->animation_period = PARTICLE_PERIOD_BACKGROUND;
		break;
	case PARTICLE_TYPE_EXAUST_BOMB_BOOM:
		this->animation_period = PARTICLE_PERIOD_EXAUST_BOMB_BOOM;
		break;
	case PARTICLE_TYPE_EXAUST_BURNOUT:
		this->animation_period = PARTICLE_PERIOD_EXAUST_BURNOUT;
		break;
	case PARTICLE_TYPE_EXAUST_ENGINE:
		this->animation_period = PARTICLE_PERIOD_EXAUST_ENGINE;
		break;
	case PARTICLE_TYPE_EXAUST_SHOOT:
		this->animation_period = PARTICLE_PERIOD_EXAUST_SHOOT;
		break;
	case PARTICLE_TYPE_PORTAL_IN:
		this->animation_period = PARTICLE_PERIOD_PORTAL_IN;
		break;
	case PARTICLE_TYPE_PORTAL_OUT:
		this->animation_period = PARTICLE_PERIOD_PORTAL_OUT;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID:
		this->animation_period = PARTICLE_PERIOD_SHARDS_ASTEROID;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID_POWERED:
		this->animation_period = PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED;
		break;
	case PARTICLE_TYPE_SHARDS_MAP_ELEMENT:
		this->animation_period = PARTICLE_PERIOD_SHARDS_MAP_ELEMENT;
		break;
	case PARTICLE_TYPE_SHARDS_PILOT:
		this->animation_period = PARTICLE_PERIOD_SHARDS_PILOT;
		break;
	case PARTICLE_TYPE_SHARDS_SHIP:
		this->animation_period = PARTICLE_PERIOD_SHARDS_SHIP;
		break;
	default:
		std::cout << "ERROR::PARTICLE::SET_AUTO_PERIOD::Animation period seted failed. Type of the patricle undeclarates." << std::endl;
		exist = false;
		return;
	}
}

void Particle::SetAutoPostpone(EngineTypes::Particle::type_t type)
{
	switch (type)
	{
	case PARTICLE_TYPE_BACKGROUND:
		this->animation_postpone = PARTICLE_POSTPONE_BACKGROUND;
		break;
	case PARTICLE_TYPE_EXAUST_BOMB_BOOM:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_BOMB_BOOM;
		break;
	case PARTICLE_TYPE_EXAUST_BURNOUT:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_BURNOUT;
		break;
	case PARTICLE_TYPE_EXAUST_ENGINE:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_ENGINE;
		break;
	case PARTICLE_TYPE_EXAUST_SHOOT:
		this->animation_postpone = PARTICLE_POSTPONE_EXAUST_SHOOT;
		break;
	case PARTICLE_TYPE_PORTAL_IN:
		this->animation_postpone = PARTICLE_POSTPONE_PORTAL_IN;
		break;
	case PARTICLE_TYPE_PORTAL_OUT:
		this->animation_postpone = PARTICLE_POSTPONE_PORTAL_OUT;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_ASTEROID;
		break;
	case PARTICLE_TYPE_SHARDS_ASTEROID_POWERED:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_ASTEROID_POWERED;
		break;
	case PARTICLE_TYPE_SHARDS_MAP_ELEMENT:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_MAP_ELEMENT;
		break;
	case PARTICLE_TYPE_SHARDS_PILOT:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_PILOT;
		break;
	case PARTICLE_TYPE_SHARDS_SHIP:
		this->animation_postpone = PARTICLE_POSTPONE_SHARDS_SHIP;
		break;
	default:
		std::cout << "ERROR::PARTICLE::SET_AUTO_POSTPONE::Animation postond seted failed. Type of the patricle undeclarates." << std::endl;
		exist = false;
		return;
	}
}

void Particle::Unlink()
{
	linked = false;
	pointer_to_entity = nullptr;
}

void Particle::Update(GameTypes::tic_t current_tic)
{
	if (pointer_to_entity != nullptr && pointer_to_entity->exist)
	{
		position = pointer_to_entity->GetPosition();
		angle = pointer_to_entity->GetAngle();
		radius = pointer_to_entity->radius;
	}

	if (active)
	{
		animation = (float)((current_tic - spawn_tic - animation_postpone) % animation_period) / (float)animation_period;
	}
}

void Particle::operator=(Particle particle)
{
	active = particle.active;
	angle = particle.angle;
	animation = particle.animation;
	animation_period = particle.animation_period;
	animation_postpone = particle.animation_postpone;
	color = particle.color;
	exist = particle.exist;
	finish_tic = particle.finish_tic;
	linked = particle.linked;
	pointer_to_entity = particle.pointer_to_entity;
	position = particle.position;
	radius = particle.radius;
	spawn_tic = particle.spawn_tic;
	type = particle.type;
}

Particle::~Particle()
{
}