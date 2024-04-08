#include "../Entity.h"



DynamicEntity::DynamicEntity() :
	Entity(),
	angular_velocity(0.0f),
	force_collision_coeffisient(DEFAULT_FORCE_COLLISION_COEFFICIENT),
	force_resistance_air_coefficient(DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT)
{
}

DynamicEntity::DynamicEntity(const DynamicEntity& dynamic_entity) :
	Entity(dynamic_entity),
	angular_velocity(dynamic_entity.angular_velocity),
	force(dynamic_entity.force),
	force_collision_coeffisient(dynamic_entity.force_collision_coeffisient),
	force_resistance_air_coefficient(dynamic_entity.force_resistance_air_coefficient),
	velocity(dynamic_entity.velocity)
{
}

DynamicEntity::DynamicEntity(
	const Vec2F& position, 
	const Vec2F& velocity, 
	float radius,
	float angle,
	float angular_velocity,
	float force_collision_coeffisient, 
	float force_resistance_air_coefficient,
	bool exist) 
	:
	Entity(
		position,
		radius,
		angle, 
		exist),
	angular_velocity(angular_velocity),
	force_collision_coeffisient(force_collision_coeffisient),
	force_resistance_air_coefficient(force_resistance_air_coefficient),
	velocity(velocity)
{
}

void DynamicEntity::AddForce(const Vec2F& force)
{
	this->force += force;
}

void DynamicEntity::AddForceAlongDirection(float force)
{
	this->force += direction * force;
}

void DynamicEntity::AddAngularVelocity(float angular_velocity)
{
	this->angular_velocity += angular_velocity;
}

void DynamicEntity::AddGravityForce(float gravity_coeffisient, const Vec2F& forced_point)
{
	Vec2F gvnn = forced_point - position;//grav_vec_not_normalize
	force += gvnn.Normalize() * gravity_coeffisient / (gvnn.x * gvnn.x + gvnn.y * gvnn.y);
}

void DynamicEntity::AddVelocity(const Vec2F& velocity)
{
	this->velocity += velocity;
}

void DynamicEntity::ClearForce()
{
	force.Set(0.0f, 0.0f);
}

bool DynamicEntity::Collision(DynamicEntity& entity)
{
	float distance;
	if ((distance = GetDistance(entity)) > 0.0f)
	{
		return false;
	}
	Vec2F collision_direction = position - entity.position;
	Vec2F local_velocity = velocity - entity.velocity;

	const float& sum_r_c = (entity.radius + radius) * 20.0f;

	if (collision_direction * local_velocity < 0.0f)
	{
		local_velocity = collision_direction.Project(local_velocity);
		velocity -= local_velocity / 2.0f;
		entity.velocity += local_velocity / 2.0f;
		position -= collision_direction.Normalize() * distance * radius / sum_r_c;
		entity.position += collision_direction.Normalize() * distance * entity.radius / sum_r_c;
	}

	return true;
}

bool DynamicEntity::Collision(const StaticEntity& entity)
{
	float distance;
	if ((distance = GetDistance(entity)) > 0.0f)
	{
		return false;
	}
	Vec2F collision_direction = position - entity.GetPosition();
	Vec2F local_velocity = velocity - entity.GetVelocity();

	if (collision_direction * local_velocity < 0.0f)
	{
		local_velocity = collision_direction.Project(local_velocity);
		velocity -= local_velocity;
		position -= collision_direction.Normalize() * distance;
	}


	return true;
}

bool DynamicEntity::Collision(const Map::Rectangle& rectangle)
{
	if (rectangle.IsCheckCollisionsInside())
	{
		if (CollisionInside(rectangle))
		{
			return true;
		}

	}
	else if (rectangle.IsCheckCollisionsOutside())
	{
		return CollisionOutside(rectangle);
	}

	return false;
}

bool DynamicEntity::Collision(const Map::Cyrcle& cyrcle)
{
	float distance;
	bool inside;
	if ((distance = GetDistance(cyrcle, &inside)) > 0.0f)
	{
		return false;
	}

	//from this to cyrcle
	const Vec2F& collision_direction = cyrcle.GetPosition() - position;
	const Vec2F& relative_velocisy = velocity - cyrcle.GetVelocity();
	if (inside)
	{
		const Vec2F& d_pos = cyrcle.GetPosition() - collision_direction.Normalize() * (cyrcle.GetRadius() - radius - force_collision_coeffisient);
		velocity = relative_velocisy - relative_velocisy.ProjectSign(position - d_pos) + cyrcle.GetVelocity();
		force -= force.ProjectSign(position - d_pos);
		position = d_pos;
		return true;
	}

	const Vec2F& d_pos = cyrcle.GetPosition() - collision_direction.Normalize() * (cyrcle.GetRadius() + radius + force_collision_coeffisient);
	velocity = relative_velocisy - relative_velocisy.ProjectSign(position - d_pos) + cyrcle.GetVelocity();
	force -= force.ProjectSign(position - d_pos);
	position = d_pos;
	return true;
}

bool DynamicEntity::Collision(const Map::Polygon& polygon)
{
	if (!polygon.exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon.PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment treck = GetLastTreck();
	Segment side = Segment(polygon.points_array[p_count - 1], polygon.points_array[0], true);
	bool collision = false;
	Vec2F collision_direction;//direction from position to collision point
	float distance;

	Vec2F d_pos;

	if (p_count > 2 && polygon.IsClosed())
	{
		distance = side.Distance(position, &collision_direction);
		if (distance < radius || side.IsIntersection(treck))
		{
			d_pos = (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_coeffisient);
			force -= force.ProjectSign(d_pos);
			position += d_pos;
			collision_direction -= position;
			velocity -= collision_direction.ProjectSign(velocity);
			velocity += (-collision_direction).ProjectSign(polygon.Velocity(collision_direction + position) - velocity);

			collision = true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		side.Set(polygon.points_array[p - 1], polygon.points_array[p], true);

		distance = side.Distance(position, &collision_direction);
		if (distance < radius || side.IsIntersection(treck))
		{
			d_pos = (position - collision_direction).Normalize() * (radius - position.Distance(collision_direction) + force_collision_coeffisient);
			force -= force.ProjectSign(d_pos);
			position += d_pos;
			collision_direction -= position;
			velocity -= collision_direction.ProjectSign(velocity);
			velocity += (-collision_direction).ProjectSign(polygon.Velocity(collision_direction + position) - velocity);

			collision = true;
		}
	}
	return collision;
}

bool DynamicEntity::Collision(const Map::MapData& map)
{
	bool collision = false;
	for (auto& element : map.cyrcles_array)
	{
		if (element.exist)
		{
			collision |= Collision(element);
		}
	}
	for (auto& element : map.polygons_array)
	{
		if (element.exist)
		{
			collision |= Collision(element);
		}
	}
	for (auto& element : map.rectangles_array)
	{
		if (element.exist)
		{
			collision |= Collision(element);
		}
	}
	return collision;
}

bool DynamicEntity::CollisionInside(const Map::Rectangle& rectangle)
{
	if (!IsInside(rectangle))
	{
		return false;
	}

	bool collision = false;
	const Vec2F rectangle_velocity = rectangle.GetVelocity();

	if (rectangle.GetUpRightPoint().y - position.y - velocity.y < radius)
	{
		position.y = rectangle.GetUpRightPoint().y - radius;
		velocity.y = fminf(velocity.y, rectangle_velocity.y);
		force.y = fminf(force.y, 0.0f);

		collision = true;
	}

	if (rectangle.GetUpRightPoint().x - position.x - velocity.x < radius)
	{
		position.x = rectangle.GetUpRightPoint().x - radius;
		velocity.x = fminf(velocity.x, rectangle_velocity.x);
		force.x = fminf(force.x, 0.0f);

		collision = true;
	}

	if (position.y + velocity.y - rectangle.GetDownLeftPoint().y < radius)
	{
		position.y = rectangle.GetDownLeftPoint().y + radius;
		velocity.y = fmaxf(velocity.y, rectangle_velocity.y);
		force.y = fmaxf(force.y, 0.0f);

		collision = true;
	}

	if (position.x + velocity.x - rectangle.GetDownLeftPoint().x < radius)
	{
		position.x = rectangle.GetDownLeftPoint().x + radius;
		velocity.x = fmaxf(velocity.x, rectangle_velocity.x);
		force.x = fmaxf(force.x, 0.0f);

		collision = true;
	}

	return collision;
}

static bool InsideRectangle(const Vec2F& point_position, const Vec2F& up_right_corner_position, const Vec2F& down_left_corner_position)
{
	return up_right_corner_position > point_position && point_position > down_left_corner_position;
}

bool DynamicEntity::CollisionCheckPosition(const Map::Rectangle& rectangle, Vec2F& point, Vec2F& force_vector)
{
	const auto& up_rigth = rectangle.GetUpRightPoint();
	const auto& down_left = rectangle.GetDownLeftPoint();
	if (InsideRectangle(position, up_rigth, down_left)) return false;

	const auto& up_left = Vec2F(down_left.x, up_rigth.y);
	const auto& down_right = Vec2F(up_rigth.x, down_left.y);
	const auto& up_up_right = Vec2F(up_rigth.x, up_rigth.y + radius);
	const auto& up_right_right = Vec2F(up_rigth.x + radius, up_rigth.y);
	const auto& down_down_left = Vec2F(down_left.x, down_left.y - radius);
	const auto& down_left_left = Vec2F(down_left.x - radius, down_left.y);

	if (InsideRectangle(position, up_up_right, up_left)) // up rectangle
	{
		point = Vec2F(position.x, up_up_right.y + force_collision_coeffisient);
		force_vector = Vec2F(0.0f, 1.0f);
		return true;
	}

	if (InsideRectangle(position, up_right_right, down_right)) // right rectangle
	{
		point = Vec2F(up_right_right.x + force_collision_coeffisient, position.y);
		force_vector = Vec2F(1.0f, 0.0f);
		return true;
	}

	if (InsideRectangle(position, down_right, down_down_left)) // down rectangle
	{
		point = Vec2F(position.x, down_down_left.y - force_collision_coeffisient);
		force_vector = Vec2F(0.0f, -1.0f);
		return true;
	}

	if (InsideRectangle(position, up_left, down_left_left)) // left rectangle
	{
		point = Vec2F(down_left_left.x - force_collision_coeffisient, position.y);
		force_vector = Vec2F(-1.0f, 0.0f);
		return true;
	}

	const auto& up_up_rigt_rigth = up_rigth + radius;
	const auto& down_down_left_left = down_left - radius;

	const auto& down_right_right = Vec2F(down_right.x + radius, down_right.y);
	const auto& down_down_right = Vec2F(down_right.x, down_right.y - radius);
	const auto& up_left_left = Vec2F(down_right.x - radius, down_right.y);
	const auto& up_up_left = Vec2F(down_right.x, down_right.y + radius);

	if (InsideRectangle(position, up_up_rigt_rigth, up_rigth) && // up right cyrcle
		position.DistancePow2(up_rigth) < radius * radius)
	{
		force_vector = (position - up_rigth).Normalize();
		point = up_rigth + force_vector * (radius + force_collision_coeffisient);
		return true;
	}

	if (InsideRectangle(position, down_right_right, down_down_right) && // down right cyrcle
		position.DistancePow2(down_right) < radius * radius)
	{
		force_vector = (position - down_right).Normalize();
		point = down_right + force_vector * (radius + force_collision_coeffisient);
		return true;
	}

	if (InsideRectangle(position, down_left, down_down_left_left) && // down left cyrcle
		position.DistancePow2(down_left) < radius * radius)
	{
		force_vector = (position - down_left).Normalize();
		point = down_left + force_vector * (radius + force_collision_coeffisient);
		return true;
	}

	if (InsideRectangle(position, up_up_left, up_left_left) && // up left cyrcle
		position.DistancePow2(up_left) < radius * radius)
	{
		force_vector = (position - up_left).Normalize();
		point = up_left + force_vector * (radius + force_collision_coeffisient);
		return true;
	}

	return false;
}

bool DynamicEntity::CollisionCheckIntersections(const Map::Rectangle& rectangle, Vec2F& intersect_point, Vec2F& force_vector)
{
	const auto& relative_velocity = velocity - rectangle.GetVelocity();

	const bool& treck_too_short = relative_velocity.LengthPow2() < radius * radius / 100.0f;
	if (treck_too_short) return false;

	const auto& treck = Segment(position, relative_velocity);
	auto side = rectangle.GetUpSide();
	side.point.y += radius;
	if (treck.Intersection(side, &intersect_point))
	{
		intersect_point += Vec2F(0.0f, force_collision_coeffisient);
		force_vector = Vec2F(0.0f, 1.0f);
		return true;
	}

	side = rectangle.GetLeftSide();
	side.point.x -= radius;
	if (treck.Intersection(side, &intersect_point))
	{
		intersect_point -= Vec2F(force_collision_coeffisient, 0.0f);
		force_vector = Vec2F(-1.0f, 0.0f);
		return true;
	}

	side = rectangle.GetDownSide();
	side.point.y -= radius;
	if (treck.Intersection(side, &intersect_point))
	{
		intersect_point -= Vec2F(0.0f, force_collision_coeffisient);
		force_vector = Vec2F(0.0f, -1.0f);
		return true;
	}

	side = rectangle.GetRightSide();
	side.point.x += radius;
	if (treck.Intersection(side, &intersect_point))
	{
		intersect_point += Vec2F(force_collision_coeffisient, 0.0f);
		force_vector = Vec2F(1.0f, 0.0f);
		return true;
	}

	return false;
}

bool DynamicEntity::CollisionOutside(const Map::Rectangle& rectangle)
{
	if (
		GetDistance(rectangle.GetPosition()) >
		rectangle.GetSize().Length() / 2.0f + velocity.Length())
	{
		return false;
	}

	Vec2F point;
	Vec2F force_vector;

	if (CollisionCheckIntersections(rectangle, point, force_vector))
	{
		position = point;
		velocity -= (-force_vector).ProjectSign(velocity);
		force -= (-force_vector).ProjectSign(velocity);
		return true;
	}

	if (CollisionCheckPosition(rectangle, point, force_vector))
	{
		position = point;
		velocity -= (-force_vector).ProjectSign(velocity);
		force -= (-force_vector).ProjectSign(velocity);
		return true;
	}

	return false;
}

float DynamicEntity::GetAngularVelocity() const
{
	return angular_velocity;
}

Segment DynamicEntity::GetLastTreck() const
{
	return Segment(position, -velocity);
}

Vec2F DynamicEntity::GetCollisionDirection(const Vec2F& point) const
{
	Vec2F nearest_point;
	if (GetTreck().Distance(point, &nearest_point) >= radius)
	{
		return Vec2F();
	}

	const Vec2F& nearest_intersection_point =
		position + velocity.Length(position.Distance(nearest_point) - sqrt(radius * radius - (nearest_point - point).LengthPow2()));
	return nearest_intersection_point - point;
}

Vec2F DynamicEntity::GetCollisionDirection(const Vec2F& point, const Vec2F& nearest_point) const
{
	const Vec2F& nearest_intersection_point =
		position + velocity.Length(position.Distance(nearest_point) - sqrt(radius * radius - (nearest_point - point).LengthPow2()));
	return nearest_intersection_point - point;
}


Segment DynamicEntity::GetTreck() const
{
	return Segment(position, velocity);
}

Segment DynamicEntity::GetTreckDouble() const
{
	return Segment(position, velocity * 2.0f);
}

Segment DynamicEntity::GetTreckDoubleRelative(const DynamicEntity& entity) const
{
	return Segment(position, (velocity - entity.velocity) * 2.0f);
}

Segment DynamicEntity::GetTreckRelative(const DynamicEntity& entity) const
{
	return Segment(position, velocity - entity.velocity);
}

Segment DynamicEntity::GetTreckRelativeDouble(const DynamicEntity& entity) const
{
	return Segment(position, velocity - entity.velocity * 2.0f);
}

Vec2F DynamicEntity::GetVelocity() const
{
	return velocity;
}

template <typename ThingT>
bool DynamicEntity::IsCollision(const ThingT& point) const
{
	if (IsTooSlow())
	{
		return point.Distance(position) < radius;
	}
	return Segment(position, -velocity).Distance(point) < radius;
}
template bool DynamicEntity::IsCollision<Vec2F>(const Vec2F& point) const;
template bool DynamicEntity::IsCollision<Line>(const Line& point) const;
template bool DynamicEntity::IsCollision<Beam>(const Beam& point) const;
template bool DynamicEntity::IsCollision<Segment>(const Segment& point) const;

template<>
bool DynamicEntity::IsCollision<DynamicEntity>(const DynamicEntity& entity) const
{
	const float r_sum = radius + entity.radius;

	if (velocity.LengthPow2() < radius * radius)
	{
		if (entity.velocity.LengthPow2() < entity.radius * entity.radius)
		{
			return (position + velocity).DistancePow2(entity.position + entity.velocity) < r_sum * r_sum;
		}
		return entity.GetTreck().Distance(position + velocity) < radius + entity.radius;
	}
	if (entity.velocity.LengthPow2() < entity.radius * entity.radius)
	{
		return GetTreck().Distance(entity.position + entity.velocity) < radius + entity.radius;
	}
	if ((entity.GetVelocity() - velocity).LengthPow2() < r_sum * r_sum / 16.0f)
	{
		return (position + velocity).DistancePow2(entity.position + entity.velocity) < r_sum * r_sum;
	}

	const float& p1x = position.x;
	const float& p1y = position.y;
	const float& v1x = velocity.x;
	const float& v1y = velocity.y;
	const float& p2x = entity.position.x;
	const float& p2y = entity.position.y;
	const float& v2x = entity.velocity.x;
	const float& v2y = entity.velocity.y;

	const float& dvx = v1x - v2x;
	const float& dvy = v1y - v2y;

	const float& t_min = // The distance between objects is minimum at this time.
		((p1x - p2x) * (v1x - v2x) + (p1y - p2y) * (v1y - v2y)) / 
		(dvx * dvx + dvy * dvy);

	const float& dx = (p1x - p2x + (v1x - v2x) * t_min);
	const float& dy = (p1y - p2y + (v1y - v2y) * t_min);
	const float& d_min_p2 = dx * dx + dy * dy;
	return d_min_p2 < r_sum * r_sum;
}

template<>
bool DynamicEntity::IsCollision<StaticEntity>(const StaticEntity& entity) const
{
	if (IsTooSlow())
	{
		return entity.GetPosition().Distance(position) < radius;
	}
	return Segment(position, -velocity).Distance(entity.GetPosition()) < radius + entity.radius;
}

template<>
bool DynamicEntity::IsCollision<Laser>(const Laser& laser) const
{
	if (IsTooSlow())
	{
		return laser.GetSegment().Distance(position) < radius + laser.width;
	}
	return laser.GetSegment().Distance(GetTreck()) < radius + laser.width;
}

template<>
bool DynamicEntity::IsCollision<Map::Rectangle>(const Map::Rectangle& rectangle) const
{
	const auto& relative_velocity = velocity - rectangle.GetVelocity();
	const auto& track = Segment(position, relative_velocity);

	if (rectangle.GetUpSide().Distance(track) < radius) return true;
	if (rectangle.GetDownSide().Distance(track) < radius) return true;
	if (rectangle.GetRightSide().Distance(track) < radius) return true;
	if (rectangle.GetLeftSide().Distance(track) < radius) return true;

	if (track.Distance(rectangle.GetUpRightPoint()) < radius) return true;
	if (track.Distance(rectangle.GetDownRightPoint()) < radius) return true;
	if (track.Distance(rectangle.GetUpLeftPoint()) < radius) return true;
	if (track.Distance(rectangle.GetDownLeftPoint()) < radius) return true;

	return false;
}

template<>
bool DynamicEntity::IsCollision<Map::Cyrcle>(const Map::Cyrcle& cyrcle) const
{
	const Segment trace(position, velocity - cyrcle.GetVelocity());
	const Vec2F& cyrcle_position = cyrcle.GetPosition();
	const float& cyrcle_radius = cyrcle.GetRadius();

	bool object_too_far = trace.Distance(cyrcle_position) > cyrcle_radius;
	bool first_point_inside = trace.point.Distance(cyrcle_position) < cyrcle_radius - radius;
	bool second_point_inside = trace.SecondPoint().Distance(cyrcle_position) < cyrcle_radius - radius;

	return !(object_too_far || first_point_inside && second_point_inside);

	return
		!(trace.Distance(cyrcle_position) > cyrcle_radius ||
		trace.point.Distance(cyrcle_position) < cyrcle_radius - radius &&
		trace.SecondPoint().Distance(cyrcle_position) < cyrcle_radius - radius);
}

template<>
bool DynamicEntity::IsCollision<Map::Polygon>(const Map::Polygon& polygon) const
{
	if (!polygon.exist)
	{
		return false;
	}
	EngineTypes::Map::array_length_t p_count = polygon.PointsCount();
	if (p_count < 2)
	{
		return false;
	}
	Segment treck = GetTreck();

	if (p_count > 2 && polygon.IsClosed())
	{
		if (IsCollision(Segment(polygon.points_array[p_count - 1], polygon.points_array[0], true)))
		{
			return true;
		}
	}

	for (EngineTypes::Map::array_length_t p = 1; p < p_count; p++)
	{
		if (IsCollision(Segment(polygon.points_array[p - 1], polygon.points_array[p], true)))
		{
			return true;
		}
	}
	return false;
}

template<>
bool DynamicEntity::IsCollision<Map::MapData>(const Map::MapData& map) const
{
	for (auto& element : map.cyrcles_array)
	{
		if (!element.exist) continue;
		if (IsCollision(element)) return true;
	}
	for (auto& element : map.polygons_array)
	{
		if (!element.exist) continue;
		if (IsCollision(element)) return true;
	}
	for (auto& element : map.rectangles_array)
	{
		if (!element.exist) continue;
		if (IsCollision(element)) return true;
	}
	return false;
}

bool DynamicEntity::IsTooSlow() const
{
	return velocity.LengthPow2() < radius * radius / 16.0f;
}

//The function updates position and velocity of entity and clears forces' data.
void DynamicEntity::Update()
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

	velocity *= 1.0f - force_resistance_air_coefficient;
	velocity += force;
	Move(velocity);
	force.Set(0.0f, 0.0f);
}

void DynamicEntity::Set(const DynamicEntity* dynamic_entity)
{
	Entity::Set(dynamic_entity);
	angular_velocity = dynamic_entity->angular_velocity;
	force_collision_coeffisient = dynamic_entity->force_collision_coeffisient;
	force_resistance_air_coefficient = dynamic_entity->force_resistance_air_coefficient;
	velocity = dynamic_entity->velocity;
}

void DynamicEntity::Set(
	const Vec2F& position,
	const Vec2F& velocity,
	float radius, 
	float angle,
	float angular_velocity,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	bool exist)
{
	Entity::Set(position, radius, angle, exist);
	this->force_resistance_air_coefficient = angular_velocity;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->velocity = velocity;
}

void DynamicEntity::SetAngularVelocity(float angular_velocity)
{
	this->angular_velocity = angular_velocity;
}

void DynamicEntity::SetCoefficients(float force_collision_coeffisient, float force_resistance_air_coefficient)
{
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
}

void DynamicEntity::SetVelocity(const Vec2F& velocity)
{
	this->velocity = velocity;
}

void DynamicEntity::operator=(const DynamicEntity& dynamic_entity)
{
	Entity::operator=(dynamic_entity);
	angular_velocity = dynamic_entity.angular_velocity;
	force = dynamic_entity.force;
	force_collision_coeffisient = dynamic_entity.force_collision_coeffisient;
	force_resistance_air_coefficient = dynamic_entity.force_resistance_air_coefficient;
	velocity = dynamic_entity.velocity;
}

DynamicEntity::~DynamicEntity()
{
}