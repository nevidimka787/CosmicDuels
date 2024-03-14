#pragma once

//Count of tiks between pre last ship was killed and match will be completed.
#define GAME_END_MATCH_DELLAY 100

//Maximum count of tiks between two clicks needed for activated double click event.
#define GAME_DOUBLE_CLK_TIME	10
//Count of tiks between to times of activating burnout.
#define GAME_BURNOUT_COULDOWN	50
//Count of tiks during whitch the camera will be having bluring effect.
#define GAME_BOMB_STUNING_TIME	1000

/*
asteroids, bombs, bonuses, bullets,
knifes, lasers, particles, ships,
pilots, players, gravity generators,
turrets, mega lasers
*/
#define GAME_ENTITIES_TYPES_COUNT 13



#define GAME_OBJECT_TYPES_COUNT		1

#define GAME_OBJECT_TYPE_NULL			0u
#define GAME_OBJECT_TYPE_ASTEROID		1u
#define GAME_OBJECT_TYPE_BULLET			2u
#define GAME_OBJECT_TYPE_BOMB			3u
#define GAME_OBJECT_TYPE_KNIFE			4u
#define GAME_OBJECT_TYPE_LASER_BOMB		5u
#define GAME_OBJECT_TYPE_LOOP_BULLET	6u
#define GAME_OBJECT_TYPE_LOOP_BOMB		7u
#define GAME_OBJECT_TYPE_LOOP_KNIFE		8u

#define GAME_OBJECT_TYPE_LOOP_BOMB_KNIFE	9u
#define GAME_OBJECT_TYPE_LASER_BOMB_KNIFE	10u
#define GAME_OBJECT_TYPE_LOOP_LASER_KNIFE	11u


//tics to next shoot
#define GAME_DELLAY_BETWEEN_SHOOTS		30
#define GAME_ADD_DELLAY_BETWEEN_TRIPLES	10 //dellay added to GAME_DELLAY_BETWEEN_SHOOTS
#define GAME_ADD_DELLAY_BONUS_USE		20 //dellay added to GAME_DELLAY_BETWEEN_SHOOTS
#define GAME_ADD_DELLAY_COMBO_USE		5  //dellay added to GAME_ADD_DELLAY_BETWEEN_BONUSES_USE

//The maximum count of deceleration areas on the map.
#define GAME_DECEL_AREAS_MAX_COUNT	32
//The maximum count of gravity generators on the map.
#define GAME_GRAV_GENS_MAX_COUNT	32
//The maximum possible count of players on the map.
#define GAME_PLAYERS_MAX_COUNT		4
//The maximum possible count of knifes on the map.
#define GAME_KNIFES_MAX_COUNT		(UINT8_MAX - 1)
//The maximum possible count of lasers on the map.
#define GAME_LASERS_MAX_COUNT		(UINT8_MAX - 1)
//The maximum possible count of bombs on the map.
#define GAME_BOMBS_MAX_COUNT		(UINT8_MAX - 1)
//The maximum possible count of turrets on the map.
#define GAME_TURRETS_MAX_COUNT		32
//The maximum possible count of lasers on the map.
#define GAME_MEGA_LASERS_MAX_COUNT	4
//The maximum possible count of asteroids on the map.
#define GAME_ASTEROIDS_MAX_COUNT	(UINT8_MAX - 1)
//The maximum possible count of annihilation area generators on the map.
#define GAME_ANNIH_AREAS_GENS_MAX_COUNT	32
//The maximum possible count of bonuses on the map.
#define GAME_BONUSES_MAX_COUNT		(UINT8_MAX - 1)
//The maximum possible count of bullets on the map.
#define GAME_BULLETS_MAX_COUNT		(UINT16_MAX - 1)
//The maximum possible count of particles on the map.
#define GAME_DYNAMIC_PARTICLES_MAX_COUNT	(UINT16_MAX - 1)
//The maximum possible count of particles on the map.
#define GAME_PARTICLES_MAX_COUNT	(UINT16_MAX - 1)
//The maximum possible count of portals on the map.
#define GAME_PORTALS_MAX_COUNT		(UINT8_MAX - 1)
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
//Default anguar velocity of ships.
#define GAME_SHIP_ANGULAR_VELOCITY				0.03f
//Default angular velocity of pilots.
#define GAME_PILOT_ANGULAR_VELOCITY				GAME_SHIP_ANGULAR_VELOCITY

	//game engine rules

	//bonus pull

//bonus id
#define GAME_BONUS_ID_LOOP		0u
//bonus id
#define GAME_BONUS_ID_LASER		1u
//bonus id
#define GAME_BONUS_ID_BOMB		2u
//bonus id
#define GAME_BONUS_ID_KNIFE		3u
//buff id
#define GAME_BONUS_ID_TRIPLE	4u
//buff id
#define GAME_BONUS_ID_SHIELD	5u
//buff id
#define GAME_BONUS_ID_STREAM	6u
//rule id
#define GAME_BONUS_ID_REVERS	7u


	//bonus pull

	//map pull

//map id.
#define MAP_ORBIT_MAP			0u
//map id.
#define MAP_TURRET_ON_CENTER	1u
//map id.
#define MAP_CYRCLE_ON_CENTER	2u
//map id.
#define MAP_DECELERATION_AREA	3u
//map id.
#define MAP_MEGA_LASERS			4u
//map id.
#define MAP_DYNAMICAL			5u
//map id.
#define MAP_DESTROYABLE			6u
//map id.
#define MAP_AGGRESSIVE			7u
//map id.
#define MAP_BROKEN				8u
//map id.
#define MAP_PORTAL				9u
//map id
#define MAP_NO_CENTER			10u
//map id
#define MAP_FORTRES				11u

	//map pull

	//object pull

//object id
#define GAME_OBJECT_ASTEROID	0u

	//object pull

	//game rules

//If the rule applys then players will spawn in random spawns
//else players will appear in places corresponding to the positions of the buttons on the screen.
#define GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE		(1u << 0u)
//If the rule applys then spawning players will have random directions
//else players will hahe default direction.
#define GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE		(1u << 1u)
//If the rule applys then players will spawn with bonuses
//else players will spawn with out bonuses.
#define GAME_RULE_PLAYERS_SPAWN_THIS_BONUS				(1u << 2u)
//If the rule applys then spawning players will have different bonuses
//else players will have same bonuses.
#define GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS	(1u << 3u)
//If the rule applys then every collecting by ship bonus will multiple free times.
#define GAME_RULE_TRIPLE_BONUSES						(1u << 4u)
//Spawning players will have shields.
#define GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD				(1u << 5u)
//Spawning players will have powered gun shooting free bullets.
#define GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE				(1u << 6u)
//NOT USE
#define GAME_RULE_TEAM_PLAY								(1u << 7u)
//Palyer can kill player from same team. 
#define GAME_RULE_FRENDLY_FIRE							(1u << 8u)
//Player can restor player frome same team.
#define GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE			(1u << 9u)
//If the rule apply then pilots can autorestore and for completing math need kill pilots
//else pilots can not autorestore and for completing math need kill ships only.
#define GAME_RULE_NEED_KILL_PILOT						(1u << 10u)
//NOT USE
#define GAME_RULE_RANDOMLY_MAP							(1u << 11u)
//NOT USE
#define GAME_RULE_BONUSES_IS_SPAWNING					(1u << 12u)
//NOT REALISATED
#define GAME_RULE_BALANCE_ACTIVE						(1u << 13u)
//If the rule applys then bullets will be destroed then collision with knifes and knifes will be lossing health
//else bullets will not collision with knifes.
#define GAME_RULE_KNIFES_CAN_DESTROY_BULLETS			(1u << 14u)

//game rules activing after start of the game
#define GAME_RULE_DEFAULT_GAME_RULES (GAME_RULE_PLAYERS_SPAWN_THIS_BONUS | GAME_RULE_NEED_KILL_PILOT | GAME_RULE_BONUSES_IS_SPAWNING)

	//game rules

	//match rules

//how many times elements will be swaping
#define GAME_RANDOM_ITERATIONS_COUNT			8u
//count of maps
#define GAME_MAPS_COUNT							12u
//count of objects types
#define GAME_OBJECTS_COUNT						1u
//count og bonuses types
#define GAME_BONUS_INVENTORY_SIZE				8u
//How mutch differenc in score need to activating autobalance.
#define GAME_BALANCE_ACTIVATE_DIFFERENCE_SCORES	3u
#define GAME_SCORE_START	0

#define GAME_FULL_MATCH_ROUNDS		5
#define GAME_WINNERS_MATCH_ROUNDS	1
#define GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE	0.8f

	//matc rules

	//buttons

	//main menu

//button id
#define BUTTON_ID__START_MATCH			1u
//button id
#define BUTTON_ID__START_GAME			2u
//button id
#define BUTTON_ID__GO_TO_OPTINS_MENU	3u
//button id
#define BUTTON_ID__EXIT					4u

	//main menu

	//options menu

//set gamerule
#define BUTTON_ID__SET_RANDOM_SPAWN					10u
//set gamerule
#define BUTTON_ID__SET_RANDOM_SPAWN_DIRECTION		11u
//set gamerule
#define BUTTON_ID__SET_SPAWN_THIS_BONUS				12u
//set gamerule
#define BUTTON_ID__SET_SPAWN_THIS_DIFFERENT_BONUSES	13u
//set gamerule
#define BUTTON_ID__SET_TRIPLE_BONUSES				14u
//set gamerule
#define BUTTON_ID__SET_SPAWN_THIS_TRIPLE_BAFF		15u
//set gamerule
#define BUTTON_ID__SET_SPAWN_THIS_SHIELD_BAFF		16u
//set gamerule
#define BUTTON_ID__SET_ACTIVE_BALANCE				17u
//set gamerule
#define BUTTON_ID__SET_FRIEDLY_SHEEP_CAN_RESTORE	18u
//set gamerule
#define BUTTON_ID__SET_ACTIVE_FRIENDLY_FIRE			19u
//set gamerule
#define BUTTON_ID__SET_PILOT_CAN_RESPAWN			20u
//set gamerule
#define BUTTON_ID__SET_NEED_KILL_PILOT				21u
//set gamerule
#define BUTTON_ID__SET_KNIFES_CAN_DESTROY_BULLETS	22u

//menu navigation
#define BUTTON_ID__GO_TO_SELECT_MAP_MENU			30u
//menu navigation
#define BUTTON_ID__GO_TO_SELECT_BONUSES_MENU		31u
//menu navigation
#define BUTTON_ID__GO_TO_SELECT_START_BONUSES_MENU	32u
//menu navigation
#define BUTTON_ID__GO_TO_SELECT_OBJECTS_MENU		33u

	//options menu

	//pause menu

//menu navigation
#define BUTTON_ID__RESUME_MATCH		50u
//menu navigation
#define BUTTON_ID__GO_TO_MAIN_MENU	51u

	//pause menu

//controles (NOT REALISATED)
#define BUTTON_ID__SHIP1_SHOOT		60u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP1_ROTATE		61u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP2_SHOOT		62u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP2_ROTATE		63u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP3_SHOOT		64u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP3_ROTATE		65u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP4_SHOOT		66u
//controles (NOT REALISATED)
#define BUTTON_ID__SHIP4_ROTATE		67u

	//select sheeps menu

//selected teams
#define BUTTON_ID__SELECT_SHIP_1	80u
//selected teams
#define BUTTON_ID__SELECT_SHIP_2	81u
//selected teams
#define BUTTON_ID__SELECT_SHIP_3	82u
//selected teams
#define BUTTON_ID__SELECT_SHIP_4	83u

	//select sheeps menu

	//select bonuses menu

//select bonus
#define BUTTON_ID__SELECT_BONUS				100u
//select bonus
#define BUTTON_ID__SELECT_BONUS_LOOP		(BUTTON_ID__SELECT_BONUS + 0u)
//select bonus
#define BUTTON_ID__SELECT_BONUS_LASER		(BUTTON_ID__SELECT_BONUS + 1u)
//select bonus
#define BUTTON_ID__SELECT_BONUS_BOMB		(BUTTON_ID__SELECT_BONUS + 2u)
//select bonus
#define BUTTON_ID__SELECT_BONUS_KNIFE		(BUTTON_ID__SELECT_BONUS + 3u)

	//select bonuses menu

	//select objects menu

//select object
#define BUTTON_ID__SELECT_OBJECT			200u
//select object
#define BUTTON_ID__SELECT_OBJECT_ASTEROID	(BUTTON_ID__SELECT_OBJECT + 0u)

	//select objects menu

	//select map pull menu

//select map
#define BUTTON_ID__SELECT_MAP	300u
//select map
#define BUTTON_ID__SELECT_MAP_0	(BUTTON_ID__SELECT_MAP + MAP_ORBIT_MAP)
//select map
#define BUTTON_ID__SELECT_MAP_1	(BUTTON_ID__SELECT_MAP + MAP_TURRET_ON_CENTER)
//select map
#define BUTTON_ID__SELECT_MAP_2	(BUTTON_ID__SELECT_MAP + MAP_CYRCLE_ON_CENTER)
//select map
#define BUTTON_ID__SELECT_MAP_3	(BUTTON_ID__SELECT_MAP + MAP_DECELERATION_AREA)
//select map
#define BUTTON_ID__SELECT_MAP_4	(BUTTON_ID__SELECT_MAP + MAP_MEGA_LASERS)
//select map
#define BUTTON_ID__SELECT_MAP_5	(BUTTON_ID__SELECT_MAP + MAP_DYNAMICAL)
//select map
#define BUTTON_ID__SELECT_MAP_6	(BUTTON_ID__SELECT_MAP + MAP_DESTROYABLE)
//select map
#define BUTTON_ID__SELECT_MAP_7	(BUTTON_ID__SELECT_MAP + MAP_AGGRESSIVE)
//select map
#define BUTTON_ID__SELECT_MAP_8	(BUTTON_ID__SELECT_MAP + MAP_BROKEN)

	//select map pull menu

	//ships bonuses

#define SHIP_BONUS__LOOP_BULLETS_IN_LOOP	SHIP_BULLETS_IN_LOOP
#define SHIP_BONUS__LOOP_BULLETS_RADIUS		(BULLET_DEFAULT_RADIUS / 2.0f)
#define SHIP_BONUS__LOOP_BULLET_VELOCITY	BULLET_DEFAULT_VELOCITY

	//ships bonuses

	//ships super bonuses

//count of bombs in "loop of bombs" ability
#define SHIP_SUPER_BONUS__BOMBS_IN_LOOP			8u
//velocity of each bomb in loop in "loop of bombs" ability
#define SHIP_SUPER_BONUS__BOMBS_LOOP_VELOCITY	BULLET_DEFAULT_VELOCITY
//count of bombs in one shoot in "line of bombs" ability
#define SHIP_SUPER_BONUS__BOMBS_IN_LASER		14u
//velocity of fastest bomb in one shooted line in "line of bombs" ability
#define SHIP_SUPER_BONUS__BOMBS_LASER_VELOCITY	(BULLET_DEFAULT_VELOCITY * 2.0f)
//radius of each bomb in one shooted line in "line of bombs" ability
#define SHIP_SUPER_BONUS__BOMBS_LASER_RADIUS	(BOMB_DEFAULT_RADIUS / 2.0f)
//count of knifes in loop in "cyrcle knife" ability
#define SHIP_SUPER_BONUS__KNIFES_IN_LOOP		16u

//count of bullets in loop created by small asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_SMALL		10
//radius of bullets in loop created by small asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_SMALL		(BULLET_DEFAULT_RADIUS / 2.0f)
//count of bullets in loop created by medium asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_MEDIUM	(SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_SMALL * 2)
//radius of bullets in loop created by medium asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_MEDIUM		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_SMALL
//count of bullets in loop created by big asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_BIG		(SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_MEDIUM * 2)
//radius of bullets in loop created by big asteroid and "powered laser" ability
#define SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_BIG		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_SMALL

//count of health points of knife created by "long knife" ability
#define SHIP_SUPER_BONUS__LONG_KNIFE_HEALTH	10u

//Count of tick in that annihilation area will be existing.
#define SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD	600u

#define SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP	24
#define SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP_DEFAULT_VELOCITY		(BULLET_DEFAULT_VELOCITY * 1.0f)

#define SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER	25
#define SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER_DEFAULT_VELOCITY		(BULLET_DEFAULT_VELOCITY * 1.0f)

#define SHIP_SUPER_BONUS__LONG_KNIFES_IN_LOOP	10


	//ships super bonuses

	//threads

//Time (in microseconds) between two physic tiks.
#define THREAD_PHYSIC_TIK_PERIOD	(unsigned)(10000.0f * 0.995f)
//Time (in milliseconds) between two console updates.
#define THREAD_INPUT_TIK_PERIOD		10

//shift to data cell
#define THREAD_PHASE_1		0u
//shift to data cell
#define THREAD_PHASE_2		1u
//cell value
#define THREAD_COMPLETE		0x01u
//mask of one cell
#define THREAD_MASK_CELL	0x03u
//fill full
#define THREAD_MASK_FULL	0xFFFFu
//not fill
#define THREAD_MASK_NULL	0x0000u
#define THREAD_NEW_CYCLE	0x0000u
//shift to data cell
#define THREAD_0			0u
//shift to data cell
#define THREAD_1			2u
//shift to data cell
#define THREAD_2			4u
//shift to data cell
#define THREAD_3			6u

//template
#define THREAD_PHASE_1_COMPLETE	(				\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_0)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_1)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_2)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_3)		\
	)

//template
#define THREAD_PHASE_2_COMPLETE	(				\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_0)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_1)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_2)	|	\
	(THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_3)		\
	)

//template
#define THREAD_ALL_COMPLETE ((THREAD_COMPLETE << THREAD_PHASE_1) | (THREAD_COMPLETE << THREAD_PHASE_2))
//template
#define THREAD_ALL_PHASE_COMPLETE	(		\
	(THREAD_ALL_COMPLETE << THREAD_0)	|	\
	(THREAD_ALL_COMPLETE << THREAD_1)	|	\
	(THREAD_ALL_COMPLETE << THREAD_2)	|	\
	(THREAD_ALL_COMPLETE << THREAD_3)		\
	)


	//threads
