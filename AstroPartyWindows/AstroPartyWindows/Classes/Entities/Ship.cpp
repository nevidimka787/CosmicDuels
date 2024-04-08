#include "../Entity.h"

#include <vector>

Ship::Ship() 
	:
	ControledEntity(),
	bonus_inventory(BONUS_NOTHING),
	buff_inventory(BONUS_NOTHING),
	bullets_in_magazine(0),
	burnout(0),
	burnout_coldown(0),
	magazine_size(0),
	unbrakable(0),
	objects_in_creating_proccess(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

Ship::Ship(const Ship& ship) 
	:
	ControledEntity(ship),
	bonus_inventory(ship.bonus_inventory),
	buff_inventory(ship.buff_inventory),
	bullets_in_magazine(ship.bullets_in_magazine),
	burnout(ship.burnout),
	burnout_coldown(ship.burnout_coldown),
	magazine_size(ship.magazine_size),
	unbrakable(ship.unbrakable),
	objects_in_creating_proccess(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

Ship::Ship(
	const Vec2F& position,
	const Vec2F& velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team,
	const GameTypes::control_flags_t* controle_flags,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	GameTypes::tic_t burnout_coldown,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t start_bullets_count,
	bool exist)
	:
	ControledEntity(
		position,
		velocity,
		radius,
		player_number,
		player_team,
		controle_flags,
		heat_box_vertexes_array,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		exist),
	bonus_inventory(bonus_inventory),
	buff_inventory(buff_inventory),
	bullets_in_magazine(start_bullets_count),
	burnout(burnout),
	burnout_coldown(burnout_coldown),
	magazine_size(max_bullets_count),
	unbrakable(unbrakable),
	objects_in_creating_proccess(0),
	element_type(GAME_OBJECT_TYPE_NULL)
{
}

bool Ship::ActivateAvailableBuffs()
{
	bool update = false;
	if (!(buff_inventory & SHIP_BUFF_TRIPLE) && (bonus_inventory & (BONUS_BUFF_TRIPLE * 3)))
	{
		buff_inventory += SHIP_BUFF_TRIPLE;
		bonus_inventory -= BONUS_BUFF_TRIPLE;
		update = true;
	}
	if (!(buff_inventory & SHIP_BUFF_SHIELD) && (bonus_inventory & (BONUS_BUFF_SHIELD * 3)))
	{
		buff_inventory += SHIP_BUFF_SHIELD;
		bonus_inventory -= BONUS_BUFF_SHIELD;
		update = true;
	}
	return update;
}

void Ship::ActivateBuffNoCheck(EngineTypes::Ship::inventory_t buff)
{
	buff_inventory |= buff;
}

void Ship::AddBullet()
{
	if (bullets_in_magazine < magazine_size)
	{
		bullets_in_magazine++;
	}
}

void Ship::AddBullets(GameTypes::entities_count_t bullets_count)
{
	if (bullets_count < magazine_size - bullets_in_magazine)
	{
		bullets_in_magazine += bullets_count;
	}
	else
	{
		bullets_in_magazine = magazine_size;
	}
}

void Ship::AddBulletsToMax()
{
	bullets_in_magazine = magazine_size;
}

EngineTypes::Bonus::inventory_t Ship::BonusInfo()
{
	return bonus_inventory;
}

void Ship::Burnout(float power, bool rotate_clockwise, GameTypes::tic_t burnout_period)
{
	if (burnout_coldown > 0)
	{
		return;
	}
	if (rotate_clockwise)
	{
		force -= direction.PerpendicularClockwise().Normalize() * power;
		angle += (float)M_PI / 2.0f;
	}
	else
	{
		force += direction.PerpendicularClockwise().Normalize() * power;
		angle += -(float)M_PI / 2.0f;
	}
	UpdateDirection();
	burnout = burnout_period;
}

bool Ship::CanCreatingBullet() const
{
	return bullets_in_magazine > 0;
}

bool Ship::CanCreatingObject() const
{
	return objects_in_creating_proccess > 0;
}

Bullet Ship::CreateBullet()
{
	if (bullets_in_magazine == 0)
	{
		return Bullet();
	}

	AddForceAlongDirection(-SHIP_SHOOT_FORCE);
	bullets_in_magazine--;
	reoading_dellay = SHIP_DEFAULT_REALOADING_DELLAY;

	return Bullet(
		position + direction.Normalize() * (radius - BULLET_DEFAULT_RADIUS),
		direction * BULLET_DEFAULT_VELOCITY + velocity - direction.PerpendicularClockwise() * angular_velocity * radius,
		GetPlayerNumber(),
		GetTeamNumber(),
		true,
		angle);
}

DynamicParticle Ship::CreateBurnoutExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_BURNOUT,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP,
		GetColor(),
		PARTICLE_PERIOD_EXAUST_ENGINE,
		PARTICLE_POSTPONE_EXAUST_ENGINE,
		current_tic + PARTICLE_PERIOD_EXAUST_ENGINE);
}

DynamicParticle Ship::CreateEnginExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity - direction * 0.01f,
		radius,
		angle,
		0.0f,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_ENGINE,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP,
		GetColor(),
		PARTICLE_PERIOD_EXAUST_ENGINE,
		PARTICLE_POSTPONE_EXAUST_ENGINE,
		current_tic + PARTICLE_PERIOD_EXAUST_ENGINE);
}

DynamicParticle Ship::CreateShards(GameTypes::tic_t current_tic)
{

	return DynamicParticle(
		current_tic,
		position,
		velocity,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_SHARDS_SHIP,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_SHARDS_SHIP,
		PARTICLE_POSTPONE_SHARDS_SHIP,
		current_tic + PARTICLE_PERIOD_SHARDS_SHIP + PARTICLE_POSTPONE_SHARDS_SHIP);
}

DynamicParticle Ship::CreateShootingExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position + direction * radius,
		velocity + direction * BULLET_DEFAULT_MIN_VELOCITY,
		radius,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_SHOOT,
		DYNAMIC_PARTICLE_PROPERTY_NULL,
		Color3F(1.0f, 1.0f, 0.5f),
		PARTICLE_PERIOD_EXAUST_SHOOT,
		PARTICLE_POSTPONE_EXAUST_SHOOT,
		current_tic + PARTICLE_PERIOD_EXAUST_SHOOT + PARTICLE_POSTPONE_EXAUST_SHOOT);
}

void Ship::ClearInventory()
{
	buff_inventory = 0x0;
	bonus_inventory = 0x0;
	bullets_in_magazine = 0x0;
	magazine_size = SHIP_DEFAULT_MAGAZINE_SIZE;
}

Bullet Ship::CreateTriple(uint8_t bullet_number)
{
	if (bullets_in_magazine < 3)
	{
		return Bullet();
	}
	
	switch (bullet_number)
	{
	case 0:
		return Bullet(
			position + direction * radius,
			direction * BULLET_DEFAULT_VELOCITY + velocity,
			GetPlayerNumber(),
			GetTeamNumber());
	case 1:
		return Bullet(
			position - direction.PerpendicularClockwise() * radius * 0.75f,
			direction * BULLET_DEFAULT_VELOCITY + velocity,
			GetPlayerNumber(),
			GetTeamNumber());
	case 2:
		bullets_in_magazine -= 3;
		return Bullet(
			position + direction.PerpendicularClockwise() * radius * 0.75f,
			direction * BULLET_DEFAULT_VELOCITY + velocity,
			GetPlayerNumber(),
			GetTeamNumber());
	default:
		return Bullet();
	}
}

bool Ship::CreatingEntities(
	GameTypes::entities_count_t objects_count,	//count of objects in creating loop
	GameTypes::objects_types_count_t object_type	//type of elemnts in creating loop
)
{
	if (this->objects_in_creating_proccess > 0)
	{
		return false;
	}

	this->objects_in_creating_proccess = objects_count;
	this->element_type = object_type;

	return true;
}

GameTypes::entities_count_t Ship::GetElemntFromList()
{
	if (objects_in_creating_proccess == 0)
	{
		return 0;
	}
	objects_in_creating_proccess--;
	return objects_in_creating_proccess + (uint16_t)1;
}

Bomb Ship::CreateBomb()
{
	return Bomb(
		Vec2F(-0.25f, 0.0f) * GetModelMatrix(),
		velocity,
		GetTeamNumber(),
		GetTeamNumber());
}

Laser Ship::CreateLaser()
{
	AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	return Laser(this, LASER_DEFAULT_LOCAL_BEAM);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Segment new_knife_segment;
	switch (knife_number)
	{
	case 0:
		return Knife(this, Segment(Vec2F(-0.35f, sqrt(3.0f) / 4.0f), Vec2F(0.75f, 0.0f)));
	case 1:
		return Knife(this, Segment(Vec2F(-0.35f, sqrt(3.0f) / -4.0f), Vec2F(0.75f, 0.0f)));
	default:
		return Knife();
	}
}

void Ship::DecrementSizeOfMagasize(GameTypes::entities_count_t cells_count)
{
	if (magazine_size >= cells_count)
	{
		magazine_size -= cells_count;
		return;
	}
	magazine_size = 0;
	return;
}

Pilot Ship::Destroy()
{
	Pilot new_pilot = Pilot(
		position,
		Vec2F(),
		GetPlayerNumber(),
		GetTeamNumber(),
		GetControleFlagsP(),
		PILOT_DEFAULT_RESPAWN_TIMER,
		std::vector<Vec2F>(),
		angle);
	new_pilot.AddForce(velocity);
	return new_pilot;
}

int Ship::GetBonusInventoryAsBoolList() const
{
	return
		(int)(
			((bonus_inventory & (BONUS_LOOP * BONUS_CELL)) ? (1u << 0u) : 0u) +		//zero bit is a loop cell
			((bonus_inventory & (BONUS_LASER * BONUS_CELL)) ? (1u << 1u) : 0u) +	//first bit is a laser cell
			((bonus_inventory & (BONUS_BOMB * BONUS_CELL)) ? (1u << 2u) : 0u) +		//second bit is a bomb cell
			((bonus_inventory & (BONUS_KNIFE * BONUS_CELL)) ? (1u << 3u) : 0u));	//third bit is a knife cell
}

GameTypes::entities_count_t Ship::GetBulletsCountInMagasine() const
{
	return bullets_in_magazine;
}

GameTypes::entities_count_t Ship::GetSizeOfMagazine() const
{
	return magazine_size;
}

GameTypes::objects_types_count_t Ship::GetTypeOfElemntInLoop() const
{
	return element_type;
}

bool Ship::IsHaveBonus(EngineTypes::Bonus::inventory_t bonus) const
{
	return bonus_inventory & (bonus * 3);
}

bool Ship::IsHaveBuff(EngineTypes::Ship::inventory_t buff) const
{
	return buff_inventory & buff;
}

void Ship::IncrementSizeOfMagazine(GameTypes::entities_count_t cells_count)
{
	if (magazine_size <= SHIP_MAGAZINE_MAX_SIZE - cells_count)
	{
		magazine_size += cells_count;
		return;
	}
	magazine_size = SHIP_MAGAZINE_MAX_SIZE;
	return;
}

bool Ship::IsUnbrakable() const
{
	return unbrakable;
}

Bonus Ship::LoseBonus()
{
	bonus_inventory &= BONUS_BONUS;
	if (bonus_inventory)
	{
		return Bonus(position, velocity, bonus_inventory);
	}
	return Bonus();
}

void Ship::Set(const Ship* ship)
{
	ControledEntity::Set(ship);

	bonus_inventory = ship->bonus_inventory;
	buff_inventory = ship->buff_inventory;
	burnout = ship->burnout;
	burnout_coldown = ship->burnout_coldown;
	magazine_size = ship->magazine_size;
	unbrakable = ship->unbrakable;
}

void Ship::Set(
	const Vec2F& position,
	const Vec2F& velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team,
	const GameTypes::control_flags_t* controle_flags,
	const std::vector<Vec2F>& heat_box_vertexes_array,
	float angle,
	EngineTypes::Bonus::inventory_t bonus_inventory,
	EngineTypes::Ship::inventory_t buff_inventory,
	GameTypes::tic_t unbrakable,
	GameTypes::tic_t burnout,
	GameTypes::tic_t burnout_coldown,
	float angular_velocity,
	float radius,
	float force_collision_coeffisient,
	float force_resistance_air_coefficient,
	GameTypes::entities_count_t max_bullets_count,
	GameTypes::entities_count_t current_bullets_count,
	bool exist)
{
	ControledEntity::Set(
	position,
		velocity,
		radius,
		player_number,
		player_team,
		controle_flags,
		heat_box_vertexes_array,
		angle,
		angular_velocity,
		force_collision_coeffisient,
		force_resistance_air_coefficient);

	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->burnout = burnout;
	this->burnout_coldown = burnout_coldown;
	this->magazine_size = max_bullets_count;
	this->unbrakable = unbrakable;
}

void Ship::SetSizeOfMagazine(GameTypes::entities_count_t bullets_count)
{
	magazine_size = bullets_count;
	if (bullets_in_magazine > magazine_size)
	{
		bullets_in_magazine = magazine_size;
	}
}

void Ship::SetUnbrakablePeriod(GameTypes::tic_t period)
{
	unbrakable = period;
}

bool Ship::SpendBonus(EngineTypes::Bonus::inventory_t bonus)
{
	if (IsHaveBonus(bonus))
	{
		bonus_inventory -= bonus;
		return true;
	}
	return false;
}

void Ship::SpendBonusNoCheck(EngineTypes::Bonus::inventory_t bonus)
{
	bonus_inventory -= bonus;
}

bool Ship::SpendBuff(EngineTypes::Ship::inventory_t buff)
{
	if (IsHaveBuff(buff))
	{
		buff_inventory &= SHIP_BUFF_ALL - buff;
		return true;
	}
	return false;
}

void Ship::SpendBuffNoCheck(EngineTypes::Ship::inventory_t buff)
{
	buff_inventory &= SHIP_BUFF_ALL - buff;
}

void Ship::StopCreatingLoop()
{
	objects_in_creating_proccess = 0;
}

void Ship::TakeBonus(Bonus* bonus, bool as_triple)
{
	EngineTypes::Bonus::inventory_t new_inventory = bonus->bonus_inventory;
	if (as_triple)
	{
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_CELLS_COUNT * 2; i += 2)
		{
			if (new_inventory & (BONUS_CELL << i))
			{
				bonus_inventory |= BONUS_CELL << i;
			}
		}
	}
	else
	{
		EngineTypes::Bonus::inventory_t count;
		for (EngineTypes::Bonus::inventory_length_t i = 0; i < BONUS_CELLS_COUNT * 2; i += 2)
		{
			count = (new_inventory & (BONUS_CELL << i)) >> i;
			if (count)
			{
				count += (bonus_inventory & (BONUS_CELL << i)) >> i;
				if (count > BONUS_CELL)
				{
					bonus_inventory |= BONUS_CELL << i;
				}
				else
				{
					bonus_inventory &= BONUS_ALL - (BONUS_CELL << i);
					bonus_inventory |= count << i;
				}
			}
		}
	}
}

void Ship::Update()
{
	if (burnout > 0)
	{
		AddForceAlongDirection(SHIP_BURNOUT_FORCE);
		burnout--;
	}
	if (unbrakable > 0)
	{
		unbrakable--;
	}
	if (burnout_coldown > 0)
	{
		burnout_coldown--;
	}

	if (reoading_dellay > 0)
	{
		reoading_dellay--;
	}
	else
	{
#if SHIP_DEFAULT_RELOADING_PERIOD == 0
		AddBulletsToMax();
#else
		AddBullet();
		reoading_dellay = SHIP_DEFAULT_RELOADING_PERIOD;
#endif
	}

	DynamicEntity::Update();
}

void Ship::UpdateMatrix()
{
	Mat3x2F* mm = GetModelMatrixPointer();
	mm->SetByPosition(position);
	mm->RotateThis(angle);
	mm->ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}

void Ship::operator=(const Ship& ship)
{
	ControledEntity::operator=(ship);

	this->bonus_inventory = ship.bonus_inventory;
	this->buff_inventory = ship.buff_inventory;
	this->burnout = ship.burnout;
	this->burnout_coldown = ship.burnout_coldown;
	this->magazine_size = ship.magazine_size;
	this->unbrakable = ship.unbrakable;
}

Ship::~Ship()
{
}
