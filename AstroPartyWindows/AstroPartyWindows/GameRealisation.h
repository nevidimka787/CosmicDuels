#ifndef GAME_REALISATION_H
#define GAME_REALISATION_H

#include <iostream>
#include <limits.h>
#include <random>
#include "GameEngine.h"
#include "Menu.h"

namespace Game
{
	//game engine rules

#define GAME_ANTY_GRAVS_MAX_COUNT	2
#define GAME_PLAYERS_MAX_COUNT		4
#define GAME_KNIFES_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
#define GAME_LAZERS_MAX_COUNT		(GAME_PLAYERS_MAX_COUNT * 2)
#define GAME_BOMBS_MAX_COUNT		(UINT8_MAX - 1)
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

#define RANDOM_ITERATIONS_COUNT				8
#define MAPS_COUNT							1
#define OBJECTS_COUNT						1
#define BALANCE_ACTIVATE_DIFFERENCE_SCORES	3

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
#define BUTTON_ID_SET_ACTIVE_BALANCE			16
#define BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE	17
#define BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE		18
#define BUTTON_ID_SET_PILOT_CAN_RESPAWN			19
#define BUTTON_ID_SET_NEED_KILL_PILOT			20

#define BUTTON_ID_GO_TO_SELECT_MAP_MENU			30
#define BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU		32

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

	//buttons

	//typedefs
	
	typedef typename uint8_t players_count_t;
	typedef typename uint16_t entities_count_t;
	typedef typename uint8_t map_elements_count_t;
	typedef typename uint16_t maps_count_t;
	typedef typename uint32_t game_rules_t;
	typedef typename uint8_t score_t;

#define PLAYERS_COUNT_T_MAX			UINT8_MAX
#define ENTITIES_COUNT_T_MAX		UINT16_MAX
#define MAP_ELEMENTS_COUNT_T_MAX	UINT8_MAX
#define MAPS_COUNT_T_MAX			UINT16_MAX
#define GAME_RULES_T_MAX			UINT32_MAX
#define SCORE_T_MAX					UINT8_MAX

#define PLAYERS_COUNT_T_MIN			UINT8_MIN
#define ENTITIES_COUNT_T_MIN		UINT16_MIN
#define MAP_ELEMENTS_COUNT_T_MIN	UINT8_MIN
#define MAPS_COUNT_T_MIN			UINT16_MIN
#define GAME_RULES_T_MIN			UINT32_MIN
#define SCORE_T_MIN					UINT8_MIN

	//typedefs

	//game variables

	//Game status true = The game is on. false = The game stops.
	static bool start_game;
	//Game status true = The game is paused. false = The game is on.
	static bool pause_game;

	//Controled entities refer to this array.
	static bool* shoot_keys;
	//Controled entities refer to this array.
	static bool* rotate_keys;
	//true = The sheep can shoot one time. false = The sheep can't shoot.
	static bool* sheeps_can_shoot;

	static maps_count_t selected_maps_id_array_length;

	//Current scores of players.
	static score_t* scores;
	//Last scores of plauers. 
	static score_t* last_match_scores;
	//Id of the current map of the level.
	static maps_count_t current_map_id;

	//Count of gravity generators on the map.
	static map_elements_count_t grav_gens_count;
	//Count of players on the map.
	static players_count_t players_count;
	//Count of ships on the map.
	static players_count_t ships_count;
	//Count of pilots on the map.
	static players_count_t pilots_count;
	//Count of knifes on the map.
	static entities_count_t knifes_count;
	//Count of lazers on the map.
	static entities_count_t lasers_count;
	//Count of bombs on the map.
	static entities_count_t bombs_count;
	//Count of turels on the map.
	static entities_count_t turels_count;
	//Count of map's lazers on the map.
	static entities_count_t mega_lazers_count;
	//Count of asteroids on the map.
	static entities_count_t asteroids_count;
	//Count of bonuses on the map.
	static entities_count_t bonuses_count;
	//Count of bullets on the map.
	static entities_count_t bullets_count;
	//Count of particles on the map.
	static entities_count_t particles_count;
	//Count of map's rectangles on the map.
	static map_elements_count_t rectangles_count;

	//Bonus in all ships at the start of the game.
	static Bonus::bonus_t start_bonus;

	//This variable stores all the values of rules of ghe game. To change the values of rules of ghe game use |= and &=. To get the values of rules of the game use &.
	static game_rules_t game_rules;

	//game variables

	//game lists

	//This array contains flags for maps that will be used in the current match.
	static bool* map_pull_array;
	//This array contains maps' ids that will be used in the current match.
	static maps_count_t* selected_maps_id_array;
	//This array contains objects that will be spawned in the current match.
	static bool* object_pull_array;

	//game lists

	//game objects

	//Array of shiips.
	static Ship* ships;
	//Array of pilots.
	static Pilot* pilots;

	//Array of bullets.
	static Bullet* bullets;
	//Array of knifes.
	static Knife* knifes;
	//Array of lazers.
	static Laser* lasers;
	//Array of bombs.
	static Bomb* bombs;
	//Array of particles.
	static DynamicEntity* particles;
	//Array of asteroids.
	static Asteroid* asteroids;
	//Array of bonuses.
	static Bonus* bonuses;

	//Object stores data about map on current level.
	static Map* map;
	//Array of rectangles.
	static Rectangle* rectangles;
	//Array of gravity generators.
	static GravGen* grav_gens;
	//Array of turels.
	static Turel* turels;
	//Array of lasers of the map.
	static MegaLaser* mega_lasers;

	//game objects

	//menu objects

	//Pointer to current active menu. This that pointer work all drawing functions.
	static Menu* current_active_menu;

	//Buttons on the game screen to controle ships and players.
	static Menu* ships_control_menu;

	static Menu* main_menu;
	static Menu* option_menu;
	static Menu* pause_menu;

	static Menu* ships_select_menu;
	static Menu* team_sheeps_select_menu;
	static Menu* map_pull_select_menu;
	static Menu* spawning_objects_select_menu;

	//menu objects

	//temp containers
	namespace Temp
	{
		//Memory space for temporary values.
		static Ship ship;
		//Memory space for temporary values.
		static Pilot pilot;
		//Memory space for temporary values.
		static Bullet bullet;
		//Memory space for temporary values.
		static Knife knife;
		//Memory space for temporary values.
		static Laser lazer;
		//Memory space for temporary values.
		static Bomb bomb;
		//Memory space for temporary values.
		static DynamicEntity particle;
		//Memory space for temporary values.
		static Asteroid asteroid;
		//Memory space for temporary values.
		static Bonus bonus;

		//Memory space for temporary pointers.
		static Ship* ship_p;
		//Memory space for temporary pointers.
		static Pilot* pilot_p;
		//Memory space for temporary pointers.
		static Bullet* bullet_p;
		//Memory space for temporary pointers.
		static Knife* knife_p;
		//Memory space for temporary pointers.
		static Laser* lazer_p;
		//Memory space for temporary pointers.
		static Bomb* bomb_p;
		//Memory space for temporary pointers.
		static DynamicEntity* particle_p;
		//Memory space for temporary pointers.
		static Asteroid* asteroid_p;
		//Memory space for temporary pointers.
		static Bonus* bonus_p;
	}


	namespace Init
	{
		//The function runs when the program starts.
		//The functions nitialisates all arrays this entities.
		inline void Game();
		//The function runs when the mach starts.
		//The functions nitialisates all arrays this maps.
		inline void Mach();
		//The function runs when the level initialisates.
		//The function generates the map and spawning players.
		inline void Level();
		//The function runs when the program starts.
		//The function creates all nemus objects.
		inline void Menus();
	}

	//The function calculates forces of collisions between entities in the array.
	void DynamicEntitiesCollisions(DynamicEntity* entities, entities_count_t entities_count);
	//The function calculates forces of collisions between entities from two arrays.
	void DynamicEntitiesCollisions(DynamicEntity* entities1, entities_count_t entities1_count, DynamicEntity* entities2, entities_count_t entities2_count);
	//The function calculates forces of collisions between entities in the array and elements of the map.
	void DynamicEntitiesCollisions(DynamicEntity* entities, entities_count_t entities_count, Map* map);
	//The function adds the specified force to all entities in the array.
	void DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, Vec2F* force);
	//The function adds the force of the gravity generator to all entities in the array.
	void DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, GravGen* grav_gen);
	//The function adds forces of the all gravity generators to all entities in the array.
	void DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, GravGen* grav_gens, map_elements_count_t grav_gens_count);

	namespace Update
	{
		inline void All();

		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Asteroids();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Bombs();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Bonuses();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Bullets();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Knifes();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Lazers();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Pilots();
		//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
		void Ships();

		//Recalculate the positions of all entities.
		inline void TransportAll();

		//Recalculate the position of entity.
		void TransportBullets();
		//Recalculate the position of entity.
		void TransportBombs();
		//Recalculate the position of entity.
		void TransportSheeps();
		//Recalculate the position of entity.
		void TransportPilots();
		//Recalculate the position of entity.
		void TransportKnifes();
		//Recalculate the position of entity.
		void TransportLazers();
		//Recalculate the position of entity.
		void TransportAsteroids();
		//Recalculate the position of entity.
		void TransportBonuses();

		void BulletsDestroy();

		//All bombs that can be detonated will detonate.
		void BombsChainReaction();

		namespace Destroy
		{
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Asteroid* asteroid);
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Bonus* bonus);
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Bullet* bulet);
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Knife* fnife);
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Pilot* pilot);
			//The function destroys the entity with the killer entity.
			inline void Entity(KillerEntity* destried_entity, Ship* sheep);
		}
	}

	

	//Function call every game tic.
	void Recalculate();
	//Sheeps create bullets. (Not use. Realisation transmit to Update::Shiips())
	void SheepsShoot();

	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Bonus new_bonus);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Asteroid new_asteroid);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Bullet new_bullet);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Knife new_knife);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Bomb new_mine);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(Laser new_lazer);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	inline void AddEntity(DynamicEntity new_particle);

	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Bonus* deleting_bonus);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Asteroid* deleting_asteroid);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Bullet* deleting_bullet);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Knife* deleting_knife);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Bomb* deleting_mine);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteEntity(Laser* deleting_lazer);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	inline void DeleteParticle(DynamicEntity* deleting_particle);

	//The function returns a random bonus type.
	Bonus::bonus_t GenerateRandomBonus();
	//The function returns a random id of map that is in the selected_maps_id_array.
	maps_count_t GenerateRandomMapId();
	//The function return the maximum score of all players.
	score_t GetMaxScore();
};

#endif //GAME_REALISATION_H

