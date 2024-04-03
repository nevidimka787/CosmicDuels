#include "../Entity.h"



SupportEntity::SupportEntity() :
	StaticEntity(),
	host_p(nullptr),
	host_matrix_p(nullptr),
	local_angle(0),
	host_number(0),
	host_team(0)
{
}

SupportEntity::SupportEntity(const SupportEntity& support_entity) :
	StaticEntity(support_entity),
	host_p(support_entity.host_p),
	host_matrix_p(support_entity.host_matrix_p),
	host_number(support_entity.host_number),
	host_team(support_entity.host_team),
	local_angle(support_entity.local_angle),
	local_direction(support_entity.local_direction),
	local_position(support_entity.local_position)
{
	this->position *= *host_matrix_p;
}

SupportEntity::SupportEntity(
	const ControledEntity* host,
	const Vec2F& position,
	float radius,
	float angle,
	bool exist)
	:
	StaticEntity(
		position,
		radius,
		angle + host->GetAngle(),
		exist),
	host_p(host),
	host_matrix_p(host->GetModelMatrixPointerConst()),
	host_number(host->GetPlayerNumber()),
	host_team(host->GetTeamNumber()),
	local_angle(angle),
	local_direction(Vec2F(1.0f, 0.0f).Rotate(angle)),
	local_position(position)
{
	this->position *= *host_matrix_p;
}

SupportEntity::SupportEntity(
	const ControledEntity* host,
	const Vec2F* position,
	float radius, 
	float angle,
	bool exist) 
	:
	StaticEntity(
		position,
		radius,
		angle + host->GetAngle(),
		exist),
	host_p(host),
	host_matrix_p(host->GetModelMatrixPointerConst()),
	host_number(host->GetPlayerNumber()),
	host_team(host->GetTeamNumber()),
	local_angle(angle),
	local_direction(Vec2F(1.0f, 0.0f).Rotate(angle)),
	local_position(*position)
{
	this->position *= *host_matrix_p;
}

bool SupportEntity::IsCreatedBy(const ControledEntity& potential_host) const
{
	return host_number == potential_host.GetPlayerNumber();
}

bool SupportEntity::IsCreatedByTeam(const ControledEntity& potential_host) const
{
	return host_team == potential_host.GetTeamNumber();
}

float SupportEntity::GetAngle() const
{
	return local_angle;
}

Vec2F SupportEntity::GetDirection() const
{
	return local_direction;
}

Vec2F SupportEntity::GetGlobalAngle() const
{
	return angle;
}

Vec2F SupportEntity::GetGlobalPosition() const
{
	return position;
}

const ControledEntity* SupportEntity::GetHostP() const
{
	return host_p;
}

Vec2F SupportEntity::GetNormalizeDirection() const
{
	return local_direction.Normalize();
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterNumber() const
{
	return host_number;
}

GameTypes::players_count_t SupportEntity::GetPlayerMasterTeamNumber() const
{
	return host_team;
}

const Vec2F& SupportEntity::GetPosition() const
{
	return local_position;
}

void SupportEntity::Set(const SupportEntity* support_entity)
{
	angle = support_entity->angle;
	direction = support_entity->direction;
	exist = support_entity->exist;
	host_p = support_entity->host_p;
	host_matrix_p = support_entity->host_matrix_p;
	host_number = support_entity->host_number;
	host_team = support_entity->host_team;
	local_angle = support_entity->local_angle;
	local_direction = support_entity->local_direction;
	local_position = support_entity->local_position;
	position = support_entity->position;
	radius = support_entity->radius;
}

void SupportEntity::Set(
	const ControledEntity* host,
	Vec2F position, 
	float radius, 
	float angle, 
	bool exist)
{
	if (host == nullptr)
	{
		return;
	}
	this->angle = host->GetAngle();
	this->direction = host->GetDirectionNotNormalize();
	this->exist = exist;
	this->host_number = host->GetPlayerNumber();
	this->host_team = host->GetTeamNumber();
	this->local_angle = angle;
	this->local_direction = Vec2F(1.0f, 0.0f).Rotate(angle);
	this->local_position = position;
	this->position = host->GetPosition();
	this->radius = radius;

	this->host_p = host;
	this->host_matrix_p = host->GetModelMatrixPointerConst();
}

void SupportEntity::Set(
	const ControledEntity* host,
	const Vec2F* position,
	float radius,
	float angle,
	bool exist)
{
	if (host == nullptr)
	{
		return;
	}
	this->angle = host->GetAngle();
	this->direction = host->GetDirectionNotNormalize();
	this->exist = exist;
	this->host_number = host->GetPlayerNumber();
	this->host_team = host->GetTeamNumber();
	this->local_angle = angle;
	this->local_direction = Vec2F(1.0f, 0.0f).Rotate(angle);
	this->local_position = *position;
	this->position = host->GetPosition();
	this->radius = radius;

	this->host_p = host;
	this->host_matrix_p = host->GetModelMatrixPointerConst();
}

void SupportEntity::SetAngle(float angle)
{
	local_angle = angle;
}

void SupportEntity::SetDirection(Vec2F direction)
{
	local_direction = direction.Normalize();
}

void SupportEntity::SetDirection(const Vec2F* direction)
{
	local_direction = direction->Normalize();
}

void SupportEntity::SetNotNormalizeDirection(Vec2F direction)
{
	local_direction = direction;
}

void SupportEntity::SetNotNormalizeDirection(const Vec2F* direction)
{
	local_direction = *direction;
}

void SupportEntity::SetHost(const ControledEntity* host)
{
	host_p = host;
	host_matrix_p = host->GetModelMatrixPointerConst();
}

void SupportEntity::SetPosition(Vec2F position)
{
	local_position = position;
}

void SupportEntity::SetPosition(const Vec2F* position)
{
	local_position = *position;
}

void SupportEntity::Update()
{
	if (host_p == nullptr)
	{
		return;
	}
	angle = local_angle + host_p->GetAngle();
	direction = local_direction * (Mat2F)*host_matrix_p;
	position = local_position * *host_matrix_p;
	StaticEntity::Update();
}

void SupportEntity::UpdateDirection()
{
	local_direction = Vec2F(direction.Length(), 0.0f).Rotate(local_angle);
}

void SupportEntity::operator=(const SupportEntity& support_entity)
{
	StaticEntity::operator=(support_entity);

	host_p = support_entity.host_p;
	host_matrix_p = support_entity.host_matrix_p;
	host_number = support_entity.host_number;
	host_team = support_entity.host_team;
	local_angle = support_entity.local_angle;
	local_direction = support_entity.local_direction;
	local_position = support_entity.local_position;
}

SupportEntity::~SupportEntity()
{
}