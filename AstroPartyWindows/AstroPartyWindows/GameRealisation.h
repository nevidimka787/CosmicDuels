#ifndef GAME_REALISATION_H
#define GAME_REALISATION_H

#include <iostream>
#include <limits.h>
#include <random>
#include "GameEngine.h"
#include "Menu.h"
#include "MenuFunctions.h"

namespace Game
{
	//game engine rules

#define GAME_ANTY_GRAVS_MAX_COUNT	2
#define GAME_PLAYERS_MAX_COUNT		4
#define GAME_KNIFES_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
#define GAME_LAZERS_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
#define GAME_MINES_MAX_COUNT		(UINT8_MAX - 1)
#define GAME_TURELS_MAX_COUNT		1
#define GAME_MEGA_LAZERS_MAX_COUNT	2
#define GAME_ASTEROIDS_MAX_COUNT	16
#define GAME_BONUSES_MAX_COUNT		16
#define GAME_BULLETS_MAX_COUNT		(UINT16_MAX - 1)
#define GAME_PARTICLES_MAX_COUNT	(UINT16_MAX - 1)
#define GAME_RECTANGLES_MAX_COUNT	128

	//game engine rules

	//map pull

#define MAP_TEST_MAP 0x00

	//map pull

	//game rules

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

#define GAME_RULE_DEFAULT_GAME_RULES (GAME_RULE_PLAYERS_SPAWN_THIS_BONUS | GAME_RULE_START_BONUS_RANDOMIZE | GAME_RULE_NEED_KILL_PILOT | GAME_RULE_ASTEROIDS_IS_SPAWNING | GAME_RULE_BONUSES_IS_SPAWNING)

	//game rules

	//match rules

#define RANDOM_ITERATIONS_COUNT 8
#define MAPS_COUNT 1
#define BALANCE_ACTIVATE_DIFFERENCE_SCORES 3

	//matc rules

	//buttons
	
	//main menu

#define BUTTON_ID_START_MATCH		0
#define BUTTON_ID_GO_TO_OPTINS_MENU	1
#define BUTTON_ID_EXIT				2

	//main menu

	//options menu

#define BUTTON_ID_SET_RANDOM_SPAWN				10
#define BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION	11
#define BUTTON_ID_SET_SPAWN_THIS_BONUS			12
#define BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS	13
#define BUTTON_ID_SET_SPAWN_WHIS_TRIPLE_BUFF	14
#define BUTTON_ID_SET_SPAWN_WHIT_SHIELD_BAFF	15
#define BUTTON_ID_GO_TO_SELECT_MACH_TYPE_MENU	16
#define BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE	17
#define BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE		18
#define BUTTON_ID_SET_PILOT_CAN_RESPAWN			19
#define BUTTON_ID_SET_NEED_KILL_PILOT			20
#define BUTTON_ID_GO_TO_SELECT_MAP_MENU			21
#define BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU		22
#define BUTTON_ID_SET_ACTIVE_BALANCE			23

	//options menu

	//pause menu

#define BUTTON_ID_RESUME_MATCH		50
#define BUTTON_ID_GO_TO_MAIN_MENU	51

	//pause menu

	//select sheeps menu

#define BUTTON_ID_SELECT_SHEEP_1	60
#define BUTTON_ID_SELECT_SHEEP_2	61
#define BUTTON_ID_SELECT_SHEEP_3	62
#define BUTTON_ID_SELECT_SHEEP_4	63

	//select sheeps menu

	//select objects menu

#define BUTTON_ID_SELECT_OBJECT_ASTEROID	100

	//select objects menu

	//select map pull menu

#define BUTTON_ID_SELECT_MAP	200
#define BUTTON_ID_SELECT_MAP_0 (BUTTON_ID_SELECT_MAP + MAP_TEST_MAP)

	//select map pull menu

	//buttons


	//game variables

	static bool start_game;
	static bool pause_game;

	static bool* shoot_keys;
	static bool* rotate_keys;
	static bool* sheeps_can_shoot;

	static uint8_t* scores;
	static uint8_t* last_match_scores;
	static uint8_t current_map_id;

	static uint8_t grav_gens_count;
	static uint8_t players_count;
	static uint8_t sheeps_count;
	static uint8_t pilots_count;
	static uint8_t knifes_count;
	static uint8_t lazers_count;
	static uint8_t mines_count;
	static uint8_t turels_count;
	static uint8_t mega_lazers_count;
	static uint16_t asterooids_count;
	static uint16_t bonuses_count;
	static uint16_t bullets_count;
	static uint16_t particles_count;
	static uint16_t rectangles_count;

	static uint16_t start_bonus;

	static uint32_t game_rules;

	//game variables

	//game lists

	static uint32_t* map_pull_array;

	//game lists

	//game objects

	static Sheep* sheeps;
	static Pilot* pilots;

	static Bullet* bullets;
	static Knife* knifes;
	static Lazer* lazers;
	static Mine* mines;
	static DynamicEntity* particles;
	static Asteroid* asteroids;
	static Bonus* bonuses;

	static Map* map;
	static Rectangle* rectangles;
	static GravGen* grav_gens;
	static Turel* turels;
	static MegaLazer* mega_lazers;

	//game objects

	//menu objects

	static Menu* current_active_menu;

	static Menu* sheeps_control_menu;

	static Menu* main_menu;
	static Menu* option_menu;
	static Menu* pause_menu;

	static Menu* sheeps_select_menu;
	static Menu* team_sheeps_select_menu;
	static Menu* maps_select_menu;
	static Menu* spawning_objects_select_menu;

	//menu objects

	void GameInit();
	void MachInit();
	void LevelInit(uint8_t map_id);
	void MenusInit();

	void DynamicEntitiesCollisions(DynamicEntity* entities, uint16_t entities_count);
	void DynamicEntitiesCollisions(DynamicEntity* entities1, uint16_t entities1_count, DynamicEntity* entities2, uint16_t entities2_count);
	void DynamicEntitiesCollisions(DynamicEntity* entities, uint16_t entities_count, Map* map);
	void MinesCollisions();
	void DynamicEntitiesAddForce(DynamicEntity* entities, uint16_t entities_count, Vec2F* force);
	void DynamicEntitiesAddForce(DynamicEntity* entities, uint16_t entities_count, GravGen* grav_gen);
	void DynamicEntitiesAddForce(DynamicEntity* entities, uint16_t entities_count, GravGen* grav_gens, uint8_t grav_gens_count);

	void UpdateBullets();
	void UpdateMines();
	void Recalculate();
	void SheepsShoot();

	//Not checking nullprt!
	void AddEntity(Bonus* new_bonus);
	//Not checking nullprt!
	void AddEntity(Asteroid* new_asteroid);
	//Not checking nullprt!
	void AddEntity(Bullet* new_bullet);
	//Not checking nullprt!
	void AddEntity(Knife* new_knife);
	//Not checking nullprt!
	void AddEntity(Mine* new_mine);
	//Not checking nullprt!
	void AddEntity(Lazer* new_lazer);
	//Not checking nullprt!
	void AddEntity(DynamicEntity* new_particle);

	//Not checking nullprt!
	void DeleteEntity(Bonus* deleting_bonus);
	//Not checking nullprt!
	void DeleteEntity(Asteroid* deleting_asteroid);
	//Not checking nullprt!
	void DeleteEntity(Bullet* deleting_bullet);
	//Not checking nullprt!
	void DeleteEntity(Knife* deleting_knife);
	//Not checking nullprt!
	void DeleteEntity(Mine* deleting_mine);
	//Not checking nullprt!
	void DeleteEntity(Lazer* deleting_lazer);
	//Not checking nullprt!
	void DeleteParticle(DynamicEntity* deleting_particle);

	uint8_t GenerateRandomBonus();
	uint8_t GetRandomMap();
	uint8_t GetMaxScore();
};

#endif //GAME_REALISATION_H

