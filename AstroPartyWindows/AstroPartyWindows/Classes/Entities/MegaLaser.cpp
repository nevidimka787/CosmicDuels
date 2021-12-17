#include "../Entity.h"



MegaLaser::MegaLaser() :
	AggressiveEntity(),
	active(false),
	width(0.0f)
{
}

MegaLaser::MegaLaser(const MegaLaser& mega_laser) :
	AggressiveEntity(mega_laser),
	active(mega_laser.active),
	width(mega_laser.width)
{
}

MegaLaser::MegaLaser(
	const Segment* segment,
	float width,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	bool active,
	bool exist) 
	:
	AggressiveEntity(
		&segment->point,
		segment->vector.Length(),
		segment->vector.AbsoluteAngle(),
		attack_dellay,
		attack_period,
		inactive_period,
		shoots_count,
		exist),
	active(active),
	width(width)
{
}

Segment MegaLaser::GetSegment() const
{
	return Segment(position, direction * radius);
}

bool MegaLaser::IsCollision(const Beam* beam) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);
	Line perpendicular = Line(
		beam->point,
		beam->vector.Perpendicular());
	Vec2F intersect;

	return
		main.Intersection(&perpendicular, &intersect) && intersect.Distance(beam->point) < width ||
		start.IsIntersection(beam) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(beam);
}

bool MegaLaser::IsCollision(const Line* line) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);

	return
		Line(line->point, line->vector.Perpendicular()).IsIntersection(&main) ||
		start.IsIntersection(line) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(line);
}

bool MegaLaser::IsCollision(const Segment* segment) const
{
	Segment main = Segment(
		position,
		direction * radius);
	Segment start = Segment(
		position + direction.Perpendicular() * width,
		direction.PerpendicularClockwise() * width * 2.0f);
	Line perpendicular = Line(
		segment->point,
		segment->vector.Perpendicular());
	Vec2F intersect;

	if (main.Intersection(&perpendicular, &intersect) && intersect.Distance(segment->point) < width)
	{
		return true;
	}

	perpendicular.point += segment->vector;

	return
		main.Intersection(&perpendicular, &intersect) && intersect.Distance(segment->SecondPoint()) < width ||
		start.IsIntersection(segment) ||
		Segment(start.point + main.vector, start.vector).IsIntersection(segment);
}

void MegaLaser::Rotate(float angle)
{
	direction.RotateThis(angle);
	this->angle += angle;
}

void MegaLaser::Set(const MegaLaser* mega_laser)
{
	active = mega_laser->active;
	angle = mega_laser->angle;
	attack_dellay = mega_laser->attack_dellay;
	attack_period = mega_laser->attack_period;
	direction = mega_laser->direction;
	exist = mega_laser->exist;
	inactive_period = mega_laser->inactive_period;
	last_position = mega_laser->last_position;
	position = mega_laser->position;
	radius = mega_laser->radius;
	shoots_count = mega_laser->shoots_count;
}

void MegaLaser::Set(
	const Segment* segment,
	float width,
	GameTypes::tic_t attack_dellay,
	GameTypes::tic_t attack_period,
	GameTypes::tic_t inactive_period,
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count,
	bool active,
	bool exist)
{
	this->active = active;
	this->attack_dellay = attack_dellay;
	this->attack_period = attack_period;
	this->direction = segment->vector.Normalize();
	UpdateAngle();
	this->exist = exist;
	this->inactive_period = inactive_period;
	this->position = segment->point;
	this->radius = segment->vector.Length();
	this->shoots_count = shoots_count;
	this->width = width;

	direction *= radius;
}

bool MegaLaser::IsShooting(GameTypes::tic_t current_tic) const
{
	if (current_tic < attack_dellay) return false;
	return ((current_tic - attack_dellay) % attack_period) > inactive_period;
}

void MegaLaser::operator=(MegaLaser mega_laser)
{
	active = mega_laser.active;
	angle = mega_laser.angle;
	attack_dellay = mega_laser.attack_dellay;
	attack_period = mega_laser.attack_period;
	direction = mega_laser.direction;
	exist = mega_laser.exist;
	inactive_period = mega_laser.inactive_period;
	last_position = mega_laser.last_position;
	position = mega_laser.position;
	radius = mega_laser.radius;
	shoots_count = mega_laser.shoots_count;
	width = mega_laser.width;
}

MegaLaser::~MegaLaser()
{
}