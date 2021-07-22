#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include <iostream>
#include "Vec.h"
#include "Line.h"

class Entity;
class StaticEntity;
class MegaLazer;
class Turel;
class DynamicEntity;
class Mine;
class Bonus;
class ControledEntity;
class Sheep;
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
	void Move(Vec2F* delta);

	~Entity();
};

class DynamicEntity : public Entity
{
#define DEFAULT_FORCE_COLLISION_COEFFICIENT			0.001
#define DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT	0.001
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

	~DynamicEntity();
};

class StaticEntity : public Entity
{
protected:
	Vec2F* position;
	Vec2F* last_position;
public:
	StaticEntity();
	StaticEntity(Vec2F* position, float angle);
	
	Vec2F GetVelosity();
	void Recalculate();
	
	~StaticEntity();
};

class Bonus : public DynamicEntity
{
#define BONUSES_COUNT	4
#define BUFS_COUNT		2
#define BONUS_DATA_LENGTH (sizeof(uint8_t) * 8 - 2)
#define BONUS		0x00FF //0000 0000 1111 1111
#define BUFF		0x3F00 //0011 1111 0000 0000
#define GAME_RULE	0xC000 //1100 0000 0000 0000

#define BONUS_LOOP		0x0001
#define BONUS_LASER		0x0004
#define BONUS_MINE		0x0008
#define BONUS_SHNECK	0x0010
#define BUFF_TRIPLE		0x0100
#define BUFF_SHIELD		0x0400
#define GAME_REVERSE	0x8000
protected:
public:
	uint16_t bonus_type;
	Bonus();
	Bonus(uint16_t BONUS_TYPE, Vec2F* position, Vec2F* velocity);
	
	/*
		If an object has more than one bonus type,
		bonus type will be passed as a new object,
		otherwise the function will return 'nullptr'.
	*/
	Bonus* Division();
	uint16_t GetType();


	~Bonus();
};

class Asteroid : public DynamicEntity
{
protected:
	uint16_t buff_bonus;
public:
	Asteroid();
	Asteroid(uint16_t buff_bonus);
	Bonus* Destroy();
	~Asteroid();
};

class ControledEntity : public DynamicEntity
{
protected:
	uint8_t player_number;
	void* rotate_input_value_pointer;
	void* shoot_input_value_pointer;
public:
	ControledEntity();
	ControledEntity(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity);
	
	uint8_t GetPlauerNumber();
	bool GetRotateInputValue();
	bool GetShootInputValue();
	
	~ControledEntity();
};

class Sheep : public ControledEntity
{
#define BULLET_DEFAULT_VELOCITY 0.1f
#define BULLET_DEFAULT_RADIUS	0.01f
#define BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT 0.001f
protected:
	uint16_t baff_bonus;
	uint16_t active_baff_bonus;
public:
	Sheep(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity);
	Sheep(uint8_t player_number, void* rotate_input_value_pointer, void* shoot_input_value_pointer, Vec2F* position, Vec2F* velocity, float angle, float angular_velocity, uint16_t baff_bonus);
	
	void ActivateBonus();
	DynamicEntity* CreateBullet();
	DynamicEntity* CreateTriple(uint8_t bullet_number);
#define BULLETS_IN_LOOP 24
	DynamicEntity* CreateLoop(uint8_t bullet_number);
	Mine* CreateMine();
	Beam* CreateLazer();
	Segment* CreateKnife(uint8_t knife_number);
	void GetBonus(Bonus* bonus);
	Bonus* LoseBonus();
	Pilot* Destroy();
	
	~Sheep();
};

class Pilot : public ControledEntity
{
protected:
public:
	Pilot(uint8_t player_number, void* rotate_keyboard_key_pointer, void* move_keyboard_key_pointer, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity);

	Sheep* Respawn();

	~Pilot();
};

class Turel : public StaticEntity
{
protected:
public:
	Turel(Vec2F* position, float angle);
	
	DynamicEntity* Shoot();

	~Turel();
};

class MegaLazer : public StaticEntity
{
protected:
	bool active;
	Vec2F* point2;
public:
	MegaLazer(Segment* lazer_segment, float angle);
	
	void StartShoot();
	void StopShoot();
	bool IsShooting();

	~MegaLazer();
};

class Mine : public DynamicEntity
{
#define MINE_DEFAULT_TIMER 100
#define MiNE_BOOM_TIMER 10
#define MINE_BOOM_RADIUS 0.1f
protected:
	uint8_t player_master_number;
	uint8_t animation_tik;
	bool active;
	bool boom;
public:
	Mine(uint8_t player_master_number, Vec2F* position, Vec2F* velosity, float angle, float angular_velosity);
	
	void Activate();
	void Boom();
	void Recalculate();

	~Mine();
};

class MapElement
{
protected:
	Vec2F* position;
	Vec2F* last_position;
public:
	MapElement();

	Vec2F GetPosition();
	Vec2F GetVelocity();
	void Move(Vec2F* move_vector);
	void SetPosition(Vec2F* position);

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
	uint8_t show_sides;
	uint8_t collision_sides;
	Rectangle();
	Rectangle(Vec2F* point1, Vec2F* point2);

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
	Cyrcle(Vec2F* position, float radius);

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
	Polygon(Vec2F* position, Vec2F* default_points_array, uint32_t points_array_length);
	
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

	~Map();
};

#endif //GAME_ENGINE_H