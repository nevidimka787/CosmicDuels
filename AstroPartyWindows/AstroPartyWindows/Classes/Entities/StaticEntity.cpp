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
	Vec2F position,
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

StaticEntity::StaticEntity(
	const Vec2F* position,
	float radius,
	float angle,
	bool exist) 
	:
	Entity(
		position,
		radius,
		angle,
		exist),
	last_position(*position)
{
}

Vec2F StaticEntity::GetVelocity() const
{
	return position - last_position;
}

void StaticEntity::Set(StaticEntity* static_entity)
{
	angle = static_entity->angle;
	direction = static_entity->direction;
	exist = static_entity->exist;
	last_position = static_entity->last_position;
	position = static_entity->position;
	radius = static_entity->radius;
}

void StaticEntity::Set(
	Vec2F position,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = position;
	this->radius = radius;
}

void StaticEntity::Set(
	const Vec2F* position,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	UpdateDirection();
	this->exist = exist;
	this->position = *position;
	this->radius = radius;
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

void StaticEntity::operator=(StaticEntity static_entity)
{
	angle = static_entity.angle;
	direction = static_entity.direction;
	exist = static_entity.exist;
	last_position = static_entity.last_position;
	position = static_entity.position;
	radius = static_entity.radius;
}

StaticEntity::~StaticEntity()
{
}