#pragma once

/*
Asteroid
*/
#define GAME_OBJECT_TYPES_COUNT	1

#define GAME_ANTY_GRAVS_MAX_COUNT	2
//The maximum possible count of players on the map.
#define GAME_PLAYERS_MAX_COUNT		4
	//The maximum possible count of knifes on the map.
#define GAME_KNIFES_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
	//The maximum possible count of lasers on the map.
#define GAME_LASERS_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
	//The maximum possible count of bombs on the map.
#define GAME_BOMBS_MAX_COUNT		(UINT8_MAX - 1)
	//The maximum possible count of turels on the map.
#define GAME_TURELS_MAX_COUNT		1
	//The maximum possible count of lasers on the map.
#define GAME_MEGA_LASERS_MAX_COUNT	2
	//The maximum possible count of asteroids on the map.
#define GAME_ASTEROIDS_MAX_COUNT	16
	//The maximum possible count of bonuses on the map.
#define GAME_BONUSES_MAX_COUNT		32
	//The maximum possible count of bullets on the map.
#define GAME_BULLETS_MAX_COUNT		(UINT16_MAX - 1)
	//The maximum possible count of particles on the map.
#define GAME_PARTICLES_MAX_COUNT	(UINT16_MAX - 1)
	//The maximum possible count of map's rectangles on the map.
#define GAME_RECTANGLES_MAX_COUNT	128

	//KnockBack force of the pilot.
#define GAME_KNOCKBACK_FORCE_OF_THE_PILOT	0.001f
	//KnockBack force of the ship.
#define GAME_KNOCKBACK_FORCE_OF_THE_SHIP	0.00001f
	//Ship's gravitation force.
#define GAME_SHIP_GRAVITATION_FORCE			0.001f
	//Sheep's radius of influence.
#define GAME_SHIP_INFLUENCE_RADIUS			0.01f
#define GAME_SHIP_ANGULAR_VELOCITY			0.1f
#define GAME_PILOT_ANGULAR_VELOCITY			0.1f

	//game engine rules

	//map pull

	//This map is first map on every match.
#define MAP_TEST_MAP 0x00

	//map pull

	//game rules

	//If rule applys, players will spawn in random spawns.
	//If the rule does not apply, players will appear in places corresponding to the positions of the buttons on the screen.
#define GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE	(1 << 0)
#define GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE	(1 << 1)
#define GAME_RULE_PLAYERS_SPAWN_THIS_BONUS			(1 << 2)
#define GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS	(1 << 3)
#define GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD			(1 << 4)
#define GAME_RULE_START_BONUS_RANDOMIZE				(1 << 5)
#define GAME_RULE_TEAM_PLAY							(1 << 6)
#define GAME_RULE_FRENDLY_FIRE						(1 << 7)
#define GAME_RULE_PILOT_CAN_RESPAWN					(1 << 8)
#define GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE		(1 << 9)
#define GAME_RULE_NEED_KILL_PILOT					(1 << 10)
#define GAME_RULE_RANDOMLY_MAP						(1 << 11)
#define GAME_RULE_ASTEROIDS_IS_SPAWNING				(1 << 12)
#define GAME_RULE_BONUSES_IS_SPAWNING				(1 << 13)
#define GAME_RULE_BALANCE_ACTIVE					(1 << 14)
#define GAME_RULE_KNIFES_CAN_DESTROY_BULLETS			(1 << 15)

#define GAME_RULE_DEFAULT_GAME_RULES (GAME_RULE_PLAYERS_SPAWN_THIS_BONUS | GAME_RULE_START_BONUS_RANDOMIZE | GAME_RULE_NEED_KILL_PILOT | GAME_RULE_ASTEROIDS_IS_SPAWNING | GAME_RULE_BONUSES_IS_SPAWNING)

	//game rules

	//match rules

#define GAME_RANDOM_ITERATIONS_COUNT			8
#define GAME_MAPS_COUNT							1
#define GAME_OBJECTS_COUNT						1
#define GAME_BALANCE_ACTIVATE_DIFFERENCE_SCORES	3

	//matc rules

	//buttons

	//main menu

#define BUTTON_ID_START_MATCH			1
#define BUTTON_ID_START_GAME			2
#define BUTTON_ID_GO_TO_OPTINS_MENU		3
#define BUTTON_ID_EXIT					4

	//main menu

	//options menu

#define BUTTON_ID_SET_RANDOM_SPAWN					10
#define BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION		11
#define BUTTON_ID_SET_SPAWN_THIS_BONUS				12
#define BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS		13
#define BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BUFF		14
#define BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF		15
#define BUTTON_ID_SET_ACTIVE_BALANCE				16
#define BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE		17
#define BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE			18
#define BUTTON_ID_SET_PILOT_CAN_RESPAWN				19
#define BUTTON_ID_SET_NEED_KILL_PILOT				20
#define BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS	21

#define BUTTON_ID_GO_TO_SELECT_MAP_MENU				30
#define BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU			32

	//options menu

	//pause menu

#define BUTTON_ID_RESUME_MATCH		50
#define BUTTON_ID_GO_TO_MAIN_MENU	51

	//pause menu

#define BUTTON_ID_SHIP1_SHOOT		60
#define BUTTON_ID_SHIP1_ROTATE		61
#define BUTTON_ID_SHIP2_SHOOT		62
#define BUTTON_ID_SHIP2_ROTATE		63
#define BUTTON_ID_SHIP3_SHOOT		64
#define BUTTON_ID_SHIP3_ROTATE		65
#define BUTTON_ID_SHIP4_SHOOT		66
#define BUTTON_ID_SHIP4_ROTATE		67

	//select sheeps menu

#define BUTTON_ID_SELECT_SHIP_1	80
#define BUTTON_ID_SELECT_SHIP_2	81
#define BUTTON_ID_SELECT_SHIP_3	82
#define BUTTON_ID_SELECT_SHIP_4	83

	//select sheeps menu

	//select objects menu

#define BUTTON_ID_SELECT_OBJECT				100
#define BUTTON_ID_SELECT_OBJECT_ASTEROID	(BUTTON_ID_SELECT_OBJECT + 0)

	//select objects menu

	//select map pull menu

#define BUTTON_ID_SELECT_MAP	200
#define BUTTON_ID_SELECT_MAP_0	(BUTTON_ID_SELECT_MAP + MAP_TEST_MAP)

	//select map pull menu