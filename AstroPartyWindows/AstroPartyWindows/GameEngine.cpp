#include "GameEngine.h"
#include <math.h>

Entity::Entity() : position(new Vec2D()), radius(0.0), angle(0.0), direction(new Vec2D(1.0, 0.0))
{
}

Entity::Entity(Vec2D* position, double radius, double angle) : position(new Vec2D(*position)), radius(radius), angle(angle), direction(new Vec2D(cos(angle), -sin(angle)))
{
}

double Entity::GetAngle()
{
	return angle;
}

double Entity::GetDistance(Entity* entity)
{
	return fmax(entity->position->GetDistance(position) - entity->radius - radius, 0.0);
}

double Entity::GetDistance(Vec2D* point)
{
	return fmax(point->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(Line* line)
{
	return fmax(line->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(Beam* beam)
{
	return fmax(beam->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(Segment* segment)
{
	return fmax(segment->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(DynamicEntity* entity)
{
	return fmax(entity->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(StaticEntity* entity)
{
	return fmax(entity->GetDistance(position) - radius, 0.0);
}

double Entity::GetDistance(Rectangle* rectangle)
{
	Segment temp = rectangle->GetUpSide();
	double dist1 = GetDistance(&temp);
	if (dist1 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetDownSide();
	double dist2 = GetDistance(&temp);
	if (dist2 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetLeftSide();
	double dist3 = GetDistance(&temp);
	if (dist3 == 0.0)
	{
		return 0.0;
	}
	temp = rectangle->GetRightSide();
	double dist4 = GetDistance(&temp);
	if (dist4 == 0.0)
	{
		return 0.0;
	}
	if (dist1 < dist2)
	{
		if (dist3 < dist4)
		{
			if (dist1 < dist3)
			{
				return dist1;
			}
			return dist3;
		}
		if (dist1 < dist4)
		{
			return dist1;
		}
		return dist4;
	}
	if (dist3 < dist4)
	{
		if (dist2 < dist3)
		{
			return dist2;
		}
		return dist3;
	}
	if (dist2 < dist4)
	{
		return dist2;
	}
	return dist4;
}

double Entity::GetDistance(Cyrcle* cyrcle)
{
	Vec2D temp = cyrcle->GetPosition();
	return GetDistance(&temp) - cyrcle->GetRadius();
}

double Entity::GetDistance(Polygon* polygon)
{
	return 0.0;
}

Vec2D Entity::GetDirection()
{
	return *direction;
}

Vec2D Entity::GetPosition()
{
	return *position;
}

bool Entity::IsCollision(Vec2D* point)
{
	return GetDistance(point) < 0.0;
}

bool Entity::IsCollision(Line* line)
{
	return line->GetDistance(position) < 0.0;
}

bool Entity::IsCollision(Beam* beam)
{
	return beam->GetDistance(position) < 0.0;
}

bool Entity::IsCollision(Segment* segment)
{
	return segment->GetDistance(position) < 0.0;
}

bool Entity::IsCollision(DynamicEntity* entity)
{
	return GetDistance(entity->position) < 0.0;
}

bool Entity::IsCollision(StaticEntity* entity)
{
	Vec2D temp = entity->GetPosition();
	return GetDistance(&temp) < 0.0;
}

bool Entity::IsCollision(Rectangle* rectangle)
{
	Segment temp;
	if (rectangle->collision_sides & RECTANGLE_UP_SIDE)
	{
		temp = rectangle->GetUpSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_DOWN_SIDE)
	{
		temp = rectangle->GetDownSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_RIGHT_SIDE)
	{
		temp = rectangle->GetRightSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	if (rectangle->collision_sides & RECTANGLE_LEFT_SIDE)
	{
		temp = rectangle->GetLeftSide();
		if (IsCollision(&temp))
		{
			return true;
		}
	}
	return false;
}

bool Entity::IsCollision(Cyrcle* cyrcle)
{
	Vec2D temp = cyrcle->GetPosition();
	return GetDistance(&temp) < radius + cyrcle->GetRadius();
}

bool Entity::IsCollision(Polygon* polygon)
{
	return false;
}

void Entity::Rotate(double angle)
{
	this->angle += angle;
	this->direction->x = cos(angle);
	this->direction->y = -sin(angle);
}

void Entity::SetAngle(double angle)
{
	this->angle = angle;
}

void Entity::SetDirection(Vec2D* direction)
{
	*this->direction = direction->Normalize();
}

void Entity::SetPosition(Vec2D* position)
{
	*(this->position) = *position;
}

void Entity::Move(Vec2D* delta)
{
	*position += *delta;
}

Entity::~Entity()
{
	delete this->position;
}



DynamicEntity::DynamicEntity() : angular_velocity(0.0), velocity(new Vec2D()), force(new Vec2D()), FORCE_COLLISION_COEFFICIENT(DEFAULT_FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	Vec2D temp = Vec2D();
	SetPosition(&temp);
}

DynamicEntity::DynamicEntity(Vec2D* position, double angle, double radius) : angular_velocity(0.0), velocity(new Vec2D()), force(new Vec2D()), FORCE_COLLISION_COEFFICIENT(DEFAULT_FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	SetPosition(position);
}

DynamicEntity::DynamicEntity(Vec2D* position, Vec2D* velosity, double angle, double angular_velosity, double radius, double FORCE_COLLISION_COEFFICIENT, double FORCE_RESISTANSE_AIR_COEFFICIENT) : angular_velocity(angular_velosity), velocity(new Vec2D(*velosity)), force(new Vec2D()), FORCE_COLLISION_COEFFICIENT(FORCE_COLLISION_COEFFICIENT), FORCE_RESISTANSE_AIR_COEFFICIENT(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
	SetPosition(position);
}

void DynamicEntity::AddForce(Vec2D* force)
{
	*this->force += *force;
}

bool DynamicEntity::Collision(DynamicEntity* entity)
{
	if (GetDistance(entity) > radius + entity->radius)
	{
		return false;
	}
	Vec2D line = *position - *entity->position;
	*velocity -= line.Project(velocity);
	*entity->velocity -= line.Project(entity->velocity);

	return true;
}

bool DynamicEntity::Collision(StaticEntity* entity)
{
	if (GetDistance(entity) > radius + entity->radius)
	{
		return false;
	}
	*velocity -= (*position - entity->GetPosition()).Project(velocity);
	return true;
}

bool DynamicEntity::Collision(Rectangle* rectangle)
{
	Vec2D temp1 = rectangle->GetUpRightPoint();
	Vec2D temp2 = rectangle->GetUpLeftPoint();
	Segment up_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetDownRightPoint();
	temp2 = rectangle->GetDownLeftPoint();
	Segment down_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetUpRightPoint();
	temp2 = rectangle->GetDownRightPoint();
	Segment right_side = Segment(&temp1, &temp1, true);
	temp1 = rectangle->GetUpLeftPoint();
	temp2 = rectangle->GetDownLeftPoint();
	Segment left_side = Segment(&temp1, &temp1, true);

	if (up_side.GetDistance(position) < radius)
	{
		if(right_side.GetDistance(position) < radius)
		{
			Vec2D force_vec = *position - rectangle->GetUpRightPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			return true;
		}
		if (left_side.GetDistance(position) < radius)
		{
			Vec2D force_vec = *position - rectangle->GetUpLeftPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			return true;
		}
		*velocity += rectangle->GetVelocity() - Vec2D(0.0, 1.0).Project(velocity);
		*force += Vec2D(0.0, (radius - (position->y - up_side.point->y)) * FORCE_COLLISION_COEFFICIENT);
		return true;
	}
	if (down_side.GetDistance(position) < radius)
	{
		if (right_side.GetDistance(position) < radius)
		{
			Vec2D force_vec = *position - rectangle->GetDownRightPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			return true;
		}
		if (left_side.GetDistance(position) < radius)
		{
			Vec2D force_vec = *position - rectangle->GetDownLeftPoint();
			*velocity += rectangle->GetVelocity() - force_vec.Project(velocity);
			*force += force_vec * FORCE_COLLISION_COEFFICIENT;
			return true;
		}
		*velocity += rectangle->GetVelocity() - Vec2D(0.0, 1.0).Project(velocity);
		*force += Vec2D(0.0, (radius - (up_side.point->y - position->y)) * FORCE_COLLISION_COEFFICIENT);
		return true;
	}
	if (right_side.GetDistance(position) < radius)
	{
		*velocity += rectangle->GetVelocity() - Vec2D(1.0, 0.0).Project(velocity);
		*force += Vec2D((radius - (position->x - up_side.point->x)) * FORCE_COLLISION_COEFFICIENT, 0.0);
		return true;
	}
	if (left_side.GetDistance(position) < radius)
	{
		*velocity += rectangle->GetVelocity() - Vec2D(1.0, 0.0).Project(velocity);
		*force += Vec2D((radius - (up_side.point->x - position->x)) * FORCE_COLLISION_COEFFICIENT, 0.0);
		return true;
	}
	return false;
}

bool DynamicEntity::Collision(Cyrcle* cyrcle)
{
	Vec2D vec = cyrcle->GetPosition();
	if (GetDistance(&vec) > radius + cyrcle->GetRadius())
	{
		return false;
	}
	Vec2D force_vec = *position - cyrcle->GetPosition();
	*velocity -= force_vec.Project(velocity);
	*force += force_vec * FORCE_COLLISION_COEFFICIENT;

	return true;
}

bool DynamicEntity::Collision(Polygon* polygon)
{
	return false;
}

bool DynamicEntity::Collision(Map* map)
{

	for (uint8_t i = 0; i < map->rectangles_array_length; i++)
	{
		Rectangle temp = map->GetRectangle(i);
		Collision(&temp);
	}
	for (uint8_t i = 0; i < map->cyrcles_array_length; i++)
	{
		Cyrcle temp = map->GetCyrcle(i);
		Collision(&temp);
	}
	for (uint8_t i = 0; i < map->polygons_array_length; i++)
	{
		Polygon temp = map->GetPolygon(i);
		Collision(&temp);
	}
	return true;
}

double DynamicEntity::GetAngularVelocity()
{
	return angular_velocity;
}

Vec2D DynamicEntity::GetVelocity()
{
	return *velocity;
}

void DynamicEntity::Recalculate()
{
	angle += angular_velocity;

	*force -= *velocity * FORCE_RESISTANSE_AIR_COEFFICIENT;
	*velocity += *force;
	Move(velocity);
}

DynamicEntity::~DynamicEntity()
{
	delete velocity;
	delete force;
}



StaticEntity::StaticEntity() : last_position(new Vec2D()), position(new Vec2D())
{

}

StaticEntity::StaticEntity(Vec2D* position, double angle) : last_position(new Vec2D(*position)), position(new Vec2D(*position))
{
	SetPosition(position);
	SetAngle(angle);
}

Vec2D StaticEntity::GetVelosity()
{
	return *position - *last_position;
}

void StaticEntity::Recalculate()
{
	*last_position = *position;
}

StaticEntity::~StaticEntity()
{
	delete last_position;
}



Bonus::Bonus() : bonus_type(0)
{

}

Bonus::Bonus(uint16_t bonus_type, Vec2D* position, Vec2D* velocity) : bonus_type(bonus_type)
{
	*this->position = *position;
	*this->velocity = *velocity;
}

Bonus* Bonus::Division()
{
	uint16_t temp;
	for (uint8_t i = 0; i < 6; i++)
	{
		temp = 0x11 << (i << 1);
		if (bonus_type & temp)
		{
			return new Bonus(bonus_type & temp, position, velocity);
		}
	}
	return nullptr;
}

uint16_t Bonus::GetType()
{
	return bonus_type;
}

Bonus::~Bonus()
{

}



Asteroid::Asteroid() : buff_bonus(0)
{

}

Asteroid::Asteroid(uint16_t buff_bonus) : buff_bonus(buff_bonus)
{

}

Bonus* Asteroid::Destroy()
{
	return new Bonus(buff_bonus, position, velocity);
}

Asteroid::~Asteroid()
{

}



ControledEntity::ControledEntity() : player_number(0), rotate_input_value_pointer(nullptr), shoot_input_value_pointer(nullptr)
{

}

ControledEntity::ControledEntity(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2D* position, Vec2D* velosity, double angle, double angular_velosity) : player_number(player_number), rotate_input_value_pointer(rotate_input_value_pointer), shoot_input_value_pointer(shoot_input_value_pointer)
{
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
}

uint8_t ControledEntity::GetPlauerNumber()
{
	return player_number;
}

bool ControledEntity::GetRotateInputValue()
{
	return *(bool*)rotate_input_value_pointer;
}

bool ControledEntity::GetShootInputValue()
{
	return *(bool*)shoot_input_value_pointer;
}

ControledEntity::~ControledEntity()
{

}



Sheep::Sheep(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2D* position, Vec2D* velocity, double angle, double angular_velocity) : baff_bonus(0x00), active_baff_bonus(0x00)
{
	this->player_number = player_number;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
}

Sheep::Sheep(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2D* position, Vec2D* velocity, double angle, double angular_velocity, uint16_t baff_bonus) : baff_bonus(baff_bonus), active_baff_bonus(0x00)
{
	this->player_number = player_number;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
}

void Sheep::ActivateBonus()
{
	active_baff_bonus |= baff_bonus;
}

DynamicEntity* Sheep::CreateBullet()
{
	Vec2D temp1 = *direction * radius + *position;
	Vec2D temp2 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
	return new DynamicEntity(&temp1, &temp2, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT / 2.0);
}

DynamicEntity* Sheep::CreateTriple(uint8_t bullet_number)
{
	if (!(baff_bonus & BUFF_TRIPLE))
	{
		return nullptr;
	}
	Vec2D temp1, temp2;
	switch (bullet_number)
	{
	case 0:
		temp1 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return new DynamicEntity(position, &temp1, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT);
	case 1:
		temp1 = *position + direction->Rotate(0.5) * radius;
		temp2 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return new DynamicEntity(&temp1, &temp2, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT);
	case 2:
		temp1 = *position + direction->Rotate(-0.5) * radius;
		temp2 = *direction * BULLET_DEFAULT_VELOCITY + *velocity;
		return new DynamicEntity(&temp1, &temp2, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT);
	default:
		return nullptr;
	}
}

DynamicEntity* Sheep::CreateLoop(uint8_t bullet_number)
{
	if (!(baff_bonus & BONUS_LOOP))
	{
		return nullptr;
	}
	if (bullet_number == BULLETS_IN_LOOP - 1)
	{
		baff_bonus -= BONUS_LOOP;
	}
	Vec2D new_dir = direction->Rotate(2 * M_PI / BULLETS_IN_LOOP * bullet_number);
	Vec2D temp = *position + new_dir * radius;
	new_dir = new_dir * BULLET_DEFAULT_VELOCITY + *velocity;
	return new DynamicEntity(&temp, &new_dir, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT);
}

Mine* Sheep::CreateMine()
{
	if (!(baff_bonus & BONUS_LOOP))
	{
		return nullptr;
	}
	baff_bonus -= BONUS_LOOP;
	Vec2D temp = Vec2D();
	return new Mine(player_number, position, &temp, 0.0, 0.0);
}

Beam* Sheep::CreateLazer()
{
	if (!(baff_bonus & BONUS_LOOP))
	{
		return nullptr;
	}
	baff_bonus -= BONUS_LOOP;
	Vec2D temp = *position + *direction * radius;
	return new Beam(&temp, direction, false);
}

Segment* Sheep::CreateKnife(uint8_t knife_number)
{
	Vec2D temp1, temp2;
	switch (knife_number)
	{
	case 0:
		temp1 = *position - *direction + direction->Perpendicular();
		temp2 = *direction * 2.0;
		return new Segment(&temp1, &temp2, false);
	case 1:
		if (!(baff_bonus & BONUS_LOOP))
		{
			return nullptr;
		}
		baff_bonus -= BONUS_LOOP;
		temp1 = *position - *direction - direction->Perpendicular();
		temp2 = *direction * 2.0;
		return new Segment(&temp1, &temp2, false);
	default:
		return nullptr;
	}
}

void Sheep::GetBonus(Bonus* bonus)
{
	if (baff_bonus & BUFF_TRIPLE)
	{
		uint16_t temp;
		for (uint8_t i = 0; i < BONUS_DATA_LENGTH; i++)
		{
			temp = 0x11 << (i << 1);
			if (bonus->bonus_type & temp)
			{
				baff_bonus |= temp;
			}
		}
		return;
	}
	baff_bonus |= bonus->bonus_type;
}

Bonus* Sheep::LoseBonus()
{
	return new Bonus(baff_bonus, position, velocity);
}


Pilot* Sheep::Destroy()
{
	Vec2D temp = *velocity * 2.0;
	return new Pilot(player_number, rotate_input_value_pointer, shoot_input_value_pointer, position, &temp, angle, angular_velocity);
}

Sheep::~Sheep()
{

}



Pilot::Pilot(uint8_t player_number, void* rotate_keyboard_key_pointer, void* move_keyboard_key_pointer, Vec2D* position, Vec2D* velosity, double angle, double angular_velosity)
{
	this->player_number = player_number;
	rotate_input_value_pointer = rotate_keyboard_key_pointer;
	shoot_input_value_pointer = move_keyboard_key_pointer;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
}

Sheep* Pilot::Respawn()
{
	Vec2D temp = Vec2D();
	return new Sheep(player_number, rotate_input_value_pointer, shoot_input_value_pointer, position, &temp, angle, 0.0);
}

Pilot::~Pilot()
{

}



Turel::Turel(Vec2D* position, double angle)
{
	this->position = position;
	this->angle = angle;
}

DynamicEntity* Turel::Shoot()
{
	Vec2D temp1 = *direction * BULLET_DEFAULT_VELOCITY;
	Vec2D temp2 = *position + *direction * radius;
	return new DynamicEntity(&temp2, &temp1, 0.0, 0.0, BULLET_DEFAULT_RADIUS, 0.0, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT);
}

Turel::~Turel()
{

}



MegaLazer::MegaLazer(Segment* lazer_segment, double angle) : active(false)
{
	*this->position = *lazer_segment->point;
	*this->point2 = lazer_segment->GetSecondPoint();
	this->angle = angle;
}

void MegaLazer::StartShoot()
{
	if (!active)
	{
		active = true;
	}
}

void MegaLazer::StopShoot()
{
	if (active)
	{
		active = false;
	}
}

bool MegaLazer::IsShooting()
{
	return active;
}

MegaLazer::~MegaLazer()
{

}



Mine::Mine(uint8_t player_master_number, Vec2D* position, Vec2D* velosity, double angle, double angular_velosity) : animation_tik(MINE_DEFAULT_TIMER), active(false), boom(false)
{
	this->player_master_number = player_master_number;
	*this->position = *position;
	*this->velocity = *velocity;
	this->angle = angle;
	this->angular_velocity = angular_velocity;
}

void Mine::Activate()
{
	active = true;
}

void Mine::Boom()
{
	animation_tik = MiNE_BOOM_TIMER;
	radius = MINE_BOOM_RADIUS;
	boom = true;
}

void Mine::Recalculate()
{
	DynamicEntity::Recalculate();
	if (active && animation_tik > 0)
	{
		animation_tik--;
	}
	if (animation_tik == 0 && !boom)
	{
		Boom();
	}
}

Mine::~Mine()
{

}



MapElement::MapElement() : position(new Vec2D()), last_position(new Vec2D())
{

}

Vec2D MapElement::GetPosition()
{
	return *position;
}

Vec2D MapElement::GetVelocity()
{
	return *position - *last_position;
}

void MapElement::Move(Vec2D* move_vector)
{
	*position += *move_vector;
}

void MapElement::SetPosition(Vec2D* position)
{
	*this->position = *position;
}

MapElement::~MapElement()
{
	delete position;
	delete last_position;
}



Rectangle::Rectangle() : point2(new Vec2D()), show_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE), collision_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE)
{

}

Rectangle::Rectangle(Vec2D* point1, Vec2D* point2) : point2(new Vec2D(*point2)), show_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE), collision_sides(RECTANGLE_UP_SIDE | RECTANGLE_DOWN_SIDE | RECTANGLE_RIGHT_SIDE | RECTANGLE_LEFT_SIDE)
{
	*position = *point1;
}

Vec2D Rectangle::GetUpRightPoint()
{
	return Vec2D(point2->x, position->y);
}

Vec2D Rectangle::GetDownRightPoint()
{
	return *point2;
}


Vec2D Rectangle::GetDownLeftPoint()
{
	return Vec2D(position->x, point2->y);;
}


Vec2D Rectangle::GetUpLeftPoint()
{
	return *position;
}

Segment Rectangle::GetUpSide()
{
	Vec2D temp = Vec2D(point2->x, position->y);
	return Segment(position, &temp, true);
}

Segment Rectangle::GetDownSide()
{
	Vec2D temp = Vec2D(position->x, point2->y);
	return Segment(&temp, point2, true);
}

Segment Rectangle::GetRightSide()
{
	Vec2D temp = Vec2D(point2->x, position->y);
	return Segment(&temp, point2, true);
}

Segment Rectangle::GetLeftSide()
{
	Vec2D temp = Vec2D(position->x, point2->y);
	return Segment(position, &temp, true);
}

void Rectangle::Move(Vec2D* move_vector)
{
	MapElement::Move(move_vector);
	*point2 += *move_vector;
}

void Rectangle::Set(Rectangle* parent)
{
	*position = *parent->position;
	*last_position = *parent->last_position;
	*point2 = *parent->point2;
	show_sides = parent->show_sides;
	collision_sides = parent->collision_sides;

}

Rectangle::~Rectangle()
{
	delete point2;
}



Cyrcle::Cyrcle() : radius(0.0)
{

}

Cyrcle::Cyrcle(Vec2D* position, double radius) : radius(radius)
{
	*this->position = *position;
}

double Cyrcle::GetRadius()
{
	return radius;
}

void Cyrcle::SetRadius(double radius)
{
	this->radius = radius;
}

void Cyrcle::Set(Cyrcle* parent)
{
	*this->last_position = *parent->last_position;
	*this->position = *parent->position;
	this->radius = parent->radius;
}

Cyrcle::~Cyrcle()
{

}



Polygon::Polygon() : points_array(nullptr), default_points_array(nullptr), points_array_length(0)
{

}

Polygon::Polygon(Vec2D* position, Vec2D* default_points_array, uint32_t points_array_length) : points_array_length(points_array_length), default_points_array(new Vec2D[points_array_length]), points_array(new Vec2D[points_array_length - 1])
{
	*this->position = *position;
	for (uint32_t i = 0; i < points_array_length; i++)
	{
		this->default_points_array[i] = default_points_array[i];
	}
	ToDefault();
}

void Polygon::Rotate(double angle, Vec2D* rotating_point)
{
	Vec2D rot_vec;
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		rot_vec = points_array[i] - *rotating_point;
		rot_vec.RotateThis(angle);
		points_array[i] = *rotating_point + rot_vec;
	}

	rot_vec = *position - *rotating_point;
	rot_vec.RotateThis(angle);
	*position = *rotating_point + rot_vec;
}

void Polygon::Move(Vec2D* move_vector)
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] += *move_vector;
	}
	MapElement::Move(move_vector);
}

void Polygon::ToDefault()
{
	for (uint32_t i = 0; i < points_array_length - 1; i++)
	{
		points_array[i] = default_points_array[i];
	}
	*position = default_points_array[points_array_length - 1];
}

void Polygon::Set(Polygon* parent)
{
	delete points_array;
	delete default_points_array;
	points_array_length = parent->points_array_length;
	points_array = new Vec2D[points_array_length - 1];
	default_points_array = new Vec2D[points_array_length];
	for (uint32_t i = 1; i < points_array_length; i++)
	{
		points_array[i] = parent->points_array[i];
	}
}

Polygon::~Polygon()
{

}



Map::Map(Rectangle* rectangles_array, uint8_t rectangles_array_length, Cyrcle* cyrcles_array, uint8_t cyrcles_array_length, Polygon* polygons_array, uint8_t polygons_array_length) : rectangles_array_length(rectangles_array_length), cyrcles_array_length(cyrcles_array_length), polygons_array_length(polygons_array_length)
{
	if (rectangles_array_length > 0)
	{
		this->rectangles_array = new Rectangle[rectangles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->rectangles_array[i].Set(&rectangles_array[i]);
		}
	}
	else
	{
		this->rectangles_array = nullptr;
	}

	if (rectangles_array_length > 0)
	{
		this->cyrcles_array = new Cyrcle[cyrcles_array_length];
		for (uint8_t i = 0; i < rectangles_array_length; i++)
		{
			this->cyrcles_array[i].Set(&cyrcles_array[i]);
		}
	}
	else
	{
		this->cyrcles_array = nullptr;
	}

	if (cyrcles_array_length > 0)
	{
		this->polygons_array = new Polygon[polygons_array_length];
		for (uint8_t i = 0; i < polygons_array_length; i++)
		{
			this->polygons_array[i].Set(&polygons_array[i]);
		}
	}
	else
	{
		this->polygons_array = nullptr;
	}
}

Rectangle Map::GetRectangle(uint8_t number)
{
	if (number >= rectangles_array_length)
	{
		return rectangles_array[number % rectangles_array_length];
	}
	return rectangles_array[number];
}

Cyrcle Map::GetCyrcle(uint8_t number)
{
	if (number >= cyrcles_array_length)
	{
		return cyrcles_array[number % cyrcles_array_length];
	}
	return cyrcles_array[number];
}

Polygon Map::GetPolygon(uint8_t number)
{
	if (number >= polygons_array_length)
	{
		return polygons_array[number % polygons_array_length];
	}
	return polygons_array[number];
}

Map::~Map()
{
	delete rectangles_array;
	delete cyrcles_array;
	delete polygons_array;
}