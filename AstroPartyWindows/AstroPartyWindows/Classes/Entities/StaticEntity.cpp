#include "../Entity.h"



StaticEntity::StaticEntity() :
	Entity()
{
}

StaticEntity::StaticEntity(const StaticEntity& static_entity) :
	Entity(static_entity),
	last_position(static_entity.last_position)
{
}

StaticEntity::StaticEntity(
	const Vec2F& position,
	float radius,
	float angle,
	bool exist) 
	:
	Entity(
		position,
		radius,
		angle,
		exist),
	last_position(position)
{
}

Vec2F StaticEntity::GetVelocity() const
{
	return position - last_position;
}

void StaticEntity::Set(const StaticEntity* static_entity)
{
	Entity::Set(static_entity);
	last_position = static_entity->last_position;
}

void StaticEntity::Set(
	const Vec2F& position,
	float radius,
	float angle,
	bool exist)
{
	Entity::Set(position, radius, angle, exist);
}

void StaticEntity::SetPosition(Vec2F position)
{
	this->position = position;
	last_position = position;
}

void StaticEntity::SetPosition(const Vec2F* position)
{
	this->position = *position;
	last_position = *position;
}

void StaticEntity::Update()
{
	last_position = position;
}

void StaticEntity::operator=(const StaticEntity& static_entity)
{
	Entity::operator=(static_entity);
	last_position = static_entity.last_position;
}

StaticEntity::~StaticEntity()
{
}