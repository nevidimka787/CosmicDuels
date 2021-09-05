#pragma once

#include "MenuFunctions.h"
#include "OpenGLRealisation.h"

#include "../Classes/GameEngine.h"

class Game;
class OpenGL;

class Game
{
	//buttons

	//game variables
public:
	//Global tic value. Not use by functions.
	GameTypes::tic_t global_timer;
	//Global tic value for use in functions.
	GameTypes::tic_t current_tic;

	//Game status true = The game is on. false = The game stops.
	bool start_game;
	//Game status true = The game is paused. false = The game is on.
	bool pause_game;
	//Index of array is ship's player number. Values of array are numbers of teams by every ship.
	//Array changed in Game::Init::Menus.
	GameTypes::entities_count_t* teams;

	GameTypes::maps_count_t selected_maps_id_array_length;

	float area_size = GAME_ENGINE_AREA_SIZE;

	//Controle is inversing
	bool rotation_inverse;
	//Buttons positions in inveres.
	bool buttons_inverse;
	//This flag will be active after the completion of the MatchInit function and inactive after the end of the match.
	bool flag_all_entities_initialisate;

	//Current scores of players.
	GameTypes::score_t* scores;
	//Last scores of plauers. 
	GameTypes::score_t* last_match_scores;
	//Id of the current map of the level.
	GameTypes::maps_count_t current_map_id;

	//Count of gravity generators on the map.
	GameTypes::map_elements_count_t grav_gens_count;
	//Count of players on the map.
	GameTypes::players_count_t players_count;
	//Count of ships on the map.
	GameTypes::players_count_t ships_count;
	//Count of pilots on the map.
	GameTypes::players_count_t pilots_count;
	//Count of knifes on the map.
	GameTypes::entities_count_t knifes_count;
	//Count of lazers on the map.
	GameTypes::entities_count_t lasers_count;
	//Count of bombs on the map.
	GameTypes::entities_count_t bombs_count;
	//Count of turels on the map.
	GameTypes::entities_count_t turels_count;
	//Count of map's lazers on the map.
	GameTypes::entities_count_t mega_lasers_count;
	//Count of asteroids on the map.
	GameTypes::entities_count_t asteroids_count;
	//Count of bonuses on the map.
	GameTypes::entities_count_t bonuses_count;
	//Count of bullets on the map.
	GameTypes::entities_count_t bullets_count;
	//Count of particles on the map.
	GameTypes::entities_count_t particles_count;
	//Count of map's rectangles on the map.
	GameTypes::map_elements_count_t rectangles_count;
	//Count of map's rectangles on the map.
	GameTypes::map_elements_count_t cyrcles_count;
	//Count of map's rectangles on the map.
	GameTypes::map_elements_count_t polygons_count;

	//Bonus in all ships at the start of the game.
	EngineTypes::Bonus::bonus_t start_bonus;

	//This variable stores all the values of rules of ghe game. To change the values of rules of ghe game use |= and &=. To get the values of rules of the game use &.
	GameTypes::game_rules_t game_rules;

	//game variables

	//game lists


	//Controled entities refer to this array.
	bool* shoot_flags;
	//Controled entities refer to this array.
	bool* rotate_flags;
	//Controled entities refer to this array.
	bool* burnout_flags;
	//true - The sheep can shoot one time.
	//false - The sheep can't shoot.
	GameTypes::tic_t* ships_can_shoot_flags;
	//This array contains flags for maps that will be used in the current match.
	bool* map_pull_array;
	//This array contains maps' ids that will be used in the current match.
	GameTypes::maps_count_t* selected_maps_id_array;
	//This array contains objects that will be spawned in the current match.
	bool* object_pull_array;

	//game lists

	//game objects

	//Array of shiips.
	Ship* ships;
	//Array of pilots.
	Pilot* pilots;

	//Array of asteroids.
	Asteroid* asteroids;
	//Array of bonuses.
	Bonus* bonuses;
	//Array of bombs.
	Bomb* bombs;
	//Array of bullets.
	Bullet* bullets;
	//Array of knifes.
	Knife* knifes;
	//Array of lazers.
	Laser* lasers;
	//Array of particles.
	DynamicEntity* particles;

	//Object stores data about map on current level.
	Map map;
	//Array of rectangles.
	Rectangle* rectangles;
	//Array of gravity generators.
	GravGen* grav_gens;
	//Array of turels.
	Turel* turels;
	//Array of lasers of the map.
	MegaLaser* mega_lasers;

	Camera camera;

	//game objects

	//menu objects

	//Pointer to current active menu. This that pointer work all drawing functions.
	Menu* current_active_menu;

	//Buttons on the game screen to controle ships and players.
	Menu ships_control_menu;

	Menu main_menu;
	Menu option_menu;
	Menu pause_menu;

	Menu ships_select_menu;
	Menu map_pull_select_menu;
	Menu spawning_objects_select_menu;

	//menu objects

	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Bonus new_bonus);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Asteroid new_asteroid);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Bullet new_bullet);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Knife new_knife);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Bomb new_mine);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Laser new_lazer);
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(DynamicEntity new_particle);



	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bonus* deleting_bonus);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Asteroid* deleting_asteroid);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bullet* deleting_bullet);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Knife* deleting_knife);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bomb* deleting_mine);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Laser* deleting_lazer);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Turel* deleting_turel);
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveParticle(DynamicEntity* deleting_particle);
	

//temp containers

	//Memory space for temporary values.
	Asteroid temp__asteroid;
	//Memory space for temporary values.
	Beam temp__beam;
	//Memory space for temporary values.
	Bomb temp__bomb;
	//Memory space for temporary values.
	Bonus temp__bonus;
	//Memory space for temporary values.
	Bullet temp__bullet;
	//Memory space for temporary values.
	Knife temp__knife;
	//Memory space for temporary values.
	Laser temp__lazer;
	//Memory space for temporary values.
	MegaLaser temp__mega_lazer;
	//Memory space for temporary values.
	DynamicEntity temp__particle;
	//Memory space for temporary values.
	Pilot temp__pilot;
	//Memory space for temporary values.
	Segment temp__segment;
	//Memory space for temporary values.
	Ship temp__ship;
	//Memory space for temporary values.
	Turel temp__turel;
	//Memory space for temporary values.
	Vec2F temp__vector;

	//Memory space for temporary pointers.
	Asteroid* temp__asteroid_p;
	//Memory space for temporary pointers.
	Beam* temp__beam_p;
	//Memory space for temporary pointers.
	Bomb* temp__bomb_p;
	//Memory space for temporary pointers.
	Bonus* temp__bonus_p;
	//Memory space for temporary pointers.
	Bullet* temp__bullet_p;
	//Memory space for temporary pointers.
	Knife* temp__knife_p;
	//Memory space for temporary pointers.
	Laser* temp__lazer_p;
	//Memory space for temporary pointers.
	MegaLaser* temp__mega_lazer_p;
	//Memory space for temporary pointers.
	DynamicEntity* temp__particle_p;
	//Memory space for temporary pointers.
	Pilot* temp__pilot_p;
	//Memory space for temporary pointers.
	Segment* temp__segment_p;
	//Memory space for temporary pointers.
	Ship* temp__ship_p;
	//Memory space for temporary pointers.
	Turel* temp__turel_p;
	//Memory space for temporary pointers.
	Vec2F* temp__vector_p;

	void* temp__pointer1;
	void* temp__pointer2;
	


	
	//The function runs when the object created starts.
	//The functions nitialisates all arrays this entities.
	Game();
	//The function runs when the program starts.
	//The functions nitialisates all arrays this entities.
	void InitGame();
	//The function runs when the mach starts.
	//The functions nitialisates all arrays this maps.
	void InitMach();
	//The function runs when the level initialisates.
	//The function generates the map and spawning players.
	void InitLevel();
	//The function runs when the program starts.
	//The function creates all nemus objects.
	void InitMenus();

	void NextLevel();
	void EndMatch();
	

	//The function calculates forces of collisions between entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesCollisions(
		EntityType* entities,
		GameTypes::entities_count_t entities_count);
	//The function calculates forces of collisions between entities from two arrays.
	//Use temp_p1 temp_p2
	template<typename Entity1Type, typename Entity2Type>
	void DynamicEntitiesCollisions(
		Entity1Type* entities1,
		Entity2Type* entities2,
		GameTypes::entities_count_t entities1_count, 
		GameTypes::entities_count_t entities2_count);
	//The function calculates forces of collisions between entities in the array and elements of the map.
	//Use temp_p1
	template<typename EntityType>
	void DynamicEntitiesCollisions(
		Map* map,
		EntityType* entities,
		GameTypes::entities_count_t entities_count);
	//The function adds the specified force to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		Vec2F* force,
		EntityType* entities,
		GameTypes::entities_count_t entities_count);
	//The function adds the force of the gravity generator to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		GravGen* grav_gen,
		EntityType* entities,
		GameTypes::entities_count_t entities_count);
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		GravGen* grav_gens,
		GameTypes::map_elements_count_t grav_gens_count,
		EntityType* entities,
		GameTypes::entities_count_t entities_count);

	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateAsteroids();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateBombs();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateBonuses();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateBullets();
	//The function calculates the new camera's position and size.
	void UpdateCamera();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateKnifes();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateLazers();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateMap();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateMegaLazers();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdatePilots();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateShips();
	//The function calculates the influence of entities on other entities. Any entity can be destroyed. New entities can be created.
	void UpdateTurels();

	//Update the position ans velocity of entity.
	void TransportAsteroids();
	//Update the position ans velocity of entity.
	void TransportBombs();
	//Update the position ans velocity of entity.
	void TransportBonuses();
	//Update the position ans velocity of entity.
	void TransportBullets();
	//Update the position ans velocity of entity.
	void TransportKnifes();
	//Update the position ans velocity of entity.
	void TransportLasers();
	//Update the position ans velocity of entity.
	void TransportMegaLasers();
	//Update the position ans velocity of entity.
	void TransportPilots();
	//Update the position ans velocity of entity.
	void TransportShips();

	//All bombs that can be detonated will detonate.
	void BombsChainReaction();

		
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Asteroid* asteroid);
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Bonus* bonus);
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Bullet* bulet);
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Knife* knife);
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Pilot* pilot);
	//The function destroys the entity with the aggressive entity.
	template <typename AggressiveEntityType>
	void DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Ship* ship);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Asteroid* asteroid);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Bonus* bonus);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Bullet* bulet);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Knife* fnife);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Pilot* pilot);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Ship* sheep);
	//The function destroys the entity with the killer entity.
	template <typename KillerEntityType>
	void DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Turel* turel);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Asteroid* asteroid);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Bonus* bonus);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Bullet* bullet);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Knife* knife);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Pilot* pilot);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Ship* sheep);
	//The function destroys the entity with the laser.
	void DestroyEntityByAggressiveEntity(Laser* destried_entity, Turel* turel);
	//The function destroys the pilot with the ship.
	void DestroyEntityByAggressiveEntity(Ship* destried_entity, Pilot* pilot);
		

	
	//The function spawn the shep from the pilot.
	void SpawnEntity(Ship* spawner, Pilot* pilot);
		
	
	//Not checking nullprt!
	void ShipShoot(Ship* ship);

	void ShipShoot_LaserLoopBombKnife(Ship* ship);
	void ShipShoot_LaserLoopBomb(Ship* ship);
	void ShipShoot_LaserLoopKnife(Ship* ship);
	void ShipShoot_LaserBombKnife(Ship* ship);
	void ShipShoot_LoopBombKnife(Ship* ship);
	void ShipShoot_LaserLoop(Ship* ship);
	void ShipShoot_LaserBomb(Ship* ship);
	void ShipShoot_LoopBomb(Ship* ship);
	void ShipShoot_LaserKnife(Ship* ship);
	void ShipShoot_LoopKnife(Ship* ship);
	void ShipShoot_BombKnife(Ship* ship);
	void ShipShoot_Knife(Ship* ship);
	void ShipShoot_Bomb(Ship* ship);
	void ShipShoot_Loop(Ship* ship);
	void ShipShoot_Laser(Ship* ship);
	void ShipShoot_NoBonus(Ship* ship);

	void IncrementScore(GameTypes::players_count_t team_number);
	void DecrementScore(GameTypes::players_count_t team_number);

	//Function call every game tic.
	void Update();

	//The function returns a random bonus type.
	EngineTypes::Bonus::bonus_t GenerateRandomBonus();
	//The function returns a random id of map that is in the selected_maps_id_array.
	GameTypes::maps_count_t GenerateRandomMapId();
	//The function return the maximum score of all players.
	GameTypes::score_t GetMaxScore();

	//menu pointers
	MenuFunctions* object_p__menu_functions;
	//menu pointers
	uint8_t* menu_p__ships_select_buttons;
	//menu pointers
	EngineTypes::Bonus::bonus_t* menu_p__start_bonus;

	//open gl pointers
	OpenGL* object_p__open_gl_realisation;
};

