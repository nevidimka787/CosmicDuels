#pragma once

//Size of the area in that calculate all collisions.
#define GAME_ENGINE_AREA_SIZE 10.0f

#define BUTTON_STATUS_FALSE				0x00
#define BUTTON_STATUS_TRUE				0x01
#define BUTTON_STATUS_SELECT			0x02
#define BUTTON_STATUS_ACTIVE			0x04 //Button can change by click.
#define BUTTON_STATUS_CUSTOM_RED		0x08
#define BUTTON_STATUS_CUSTOM_GREEN		0x10
#define BUTTON_STATUS_CUSTOM_BLUE		0x20
#define BUTTON_STATUS_CUSTOM_PURPURE	0x40
#define BUTTON_STATUS_CUSTOM_5			0x80
#define BUTTON_STATUSE_ALL				0xFF

#define CAMERA_DEFAULT_MOVE_VELOCITY	0.02f
#define CAMERA_DEFAULT_RESIZE_VELOCITY	0.02f
#define CAMERA_DEFAULT_MARGIN			0.6f


#define DEFAULT_FORCE_COLLISION_COEFFICIENT			0.007f
#define DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT	0.01f


#define BONUS_BONUSES_COUNT		4
#define BONUS_BUFFS_COUNT		2
#define BONUS_GAME_RULES_COUNT	1
#define BONUS_TYPES_COUNT		(BONUS_BONUSES_COUNT + BONUS_BUFFS_COUNT + BONUS_GAME_RULES_COUNT)
#define BONUS_BONUS_DATA_LENGTH (sizeof(uint8_t) * 8 - 2)
#define BONUS_ALL		0xFFFF //1111 1111 1111 1111
#define BONUS_BONUS		0x00FF //0000 0000 1111 1111
#define BONUS_BUFF		0x3F00 //0011 1111 0000 0000
#define BONUS_GAME_RULE	0xC000 //1100 0000 0000 0000
#define BONUS_NO_BONUS	0x0000
#define BONUS_LOOP		0x0001 //0000 0000 0000 0001
#define BONUS_LASER		0x0004 //0000 0000 0000 0100
#define BONUS_BOMB		0x0010 //0000 0000 0001 0000
#define BONUS_KNIFE		0x0040 //0000 0000 0100 0000
#define BUFF_TRIPLE		0x0100
#define BUFF_SHIELD		0x0400
#define GAME_REVERSE	0x8000
#define BUFF_BONUS_ALL	0xFFFF
#define BONUS_DEFAULT_RADIUS 0.02f
#define BONUS_DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT 0.02f


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


#define BULLET_MUSTER_FULL		0xFF
#define BULLET_MUSTER_NOTHING	0x00
#define BULLET_MUSTER_ONLY		0x01
#define BULLET_MUSTER_KNIFES	0x02
#define BULLET_MIN_VELOCITY		0.004f
#define BULLET_DEFAULT_VELOCITY 0.013f
#define BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT 0.0003f
#define BULLET_DEFAULT_RADIUS 0.03f

#define SHIP_DEFAULT_RADIUS	0.07f
#define SHIP_UNBRAKABLE_PERIOD 100
#define BULLETS_IN_LOOP 10
#define SHIP_SHOOT_FORCE	0.002f
#define SHIP_BURNOUT_PERIOD	100


#define PILOT_DEFAULT_RESPAWN_TIMER 1000
#define PILOT_DEFAULT_RADIUS 0.03f
#define PILOT_DEFAULT_FORCE_RESISTANCE_AIR_COEFFISIENT 0.002f


#define AGGRESIVE_ENTITY_DEFAULT_ATTACK_PERIOD			100
#define AGGRESIVE_ENTITY_DEFAULT_INACTIVE_PERIOD		90
#define AGGRESIVE_ENTITY_DEFAULT_SHOOTS_COUNT			5
#define AGGRESIVE_ENTITY_HOST_ID	PLAYERS_COUNT_T_MAX


#define TUREL_DEFAULT_RADIUS	0.1f


#define GRAVITY_GENERATOR_DEFAULT_GRAVITY	0.1f
#define GRAVITY_GENERATOR_DEFAULT_RADIUS	0.01f


#define LASER_DEFAULT_SHOOT_TIME 30


#define KNIFE_DEFAULT_HEALTH 3
#define KNIFE_DEFAULT_RESISTANSE_AIR_COEFFICIENT BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT

#define BOMB_INACTIVE				0x00
#define BOMB_ACTIVE					0x01
#define BOMB_BOOM					0x02
#define BOMB_CAN_REMOVE				0x04
#define BOMB_ALL					0xFF
#define BOMB_DEFAULT_RADIUS			0.05f
#define BOMB_BOOM_RADIUS			0.8f
#define BOMB_ACTIVATION_RADIUS		(BOMB_BOOM_RADIUS * 0.9f)
#define BOMB_DEFAULT_BOOM_DELLAY	100
#define BOMB_BOOM_TIME				60
#define BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT	0.01f


#define RECTANGLE_UP_SIDE		0x01
#define RECTANGLE_DOWN_SIDE		0x02
#define RECTANGLE_RIGHT_SIDE	0x04
#define RECTANGLE_LEFT_SIDE		0x08


#define CYRCLE_DEFAULT_RADIUS 0.1f