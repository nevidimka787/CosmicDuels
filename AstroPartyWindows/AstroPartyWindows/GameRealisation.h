#ifndef GAME_REALISATION_H
#define GAME_REALISATION_H

#include <iostream>
#include <limits.h>
#include "GameEngine.h"

namespace Game
{
#define GAME_ANTY_GRAVS_MAX_COUNT	2
#define GAME_PLAYERS_MAX_COUNT		4
#define GAME_TURELS_MAX_COUNT		1
#define GAME_ASTEROIDS_MAX_COUNT	16
#define GAME_BONUSES_MAX_COUNT		16
#define GAME_BULLETS_MAX_COUNT		UINT16_MAX
#define GAME_PARTICLES_MAX_COUNT	UINT16_MAX
#define GAME_RECTANGLES_MAX_COUNT	128

#define MAP_TEST_MAP 0x00

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

#define RANDOM_ITERATIONS_COUNT 8
#define BALANCE_ACTIVATE_DIFFERENCE_SCORES 3 

	//game variables

	static bool* shoot_keys;
	static bool* rotate_keys;

	static uint8_t* scores;
	static uint8_t current_map_id;

	static uint8_t anty_gravs_count;
	static uint8_t players_count;
	static uint8_t turels_count;
	static uint8_t start_bonus;
	static uint16_t asterooids_count;
	static uint16_t bonuses_count;
	static uint16_t bullets_count;
	static uint16_t particles_count;
	static uint16_t rectangles_count;

	static uint16_t start_bonus;

	static uint32_t game_rules;

	//game variables



	//game objects

	static Sheep** sheeps;
	static Pilot** pilots;

	static DynamicEntity** bullets;
	static DynamicEntity** particles;
	static Asteroid** asteroids;
	static Bonus** bonuses;

	static Map* map;
	static Rectangle* rectangles;
	static StaticEntity** anty_gravs;
	static Turel** turels;

	//game objects

	void GameInit();
	void MachInit(uint8_t players_count, uint32_t game_rules, uint16_t start_bonus);
	void LevelInit(uint8_t map_id);
	void Recalculate();

	uint8_t GenerateRandomBonus();
	uint8_t GetMaxScore();
};

#endif //GAME_REALISATION_H

