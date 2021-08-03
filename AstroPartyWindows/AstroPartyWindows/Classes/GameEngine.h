#pragma once

#include <iostream>
#include "Vec.h"
#include "Line.h"
#include "../Types/GameRealisationTypes.h"
#include "../Types/ClassesTypes.h"

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
class MapElement;
class Rectangle;
class Cyrcle;
class Polygon;
class Map;

class Entity
{
protected:
	float angle;
	Vec2F* position;
	Vec2F* direction;
public:
	bool exist;
	float radius;
	Entity();
	Entity(Vec2F* position, float radius, float angle);

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
#define DEFAULT_FORCE_COLLISION_COEFFICIENT			0.001f
#define DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT	0.001f
protected:
	float angular_velocity;
	Vec2F* velocity;
	Vec2F* force;
public:
	const float FORCE_COLLISION_COEFFICIENT;
	const float FORCE_RESISTANSE_AIR_COEFFICIENT;
	DynamicEntity();
	DynamicEntity(Vec2F* position, float angle, float radius);
	DynamicEntity(Vec2F* position, Vec2F* velosity, float angle, float angular_velosity, float radius, float FORCE_COLLISION_COEFFICIENT, float FORCE_RESISTANSE_AIR_COEFFICIENT);
	
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
	void Set(DynamicEntity* entity);
	void SetAngularVelocity(float angulat_velocity);

	void operator=(DynamicEntity entity);

	~DynamicEntity();
};

class StaticEntity : public Entity
{
protected:
	Vec2F* last_position;
public:
	StaticEntity();
	StaticEntity(Vec2F* position, float angle);
	
	Vec2F GetVelosity();
	void Recalculate();
	void Set(StaticEntity* entity);

	void operator=(StaticEntity entity);
	
	~StaticEntity();
};

class Bonus : public DynamicEntity
{
#define BONUS_BONUSES_COUNT		4
#define BONUS_BUFFS_COUNT		2
#define BONUS_GAME_RULES_COUNT	1
#define BONUS_TYPES_COUNT		(BONUS_BONUSES_COUNT + BONUS_BUFFS_COUNT + BONUS_GAME_RULES_COUNT)
#define BONUS_BONUS_DATA_LENGTH (sizeof(uint8_t) * 8 - 2)
#define BONUS_BONUS		0x00FF //0000 0000 1111 1111
#define BONUS_BUFF		0x3F00 //0011 1111 0000 0000
#define BONUS_GAME_RULE	0xC000 //1100 0000 0000 0000

#define BONUS_NO_BONUS	0x0000
#define BONUS_LOOP		0x0001
#define BONUS_LASER		0x0004
#define BONUS_BOMB		0x0008
#define BONUS_KNIFE		0x0010
#define BUFF_TRIPLE		0x0100
#define BUFF_SHIELD		0x0400
#define GAME_REVERSE	0x8000

#define BONUS_DEFAULT_RADIUS 0.005f

protected:
public:
	ClassTypes::Bonus::bonus_t bonus_type;
	Bonus();
	Bonus(ClassTypes::Bonus::bonus_t bonus_type, Vec2F* position, Vec2F* velocity);
	
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

	void operator=(Bonus entity);

	~Bonus();
};

class Asteroid : public Bonus
{
#define ASTEROID_RADIUS_SMALL	0.0125f
#define ASTEROID_RADIUS_MEDIUM	0.025f
#define ASTEROID_RADIUS_BIG		0.05f
#define ASTEROID_MAX_RADIUS		ASTEROID_RADIUS_BIG
#define ASTEROID_DEFAULT_RADIUS	ASTEROID_RADIUS_MEDIUM
#define ASTEROID_SIZE_SMALL		0
#define ASTEROID_SIZE_MEDIUM	1
#define ASTEROID_SIZE_BIG		2
#define ASTEROID_MAX_SIZE		ASTEROID_SIZE_BIG
#define ASTEROID_DEFAULT_SIZE	ASTEROID_SIZE_MEDIUM
protected:
	uint8_t size;
public:
	Asteroid();
	Asteroid(ClassTypes::Bonus::bonus_t buff_bonus, uint8_t size);
	Bonus Destroy();
	/*
	*The function creates a new smaller asteroid.
	*Remember to remove main asteroid.
	*/
	Asteroid Division();
	uint8_t GetSize();
	uint16_t GetBuffBonus();
	void Set(Asteroid* entity);

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
	KillerEntity(GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number);
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	void Set(KillerEntity* entity);

	void operator=(KillerEntity entity);

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
	ControledEntity(GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity);
	
	GameTypes::players_count_t GetPlayerNumber();
	GameTypes::players_count_t GetTeamNumber();
	bool GetRotateInputValue();
	bool GetShootInputValue();
	void Set(ControledEntity* entity);

	void operator=(ControledEntity entity);
	
	~ControledEntity();
};

class Ship : public ControledEntity
{
#define SHIP_UNBRAKABLE_PERIOD 100

#define BULLET_DEFAULT_VELOCITY 0.1f
#define BULLET_DEFAULT_RADIUS	0.01f
#define BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT 0.001f
protected:
	ClassTypes::Bonus::bonus_t buffs_bonuses;
	ClassTypes::Bonus::bonus_t active_baffs;
	uint16_t unbrakable;
public:
	Ship();
	Ship(GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity);
	Ship(GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity, ClassTypes::Bonus::bonus_t buffs_bonuses);
	
	void ActivateBuffs();
	void BreakShield();
	Bullet CreateBullet();
	//The function does not check for the presence of a bonus.
	Bullet CreateTriple(uint8_t bullet_number);
#define BULLETS_IN_LOOP 24
	//The function does not check for the presence of a bonus.
	Bullet CreateLoop(GameTypes::entities_count_t bullet_number);
	//The function does not check for the presence of a bonus.
	Bomb CreateBomb();
	//The function does not check for the presence of a bonus.
	Laser CreateLazer();
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
	void TakeBonus(Bonus* bonus);

	void operator=(Ship entity);
	
	~Ship();
};

class Pilot : public ControledEntity
{
protected:
public:
	Pilot();
	Pilot(GameTypes::players_count_t player_number, GameTypes::players_count_t player_team_number, void* rotate_keyboard_key_pointer, void* move_keyboard_key_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity);

	Ship Respawn();
	void Set(Pilot* entity);

	void operator=(Pilot entity);

	~Pilot();
};

//Killer entity with out host.
class AggressiveEntity : public StaticEntity
{
#define AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD			30
#define AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD		970
#define AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT			3

#define AGGRESIVE_ENTITY_HOST_ID	PLAYERS_COUNT_T_MAX
protected:
	//Dellay to first attack after spawn.
	GameTypes::tic_t attack_dellay;
public:
	//Period of all attack cycle.
	GameTypes::tic_t attack_period;
	//Period between shoots.
	GameTypes::tic_t inactive_period;
	//Shoots count in one attack period.
	uint8_t shoots_count;
	AggressiveEntity();
	AggressiveEntity(GameTypes::tic_t current_tic, GameTypes::tic_t first_activation_dellay, GameTypes::tic_t attack_period, GameTypes::tic_t passive_period, uint8_t shoots_count);
	bool CanShoot(GameTypes::tic_t current_tic);
	void PostponeAttack(GameTypes::tic_t dellay);
	void Set(AggressiveEntity* entity);

	void operator=(AggressiveEntity entity);

	~AggressiveEntity();
};

class Turel : public AggressiveEntity
{
#define ATACK_ENTITY_DEFAULT_ATACK_PERIOD
public:
	Turel();
	Turel(Vec2F* position, float angle);
	
	Bullet Shoot();
	void Set(Turel* entity);

	void operator=(Turel entity);

	~Turel();
};

class GravGen : public StaticEntity
{
#define GRAVITY_GENERATOR_DEFAULT_GRAVITY 0.1f
public:
	float gravity;

	GravGen();
	GravGen(Vec2F* position, float gravity);
	void Set(GravGen* entity);

	void operator=(GravGen entity);

	~GravGen();
};

class MegaLaser : public AggressiveEntity
{
protected:
	bool active;
public:
	MegaLaser();
	MegaLaser(Segment* lazer_segment);

	Segment GetSegment();
	void Rotate(float angle);
	void Set(MegaLaser* entity);
	bool IsShooting();

	void operator=(MegaLaser entity);

	~MegaLaser();
};

class Laser : public StaticEntity
{
#define LASER_DEFAULT_SHOOT_PERIOD 100
protected:
	GameTypes::players_count_t player_master_number;
	GameTypes::players_count_t player_master_team_number;
	GameTypes::tic_t shoot_period;
public:
	Laser();
	Laser(Vec2F* position, Vec2F* direction, GameTypes::players_count_t player_master_number, GameTypes::players_count_t player_master_team_number, GameTypes::tic_t shoot_period);
	bool CanShoot(GameTypes::tic_t current_tic);
	Beam GetBeam();
	GameTypes::players_count_t GetPlayerMasterNumber();
	GameTypes::players_count_t GetPlayerMasterTeamNumber();
	void Recalculate();
	void Set(Laser* lazer);

	void operator=(Laser entity);

	~Laser();
};

class Bullet : public KillerEntity
{
protected:
public:
	Bullet();
	Bullet(Vec2F* position, Vec2F* velocity, uint8_t host_number);
	bool IsCollision(Map* map);
	void Set(Bullet* bullet);

	void operator=(Bullet entity);

	~Bullet();
};

class Knife : public KillerEntity
{
protected:
	ClassTypes::Knife::knife_health_t health;
public:
	Knife();
	Knife(Vec2F* point1, Vec2F* point2, uint8_t host_number);
	Segment GetSegment();
	void Set(Knife* knife);
	//The function will return false when health is zero.
	bool LoseHealth();

	void operator=(Knife entity);

	~Knife();
};

class Bomb : public KillerEntity
{
#define MINE_DEFAULT_TIMER 100
#define MINE_BOOM_TIMER 10
#define MINE_BOOM_RADIUS 0.1f
protected:
	uint8_t animation_tic;
	bool active;
	bool boom;
public:
	Bomb();
	Bomb(Vec2F* position, Vec2F* velosity, float angle, float angular_velosity, uint8_t player_master_number);
	
	void Activate();
	void Boom();
	bool IsActive();
	bool IsBoom();
	bool CanRemove();
	void Recalculate();
	void Set(Bomb* mine);

	void operator=(Bomb entity);

	~Bomb();
};

class MapElement
{
protected:
	Vec2F* position;
	Vec2F* last_position;
	bool unbreakable;
public:
	bool is_exist;
	MapElement();

	Vec2F GetPosition();
	Vec2F GetVelocity();
	bool IsUnbreacable();
	void Move(Vec2F* move_vector);
	void SetPosition(Vec2F* position);
	void Set(MapElement* element);

	~MapElement();
};

class Rectangle : public MapElement
{
#define RECTANGLE_UP_SIDE		0x01
#define RECTANGLE_DOWN_SIDE		0x02
#define RECTANGLE_RIGHT_SIDE	0x04
#define RECTANGLE_LEFT_SIDE		0x08
protected:
	Vec2F* point2;//down right point
public:
	ClassTypes::Rectangle::sides_t show_sides;
	ClassTypes::Rectangle::sides_t collision_sides;
	Rectangle();
	Rectangle(Vec2F* point1, Vec2F* point2, bool unbreakable);

	Vec2F GetUpRightPoint();
	Vec2F GetDownRightPoint();
	Vec2F GetDownLeftPoint();
	Vec2F GetUpLeftPoint();
	Segment GetUpSide();
	Segment GetDownSide();
	Segment GetRightSide();
	Segment GetLeftSide();
	void Move(Vec2F* move_vector);
	void Set(Rectangle* patent);

	~Rectangle();
};

class Cyrcle : public MapElement
{
protected:
	float radius;
public:
	Cyrcle();
	Cyrcle(Vec2F* position, float radius, bool unbreakable);

	float GetRadius();
	void SetRadius(float radius);
	void Set(Cyrcle* parent);

	~Cyrcle();
};

/*
* This class not recommended use in default maps.
*/
class Polygon : public MapElement
{
protected:
	Vec2F* points_array;
	Vec2F* default_points_array;
	uint32_t points_array_length;

public:
	Polygon();
	Polygon(Vec2F* position, Vec2F* default_points_array, uint32_t points_array_length, bool unbreakable);
	
	void Rotate(float angle, Vec2F* rotating_point);
	void Move(Vec2F* move_vector);
	void ToDefault();
	void Set(Polygon* parent);
	
	~Polygon();
};

class Map
{
protected:
	Rectangle* rectangles_array;
	Cyrcle* cyrcles_array;
	Polygon* polygons_array;

public:
	const uint8_t rectangles_array_length;
	const uint8_t cyrcles_array_length;
	const uint8_t polygons_array_length;
	const Vec2F size;

	Map(Rectangle* rectangles_array, uint8_t rectangles_array_length, Cyrcle* cyrcles_array, uint8_t cyrcles_array_length, Polygon* polygons_array, uint8_t polygons_array_length, Vec2F* size);

	Vec2F GetSize();
	Rectangle GetRectangle(uint8_t number);
	Cyrcle GetCyrcle(uint8_t number);
	Polygon GetPolygon(uint8_t number);
	Rectangle* GetRectanglePointer(uint8_t number);
	Cyrcle* GetCyrclePointer(uint8_t number);
	Polygon* GetPolygonPointer(uint8_t number);
	void Set(Map* entity);

	~Map();
};