#include "../Entity.h"

Portal::Portal()
	:
	StaticEntity(),
	tp_mode(0),
	tp_position(Vec2F()),
	tp_position_pointer(nullptr)
{
}

Portal::Portal(const Portal& portal)
	:
	StaticEntity(
		portal.position,
		portal.radius,
		portal.angle,
		portal.exist),
	tp_mode(portal.tp_mode),
	tp_position(portal.tp_position),
	tp_position_pointer(portal.tp_position_pointer)
{
}

Portal::Portal(
	const Vec2F& position,
	const Vec2F& tp_position,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(
		position,
		radius,
		angle,
		exist),
	tp_mode(PORTAL_MODE_POSITION),
	tp_position(tp_position),
	tp_position_pointer(nullptr)
{
}

Portal::Portal(
	const Vec2F& position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode(PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(tp_position_pointer)
{
}

Portal::Portal(
	const Vec2F& position,
	const Entity* entity,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(position, radius, angle, exist),
	tp_mode((EngineTypes::Portal::mode_t)PORTAL_MODE_POINTER),
	tp_position(Vec2F()),
	tp_position_pointer(entity->GetPositionPointer())
{
}

void Portal::Connect(const Vec2F* pointer)
{
	tp_position_pointer = pointer;
}

void Portal::Connect(const Entity* entity)
{
	tp_position_pointer = entity->GetPositionPointer();
}

Particle Portal::CreateParticles(GameTypes::tic_t current_tic) const
{
	return Particle(
		current_tic,
		position,
		angle,
		radius,
		PARTICLE_TYPE_PORTAL_IN,
		Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f),
		PARTICLE_PERIOD_PORTAL_IN,
		PARTICLE_POSTPONE_PORTAL_IN,
		current_tic + PARTICLE_POSTPONE_PORTAL_IN + PARTICLE_PERIOD_PORTAL_IN);
}

Particle Portal::CreateParticlesTP(GameTypes::tic_t current_tic, float radius) const
{
	if (tp_mode == PORTAL_MODE_POSITION)
	{
		return Particle(
			current_tic,
			tp_position,
			angle,
			radius,
			PARTICLE_TYPE_PORTAL_OUT,
			Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f) * 2.0f,
			PARTICLE_PERIOD_PORTAL_OUT,
			PARTICLE_POSTPONE_PORTAL_OUT,
			current_tic + PARTICLE_POSTPONE_PORTAL_OUT + PARTICLE_PERIOD_PORTAL_OUT);
	}
	if (tp_mode == PORTAL_MODE_POINTER)
	{
		return Particle(
			current_tic,
			*tp_position_pointer,
			angle,
			radius,
			PARTICLE_TYPE_PORTAL_OUT,
			Color3F(31.0f / 256.0f, 14.0f / 256.0f, 32.0f / 256.0f),
			PARTICLE_PERIOD_PORTAL_OUT,
			PARTICLE_POSTPONE_PORTAL_OUT,
			current_tic + PARTICLE_POSTPONE_PORTAL_OUT + PARTICLE_PERIOD_PORTAL_OUT);
	}
	return Particle();
}

void Portal::Disconnect()
{
	tp_position_pointer = nullptr;
}

bool Portal::IsConnected() const
{
	return
		tp_mode == PORTAL_MODE_POSITION ||
		tp_mode == PORTAL_MODE_POINTER && tp_position_pointer != nullptr;
}

void Portal::Set(
	const Vec2F& position,
	const Vec2F* tp_position_pointer,
	float radius,
	float angle,
	bool exist)
{
	StaticEntity::Set(position, radius, angle, exist);

	this->tp_position_pointer = tp_position_pointer;
}

void Portal::SetPosition(const Vec2F& position)
{
	this->last_position = this->position;
	this->position = position;
}

void Portal::SetTPPosition(const Vec2F& position)
{
	this->tp_position = position;
}

void Portal::SetMode(EngineTypes::Portal::mode_t mode)
{
	if (mode == PORTAL_MODE_POINTER && tp_position_pointer != nullptr)
	{
		tp_mode = PORTAL_MODE_POINTER;
		return;
	}
	if (mode == PORTAL_MODE_POSITION)
	{
		tp_mode = PORTAL_MODE_POSITION;
		return;
	}
}

template <typename EntityType>
void Portal::Teleport(EntityType& entity) const
{
	if (tp_mode == PORTAL_MODE_POINTER)
	{
		entity.SetPosition(*tp_position_pointer);
	}
	else if (tp_mode == PORTAL_MODE_POSITION)
	{
		entity.SetPosition(tp_position);
	}
	else
	{
		std::cout << "tp_mode: " << (unsigned)tp_mode << std::endl;
		return;
	}

	entity.Rotate(angle);
	entity.SetVelocity(entity.GetVelocity().RotateClockwise(angle));
	entity.ClearForce();
}
template void Portal::Teleport<Asteroid>(Asteroid& asteroid) const;
template void Portal::Teleport<Bomb>(Bomb& asteroid) const;
template void Portal::Teleport<Bonus>(Bonus& asteroid) const;
template void Portal::Teleport<Bullet>(Bullet& asteroid) const;
template void Portal::Teleport<DynamicParticle>(DynamicParticle& asteroid) const;
template void Portal::Teleport<Pilot>(Pilot& asteroid) const;
template void Portal::Teleport<Ship>(Ship& asteroid) const;

void Portal::operator=(const Portal& portal)
{
	StaticEntity::operator=(portal);
	tp_mode = portal.tp_mode;
	tp_position = portal.tp_position;
	tp_position_pointer = portal.tp_position_pointer;
}

Portal::~Portal()
{
}