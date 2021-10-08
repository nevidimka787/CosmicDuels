#pragma once

#define GAME_END_MATCH_DELLAY 100

#define GAME_DOUBLE_CLK_TIME	10
#define GAME_BURNOUT_COULDOWN	50

/*
asteroids, bombs, bonuses, bullets,
knifes, lasers, particles, ships,
pilots, players, gravity generators,
turels, mega lasers
*/
#define GAME_ENTITIES_TYPES_COUNT 13

/*
Asteroid
*/
#define GAME_OBJECT_TYPES_COUNT	1

//tics to next shoot
#define GAME_DELLAY_BETWEEN_SHOOTS			30
#define GAME_ADD_DELLAY_BETWEEN_TRIPLES		10 //dellay added to GAME_DELLAY_BETWEEN_SHOOTS
#define GAME_ADD_DELLAY_BONUS_USE	20 //dellay added to GAME_DELLAY_BETWEEN_SHOOTS
#define GAME_ADD_DELLAY_CONBO_USE		5  //dellay added to GAME_ADD_DELLAY_BETWEEN_BONUSES_USE

//The maximum count of deceleration areas on the map.
#define GAME_DECEL_AREAS_MAX_COUNT	32
//The maximum count of gravity generators on the map.
#define GAME_GRAV_GENS_MAX_COUNT	32
//The maximum possible count of players on the map.
#define GAME_PLAYERS_MAX_COUNT		4
	//The maximum possible count of knifes on the map.
#define GAME_KNIFES_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
	//The maximum possible count of lasers on the map.
#define GAME_LASERS_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
	//The maximum possible count of bombs on the map.
#define GAME_BOMBS_MAX_COUNT		(UINT8_MAX - 1)
	//The maximum possible count of turels on the map.
#define GAME_TURELS_MAX_COUNT		32
	//The maximum possible count of lasers on the map.
#define GAME_MEGA_LASERS_MAX_COUNT	32
	//The maximum possible count of asteroids on the map.
#define GAME_ASTEROIDS_MAX_COUNT	32
	//The maximum possible count of bonuses on the map.
#define GAME_BONUSES_MAX_COUNT		32
	//The maximum possible count of bullets on the map.
#define GAME_BULLETS_MAX_COUNT		(UINT16_MAX - 1)
	//The maximum possible count of particles on the map.
#define GAME_PARTICLES_MAX_COUNT	(UINT16_MAX - 1)
	//The maximum possible count of map's rectangles on the map.
#define GAME_RECTANGLES_MAX_COUNT	128

	//KnockBack force of the pilot.
#define GAME_KNOCKBACK_FORCE_OF_THE_PILOT	0.00005f
	//KnockBack force of the ship.
#define GAME_KNOCKBACK_FORCE_OF_THE_SHIP	0.0001f
	//Ship's gravitation force.
#define GAME_SHIP_GRAVITATION_FORCE			0.0001f
	//Sheep's radius of influence.
#define GAME_SHIP_INFLUENCE_RADIUS_COEFFISIENT	5.0f
#define GAME_SHIP_ANGULAR_VELOCITY				0.03f
#define GAME_PILOT_ANGULAR_VELOCITY				GAME_SHIP_ANGULAR_VELOCITY

	//game engine rules

	//bonus pull

#define GAME_BONUS_LOOP		0u
#define GAME_BONUS_LASER	1u
#define GAME_BONUS_BOMB		2u
#define GAME_BONUS_KNIFE	3u

	//bonus pull

	//map pull

	//This map is first map on every match.
#define MAP_TEST_MAP			0u
#define MAP_TUREL_ON_CENTER		1u
#define MAP_CYRCLE_ON_CENTER	2u
#define MAP_DECELERATION_AREA	3u
#define MAP_MEGA_LASERS			4u
#define MAP_DYNAMICAL			5u

	//map pull

	//object pull

#define GAME_OBJECT_ASTEROID	0u

	//object pull

	//game rules

	//If rule applys, players will spawn in random spawns.
	//If the rule does not apply, players will appear in places corresponding to the positions of the buttons on the screen.
#define GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE		(1u << 0u)
#define GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE		(1u << 1u)
#define GAME_RULE_PLAYERS_SPAWN_THIS_BONUS				(1u << 2u)
#define GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS	(1u << 3u)
#define GAME_RULE_TRIPLE_BONUSES						(1u << 4u)
#define GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD				(1u << 5u)
#define GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE				(1u << 6u)
#define GAME_RULE_TEAM_PLAY								(1u << 7u)
#define GAME_RULE_FRENDLY_FIRE							(1u << 8u)
#define GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE			(1u << 9u)
#define GAME_RULE_NEED_KILL_PILOT						(1u << 10u)
#define GAME_RULE_RANDOMLY_MAP							(1u << 11u)
#define GAME_RULE_BONUSES_IS_SPAWNING					(1u << 12u)
#define GAME_RULE_BALANCE_ACTIVE						(1u << 13u)
#define GAME_RULE_KNIFES_CAN_DESTROY_BULLETS			(1u << 14u)

#define GAME_RULE_DEFAULT_GAME_RULES (GAME_RULE_PLAYERS_SPAWN_THIS_BONUS | GAME_RULE_NEED_KILL_PILOT | GAME_RULE_BONUSES_IS_SPAWNING)

	//game rules

	//match rules

#define GAME_RANDOM_ITERATIONS_COUNT			8u
#define GAME_MAPS_COUNT							6u
#define GAME_OBJECTS_COUNT						1u
#define GAME_BONUSES_COUNT						4u
#define GAME_BALANCE_ACTIVATE_DIFFERENCE_SCORES	3u

	//matc rules

	//buttons

	//main menu

#define BUTTON_ID_START_MATCH			1u
#define BUTTON_ID_START_GAME			2u
#define BUTTON_ID_GO_TO_OPTINS_MENU		3u
#define BUTTON_ID_EXIT					4u

	//main menu

	//options menu

#define BUTTON_ID_SET_RANDOM_SPAWN					10u
#define BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION		11u
#define BUTTON_ID_SET_SPAWN_THIS_BONUS				12u
#define BUTTON_ID_SET_SPAWN_THIS_DIFFERENT_BONUSES	13u
#define BUTTON_ID_SET_TRIPLE_BONUSES				14u
#define BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BAFF		15u
#define BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF		16u
#define BUTTON_ID_SET_ACTIVE_BALANCE				17u
#define BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE		18u
#define BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE			19u
#define BUTTON_ID_SET_PILOT_CAN_RESPAWN				20u
#define BUTTON_ID_SET_NEED_KILL_PILOT				21u
#define BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS	22u

#define BUTTON_ID_GO_TO_SELECT_MAP_MENU				30u
#define BUTTON_ID_GO_TO_SELECT_BONUSES_MENU			31u
#define BUTTON_ID_GO_TO_SELECT_START_BONUSES_MENU	32u
#define BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU			33u

	//options menu

	//pause menu

#define BUTTON_ID_RESUME_MATCH		50u
#define BUTTON_ID_GO_TO_MAIN_MENU	51u

	//pause menu

#define BUTTON_ID_SHIP1_SHOOT		60u
#define BUTTON_ID_SHIP1_ROTATE		61u
#define BUTTON_ID_SHIP2_SHOOT		62u
#define BUTTON_ID_SHIP2_ROTATE		63u
#define BUTTON_ID_SHIP3_SHOOT		64u
#define BUTTON_ID_SHIP3_ROTATE		65u
#define BUTTON_ID_SHIP4_SHOOT		66u
#define BUTTON_ID_SHIP4_ROTATE		67u

	//select sheeps menu

#define BUTTON_ID_SELECT_SHIP_1	80u
#define BUTTON_ID_SELECT_SHIP_2	81u
#define BUTTON_ID_SELECT_SHIP_3	82u
#define BUTTON_ID_SELECT_SHIP_4	83u

	//select sheeps menu

	//select bonuses menu

#define BUTTON_ID_SELECT_BONUS				100u
#define BUTTON_ID_SELECT_BONUS_LOOP			(BUTTON_ID_SELECT_BONUS + 0u)
#define BUTTON_ID_SELECT_BONUS_LASER		(BUTTON_ID_SELECT_BONUS + 1u)
#define BUTTON_ID_SELECT_BONUS_BOMB			(BUTTON_ID_SELECT_BONUS + 2u)
#define BUTTON_ID_SELECT_BONUS_KNIFE		(BUTTON_ID_SELECT_BONUS + 3u)

	//select bonuses menu

	//select objects menu

#define BUTTON_ID_SELECT_OBJECT				200u
#define BUTTON_ID_SELECT_OBJECT_ASTEROID	(BUTTON_ID_SELECT_OBJECT + 0u)

	//select objects menu

	//select map pull menu

#define BUTTON_ID_SELECT_MAP	300u
#define BUTTON_ID_SELECT_MAP_0	(BUTTON_ID_SELECT_MAP + MAP_TEST_MAP)
#define BUTTON_ID_SELECT_MAP_1	(BUTTON_ID_SELECT_MAP + MAP_TUREL_ON_CENTER)
#define BUTTON_ID_SELECT_MAP_2	(BUTTON_ID_SELECT_MAP + MAP_CYRCLE_ON_CENTER)
#define BUTTON_ID_SELECT_MAP_3	(BUTTON_ID_SELECT_MAP + MAP_DECELERATION_AREA)

	//select map pull menu

	//threads

//TIme (in milliseconds) between to starts of tiks.
#define THREAD_PHYSIC_TIK_PERIOD	10
#define THREAD_INPUT_TIK_PERIOD		10

#define THREAD_PHASE_1		0u
#define THREAD_PHASE_2		1u
#define THREAD_COMPLETE		0x01u
#define THREAD_MASK_CELL	0x03u
#define THREAD_MASK_FULL	0xFFFFu
#define THREAD_MASK_NULL	0x0000u
#define THREAD_NEW_CYCLE	0x0000u
#define THREAD_0			0u
#define THREAD_1			2u
#define THREAD_2			4u
#define THREAD_3			6u

#define THREAD_PHASE_1_COMPLETE	(				\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_0)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_1)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_2)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_3)		\
	)

#define THREAD_PHASE_2_COMPLETE	(				\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_0)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_1)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_2)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_3)		\
	)

#define THREAD_ALL_COMPLETE ((THREAD_COMPLETE << THREAD_PHASE_1) | (THREAD_COMPLETE << THREAD_PHASE_2))
#define THREAD_ALL_PHASE_COMPLETE	(		\
	(THREAD_ALL_COMPLETE << THREAD_0)	|	\
	(THREAD_ALL_COMPLETE << THREAD_1)	|	\
	(THREAD_ALL_COMPLETE << THREAD_2)	|	\
	(THREAD_ALL_COMPLETE << THREAD_3)		\
	)


	//threads
