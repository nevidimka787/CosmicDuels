#include "../Entity.h"



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
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
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
		player_team_number,
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		heat_box_vertexes_array,
		heat_box_vertexes_array_length,
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

Ship::Ship(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
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
		player_team_number,
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		heat_box_vertexes_array,
		heat_box_vertexes_array_length,
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
		position + direction.Normalize() * radius,
		direction * BULLET_DEFAULT_VELOCITY + velocity - direction.PerpendicularClockwise() * angular_velocity * radius,
		player_number,
		player_team_number,
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
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_ALL | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
		GetColor(),
		PARTICLE_PERIOD_EXAUST_ENGINE,
		PARTICLE_PERIOD_EXAUST_ENGINE,
		current_tic + PARTICLE_PERIOD_EXAUST_ENGINE);
}

DynamicParticle Ship::CreateEnginExaust(GameTypes::tic_t current_tic)
{
	return DynamicParticle(
		current_tic,
		position,
		velocity - direction * 0.001f,
		radius,
		angle,
		0.0f,
		force_collision_coeffisient,
		force_resistance_air_coefficient,
		PARTICLE_TYPE_EXAUST_ENGINE,
		DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS | DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_ALL | DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE,
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
			player_number,
			player_team_number);
	case 1:
		return Bullet(
			position - direction.PerpendicularClockwise() * radius * 0.75f,
			direction * BULLET_DEFAULT_VELOCITY + velocity,
			player_number,
			player_team_number);
	case 2:
		bullets_in_magazine -= 3;
		return Bullet(
			position + direction.PerpendicularClockwise() * radius * 0.75f,
			direction * BULLET_DEFAULT_VELOCITY + velocity,
			player_number,
			player_team_number);
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

Bullet Ship::CreateLoop(GameTypes::entities_count_t bullet_number)
{
	printf("WARNING::Ship::CreateLoop::The function is overdate. Use Ship::CreeatingEntities().");

	Vec2F bullet_velocity = velocity + Vec2F(0.0f, 1.0f).Rotate(2.0f * (float)M_PI / (float)SHIP_BULLETS_IN_LOOP * (float)bullet_number) * BULLET_DEFAULT_VELOCITY;
	return Bullet(&position, &bullet_velocity,
		player_number, player_team_number, true,
		0.0f, 0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT, BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT / 2.0f,
		BULLET_DEFAULT_RADIUS / 2.0f);
}

Bomb Ship::CreateBomb()
{
	return Bomb(
		Vec2F(-0.25f, 0.0f) * model_matrix,
		velocity,
		player_team_number,
		player_team_number);
}

Laser Ship::CreateLaser()
{
	Beam laser_beam = LASER_DEFAULT_LOCAL_BEAM;
	AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	return Laser(this, &laser_beam);
}

Knife Ship::CreateKnife(uint8_t knife_number)
{
	Segment new_knife_segment;
	switch (knife_number)
	{
	case 0:
		new_knife_segment = Segment(Vec2F(-0.35f, sqrt(3.0f) / 4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	case 1:
		new_knife_segment = Segment(Vec2F(-0.35f, sqrt(3.0f) / -4.0f), Vec2F(0.75f, 0.0f));
		return Knife(this, &new_knife_segment);
	default:
		return Knife();
	}
}

Particle Ship::CreateShootingExaust(GameTypes::tic_t current_tic)
{
	return Particle(
		current_tic,
		this,
		PARTICLE_TYPE_EXAUST_SHOOT,
		Color3F(1.0f, 0.0f, 0.0f));
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
		player_number,
		player_team_number,
		burnout_input_value_pointer,
		rotate_input_value_pointer,
		shoot_input_value_pointer,
		PILOT_DEFAULT_RESPAWN_TIMER,
		nullptr,
		0,
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
		return Bonus(&position, &velocity, bonus_inventory);
	}
	return Bonus();
}

void Ship::Set(const Ship* ship)
{
	angle = ship->angle;
	angular_velocity = ship->angular_velocity;
	bonus_inventory = ship->bonus_inventory;
	buff_inventory = ship->buff_inventory;
	burnout_input_value_pointer = ship->burnout_input_value_pointer;
	burnout = ship->burnout;
	burnout_coldown = ship->burnout_coldown;
	direction = ship->direction;
	exist = ship->exist;
	force_collision_coeffisient = ship->force_collision_coeffisient;
	force_resistance_air_coefficient = ship->force_resistance_air_coefficient;
	heat_box_vertexes_array_length = ship->heat_box_vertexes_array_length;
	player_number = ship->player_number;
	player_team_number = ship->player_team_number;
	position = ship->position;
	radius = ship->radius;
	rotate_input_value_pointer = ship->rotate_input_value_pointer;
	shoot_input_value_pointer = ship->shoot_input_value_pointer;
	unbrakable = ship->unbrakable;
	velocity = ship->velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = ship->heat_box_vertexes_array[vertex];
	}
}

void Ship::Set(
	Vec2F position,
	Vec2F velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
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
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->bullets_in_magazine = current_bullets_count;
	this->burnout = burnout;
	this->burnout_coldown = burnout_coldown;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->magazine_size = max_bullets_count;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->unbrakable = unbrakable;
	this->velocity = velocity;

	delete[] this->heat_box_vertexes_array;
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
	}
	else
	{
		this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
		for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
		{
			this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
		}
	}
}

void Ship::Set(
	const Vec2F* position,
	const Vec2F* velocity,
	GameTypes::players_count_t player_number,
	GameTypes::players_count_t player_team_number,
	const void* burnout_input_value_pointer,
	const void* rotate_input_value_pointer,
	const void* shoot_input_value_pointer,
	Vec2F* heat_box_vertexes_array,
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length,
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
	this->angle = angle;
	this->angular_velocity = angular_velocity;
	this->bonus_inventory = bonus_inventory;
	this->buff_inventory = buff_inventory;
	this->bullets_in_magazine = current_bullets_count;
	this->burnout = burnout;
	this->burnout_coldown = burnout_coldown;
	this->burnout_input_value_pointer = burnout_input_value_pointer;
	UpdateDirection();
	this->exist = exist;
	this->force_collision_coeffisient = force_collision_coeffisient;
	this->force_resistance_air_coefficient = force_resistance_air_coefficient;
	this->heat_box_vertexes_array_length = heat_box_vertexes_array_length;
	this->magazine_size = max_bullets_count;
	this->player_number = player_number;
	this->player_team_number = player_team_number;
	this->position = *position;
	this->radius = radius;
	this->rotate_input_value_pointer = rotate_input_value_pointer;
	this->shoot_input_value_pointer = shoot_input_value_pointer;
	this->unbrakable = unbrakable;
	this->velocity = *velocity;

	delete[] this->heat_box_vertexes_array;
	if (heat_box_vertexes_array_length < 3 || heat_box_vertexes_array == nullptr)
	{
		this->heat_box_vertexes_array = nullptr;
		SetDefaultMatrix();
	}
	else
	{
		this->heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
		for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
		{
			this->heat_box_vertexes_array[vertex] = heat_box_vertexes_array[vertex];
		}
	}
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

bool Ship::ShouldBurnout()
{
	return *(bool*)burnout_input_value_pointer;
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
	model_matrix.SetByPosition(position);
	model_matrix.RotateThis(angle);
	model_matrix.ScaleThis(Vec2F(4.5f, 3.0f) * radius);
}


void Ship::operator=(Ship ship)
{
	angle = ship.angle;
	angular_velocity = ship.angular_velocity;
	bonus_inventory = ship.bonus_inventory;
	buff_inventory = ship.buff_inventory;
	bullets_in_magazine = ship.bullets_in_magazine;
	burnout = ship.burnout;
	burnout_coldown = ship.burnout_coldown;
	direction = ship.direction;
	exist = ship.exist;
	force = ship.force;
	force_collision_coeffisient = ship.force_collision_coeffisient;
	force_resistance_air_coefficient = ship.force_resistance_air_coefficient;
	heat_box_vertexes_array_length = ship.heat_box_vertexes_array_length;
	magazine_size = ship.magazine_size;
	player_number = ship.player_number;
	player_team_number = ship.player_team_number;
	position = ship.position;
	radius = ship.radius;
	rotate_input_value_pointer = ship.rotate_input_value_pointer;
	shoot_input_value_pointer = ship.shoot_input_value_pointer;
	unbrakable = ship.unbrakable;
	velocity = ship.velocity;

	delete[] heat_box_vertexes_array;
	heat_box_vertexes_array = new Vec2F[heat_box_vertexes_array_length];
	for (EngineTypes::ControledEntity::heat_box_vertexes_count_t vertex = 0; vertex < heat_box_vertexes_array_length; vertex++)
	{
		heat_box_vertexes_array[vertex] = ship.heat_box_vertexes_array[vertex];
	}
}

Ship::~Ship()
{
}