#pragma once

#include <iostream>
#include <vector>

#include "Math.h"
#include "Map.h"
#include "Color.h"
#include "Buffer.h"

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"

#define M_PI 3.14159265358979323846

class AggressiveEntity;
class AnnihAreaGen;
class Bomb;
class Bonus;
class Bullet;
class ControledEntity;
class DecelerationArea;
class DynamicEntity;
class DynamicParticle;
class Entity;
class GravGen;
class KillerEntity;
class Knife;
class Laser;
class Particle;
class Pilot;
class Portal;
class Ship;
class StaticEntity;
class SupportEntity;
class Turret;

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
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	float GetAngle() const;
	Vec2F GetDirectionNotNormalize() const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Beam& beam) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Map::Cyrcle& cyrcle, bool* is_inside = nullptr) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const DecelerationArea& deceler_area) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const DynamicEntity& entity) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Entity& entity) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Line& line) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Map::Polygon& polygon) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Map::Rectangle& rectangle) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Segment& segment) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const StaticEntity& entity) const;
	//Getting the distance between two closest points of objects.
	float GetDistance(const Vec2F& point) const;
	float GetFrameSize(const Entity& entity, float scale) const;
	Vec2F GetDirection() const;
	Vec2F GetPosition() const;
	const Vec2F* GetPositionPointer() const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Beam& beam) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Map::Cyrcle& cyrcle) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const DecelerationArea& deceler_area) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const DynamicEntity& entity) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Entity& entity) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Line& line) const;
	bool IsCollision(const Laser& laser) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Map::MapData& map) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Map::Polygon& polygon) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Map::Rectangle& rectangle) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Segment& segment) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const StaticEntity& entity) const;
	//If distance between two objects is less then zero, the function return true.
	bool IsCollision(const Vec2F& point) const;
	bool IsInside(const Map::Rectangle& rectangle) const;
	void Rotate(float angle);
	void Set(const Entity* entity);
	void Set(
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		bool exist = true);
	void SetAngle(float angle);
	void SetDirection(const Vec2F& direction);
	void SetDirectionNotNormalize(const Vec2F& direction);
	void SetPosition(const Vec2F& position);
	void UpdateAngle();
	void UpdateDirection();
	//need manual call
	void Move(const Vec2F& delta);

	void operator=(const Entity& entity);

	~Entity();
};

class DynamicEntity : public Entity
{
private:
	bool CollisionInside(const Map::Rectangle& rectangle);
	bool CollisionOutside(const Map::Rectangle& rectangle);
	bool CollisionCheckIntersections(const Map::Rectangle& rectangle, Vec2F& new_position, Vec2F& force_vector);
	bool CollisionCheckPosition(const Map::Rectangle& rectangle, Vec2F& new_position, Vec2F& force_vector);
	Vec2F GetCollisionDirection(const Vec2F& point, const Vec2F& neares_point) const;

protected:
	float angular_velocity;
	Vec2F velocity;
	//This variable set only by force functions.
	Vec2F force;

public:
	float force_collision_coeffisient;
	//the distance the ship will be pushed to the side
	float force_resistance_air_coefficient;
	DynamicEntity();
	DynamicEntity(const DynamicEntity& dynamic_entity);
	DynamicEntity(
		const Vec2F& position,
		const Vec2F& velocity,
		float radius,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void AddForce(const Vec2F& force);
	void AddForceAlongDirection(float force);
	void AddAngularVelocity(float angulat_velocity);
	void AddGravityForce(float gravity_coeffisient, const Vec2F& forced_point);
	void AddVelocity(const Vec2F& velocity);
	//The function sets the sum of all forces those affecting to this entity to zero.
	void ClearForce();
	/*
	If objects collide, function will be changing the physical parameters of those objects.
	*/
	//use f1
	//use v1 v2
	bool Collision(DynamicEntity& entity);
	//use f1
	//use v1 v2
	bool Collision(const StaticEntity& entity);
	//use f1 f2
	//use v1 v2 v3 v4 v5
	//use s1 s2 s3 s4
	bool Collision(const Map::Rectangle& rectangle);
	bool Collision(const Map::Cyrcle& cyrcle);
	bool Collision(const Map::Polygon& polygon);
	bool Collision(const Map::MapData& map);
	float GetAngularVelocity() const;
	Segment GetLastTreck() const;
	Vec2F GetCollisionDirection(const Vec2F& point) const;
	Segment GetTreck() const;
	Segment GetTreckDouble() const;
	Segment GetTreckDoubleRelative(const DynamicEntity& entity) const;
	Segment GetTreckRelative(const DynamicEntity& entity) const;
	Segment GetTreckRelativeDouble(const DynamicEntity& entity) const;
	Vec2F GetVelocity() const;

	bool IsTooSlow() const;
	bool IsTooSlow(const Vec2F& relative_velocity, const float& other_radius = 0.0f) const;

	template <typename ThingT>
	bool IsCollision(const ThingT& point) const;

	void Set(const DynamicEntity* dynamic_entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
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
	void SetVelocity(const Vec2F& velocity);
	void Update();

	void operator=(const DynamicEntity& dynamic_entity);

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
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	Vec2F GetVelocity() const;
	void Set(const StaticEntity* static_entity);
	void Set(
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		bool exist = true);

	void SetPosition(Vec2F position);
	void SetPosition(const Vec2F* position);
	void Update();

	void operator=(const StaticEntity& static_entity);

	~StaticEntity();
};

class Particle : public Entity
{
protected:
	bool active = false;
	GameTypes::tic_t animation_period;
	GameTypes::tic_t animation_postpone;
	GameTypes::tic_t finish_tic;
	bool linked;
	GameTypes::tic_t spawn_tic;
	EngineTypes::Particle::type_t type;

	void SetAutoPeriod(EngineTypes::Particle::type_t type);
	void SetAutoPostpone(EngineTypes::Particle::type_t type);
public:
	float animation = 0.0f;
	Color3F color;
	const Entity* pointer_to_entity;

	Particle();
	Particle(
		GameTypes::tic_t current_tic,
		const Entity* pointer_to_host,
		EngineTypes::Particle::type_t type,
		const Color3F& color,
		GameTypes::tic_t animation_period = PARTICLE_PROPERTY_AUTO,
		GameTypes::tic_t animation_postpone = PARTICLE_PROPERTY_AUTO,
		GameTypes::tic_t finish_tic = PARTICLE_PROPERTY_AUTO,
		bool exist = true);
	Particle(
		GameTypes::tic_t current_tic,
		const Vec2F& position,
		float angle,
		float radius,
		EngineTypes::Particle::type_t type,
		const Color3F& color,
		GameTypes::tic_t animation_period,
		GameTypes::tic_t animation_postpone,
		GameTypes::tic_t finish_tic,
		bool exist = true);

	//If particle active or can be active, then function return true.
	void Activate(GameTypes::tic_t current_tic);
	bool CanRemove(GameTypes::tic_t current_tic) const;
	EngineTypes::Particle::type_t GetType() const;
	bool IsActive() const;
	void Link(Entity* new_pointer);
	void Unlink();
	void Update(GameTypes::tic_t current_tic);

	void operator=(const Particle& particle);

	~Particle();
};

class DynamicParticle : public DynamicEntity
{
private:
	bool active = false;
	GameTypes::tic_t animation_period;
	GameTypes::tic_t animation_postpone;
	GameTypes::tic_t finish_tic;
	GameTypes::tic_t spawn_tic;
	EngineTypes::Particle::type_t type;
public:
	float animation;
	Color3F color;
	EngineTypes::DynamicParticle::property_t properties;

	DynamicParticle();
	DynamicParticle(
		GameTypes::tic_t current_tic,
		const Vec2F& position,
		const Vec2F& velocisy,
		float radius,
		float angle,
		float angular_velocity,
		float force_collision_coeffisient,
		float force_resistance_air_coefficient,
		EngineTypes::Particle::type_t type,
		EngineTypes::DynamicParticle::property_t properties,
		const Color3F& color,
		GameTypes::tic_t animation_period = PARTICLE_PROPERTY_AUTO,
		GameTypes::tic_t animation_postpone = PARTICLE_PROPERTY_AUTO,
		GameTypes::tic_t finish_tic = PARTICLE_PROPERTY_AUTO,
		bool exist = true);

	bool Activate(GameTypes::tic_t current_tic);
	bool CanRemove(GameTypes::tic_t current_tic) const;
	EngineTypes::Particle::type_t GetType() const;
	bool IsActive() const;
	void Update(GameTypes::tic_t current_tic);

	void operator=(const DynamicParticle& dynamical_entity);

	~DynamicParticle();
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
		const Vec2F& position,
		const Vec2F& velocity,
		EngineTypes::Bonus::inventory_t bonus_inventory,
		float angle = 0.0f,
		float angular_velocity = BONUS_DEFAULT_ANGULAR_VELOCITY,
		float radius = BONUS_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BONUS_DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	EngineTypes::Bonus::inventory_t BonusInfo() const;
	/*
		If an object has more than one bonus type,
		bonus type will be passed as a new object,
		otherwise the function will return 'nullptr'.
	*/
	//If bonus collent more then one item then function returns true.
	bool CanDivision() const;
	//The function returns new bonus that collect one item in the its inventory and remove that item from the inventory of the main bonus.
	Bonus Division();
	EngineTypes::Bonus::inventory_t GetType();
	uint8_t GetBonusesCount() const;
	uint8_t	GetBuffsCount() const;
	uint8_t GetGameRulesCount() const;
	void Set(const Bonus* entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
		float angle = 0.0f,
		float angular_velocity = BONUS_DEFAULT_ANGULAR_VELOCITY,
		float radius = BONUS_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(const Bonus& entity);

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
		const Vec2F& position,
		const Vec2F& velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
		EngineTypes::Asteroid::size_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	DynamicParticle CreateShards(GameTypes::tic_t current_tic) const;
	bool Collision(const Map::MapData& map);
	void DecrementSize();
	Bonus Destroy();
	/*
	*The function creates a new smaller asteroid.
	*Remember to remove main asteroid.
	*/
	Asteroid Division();
	EngineTypes::Asteroid::size_t GetSize() const;
	EngineTypes::Bonus::inventory_t GetBuffBonus() const;
	void Set(const Asteroid* asteroid);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		EngineTypes::Bonus::inventory_t bonus_type,
		EngineTypes::Asteroid::size_t size = ASTEROID_DEFAULT_SIZE,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void UpdateRadius();

	void operator=(const Asteroid& asteroid);

	~Asteroid();
};

class KillerEntity : public DynamicEntity
{
protected:
	GameTypes::players_count_t host_number;
	GameTypes::players_count_t host_team_number;
public:

	KillerEntity();
	KillerEntity(const KillerEntity& killer_entity);
	KillerEntity(
		const Vec2F& position,
		const Vec2F& velocity,
		float radius,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	bool CreatedBy(const ControledEntity& controled_entity) const;
	bool CreatedByTeam(const ControledEntity& controled_entity) const;
	GameTypes::players_count_t GetHostNumber() const;
	GameTypes::players_count_t GetHostTeamNumber() const;
	bool SameTeam(const KillerEntity& killer_entity) const;
	void Set(const KillerEntity* killer_entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		float radius,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	void operator=(const KillerEntity& killer_entity);

	~KillerEntity();
};

class ControledEntity : public DynamicEntity
{
private:
	std::vector<Vec2F> heat_box_vertexes_array;
	Mat3x2F model_matrix;
	GameTypes::players_count_t player_number;
	GameTypes::players_count_t player_team_number;
	const GameTypes::control_flags_t* controle_flags_pointer;
	
	void SetDefaultMatrix();

protected:
	Mat3x2F* GetModelMatrixPointer();

public:
	ControledEntity();
	ControledEntity(const ControledEntity& controled_entity);
	ControledEntity(
		const Vec2F& position,
		const Vec2F& velocity,
		float radius,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	//If map can destroy entity, the functuion return true.
	bool Collision(const Map::MapData& map);
	template<typename MapElementT>
	bool CollisionWithElement(MapElementT& element);
	Color3F GetColor() const;
	const GameTypes::control_flags_t* GetControleFlagsP() const;
	std::vector<Segment> GetGlobalHeatBox() const;
	std::vector<Segment> GetLocalHeatBox() const;
	Mat3x2F GetModelMatrix() const;
	const Mat3x2F* GetModelMatrixPointerConst() const;
	GameTypes::players_count_t GetPlayerNumber() const;
	GameTypes::players_count_t GetTeamNumber() const;
	//Check collision this heat box.
	template <typename Type>
	bool IsCollision(const Type& thing) const;
	bool IsColectEntity(const DynamicEntity& stored_entity) const;
	bool IsSameTeams(const ControledEntity& second_entity) const;
	bool IsTooSlow() const;
	void Set(const ControledEntity* entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		float radius,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		bool exist = true);

	bool ShouldDash() const;
	bool ShouldRotate() const;
	bool ShouldShoot() const;
	void UpdateMatrix();

	void operator=(const ControledEntity& entity);

	~ControledEntity();
};

class Ship : public ControledEntity
{
private:
	//Value of the coldown of burnout. If value is zero then the ship can bornout.
	GameTypes::tic_t burnout_coldown;
	//Count of objects that ship should generate in next tics.
	GameTypes::entities_count_t objects_in_creating_proccess;
	//value of the variable will not translate to other entities
	GameTypes::objects_types_count_t element_type;

	GameTypes::tic_t shoot_cooldown_time = GAME_DELLAY_BETWEEN_SHOOTS;
protected:
	EngineTypes::Bonus::inventory_t bonus_inventory;
	EngineTypes::Ship::inventory_t buff_inventory;
	GameTypes::tic_t unbrakable;
	GameTypes::tic_t burnout;

	GameTypes::entities_count_t magazine_size;
	GameTypes::entities_count_t bullets_in_magazine;
	GameTypes::tic_t reoading_dellay = 0;
public:
	Ship();
	Ship(const Ship& ship);
	Ship(
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t bonus_inventory = BONUS_NOTHING,
		EngineTypes::Ship::inventory_t buff_inventory = BONUS_NOTHING,
		GameTypes::tic_t unbrakable = SHIP_DEFAULT_UNBRAKABLE_PERIOD,
		GameTypes::tic_t burnout = 0.0f,
		GameTypes::tic_t burnout_coldown = SHIP_DEFAULT_BURNOUT_COLDOWN,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		GameTypes::entities_count_t max_bullets_count = SHIP_DEFAULT_MAGAZINE_SIZE,
		GameTypes::entities_count_t start_bullets_count = SHIP_DEFAULT_MAGAZINE_SIZE,
		bool exist = true);

	//If ship have bufs in bonuses invenory,
	//the function activates all buffs that is in the bonus inventory and return true.
	bool ActivateAvailableBuffs();
	void ActivateBuffNoCheck(EngineTypes::Ship::inventory_t buff);
	//The function adds one bullet to ship's magazine.
	void AddBullet();
	//The function adds indicated count of bullets to ship's magazine.
	void AddBullets(GameTypes::entities_count_t bulles_count);
	//The function fills ship's magazine with bullets.
	void AddBulletsToMax();
	EngineTypes::Bonus::inventory_t BonusInfo();
	void Burnout(
		float power,
		bool rotate_clockwise,
		GameTypes::tic_t burnout_period = SHIP_DEFAULT_BURNOUT_PERIOD);
	bool CanCreatingBullet() const;
	bool CanCreatingObject() const;
	//Thw ship loses all buffs, bonuses, and bullets.
	void ClearInventory();
	Bullet CreateBullet();
	// The function return dynamic particle.
	DynamicParticle CreateBurnoutExaust(GameTypes::tic_t current_tic);
	// The function return dynamic particle.
	DynamicParticle CreateEnginExaust(GameTypes::tic_t current_tic);
	// The function return dynamic particle.
	DynamicParticle CreateShards(GameTypes::tic_t current_tic);
	// the function cteare dynamic particle.
	DynamicParticle CreateShootingExaust(GameTypes::tic_t current_tic);
	//The function does not check for the presence of a bonus.
	Bullet CreateTriple(uint8_t bullet_number);
	//The function print data about loop to ship's memory.
	//If ship already has the data then the function return false
	//else the function return true.
	bool CreatingEntities(
		GameTypes::entities_count_t objects_count,	//count of objects in creating loop
		GameTypes::objects_types_count_t object_type	//type of elemnts in creating loop
	);
	//The function does not check for the presence of a bonus.
	Bomb CreateBomb();
	//The function does not check for the presence of a bonus.
	Laser CreateLaser();
	//The function does not check for the presence of a bonus.
	Knife CreateKnife(uint8_t knife_number);
	void DecrementSizeOfMagasize(GameTypes::entities_count_t cells_count = 1);
	//Create pilot
	Pilot Destroy();
	int GetBonusInventoryAsBoolList() const;
	GameTypes::entities_count_t GetBulletsCountInMagasine() const;
	GameTypes::entities_count_t GetSizeOfMagazine() const;
	//The function return number of curent element.
	//Last number of entity is 1.
	GameTypes::entities_count_t GetElemntFromList();
	GameTypes::objects_types_count_t GetTypeOfElemntInLoop() const;
	bool IsHaveBonus(EngineTypes::Bonus::inventory_t bonus) const;
	bool IsHaveBuff(EngineTypes::Ship::inventory_t buff) const;
	void IncrementSizeOfMagazine(GameTypes::entities_count_t cells_count = 1);
	bool IsUnbrakable() const;
	Bonus LoseBonus();
	void Set(const Ship* entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t bonus_inventory = BONUS_NOTHING,
		EngineTypes::Ship::inventory_t buff_inventory = BONUS_NOTHING,
		GameTypes::tic_t unbrakable = SHIP_DEFAULT_UNBRAKABLE_PERIOD,
		GameTypes::tic_t burnout = 0,
		GameTypes::tic_t burnout_coldown = SHIP_DEFAULT_BURNOUT_COLDOWN,
		float angular_velocity = 0.0f,
		float radius = SHIP_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT,
		GameTypes::entities_count_t max_bullets_count = SHIP_DEFAULT_MAGAZINE_SIZE,
		GameTypes::entities_count_t start_bullets_count = SHIP_DEFAULT_MAGAZINE_SIZE,
		bool exist = true);

	void SetSizeOfMagazine(GameTypes::entities_count_t cells_count = SHIP_DEFAULT_MAGAZINE_SIZE);
	void SetUnbrakablePeriod(GameTypes::tic_t period);
	//If ship have bonus, the function reduces the amount of this bonus and return true.
	bool SpendBonus(EngineTypes::Bonus::inventory_t bonus);
	//The function reduces the amount of this bonus.
	void SpendBonusNoCheck(EngineTypes::Bonus::inventory_t bonus);
	//If ship have buff, the function reduces this buff and return true.
	bool SpendBuff(EngineTypes::Ship::inventory_t bonus);
	//The function reduces this buff.
	void SpendBuffNoCheck(EngineTypes::Ship::inventory_t bonus);
	void StopCreatingLoop();
	void TakeBonus(Bonus* bonus, bool as_triple);
	void Update();
	void UpdateMatrix();

	void operator=(const Ship& entity);

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
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		GameTypes::tic_t respawn_timer = PILOT_DEFAULT_RESPAWN_TIMER,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t buffs_bonuses = BONUS_NOTHING,
		EngineTypes::Bonus::inventory_t active_baffs = BONUS_NOTHING,
		float angular_velocity = 0.0f,
		float radius = PILOT_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = PILOT_DEFAULT_FORCE_RESISTANCE_AIR_COEFFISIENT,
		bool exist = true);


	bool CanRespawn() const;
	DynamicParticle CreateShards(GameTypes::tic_t current_tic);
	GameTypes::tic_t GetRespawnDellay() const;
	Ship Respawn();
	void Set(const Pilot* entity);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t player_number,
		GameTypes::players_count_t player_team_number,
		const GameTypes::control_flags_t* controle_flags,
		GameTypes::tic_t respawn_timer = PILOT_DEFAULT_RESPAWN_TIMER,
		const std::vector<Vec2F>& heat_box_vertexes_array = std::vector<Vec2F>(),
		float angle = 0.0f,
		EngineTypes::Bonus::inventory_t buffs_bonuses = BONUS_NOTHING,
		EngineTypes::Bonus::inventory_t active_baffs = BONUS_NOTHING,
		float angular_velocity = 0.0f,
		float radius = PILOT_DEFAULT_RADIUS,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = PILOT_DEFAULT_FORCE_RESISTANCE_AIR_COEFFISIENT,
		bool exist = true);

	void Update();
	void UpdateMatrix();

	void operator=(const Pilot& entity);

	~Pilot();
};

class SupportEntity : public StaticEntity
{
protected:
	const ControledEntity* host_p;
	const Mat3x2F* host_matrix_p;
	GameTypes::players_count_t host_number;
	GameTypes::players_count_t host_team;
	// Relative position from the host if laser is connected to one.
	// Absolute position if laser is free from the host.
	Vec2F local_position;
	Vec2F local_direction;
	float local_angle;

public:
	SupportEntity();
	SupportEntity(const SupportEntity& support_entity);
	SupportEntity(
		const ControledEntity* host,
		const Vec2F& position,
		float radius = 0.0f,
		float angle = 0.0f,
		bool exist = true);

	bool IsCreatedBy(const ControledEntity& potencial_host) const;
	bool IsCreatedByTeam(const ControledEntity& potencial_host) const;
	//return local angle
	float GetAngle() const;
	//return local direction
	Vec2F GetDirection() const;
	Vec2F GetGlobalAngle() const;
	Vec2F GetGlobalPosition() const;
	//return pointer to host
	const ControledEntity* GetHostP() const;
	//return local direction
	Vec2F GetNormalizeDirection() const;
	GameTypes::players_count_t GetPlayerMasterNumber() const;
	GameTypes::players_count_t GetPlayerMasterTeamNumber() const;
	//return local position
	const Vec2F& GetPosition() const;
	void Set(const SupportEntity* support_entity);
	void Set(
		const ControledEntity* host,
		const Vec2F& position,
		float radius = 0.0f,
		float angle = 0.0f,
		bool exist = true);

	//set local angle
	void SetAngle(float angle);
	//set local direction
	void SetDirection(Vec2F direction);
	//set local direction
	void SetDirection(const Vec2F* direction);
	void SetHost(const ControledEntity* host);
	//set local direction
	void SetNotNormalizeDirection(Vec2F direction);
	//set local direction
	void SetNotNormalizeDirection(const Vec2F* direction);
	//set local position
	void SetPosition(Vec2F position);
	//set local position
	void SetPosition(const Vec2F* position);
	void Update();
	void UpdateDirection();

	void operator=(const SupportEntity& support_entity);

	~SupportEntity();
};

//Killer entity with out host.
class AggressiveEntity : public StaticEntity
{
protected:
	//Dellay to first attack after math begining.
	GameTypes::tic_t attack_dellay;
public:
	//Period of all attack cycle.
	GameTypes::tic_t attack_period;
	//Period between shoots sessions.
	GameTypes::tic_t inactive_period;
	//Shoots count in one attack period.
	EngineTypes::AgressiveEntity::shoots_count_t shoots_count;
	AggressiveEntity();
	AggressiveEntity(const AggressiveEntity& aggressive_entity);
	AggressiveEntity(
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool exist = true);

	bool CanShoot(GameTypes::tic_t current_tic) const;
	void PostponeAttack(GameTypes::tic_t dellay);
	void Set(const AggressiveEntity* entity);
	void Set(
		const Vec2F& position,
		float radius,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = 0,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		bool exist = true);

	void operator=(const AggressiveEntity& entity);

	~AggressiveEntity();
};

class AnnihAreaGen : public SupportEntity
{
private:
	EngineTypes::Ship::inventory_t buff_inventory;
public:
	AnnihAreaGen();
	AnnihAreaGen(const AnnihAreaGen& annih_area_generator);
	AnnihAreaGen(
		const ControledEntity* host,
		const Vec2F& position,
		EngineTypes::Ship::inventory_t buff_inventory,
		float radius = ANNIH_AREA_GEN_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	bool IsHaveShield() const;
	const Bomb& Shoot() const;
	
	void operator=(const AnnihAreaGen& annih_area_gen);

	~AnnihAreaGen();
};

class DecelerationArea : public StaticEntity
{
public:
	float deceleration_parameter;

	DecelerationArea();
	DecelerationArea(const DecelerationArea& deceleration_area);
	DecelerationArea(
		const Vec2F& position,
		float deseleration_parameter = DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
		float radius = DECELERATION_AREA_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void Set(const DecelerationArea* deceleration_area);
	void Set(
		const Vec2F& position,
		float deseleration_parameter = DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
		float radius = DECELERATION_AREA_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void operator=(const DecelerationArea& deceleration_area);

	~DecelerationArea();
};

class GravGen : public StaticEntity
{
public:
	float gravity;

	GravGen();
	GravGen(const GravGen& grav_gen);
	GravGen(
		const Vec2F& position,
		float gravity = GRAVITY_GENERATOR_DEFAULT_GRAVITY,
		float radius = GRAVITY_GENERATOR_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void Set(const GravGen* grav_gen);
	void Set(
		const Vec2F& position,
		float gravity = GRAVITY_GENERATOR_DEFAULT_GRAVITY,
		float radius = GRAVITY_GENERATOR_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void operator=(const GravGen& grav_gen);

	~GravGen();
};

class Portal : public StaticEntity
{
private:
	EngineTypes::Portal::mode_t tp_mode;
public:
	const Vec2F* tp_position_pointer;
	Vec2F tp_position;

	Portal();
	Portal(const Portal& portal);
	Portal(
		const Vec2F& position,
		const Vec2F& tp_position,
		float radius = PORTAL_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);
	Portal(
		const Vec2F& position,
		const Vec2F* tp_position_pointer = nullptr,
		float radius = PORTAL_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);
	Portal(
		const Vec2F& position,
		const Entity* entity_p,
		float radius = PORTAL_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);

	void Connect(const Vec2F* pointer);
	void Connect(const Entity* entity);

	//The function create particle on portal position and return it.
	Particle CreateParticles(GameTypes::tic_t current_tic) const;

	//The function create particle on teleport position and return it.
	Particle CreateParticlesTP(GameTypes::tic_t current_tic, float radius) const;
	void Disconnect();
	bool IsConnected() const;
	void Set(
		const Vec2F& position,
		const Vec2F* tp_position_pointer = nullptr,
		float radius = GRAVITY_GENERATOR_DEFAULT_RADIUS,
		float angle = 0.0f,
		bool exist = true);
	void SetPosition(const Vec2F& position);
	void SetTPPosition(const Vec2F& position);
	void SetMode(EngineTypes::Portal::mode_t mode);
	
	template <typename EntityType>
	void Teleport(EntityType& entity) const;

	void operator=(const Portal& portal);

	~Portal();
};

class Turret : public AggressiveEntity
{
public:
	Turret();
	Turret(const Turret& turret);
	Turret(
		const Vec2F& position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TURRET_DEFAULT_RADIUS,
		bool exist = true);

	DynamicParticle CreateShards(GameTypes::tic_t current_tic);
	Bullet Shoot();
	void Set(const Turret* entity);
	void Set(
		const Vec2F& position,
		float angle = 0.0f,
		GameTypes::tic_t attack_dellay = AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY,
		GameTypes::tic_t attack_period = AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD,
		GameTypes::tic_t inactive_period = AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD,
		EngineTypes::AgressiveEntity::shoots_count_t shoots_count = AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT,
		float radius = TURRET_DEFAULT_RADIUS,
		bool exist = true);

	void operator=(const Turret& entity);

	~Turret();
};

class Bomb : public KillerEntity
{
protected:
	GameTypes::tic_t animation_tic;
	EngineTypes::Bomb::status_t status;
public:
	GameTypes::tic_t activation_period;
	GameTypes::tic_t blinking_period;

	Bomb();
	Bomb(const Bomb& bomb);
	Bomb(
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t master1_team_number,
		GameTypes::players_count_t master2_team_number,
		GameTypes::tic_t start_animation_tic = 0,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BOMB_DEFAULT_RADIUS,
		EngineTypes::Bomb::status_t status = BOMB_STATUS_INACTIVE,
		GameTypes::tic_t activation_period = BOMB_DEFAULT_ACTIVATION_PERIOD,
		GameTypes::tic_t blinking_period = BOMB_DEFAULT_BLINKING_PERIOD,
		bool exist = true);

	Bomb(const Bullet& bullet1, const Bullet& bullet2);

	void Activate();
	void Boom(GameTypes::tic_t period = BOMB_BOOM_TIME);
	bool CanRemove() const;
	template <typename MapElementT>
	bool CollisionWithElementStatusAnother(const MapElementT& element);
	template <typename MapElementT>
	bool CollisionWithElementStatusBoom(MapElementT& element);
	bool Collision(Map::MapData& map);
	GameTypes::tic_t GetAnimationTic() const;
	bool IsActive() const;
	bool IsBoom() const;
	bool IsCreatedByAggressiveTeam() const;
	bool IsCreatedByAggressiveTeamOnly() const;
	bool IsCreatedByAggressiveTeamNotOnly() const;
	bool IsCreatedByTeam(const ControledEntity* host) const;
	GameTypes::players_count_t GetTeamNumber(const ControledEntity& not_host) const;
	//The function return true, if host will be decrrementing score after boom.
	bool IsAggressiveFor(const ControledEntity& host) const;
	void Set(const Bomb* bomb);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
		GameTypes::players_count_t player_master_number,
		GameTypes::players_count_t player_master_team_number,
		GameTypes::tic_t start_animation_tic = 0,
		float angle = 0.0f,
		float angular_velocity = 0.0f,
		float force_collision_coeffisient = DEFAULT_FORCE_COLLISION_COEFFICIENT,
		float force_resistance_air_coefficient = BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		float radius = BOMB_DEFAULT_RADIUS,
		EngineTypes::Bomb::status_t status = BOMB_STATUS_INACTIVE,
		GameTypes::tic_t activation_period = BOMB_DEFAULT_ACTIVATION_PERIOD,
		GameTypes::tic_t blinking_period = BOMB_DEFAULT_BLINKING_PERIOD,
		bool exist = true);

	void Update();

	void operator=(const Bomb& bomb);

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
		const Vec2F& position,
		const Vec2F& velocity,
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

	bool Collision(Map::MapData& map);
	template<typename MapElementT>
	bool CollisionWithElement(MapElementT& element);
	void Set(const Bullet* bullet);
	void Set(
		const Vec2F& position,
		const Vec2F& velocity,
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

	void operator=(const Bullet& bullet);

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
		const ControledEntity* host,
		const Segment& local_segment,
		EngineTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		bool exist = true);
	//The function checks collision between knife and all map's elemnts.
	//If map element is destructable, this element completes existing.
	//If after collision health of the knife is zero, the function set its parameter "exist" to false.
	bool Collision(Map::MapData& map);
	template <typename ElemetT>
	bool CollissionWithElement(ElemetT& element, const Segment& segment);
	Segment GetSegment() const;
	bool IsCollision(const Line& line) const;
	bool IsCollision(const Beam& beam) const;
	bool IsCollision(const Segment& segment) const;
	void Set(const Knife* knife);
	void Set(
		const ControledEntity* host_p,
		const Segment& local_segment,
		EngineTypes::Knife::knife_health_t health = KNIFE_DEFAULT_HEALTH,
		bool exist = true);
	//If health is zero, the function returns false.
	//Else the function decrements value of health.
	bool LoseHealth();

	void operator=(const Knife& knife);

	~Knife();
};

class Laser : public SupportEntity
{
protected:
	GameTypes::tic_t shoot_time;
	Vec2F end_point;
public:
	float width;
	size_t reflections_count;
	//if laser destroy powered asredoid and flag is active then will be create loop
	EngineTypes::Laser::property_t properties;

	Laser();
	Laser(const Laser& laser);
	Laser(
		const ControledEntity* host,
		const Beam& local_beam,
		float width = LASER_DEFAULT_WIDTH,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		EngineTypes::Laser::property_t properties = LASER_PROPERTY_NOTHING,
		size_t reflection_count = 0,
		bool exist = true);

	//The function checks collision between knife and all map's elemnts.
	//If map element is destructable, this element completes existing.
	bool Collision(Map::MapData& map, Vec2F&  nearest_position, Vec2F& nearest_perpendicular_direction);
	bool IsActive() const;
	bool CreatedBy(const ControledEntity& controled_entity) const;
	Segment GetSegment() const;
	GameTypes::tic_t GetLifeTime() const;
	GameTypes::players_count_t GetPlayerMasterNumber() const;
	GameTypes::players_count_t GetPlayerMasterTeamNumber() const;
	bool GetProperty(EngineTypes::Laser::property_t property) const;
	GameTypes::tic_t GetShootTime() const;
	bool IsCollision(const Beam& beam) const;
	bool IsCollision(const Line& line) const;
	bool IsCollision(const Segment& segment) const;
	void Set(const Laser* laser);
	void Set(
		const ControledEntity* host_p,
		const Beam& local_beam,
		float width = LASER_DEFAULT_WIDTH,
		GameTypes::tic_t shoot_time = LASER_DEFAULT_SHOOT_TIME,
		bool can_create_loops = false,
		size_t reflection_count = 0,
		bool exist = true);
	void Update();

	void operator=(const Laser& laser);

	~Laser();
};