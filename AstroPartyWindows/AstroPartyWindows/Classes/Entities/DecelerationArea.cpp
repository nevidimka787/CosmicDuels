#include "../Entity.h"



DecelerationArea::DecelerationArea() :
	StaticEntity(),
	deceleration_parameter(DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT)
{
}

DecelerationArea::DecelerationArea(const DecelerationArea& deceleration_area) :
	StaticEntity(deceleration_area),
	deceleration_parameter(deceleration_area.deceleration_parameter)
{
}

DecelerationArea::DecelerationArea(
	Vec2F position,
	float deceleration_parameter,
	float radius,
	float angle,
	bool exist) 
	:
	StaticEntity(
		position,
		radius,
		angle,
		exist),
	deceleration_parameter(deceleration_parameter)
{
}

DecelerationArea::DecelerationArea(
	const Vec2F* position,
	float deceleration_parameter,
	float radius,
	float angle,
	bool exist) 
	:
	StaticEntity(
		position,
		radius,
		angle,
		exist),
	deceleration_parameter(deceleration_parameter)
{
}

void DecelerationArea::Set(const DecelerationArea* deceleration_area)
{
	angle = deceleration_area->angle;
	deceleration_parameter = deceleration_area->deceleration_parameter;
	exist = deceleration_area->exist;
	last_position = deceleration_area->last_position;
	position = deceleration_area->position;
	radius = deceleration_area->radius;
}

void DecelerationArea::Set(
	Vec2F position,
	float deceleration_parameter,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	this->deceleration_parameter = deceleration_parameter;
	this->exist = exist;
	this->last_position = last_position;
	this->position = position;
	this->radius = radius;
}

void DecelerationArea::Set(
	const Vec2F* position,
	float deceleration_parameter,
	float radius,
	float angle,
	bool exist)
{
	this->angle = angle;
	this->deceleration_parameter = deceleration_parameter;
	this->exist = exist;
	this->last_position = last_position;
	this->position = *position;
	this->radius = radius;
}

void DecelerationArea::operator=(DecelerationArea deceleration_area)
{
	angle = deceleration_area.angle;
	deceleration_parameter = deceleration_area.deceleration_parameter;
	direction = deceleration_area.direction;
	exist = deceleration_area.exist;
	last_position = deceleration_area.last_position;
	position = deceleration_area.position;
	radius = deceleration_area.radius;
}

DecelerationArea::~DecelerationArea()
{
}