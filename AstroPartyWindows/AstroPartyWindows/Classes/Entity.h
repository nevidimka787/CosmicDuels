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
class DecelerationArea;
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
	Vec2F position;
	Vec2F direction;
	float angle;
public:
	float radius;
	bool exist;
	Entity();
	Entity(const Entity& entity);
	Entity(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	float GetAngle();
	Vec2F GetDirectionNotNormalize();
	//Getting the distance between two closest points of objects.
	float GetDistance(Beam* beam);
	//Getting the distance between two closest points of objects.
	float GetDistance(Cyrcle* cyrcle);
	//Getting the distance between two closest points of objects.
	float GetDistance(DecelerationArea* deceler_area);
	//Getting the distance between two closest points of objects.
	float GetDistance(DynamicEntity* entity);
	//Getting the distance between two closest points of objects.
	float GetDistance(Entity* entity);
	//Getting the distance between two closest points of objects.
	float GetDistance(Line* line);
	//Getting the distance between two closest points of objects.
	float GetDistance(Polygon* polygon);
	//Getting the distance between two closest points of objects.
	float GetDistance(Rectangle* rectangle);
	//Getting the distance between two closest points of objects.
	float GetDistance(Segment* segment);
	//Getting the distance between two closest points of objects.
	float GetDistance(StaticEntity* entity);
	//Getting the distance between two closest points of objects.
	float GetDistance(Vec2F point);
	//Getting the distance between two closest points of objects.
	float GetDistance(Vec2F* point);
	float GetFrameSize(Entity* entity, float scale);
	Vec2F GetDirection();
	Vec2F GetPosition();
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Beam* beam);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Cyrcle* cyrcle);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(DecelerationArea* deceler_area);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(DynamicEntity* entity);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Entity* entity);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Line* line);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Map* map);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Polygon* polygon);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Rectangle* rectangle);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Segment* segment);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(StaticEntity* entity);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Vec2F point);
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(Vec2F* point);
	void Rotate(float angle);
	void Set(Entity* entity);
	void Set(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);
	void SetAngle(float angle);
	void SetDirection(Vec2F direction);
	void SetDirection(Vec2F* direction);
	void SetDirectionNotNormalize(Vec2F direction);
	void SetDirectionNotNormalize(Vec2F* direction);
	void SetPosition(Vec2F position);
	void SetPosition(Vec2F* position);
	void UpdateAngle();
	void UpdateDirection();
	//need manual call
	void Move(Vec2F delta);
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

	bool CollisionSide(Vec2F* p1, Vec2F* p2);
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

	void AddForce(Vec2F force);
	void AddForce(Vec2F* force);
	void AddForceAlongDirection(float force);
	void AddAngularVelocity(float angulat_velocity);
	void AddGravityForce(float gravity_coeffisient, Vec2F* forced_point);
	void AddGravityForce(float gravity_coeffisient, Vec2F forced_point);
	void AddVelocity(Vec2F velocity);
	void AddVelocity(Vec2F* velocity);
	/*
	If objects collide, function will be changing the physical parameters of those objects.
	*/
	//use f1
	//use v1 v2
	bool Collision(DynamicEntity* entity);
	//use f1
	//use v1 v2
	bool Collision(StaticEntity* entity);
	//use f1 f2
	//use v1 v2 v3 v4 v5
	//use s1 s2 s3 s4
	bool Collision(Rectangle* rectangle);
	bool Collision(Cyrcle* cyrcle);
	bool Collision(Polygon* polygon);
	bool Collision(Map* map);
	float GetAngularVelocity();
	Segment GetLastTreck();
	Segment GetTreck();
	Segment GetTreckDouble();
	Segment GetTreckDoubleRelative(DynamicEntity* entity);
	Segment GetTreckRelative(DynamicEntity* entity);
	Segment GetTreckRelativeDouble(DynamicEntity* entity);
	Vec2F GetVelocity();
	bool IsCollision(Vec2F point);
	bool IsCollision(Vec2F* point);
	bool IsCollision(Line line);
	bool IsCollision(Line* line);
	bool IsCollision(Beam beam);
	bool IsCollision(Beam* beam);
	bool IsCollision(Segment segment);
	bool IsCollision(Segment* segment);
	bool IsCollision(DynamicEntity* entity);
	bool IsCollision(StaticEntity* entity);
	bool IsCollision(Rectangle* rectangle);
	bool IsCollision(Cyrcle* cyrcle);
	bool IsCollision(Polygon* polygon);
	bool IsCollision(Map* map);
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
	void Update();

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
	void Set(StaticEntity* static_entity);
	void Set(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		bool exist = true);
	void Update();

	void operator=(StaticEntity static_entity);

	~StaticEntity();
};

class Particle : public DynamicEntity
{
public:
	GameTypes::tic_t life_time;

	Particle();

	~Particle();
};

class Bonus : public DynamicEntity
{

protected:
public:
	EngineTypes::Bonus::inventory_t bonus_inventory;
	Bonus();
	Bonus(bool exist);
	Bonus(const Bonus& bonus);
	Bonus(
		Vec2F* position,
		Vec2F* velocity,
		EngineTypes::Bonus::inventory_t bonus_inventory,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float radius = BONUS_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BONUS_DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	EngineTypes::Bonus::inventory_t BonusInfo();
	/*
		If an object has more than one bonus type,
		bonus type will be passed as a new object,
		otherwise the function will return 'nullptr'.
	*/
	Bonus Division();
	EngineTypes::Bonus::inventory_t GetType();
	uint8_t GetBonusesCount();
	uint8_t	GetBuffsCount();
	uint8_t GetGameRulesCount();
	uint8_t GetTypesCount();
	void Set(Bonus* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
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
	EngineTypes::Asteroid::size_t size;
public:
	Asteroid();
	Asteroid(const Asteroid& asteroid);
	Asteroid(
		Vec2F* position,
		Vec2F* velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
		EngineTypes::Asteroid::size_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void DecrementSize();
	Bonus Destroy();
	/*
	*The function creates a new smaller asteroid.
	*Remember to remove main asteroid.
	*/
	Asteroid Division();
	EngineTypes::Asteroid::size_t GetSize();
	EngineTypes::Bonus::inventory_t GetBuffBonus();
	void Set(Asteroid* asteroid);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
		EngineTypes::Asteroid::size_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void UpdateRadius();

	void operator=(Asteroid asteroid);

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
	bool CreatedBy(ControledEntity* controled_entity);
	bool CreatedByTeam(ControledEntity* controled_entity);
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	bool SameTeam(KillerEntity* killer_entity);
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
	Vec2F* heat_box_vertexes_array;
	Mat3x2F model_matrix;
	EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_array_length;
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
		Vec2F* heat_box_vertexes = nullptr,
		EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_count = 0,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	Mat3x2F GetModelMatrix();
	Mat3x2F* GetModelMatrixPointer();
	GameTypes::players_count_t GetPlayerNumber();
	GameTypes::players_count_t GetTeamNumber();
	bool GetRotateInputValue();
	bool GetShootInputValue();
	//Check collision this heat box.
	bool IsCollision(Beam* beam);
	//Check collision this heat box.
	bool IsCollision(Bomb* bullet);
	//Check collision this heat box.
	bool IsCollision(Bullet* bullet);
	//Check collision this heat box.
	bool IsCollision(DynamicEntity* bullet);
	//Check collision this heat box.
	bool IsCollision(Knife* knife);
	//Check collision this heat box.
	bool IsCollision(Laser* laser);
	//Check collision this heat box.
	bool IsCollision(MegaLaser* mega_laser);
	//Check collision this heat box.
	bool IsCollision(Segment* segment);
	//If map can destroy entity, the functuion return true.
	bool Collision(Map* map);
	bool IsColectEntity(Entity* stored_entity);
	bool IsColectEntity(DynamicEntity* stored_entity);
	bool SameTeams(ControledEntity* second_entity);
	void Set(ControledEntity* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		float radius,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		Vec2F* heat_box_vertexes = nullptr,
		EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_count = 0,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void UpdateMatrix();

	void operator=(ControledEntity entity);

	~ControledEntity();
};

class Ship : public ControledEntity
{
protected:
	void* burnout_input_value_pointer;
	EngineTypes::Bonus::inventory_t bonus_inventory;
	EngineTypes::Ship::inventory_t buff_inventory;
	GameTypes::tic_t unbrakable;
	GameTypes::tic_t burnout;
public:
	Ship();
	Ship(const Ship& ship);
	Ship(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* burnout_input_value_pointer,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		Vec2F* heat_box_vertexes = nullptr,
		EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_count = 0,
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t bonus_inventory = BONUS_NOTHING,
		EngineTypes::Ship::inventory_t buff_inventory = BONUS_NOTHING,
		GameTypes::tic_t unbrakable = SHIP_DEFAULT_UNBRAKABLE_PERIOD,
		GameTypes::tic_t burnout = 0.0f,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	//If ship have bufs in bonuses invenory,
	//the function activates all buffs that is in the bonus inventory and return true.
	bool ActivateAvailableBuffs();
	void ActivateBuffNoCheck(EngineTypes::Ship::inventory_t buff);
	EngineTypes::Bonus::inventory_t BonusInfo();
	void Burnout(
		float power,
		bool rotate_clockwise,
		GameTypes::tic_t burnout_period = SHIP_DEFAULT_BURNOUT_PERIOD);
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
	bool HaveBonus(EngineTypes::Bonus::inventory_t bonus);
	bool HaveBuff(EngineTypes::Ship::inventory_t buff);
	bool IsUnbrakable();
	Bonus LoseBonus();
	void Set(Ship* entity);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		void* burnout_input_value_pointer,
		void* rotate_input_value_pointer,
		void* shoot_input_value_pointer,
		Vec2F* heat_box_vertexes = nullptr,
		EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_count = 0,
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t bonus_inventory = BONUS_NOTHING,
		EngineTypes::Ship::inventory_t buff_inventory = BONUS_NOTHING,
		GameTypes::tic_t unbrakable = SHIP_DEFAULT_UNBRAKABLE_PERIOD,
		GameTypes::tic_t burnout = 0,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);
	void SetUnbrakablePeriod(GameTypes::tic_t period);
	//If ship have bonus, the function reduces the amount of this bonus and return true.
	bool SpendBonus(EngineTypes::Bonus::inventory_t bonus);
	//The function reduces the amount of this bonus.
	void SpendBonusNoCheck(EngineTypes::Bonus::inventory_t bonus);
	//If ship have buff, the function reduces this buff and return true.
	bool SpendBuff(EngineTypes::Ship::inventory_t bonus);
	//The function reduces this buff.
	void SpendBuffNoCheck(EngineTypes::Ship::inventory_t bonus);
	void TakeBonus(Bonus* bonus, bool as_triple);
	void Update();
	void UpdateMatrix();

	void operator=(Ship entity);

	~Ship();
};

class Pilot : public ControledEntity
{
protected:
	GameTypes::tic_t respawn_timer;
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
		GameTypes::tic_t respawn_timer = PILOT_DEFAULT_RESPAWN_TIMER,
		Vec2F* heat_box_vertexes = nullptr,
		EngineTypes::ControledEntity::heat_box_vertexes_count_t heat_box_vertexes_count = 0,
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t buffs_bonuses = BONUS_NOTHING,
		EngineTypes::Bonus::inventory_t active_baffs = BONUS_NOTHING,
		GameTypes::tic_t unbrakable = SHIP_DEFAULT_UNBRAKABLE_PERIOD,
		float angular_velocity = 0.0f,
		float radius = PILOT_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = PILOT_DEFAULT_FORCE_RESISTANCE_AIR_COEFFISIENT,
		bool exist = true);

	GameTypes::tic_t GetRespawnDellay();
	bool CanRespawn();
	Ship Respawn();
	void Set(Pilot* entity);
	void Update();
	void UpdateMatrix();

	void operator=(Pilot entity);

	~Pilot();
};

class SupportEntity : public StaticEntity
{
protected:
	ControledEntity* host_p;
	Mat3x2F* host_matrix_p;
	GameTypes::players_count_t host_number;
	GameTypes::players_count_t host_team;
	Vec2F local_position;
	Vec2F local_direction;
	float local_angle;

public:
	SupportEntity();
	SupportEntity(const SupportEntity& support_entity);
	SupportEntity(
		ControledEntity* host,
		Vec2F* position,
		float radius = 0.0f,
		float angle = 0.0f,
		bool exist = true);
	bool CreatedBy(ControledEntity* potencial_host);
	bool CreatedByTeam(ControledEntity* potencial_host);
	//return local angle
	float GetAngle();
	//return local direction
	Vec2F GetDirection();
	//return local direction
	Vec2F GetNormalizeDirection();
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	//return local position
	Vec2F GetPosition();
	void Set(SupportEntity* support_entity);
	void Set(
		ControledEntity* host,
		Vec2F position,
		float radius = 0.0f,
		float angle = 0.0f,
		bool exist = true);
	//set local angle
	void SetAngle(float angle);
	//set local direction
	void SetDirection(Vec2F direction);
	//set local direction
	void SetDirection(Vec2F* direction);
	void SetHost(ControledEntity* host);
	//set local direction
	void SetNotNormalizeDirection(Vec2F direction);
	//set local direction
	void SetNotNormalizeDirection(Vec2F* direction);
	//set local position
	void SetPosition(Vec2F position);
	//set local position
	void SetPosition(Vec2F* position);
	void Update();
	void UpdateDirection();

	void operator=(SupportEntity support_entity);

	~SupportEntity();
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
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count;
	AggressiveEntity();
	AggressiveEntity(const AggressiveEntity& aggressive_entity);
	AggressiveEntity(
		Vec2F* position,
		float radius,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
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
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool exist = true);

	void operator=(AggressiveEntity entity);

	~AggressiveEntity();
};

class DecelerationArea : public StaticEntity
{
public:
	float deceleration_parameter;

	DecelerationArea();
	DecelerationArea(const DecelerationArea& deceleration_area);
	DecelerationArea(
		Vec2F* position,
		float deseleration_parameter = DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		float radius = DECELERATION_AREA_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void Set(DecelerationArea* deceleration_area);
	void Set(
		Vec2F* position,
		float deseleration_parameter = DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		float radius = DECELERATION_AREA_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);
	void operator=(DecelerationArea deceleration_area);

	~DecelerationArea();
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

class Turel : public AggressiveEntity
{
public:
	Turel();
	Turel(const Turel& turel);
	Turel(
		Vec2F* position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TUREL_DEFAULT_RADIUS,
		bool exist = true);

	Bullet Shoot();
	void Set(Turel* entity);
	void Set(
		Vec2F* position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TUREL_DEFAULT_RADIUS,
		bool exist = true);

	void operator=(Turel entity);

	~Turel();
};

class MegaLaser : public AggressiveEntity
{
protected:
	bool active;
public:
	float width;
	MegaLaser();
	MegaLaser(const MegaLaser& mega_laser);
	MegaLaser(
		Segment* segment,
		float width = MEGA_LASER_DEFAULT_WIDTH,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool active = false,
		bool exist = true);

	Segment GetSegment();
	void Rotate(float angle);
	void Set(MegaLaser* entity);
	void Set(
		Segment* segment,
		float width = MEGA_LASER_DEFAULT_WIDTH,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool active = false,
		bool exist = true);
	bool IsShooting(GameTypes::tic_t current_tic);

	void operator=(MegaLaser entity);

	~MegaLaser();
};

class Bomb : public KillerEntity
{
protected:
	GameTypes::tic_t animation_tic;
	EngineTypes::Bomb::status_t status;
public:
	Bomb();
	Bomb(const Bomb& bomb);
	Bomb(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t master1_team_number,
		GameTypes::players_count_t master2_team_number,
		GameTypes::tic_t animation_tic = BOMB_DEFAULT_BOOM_DELLAY,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BOMB_DEFAULT_RADIUS,
		EngineTypes::Bomb::status_t status = BOMB_INACTIVE,
		bool exist = true);

	void Activate();
	void Boom();
	bool CanRemove();
	GameTypes::tic_t GetAnimationTic();
	bool IsActive();
	bool IsBoom();
	bool CreatedByAggressiveTeam();
	bool CreatedByAggressiveTeamOnly();
	bool CreatedByAggressiveTeamNotOnly();
	bool CreatedByTeam(ControledEntity* host);
	GameTypes::players_count_t GetTeamNumber(ControledEntity* not_host);
	//The function return true, if host will be decrrementing score after boom.
	bool IsAggressiveFor(ControledEntity* host);
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
		EngineTypes::Bomb::status_t status = BOMB_INACTIVE,
		bool exist = true);
	void Update();

	void operator=(Bomb bomb);

	~Bomb();
};

class Bullet : public KillerEntity
{
protected:
public:
	float min_velocity;
	EngineTypes::Bullet::entity_t is_ignore;

	Bullet();
	Bullet(const Bullet& bullet);
	Bullet(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		bool is_collision_master = true,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BULLET_DEFAULT_RADIUS,
		float min_velocity = BULLET_DEFAULT_MIN_VELOCITY,
		bool exist = true);
	bool Collision(Map* map);
	void Set(Bullet* bullet);
	void Set(
		Vec2F* position,
		Vec2F* velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		bool is_collision_master = true,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BULLET_DEFAULT_RADIUS,
		float min_velocity = BULLET_DEFAULT_MIN_VELOCITY,
		bool exist = true);
	void Update();

	void operator=(Bullet bullet);

	~Bullet();
};

class Knife : public SupportEntity
{
protected:
	EngineTypes::Knife::knife_health_t health;
public:
	Knife();
	Knife(const Knife& knife);
	Knife(
		ControledEntity* host,
		Segment* local_segment,
		EngineTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		bool exist = true);
	//The function checks collision between knife and all map's elemnts.
	//If map element is destructable, this element completes existing.
	//If after collision health of the knife is zero, the function set its parameter "exist" to false.
	bool Collision(Map* map);
	Segment GetSegment();
	void Set(Knife* knife);
	void Set(
		ControledEntity* host,
		Segment* local_segment,
		EngineTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		bool exist = true);
	//If health is zero, the function returns false.
	//Else the function decrements value of health.
	bool LoseHealth();

	void operator=(Knife knife);

	~Knife();
};

class Laser : public SupportEntity
{
protected:
	GameTypes::tic_t shoot_time;
public:
	float width;

	Laser();
	Laser(const Laser& laser);
	Laser(
		ControledEntity* host,
		Beam* local_beam,
		float width = LASER_DEFAULT_WIDTH,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		bool exist = true);

	//The function checks collision between knife and all map's elemnts.
	//If map element is destructable, this element completes existing.
	bool Collision(Map* map);
	bool IsActive();
	bool CreatedBy(ControledEntity* controled_entity);
	Beam GetBeam();
	GameTypes::tic_t GetLifeTime();
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	void Set(Laser* laser);
	void Set(
		ControledEntity* host,
		Beam* local_beam,
		float width = LASER_DEFAULT_WIDTH,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		bool exist = true);
	void Update();

	void operator=(Laser laser);

	~Laser();
};