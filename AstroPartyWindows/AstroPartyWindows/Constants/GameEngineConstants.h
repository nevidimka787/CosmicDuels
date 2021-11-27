#pragma once

//Size of the area in that calculate all collisions.
#define GAME_ENGINE_AREA_SIZE 10.0f


#define AGGRESIVE_ENTITY_DEFAULT_ATTACK_DELLAY			0
#define AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD			100
#define AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD		90
#define AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT			5
#define AGGRESIVE_ENTITY_HOST_ID	PLAYERS_COUNT_T_MAX


#define ASTEROID_RADIUS_SMALL		0.05f
#define ASTEROID_RADIUS_MEDIUM		(ASTEROID_RADIUS_SMALL * 2)
#define ASTEROID_RADIUS_BIG			(ASTEROID_RADIUS_MEDIUM * 2)
#define ASTEROID_MAX_RADIUS			ASTEROID_RADIUS_BIG
#define ASTEROID_DEFAULT_RADIUS		ASTEROID_RADIUS_MEDIUM
#define ASTEROID_SIZE_SMALL			0
#define ASTEROID_SIZE_MEDIUM		1
#define ASTEROID_SIZE_BIG			2
#define ASTEROID_MAX_SIZE			ASTEROID_SIZE_BIG
#define ASTEROID_DEFAULT_SIZE		ASTEROID_SIZE_MEDIUM
#define ASTEROID_DEEFAULT_VELOCITY	0.005f


#define BOMB_STATUS_INACTIVE		0x00
#define BOMB_STATUS_ACTIVE			0x01
#define BOMB_STATUS_BOOM			0x02
#define BOMB_STATUS_CAN_REMOVE		0x04
#define BOMB_STATUS_ALL				0xFF

#define BOMB_DEFAULT_RADIUS			0.05f
#define BOMB_BOOM_RADIUS_COEF		14.0f
#define BOMB_ACTIVATION_RADIUS_COEF	(BOMB_BOOM_RADIUS_COEF * 0.9f)
#define BOMB_DEFAULT_BOOM_DELLAY	100
#define BOMB_BOOM_TIME				60
#define BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT	0.01f


#define BONUS_CELL			0x0003 //0000 0000 0000 0011
#define BONUS_BONUS			0x00FF //0000 0000 1111 1111
#define BONUS_BONUS_FIRST	0x0003 //0000 0000 0000 0011
#define BONUS_BONUS_LAST	0x00C0 //0000 0000 1100 0000
#define BONUS_BUFF			0x3F00 //0011 1111 0000 0000
#define BONUS_BUFF_FIRST	0x0300 //0000 0011 0000 0000
#define BONUS_BUFF_LAST		0x3000 //0011 0000 0000 0000
#define BONUS_RULE			0xC000 //1100 0000 0000 0000
#define BONUS_RULE_FIRST	0xC000 //1100 0000 0000 0000
#define BONUS_RULE_LAST		0xC000 //1100 0000 0000 0000
#define BONUS_CELLS_COUNT		8u
#define BONUS_BONUS_CELLS_COUNT	4u
#define BONUS_BONUS_FIRST_CELL	0u
#define BONUS_BONUS_LAST_CELL	(BONUS_BONUS_FIRST_CELL + BONUS_BONUS_CELLS_COUNT - 1u)
#define BONUS_BUFF_CELLS_COUNT	3u
#define BONUS_BUFF_FIRST_CELL	4u
#define BONUS_BUFF_LAST_CELL	(BONUS_BUFF_FIRST_CELL + BONUS_BUFF_CELLS_COUNT - 1u)
#define BONUS_RULE_CELLS_COUNT	1u
#define BONUS_RULE_FIRST_CELL	7u
#define BONUS_RULE_LAST_CELL	(BONUS_RULE_FIRST_CELL + BONUS_RULE_CELLS_COUNT - 1u)

#define BONUS_NOTHING		0x0000 //0000 0000 0000 0000
#define BONUS_LOOP			0x0001 //0000 0000 0000 0001
#define BONUS_LASER			0x0004 //0000 0000 0000 0100
#define BONUS_BOMB			0x0010 //0000 0000 0001 0000
#define BONUS_KNIFE			0x0040 //0000 0000 0100 0000
#define BONUS_BUFF_TRIPLE	0x0100 //0000 0001 0000 0000
#define BONUS_BUFF_SHIELD	0x0400 //0000 0100 0000 0000
#define BONUS_RULE_REVERSE	0x4000 //0100 0000 0000 0000
#define BONUS_ALL			0xFFFF //1111 1111 1111 1111

#define BONUS_DEFAULT_RADIUS							0.02f
#define BONUS_DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT	0.02f


#define BULLET_IGNORE_MASK_FULL			0xFF
#define BULLET_IGNORE_NOTHING			0x00
#define BULLET_IGNORE_MUSTER			0x01
#define BULLET_IGNORE_KNIFES_OF_MASTER	0x02

#define BULLET_DEFAULT_MIN_VELOCITY		0.004f
#define BULLET_DEFAULT_VELOCITY			0.013f
#define BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT 0.0003f
#define BULLET_DEFAULT_RADIUS			0.02f


#define BUTTON_STATUS_FALSE				0x00
#define BUTTON_STATUS_TRUE				0x01
#define BUTTON_STATUS_SELECT			0x02
#define BUTTON_STATUS_ACTIVE			0x04 //Button can change by click.
#define BUTTON_STATUS_CUSTOM_RED		0x08
#define BUTTON_STATUS_CUSTOM_GREEN		0x10
#define BUTTON_STATUS_CUSTOM_BLUE		0x20
#define BUTTON_STATUS_CUSTOM_PURPURE	0x40
#define BUTTON_STATUS_CUSTOM_5			0x80
#define BUTTON_STATUS_ALL				0xFF

#define CAMERA_DEFAULT_MOVE_VELOCITY	0.02f
#define CAMERA_DEFAULT_RESIZE_VELOCITY	0.02f
#define CAMERA_DEFAULT_MARGIN			0.5f
#define CAMERA_DEFAULT_HIGH_LIMITS		10.0f
#define CAMERA_DEFAULT_LOW_LIMITS		1.0f

#define CAMERA_LOW_MOVE_VELOCITY		CAMERA_DEFAULT_MOVE_VELOCITY
#define CAMERA_LOW_RESIZE_VELOCITY		CAMERA_DEFAULT_RESIZE_VELOCITY
#define CAMERA_HIGH_MOVE_VELOCITY		(5000.0 * CAMERA_LOW_MOVE_VELOCITY)
#define CAMERA_HIGH_RESIZE_VELOCITY		(5000.0 * CAMERA_DEFAULT_RESIZE_VELOCITY)
#define CAMERA_UPDATE_COEFF_PARAM		1.01f


#define CYRCLE_DEFAULT_RADIUS 0.1f


#define DECELERATION_AREA_DEFAULT_RADIUS					0.1f
#define DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT	0.02f //how much velocity will lose every tic


#define DEFAULT_FORCE_COLLISION_COEFFICIENT			0.002f
//the distance the ship will be pushed to the side
#define DEFAULT_FORCE_COLLISION_DELTA				0.001f
#define DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT	0.01f


#define DYNAMIC_PARTICLE_PROPERTY_NULL							0x00

#define DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_BOMB				0x01
#define DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_LASER				0x02
#define DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_MAP				0x04
#define DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_MEGA_LASER		0x08
#define DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS	0x10
#define DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP					0x20
#define DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_SHIP				0x40
#define DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE		0x80

#define DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_ALL	(				\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS	|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP					|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_SHIP				|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE			\
	)

#define DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_ALL (				\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_BOMB				|	\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_LASER				|	\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_MAP					\
	)

#define DYNAMIC_PARTICLE_PROPERTY_FULL (						\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_BOMB				|	\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_LASER				|	\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_MAP				|	\
	DYNAMIC_PARTICLE_PROPERTY_DESTROED_BY_MEGA_LASER		|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_GRAVITY_GENERATORS	|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP					|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_SHIP				|	\
	DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_AIR_RESISTANCE			\
	)


#define GRAVITY_GENERATOR_DEFAULT_GRAVITY	0.00003f
#define GRAVITY_GENERATOR_DEFAULT_RADIUS	0.1f


#define KNIFE_DEFAULT_HEALTH						3
#define KNIFE_DEFAULT_RESISTANSE_AIR_COEFFICIENT	BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT


#define LASER_DEFAULT_WIDTH			0.02f
#define LASER_DEFAULT_SHOOT_TIME	30


#define LOG_NULL				0x00000000 //0000 0000 0000 0000 0000 0000 0000 0000
#define LOG_MASK_BITS			0x0000000F //0000 0000 0000 0000 0000 0000 0000 1111
#define LOG_MASK_HEAD			0xF0000000 //1111 0000 0000 0000 0000 0000 0000 0000
#define LOG_MASK_DATA			0x0FFFFFFF //0000 1111 1111 1111 1111 1111 1111 1111
#define LOG_MASK_FULL			0xFFFFFFFF //1111 1111 1111 1111 1111 1111 1111 1111

#define LOG_HEAD				28 //collect header
#define LOG_CHANGE_SCORE		0x01 //score will change

#define LOG_DATA				0 //collect data

#define LOG_DATA_SCORE			0 //collect info about score changes
#define LOG_INCREMENT			0x01 //score will incrementation
#define LOG_DECREMENT			0x00 //score will decrementation

#define LOG_DATA_SHIP			4 //collect info about ship number
#define LOG_DATA_TEAM			8 //collect info about team number


#define MAP_PROPERTY_NO_PROPERTY	0x00 //element is breakable and passive
#define MAP_PROPERTY_UNBREACABLE	0x01 //element is unbreakable
#define MAP_PROPERTY_KILLER			0x02 //element can destroy players
#define MAP_PROPERTY_AGRESSIVE		0x04 //element can destroy entities with out players
#define MAP_PROPERTY_CLOSED			0x08 //element is polygon, else element is broren line

#define MAP_DEFAULT_PROPERTIES		(MAP_PROPERTY_UNBREACABLE)


#define MEGA_LASER_DEFAULT_WIDTH	0.05f


#define PARTICLE_TYPE_NULL							0u
#define PARTICLE_TYPE_BACKGROUND					1u
#define PARTICLE_TYPE_EXAUST_ENGINE					2u
#define PARTICLE_TYPE_EXAUST_BURNOUT				3u
#define PARTICLE_TYPE_EXAUST_BOMB_BOOM				4u
#define PARTICLE_TYPE_EXAUST_SHOOT					5u
#define PARTICLE_TYPE_PORTAL_IN						6u
#define PARTICLE_TYPE_PORTAL_OUT					7u
#define PARTICLE_TYPE_SHARDS_ASTEROID				8u
#define PARTICLE_TYPE_SHARDS_ASTEROID_POWERED		9u
#define PARTICLE_TYPE_SHARDS_MAP_ELEMENT			10u
#define PARTICLE_TYPE_SHARDS_PILOT					11u
#define PARTICLE_TYPE_SHARDS_SHIP					12u

#define PARTICLE_POSTPONE_BACKGROUND				0lu
#define PARTICLE_POSTPONE_EXAUST_BOMB_BOOM			0lu
#define PARTICLE_POSTPONE_EXAUST_BURNOUT			0lu
#define PARTICLE_POSTPONE_EXAUST_ENGINE				0lu
#define PARTICLE_POSTPONE_EXAUST_SHOOT				0lu
#define PARTICLE_POSTPONE_PORTAL_IN					0lu
#define PARTICLE_POSTPONE_PORTAL_OUT				0lu
#define PARTICLE_POSTPONE_SHARDS_ASTEROID			0lu
#define PARTICLE_POSTPONE_SHARDS_ASTEROID_POWERED	0lu
#define PARTICLE_POSTPONE_SHARDS_MAP_ELEMENT		0lu
#define PARTICLE_POSTPONE_SHARDS_PILOT				0lu
#define PARTICLE_POSTPONE_SHARDS_SHIP				1lu

#define PARTICLE_PERIOD_BACKGROUND					1lu
#define PARTICLE_PERIOD_EXAUST_ENGINE				100lu
#define PARTICLE_PERIOD_EXAUST_BURNOUT				100lu
#define PARTICLE_PERIOD_EXAUST_BOMB_BOOM			1lu
#define PARTICLE_PERIOD_EXAUST_SHOOT				1lu
#define PARTICLE_PERIOD_PORTAL_IN					50lu
#define PARTICLE_PERIOD_PORTAL_OUT					50lu
#define PARTICLE_PERIOD_SHARDS_ASTEROID				100lu
#define PARTICLE_PERIOD_SHARDS_ASTEROID_POWERED		100lu
#define PARTICLE_PERIOD_SHARDS_MAP_ELEMENT			100lu
#define PARTICLE_PERIOD_SHARDS_PILOT				100lu
#define PARTICLE_PERIOD_SHARDS_SHIP					100lu

#define PARTICLE_PROPERTY_AUTO			0x00
#define PARTICLE_ANIMATION_NOT_FINISH	0x00


#define PILOT_DEFAULT_RESPAWN_TIMER		1000
#define PILOT_DEFAULT_RADIUS			0.03f
#define PILOT_DEFAULT_FORCE_RESISTANCE_AIR_COEFFISIENT 0.002f


#define PORTAL_DEFAULT_RADIUS	GRAVITY_GENERATOR_DEFAULT_RADIUS
#define PORTAL_MODE_POINTER		1u
#define PORTAL_MODE_POSITION	2u


#define RECTANGLE_UP_SIDE		0x01
#define RECTANGLE_DOWN_SIDE		0x02
#define RECTANGLE_RIGHT_SIDE	0x04
#define RECTANGLE_LEFT_SIDE		0x08


#define SHIP_BUFF_NOTHING	0x00 //0000 0000
#define SHIP_BUFF_TRIPLE	0x01 //0000 0001
#define SHIP_BUFF_SHIELD	0x02 //0000 0010
#define SHIP_BUFF_ALL		0xFF //1111 1111


#define TUREL_DEFAULT_RADIUS	0.1f

#define SHIP_DEFAULT_RADIUS				0.07f
#define SHIP_DEFAULT_UNBRAKABLE_PERIOD	100
#define SHIP_DEFAULT_BURNOUT_PERIOD		7
#define SHIP_BULLETS_IN_LOOP			20
#define SHIP_SHOOT_FORCE				0.002f
#define SHIP_BURNOUT_FORCE				0.0005f