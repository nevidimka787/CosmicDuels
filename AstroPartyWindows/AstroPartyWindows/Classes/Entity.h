#pragma once

#include <iostream>

#include "Math.h"
#include "Map.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

class Entity;
class StaticEntity;
class MegaLaser;
class AggressiveEntity;
class Turel;
class GravGen;
class DynamicEntity;
class KillerEntity;
class Bullet;
class Laser;
class Knife;
class Bomb;
class Bonus;
class ControledEntity;
class Ship;
class Pilot;

class Entity
{
protected:
	float angle;
	Vec2F position;
	Vec2F direction;
public:
	bool exist;
	float radius;
	Entity();
	Entity(const Entity& entity);
	Entity(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	float GetAngle();
	/*
	Getting the distance between two closest points of objects.
	*/
	float GetDistance(Entity* entity);
	float GetDistance(Vec2F* point);
	float GetDistance(Line* line);
	float GetDistance(Beam* beam);
	float GetDistance(Segment* segment);
	float GetDistance(DynamicEntity* entity);
	float GetDistance(StaticEntity* entity);
	float GetDistance(Rectangle* rectangle);
	float GetDistance(Cyrcle* cyrcle);
	float GetDistance(Polygon* polygon);
	Vec2F GetPosition();
	Vec2F GetDirection();
	void Rotate(float angle);
	void Set(Entity* entity);
	void Set(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);
	void SetAngle(float angle);
	void SetDirection(Vec2F* direction);
	void SetPosition(Vec2F* position);
	bool IsCollision(Vec2F* point);
	bool IsCollision(Line* line);
	bool IsCollision(Beam* beam);
	bool IsCollision(Segment* segment);
	bool IsCollision(DynamicEntity* entity);
	bool IsCollision(StaticEntity* entity);
	bool IsCollision(Rectangle* rectangle);
	bool IsCollision(Cyrcle* cyrcle);
	bool IsCollision(Polygon* polygon);
	bool IsCollision(Map* map);
	void UpdateAngle();
	void UpdateDirection();
	void Move(Vec2F* delta);

	void operator=(Entity entity);

	~Entity();
};

class DynamicEntity : public Entity
{
protected:
	float angular_velocity;
	Vec2F velocity;
	//This variable set only by force functions.
	Vec2F force;
public:
	float force_collision_coeffisient;
	float force_resistance_air_coefficient;
	DynamicEntity();
	DynamicEntity(const DynamicEntity& dynamic_entity);
	DynamicEntity(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void AddForce(Vec2F* force);
	void AddForceAlongDirection(float force);
	void AddAngularVelocity(float angulat_velocity);
	void AddGravityForce(float gravity_coeffisient, Entity* forced_entity);
	void AddGravityForce(float gravity_coeffisient, Vec2F* forced_point);
	/*
	If objects collide, function will be changing the physical parameters of those objects.
	*/
	bool Collision(DynamicEntity* entity);
	bool Collision(StaticEntity* entity);
	bool Collision(Rectangle* rectangle);
	bool Collision(Cyrcle* cyrcle);
	bool Collision(Polygon* polygon);
	bool Collision(Map* map);
	float GetAngularVelocity();
	Vec2F GetVelocity();
	void Recalculate();
	void Set(DynamicEntity* dynamic_entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void SetAngularVelocity(float angulat_velocity);
	void SetCoefficients(
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT);
	void SetVelocity(Vec2F* velocity);

	void operator=(DynamicEntity dynamic_entity);

	~DynamicEntity();
};

class StaticEntity : public Entity
{
protected:
	Vec2F last_position;
public:
	StaticEntity();
	StaticEntity(const StaticEntity& static_entity);
	StaticEntity(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	Vec2F GetVelocity();
	void Recalculate();
	void Set(StaticEntity* static_entity);
	void Set(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	void operator=(StaticEntity static_entity);

	~StaticEntity();
};

class Bonus : public DynamicEntity
{

protected:
public:
	ClassTypes::Bonus::bonus_t bonus_type;
	Bonus();
	Bonus(const Bonus& bonus);
	Bonus(
		Vec2F* position,
		Vec2F* velocity,
		ClassTypes::Bonus::bonus_t bonus_type,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float radius = BONUS_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	/*
		If an object has more than one bonus type,
		bonus type will be passed as a new object,
		otherwise the function will return 'nullptr'.
	*/
	Bonus Division();
	ClassTypes::Bonus::bonus_t GetType();
	uint8_t GetBonusesCount();
	uint8_t	GetBuffsCount();
	uint8_t GetGameRulesCount();
	uint8_t GetTypesCount();
	void Set(Bonus* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		ClassTypes::Bonus::bonus_t bonus_type,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float radius = BONUS_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(Bonus entity);

	~Bonus();
};

class Asteroid : public Bonus
{
protected:
	uint8_t size;
public:
	Asteroid();
	Asteroid(const Asteroid& asteroid);
	Asteroid(
		Vec2F* position,
		Vec2F* velocity,
		ClassTypes::Bonus::bonus_t bonus_type,
		uint8_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	Bonus Destroy();
	/*
	*The function creates a new smaller asteroid.
	*Remember to remove main asteroid.
	*/
	Asteroid Division();
	uint8_t GetSize();
	uint16_t GetBuffBonus();
	void Set(Asteroid* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		ClassTypes::Bonus::bonus_t bonus_type,
		uint8_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(Asteroid entity);

	~Asteroid();
};

class KillerEntity : public DynamicEntity
{
protected:
	GameTypes::players_count_t player_master_number;
	GameTypes::players_count_t player_master_team_number;
public:
	KillerEntity();
	KillerEntity(const KillerEntity& killer_entity);
	KillerEntity(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	void Set(KillerEntity* killer_entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(KillerEntity killer_entity);

	~KillerEntity();
};

class ControledEntity : public DynamicEntity
{
protected:
	GameTypes::players_count_t player_number;
	GameTypes::players_count_t player_team_number;
	void* rotate_input_value_pointer;
	void* shoot_input_value_pointer;
public:
	ControledEntity();
	ControledEntity(const ControledEntity& controled_entity);
	ControledEntity(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	GameTypes::players_count_t GetPlayerNumber();
	GameTypes::players_count_t GetTeamNumber();
	bool GetRotateInputValue();
	bool GetShootInputValue();
	void Set(ControledEntity* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(ControledEntity entity);

	~ControledEntity();
};

class Ship : public ControledEntity
{
protected:
	ClassTypes::Bonus::bonus_t buffs_bonuses;
	ClassTypes::Bonus::bonus_t active_baffs;
	GameTypes::tic_t unbrakable;
public:
	Ship();
	Ship(const Ship& ship);
	Ship(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		float angle = 0.0f,
		ClassTypes::Bonus::bonus_t buffs_bonuses = BONUS_NO_BONUS,
		ClassTypes::Bonus::bonus_t active_baffs = BONUS_NO_BONUS,
		GameTypes::tic_t unbrakable = SHIP_UNBRAKABLE_PERIOD,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void ActivateBuffs();
	void BreakShield();
	Bullet CreateBullet();
	//The function does not check for the presence of a bonus.
	Bullet CreateTriple(uint8_t bullet_number);
	//The function does not check for the presence of a bonus.
	Bullet CreateLoop(GameTypes::entities_count_t bullet_number);
	//The function does not check for the presence of a bonus.
	Bomb CreateBomb();
	//The function does not check for the presence of a bonus.
	Laser CreateLaser();
	//The function does not check for the presence of a bonus.
	Knife CreateKnife(uint8_t knife_number);
	Pilot Destroy();
	ClassTypes::Bonus::bonus_t GetActiveBaffs();
	bool HaveBonus(ClassTypes::Bonus::bonus_t bonus);
	Bonus LoseBonus();
	void Recalculate();
	//If ship have bonus, the function reduces the amount of this bonus and return true.
	bool SpendBonus(ClassTypes::Bonus::bonus_t bonus);
	//The function reduces the amount of this bonus.
	void SpendBonusNoCheck(ClassTypes::Bonus::bonus_t bonus);
	void Set(Ship* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		float angle = 0.0f,
		ClassTypes::Bonus::bonus_t buffs_bonuses = BONUS_NO_BONUS,
		ClassTypes::Bonus::bonus_t active_baffs = BONUS_NO_BONUS,
		GameTypes::tic_t unbrakable = SHIP_UNBRAKABLE_PERIOD,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void TakeBonus(Bonus* bonus);

	void operator=(Ship entity);

	~Ship();
};

class Pilot : public ControledEntity
{
protected:
public:
	Pilot();
	Pilot(const Pilot& pilot);
	Pilot(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		float angle = 0.0f,
		ClassTypes::Bonus::bonus_t buffs_bonuses = BONUS_NO_BONUS,
		ClassTypes::Bonus::bonus_t active_baffs = BONUS_NO_BONUS,
		GameTypes::tic_t unbrakable = SHIP_UNBRAKABLE_PERIOD,
		float angular_velocity = 0.0f,
		float radius = PILOT_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	Ship Respawn();
	void Set(Pilot* entity);

	void operator=(Pilot entity);

	~Pilot();
};

//Killer entity with out host.
class AggressiveEntity : public StaticEntity
{
protected:
	//Dellay to first attack after spawn.
	GameTypes::tic_t attack_dellay;
public:
	//Period of all attack cycle.
	GameTypes::tic_t attack_period;
	//Period between shoots.
	GameTypes::tic_t inactive_period;
	//Shoots count in one attack period.
	ClassTypes::AgressiveEntity::shoots_count_t shoots_count;
	AggressiveEntity();
	AggressiveEntity(const AggressiveEntity& aggressive_entity);
	AggressiveEntity(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool exist = true);
	bool CanShoot(GameTypes::tic_t current_tic);
	void PostponeAttack(GameTypes::tic_t dellay);
	void Set(AggressiveEntity* entity);
	void Set(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool exist = true);

	void operator=(AggressiveEntity entity);

	~AggressiveEntity();
};

class Turel : public AggressiveEntity
{
public:
	Turel();
	Turel(const Turel& turel);
	Turel(
		Vec2F* position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TUREL_DEFAULT_RADIUS,
		bool exist = true);

	Bullet Shoot();
	void Set(Turel* entity);
	void Set(
		Vec2F* position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TUREL_DEFAULT_RADIUS,
		bool exist = true);

	void operator=(Turel entity);

	~Turel();
};

class GravGen : public StaticEntity
{
public:
	float gravity;

	GravGen();
	GravGen(const GravGen& grav_gen);
	GravGen(
		Vec2F* position,
		float gravity = GRAVITY_GENERATOR_DEFAULT_GRAVITY,
		float radius = GRAVITY_GENERATOR_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);
	void Set(GravGen* grav_gen);
	void Set(
		Vec2F* position,
		float gravity = GRAVITY_GENERATOR_DEFAULT_GRAVITY,
		float radius = GRAVITY_GENERATOR_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void operator=(GravGen grav_gen);

	~GravGen();
};

class MegaLaser : public AggressiveEntity
{
protected:
	bool active;
public:
	MegaLaser();
	MegaLaser(const MegaLaser& mega_laser);
	MegaLaser(
		Segment* segment,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool active = false,
		bool exist = true);

	Segment GetSegment();
	void Rotate(float angle);
	void Set(MegaLaser* entity);
	void Set(
		Segment* segment,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		ClassTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool active = false,
		bool exist = true);
	bool IsShooting();

	void operator=(MegaLaser entity);

	~MegaLaser();
};

class Laser : public StaticEntity
{
protected:
	GameTypes::players_count_t player_master_number;
	GameTypes::players_count_t player_master_team_number;
	GameTypes::tic_t shoot_time;
public:
	Laser();
	Laser(const Laser& laser);
	Laser(
		Beam* beam,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		bool exist = true);
	bool CanShoot(GameTypes::tic_t current_tic);
	Beam GetBeam();
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	void Recalculate();
	void Set(Laser* laser);
	void Set(
		Beam* beam,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		bool exist = true);

	void operator=(Laser laser);

	~Laser();
};

class Bullet : public KillerEntity
{
protected:
public:
	Bullet();
	Bullet(const Bullet& bullet);
	Bullet(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BULLET_DEFAULT_RADIUS,
		bool exist = true);
	bool IsCollision(Map* map);
	void Set(Bullet* bullet);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BULLET_DEFAULT_RADIUS,
		bool exist = true);

	void operator=(Bullet bullet);

	~Bullet();
};

class Knife : public KillerEntity
{
protected:
	ClassTypes::Knife::knife_health_t health;
public:
	Knife();
	Knife(const Knife& knife);
	Knife(
		Segment* segment,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		ClassTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = KNIFE_DEFAULT_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	Segment GetSegment();
	void Set(Knife* knife);
	void Set(
		Segment* segment,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		ClassTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = KNIFE_DEFAULT_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	//The function will return false when health is zero.
	bool LoseHealth();

	void operator=(Knife knife);

	~Knife();
};

class Bomb : public KillerEntity
{
protected:
	GameTypes::tic_t animation_tic;
	bool active;
	bool boom;
public:
	Bomb();
	Bomb(const Bomb& bomb);
	Bomb(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		GameTypes::tic_t animation_tic = BOMB_DEFAULT_BOOM_DELLAY,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BOMB_DEFAULT_RADIUS,
		bool active = false,
		bool boom = false,
		bool exist = true);

	void Activate();
	void Boom();
	bool IsActive();
	bool IsBoom();
	bool CanRemove();
	void Recalculate();
	void Set(Bomb* bomb);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		GameTypes::tic_t animation_tic = BOMB_DEFAULT_BOOM_DELLAY,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BOMB_DEFAULT_RADIUS,
		bool active = false,
		bool boom = false,
		bool exist = true);

	void operator=(Bomb bomb);

	~Bomb();
};