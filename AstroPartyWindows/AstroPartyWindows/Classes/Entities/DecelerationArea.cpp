#include "../Entity.h"



DecelerationArea::DecelerationArea() :
	StaticEntity(),
	deceleration_parameter(DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT)
{
}

DecelerationArea::DecelerationArea(const DecelerationArea& deceleration_area) :
	StaticEntity(deceleration_area),
	deceleration_parameter(deceleration_area.deceleration_parameter)
{
}

DecelerationArea::DecelerationArea(
	const Vec2F& position,
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
	StaticEntity::Set(deceleration_area);

	deceleration_parameter = deceleration_area->deceleration_parameter;
}

void DecelerationArea::Set(
	const Vec2F& position,
	float deceleration_parameter,
	float radius,
	float angle,
	bool exist)
{
	StaticEntity::Set(position, radius, angle, exist);

	this->deceleration_parameter = deceleration_parameter;
}

void DecelerationArea::operator=(const DecelerationArea& deceleration_area)
{
	StaticEntity::operator=(deceleration_area);

	deceleration_parameter = deceleration_area.deceleration_parameter;
}

DecelerationArea::~DecelerationArea()
{
}