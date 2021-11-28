#pragma once

#include "MenuFunctions.h"
#include "OpenGLRealisation.h"

#include "../Classes/GameEngine.h"

#include <shared_mutex>

class Game;
class OpenGL;

class Game
{
	//buttons

	//game variables
public:
	//Global tic value. Not use by functions.
	GameTypes::tic_t global_timer;
	//Global tic value when inverse flag was change value.
	GameTypes::tic_t rotate_inverse_chage_tic;
	//Global tic value for use in functions.
	GameTypes::tic_t end_match_tic;
	GameTypes::tic_t stuning_timer;

	//Game status true = The game is on. false = The game stops.
	bool play_round;
	//Game status true = The game start next raund.
	bool play_match;
	//Game status true = The game is paused. false = The game is on.
	bool pause_round;
	//Index of array is ship's player number. Values of array are numbers of teams by every ship.
	//Array changed in Game::Init::Menus.
	GameTypes::entities_count_t* teams;
	//Index is team number
	//Value is count of exist players in team
	GameTypes::players_count_t* players_in_team;

	GameTypes::maps_count_t selected_maps_id_array_length;

	//Controle is inversing
	bool rotation_inverse;
	//Buttons positions in inveres.
	bool buttons_inverse;
	//This flag will be active after the completion of the MatchInit function and inactive after the end of the match.
	bool flag_all_entities_initialisate;
	//Function CheckEndMatch is active.
	bool flag_update_end_match;
	//If true match will finish.
	bool flag_end_match;
	//If true results of round are drawind.
	bool flag_round_results;

	GameTypes::maps_count_t current_event;

	//Current scores of players.
	GameTypes::score_t* scores;
	//Last scores of plauers. 
	GameTypes::score_t* last_match_scores;
	//Id of the current map of the level.
	GameTypes::maps_count_t current_map_id;
	//After each round value decrements.
	//Final round have number 1.
	//If after the final of round the winer not defined value not decrements.
	//If value is 0, the match ends.
	GameTypes::score_t end_match_score;

	GameTypes::map_elements_count_t deceler_areas_count;
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
	GameTypes::map_elements_count_t turels_count;
	//Count of map's lazers on the map.
	GameTypes::map_elements_count_t mega_lasers_count;
	//Count of asteroids on the map.
	GameTypes::entities_count_t asteroids_count;
	//Count of bonuses on the map.
	GameTypes::entities_count_t bonuses_count;
	//Count of bullets on the map.
	GameTypes::entities_count_t bullets_count;
	//Count of particles on the map.
	GameTypes::entities_count_t dynamic_particles_count;
	//Count of particles on the map.
	GameTypes::entities_count_t particles_count;
	//Count of portals on the map.
	GameTypes::map_elements_count_t portals_count;

	//Bonus in all ships at the start of the game.
	EngineTypes::Bonus::inventory_t start_bonus;

	//This variable stores all the values of rules of ghe game. To change the values of rules of ghe game use |= and &=. To get the values of rules of the game use &.
	GameTypes::game_rules_t game_rules;

	GameTypes::thread_flags_t threads_statuses;

	//game variables

	//game lists


	//Controled entities refer to this array.
	bool* shoot_flags;
	//Controled entities refer to this array.
	bool* rotate_flags;
	//Tumer for check double clk for ships' burnout.
	int32_t* burnout_double_clk_timer;
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
	//This array contains bonuses that will be spawned in the current match.
	bool* bonus_pull_array;

	//game lists

	//game objects

	Logs logs;

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
	//Array of dynamic particles.
	DynamicParticle* dynamic_particles;
	//Array of knifes.
	Knife* knifes;
	//Array of lazers.
	Laser* lasers;
	//Array of particles.
	Particle* particles;

	//Object stores data about map on current level.
	Map map;
	//Array of deceleration areas.
	DecelerationArea* deceler_areas;
	//Array of gravity generators.
	GravGen* grav_gens;
	//Array of turels.
	Turel* turels;
	//Array of lasers of the map.
	MegaLaser* mega_lasers;
	//Array of porttals
	Portal* portals;

	Camera camera;

	//game objects

	//game mutexes

	std::shared_mutex asteroids_array_mtx;
	std::shared_mutex bombs_array_mtx;
	std::shared_mutex bonuses_array_mtx;
	std::shared_mutex bullets_array_mtx;
	std::shared_mutex camera_data_mtx;
	std::shared_mutex deceler_areas_array_mtx;
	std::shared_mutex dynamic_particles_array_mtx;
	std::shared_mutex grav_gens_array_mtx;
	std::shared_mutex input_values_mtx;
	std::shared_mutex knifes_array_mtx;
	std::shared_mutex lasers_array_mtx;
	std::shared_mutex log_data_mtx;
	std::shared_mutex map_data_mtx;
	std::shared_mutex mega_lasers_array_mtx;
	std::shared_mutex particles_array_mtx;
	std::shared_mutex pilots_array_mtx;
	std::shared_mutex portals_array_mtx;
	std::shared_mutex ships_array_mtx;
	std::shared_mutex turels_array_mtx;

	//The thread was locked before start first phase of the physics calculation.
	std::shared_mutex threads_statuses_mtx;

	std::shared_mutex thread_0_update;
	std::shared_mutex thread_1_update;
	std::shared_mutex thread_2_update;
	std::shared_mutex thread_3_update;

	//game mutexes

	//phase flags

	//Flags store information about completing first phase by current thread.
	GameTypes::thread_flags_t thread_flags;

	//phase flags

	//menu objects

	//Pointer to current active menu. This that pointer work all drawing functions.
	Menu* current_active_menu;

	//Buttons on the game screen to controle ships and players.
	Menu ships_control_menu;

	Menu main_menu;
	Menu option_menu;
	Menu pause_menu;

	Menu ships_select_menu;
	Menu bonus_pull_select_menu;
	Menu map_pull_select_menu;
	Menu spawning_objects_select_menu;

	//menu objects

	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(Bonus new_bonus);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Asteroid new_asteroid);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Bullet new_bullet);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(DecelerationArea new_deceleration_area);

	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(DynamicParticle new_particle);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(GravGen new_grav_gen);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(Knife new_knife);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Bomb new_bomb);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Laser new_lazsr);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(MegaLaser new_laser);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(Particle new_particle);

	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Portal new_portal);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(Pilot new_particle);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(Ship new_particle);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void AddEntity(Turel new_lazer);



	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bonus* deleting_bonus);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Asteroid* deleting_asteroid);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bomb* deleting_bomb);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Bullet* deleting_bullet);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void RemoveEntity(DecelerationArea* new_deceleration_area);

	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void RemoveEntity(DynamicParticle* new_deceleration_area);
	
	//Function adds entity to array that store entities of the same type.
	//Not checking nullprt!
	void RemoveEntity(GravGen* new_grav_gen);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Knife* deleting_knife);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Laser* deleting_laser);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(MegaLaser* deleting_mega_laser);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Particle* deleting_particle);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Pilot* deleting_particle);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Ship* deleting_particle);
	
	//Function removes the specified entity from the array.
	//Not checking nullprt!
	void RemoveEntity(Turel* deleting_turel);


	
	//The function runs when the object created starts.
	//The functions nitialisates all arrays this entities.
	Game();
	
	//The function runs when the program starts.
	//The functions nitialisates all arrays this entities.
	void InitGame();
	
	//The function runs when the mach starts.
	//The functions nitialisates all arrays this maps.
	void InitMatch();
	
	//The function runs when the level initialisates.
	//The function generates the map and spawning players.
	void InitLevel();
	
	//The function runs when the program starts.
	//The function creates all nemus objects.
	void InitMenus();

	void NextLevel();
	void EndMatch();

	//Team's numbers start from 1.
	void DecrementPlayersCountInTeam(GameTypes::players_count_t team_number);
	
	//Team's numbers start from 1.
	void IncrementPlayersCountInTeam(GameTypes::players_count_t team_number);
	
	void CheckEndMatch();

	//mtx: camera -> ship -> map
	void RoundResultsInit();
	
	//The function show round results.
	//The function block timer.
	//If logs not empty, the function return true.
	bool RoundResults();

	void PollEvents();
	
	//"Test"
	void Event0();
	
	//"Turel"
	void Event1();
	
	//"Grav Gen"
	void Event2();
	
	//"Deceleration area"
	void Event3();
	
	//"Mega lasers"
	void Event4();
	
	//"Dynamical"
	void Event5();

	//"Destroyable"
	void Event6();

	//"Aggressive"
	void Event7();

	//"Broken"
	void Event8();

	//"Portal"
	void Event9();

	//"Test"
	void CreateMap0(Vec2F* ships_positions, float* ships_angles);
	
	//"Turel"
	void CreateMap1(Vec2F* ships_positions, float* ships_angles);
	
	//"Grav Gen"
	void CreateMap2(Vec2F* ships_positions, float* ships_angles);
	
	//"Deceleration area"
	void CreateMap3(Vec2F* ships_positions, float* ships_angles);
	
	//"Mega lasers"
	void CreateMap4(Vec2F* ships_positions, float* ships_angles);
	
	//"Dynamical"
	void CreateMap5(Vec2F* ships_positions, float* ships_angles);

	//"Destryable"
	void CreateMap6(Vec2F* ships_positions, float* ships_angles);

	//"Aggressive"
	void CreateMap7(Vec2F* ships_positions, float* ships_angles);

	//"Broken"
	void CreateMap8(Vec2F* ships_positions, float* ships_angles);

	//"Portal"
	void CreateMap9(Vec2F* ships_positions, float* ships_angles);

	//Memory functions

	//Get space from memory for arrays.
	//Update values that was linking with arrays.
	void MemoryLock();
	
	//Set pbjects in arrays default.
	//Update values that was linking with arrays.
	void MemorySetDefault();
	
	//Delete arrays.
	//Update values that was linking with arrays.
	void MemoryFree();

	//Memory functions
	

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
	
	//The function calculates forces of collisions between entities in the array and elements of the map.
	void DynamicEntitiesCollisions(
		Map* map,
		Bomb* entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds the specified force to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		Vec2F force,
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
	
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	void DynamicEntitiesAddForce(
		GravGen* grav_gens,
		GameTypes::map_elements_count_t grav_gens_count,
		Bomb* entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		DecelerationArea* deceler_area,
		GameTypes::map_elements_count_t deceler_area_count,
		EntityType* entities,
		GameTypes::entities_count_t entities_count);

	//thread functions


	//thread functions

	/*
	thread lock map phase 1

	 0. deceler_area
	 1. grav_gen
	 2. camera
	 3. portal
	 4. ship
	 5. pilot
	 6. input_values
	 7. mega_laser
	 8. laser
	 9. bomb
	10. knife
	11. turel
	12. bullet
	13. asteroid
	14. bonus
	15. map
	16. particle
	17. log
	
	//deceler_area -> grav_gen -> camera -> portal -> ship -> pilot -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turel -> bullet -> asteroid -> bonus -> map -> particle -> dynamic_particle -> log

	bomb										bomb chain reaction
	bomb -> asteroid -> bonus					bomb destroys asteroid after that bonus spawns
	bomb -> bonus								bomb destroys bonus
	bomb -> bullet								bomb destroys bullet
	bomb -> bullet								bomb was activated by bullet
	bomb -> bullet								bomb was spawned by two anigilated bullets
	bomb -> knife								bomb destroys knife
	bomb -> turel								bomb destroys turel
	bullet -> asteroid -> bonus					bullet destroys asteroid after that bonus spawns
	bullet -> map								bullet destroys map
	camera -> ship -> pilot						camera focused on ships and pilots
	knife -> asteroid -> bonus					knife destroys asteroid after that bonus spawns
	knife -> bullet								knife destroys bullet
	knife -> map								knife destroys map
	knife -> turel								knife destroys turel
	laser										laser checks that it can be removed
	laser -> asteroid -> bonus					laser destroys asteroid after that bonus spawns
	laser -> bomb								laser detonates bomb
	laser -> bonus								laser destroys bonus
	laser -> bullet								laser destroys bullet
	laser -> knife								laser destroys knife
	laser -> map								laser destroys map
	laser -> turel								laser destroys turel
	mega_laser -> asteroid -> bonus				mega_laser destroys asteroid after that bonus spawns
	mega_laser -> bomb							mega_laser detonates bomb
	mega_laser -> bonus							mega_laser destroys bonus
	mega_laser -> bullet						mega_laser destroys bullet
	mega_laser -> knife							mega_laser destroys knife
	mega_laser -> map							mega_laser destroys map
	mega_laser -> turel							mega_laser destroys turel
	portal -> asteroid							portal teleported asteroid
	portal -> bomb								portal teleported bonus
	portal -> bonus								portal teleported bonus
	portal -> bullet							portal teleported bullet
	portal -> dynamic_particle					portal teleported dynamic_particle
	portal -> pilot								portal teleported pilot
	portal -> ship								portal teleported ship
	pilot -> bomb -> log						pilot was destroed by bomb
	pilot -> bullet -> log						pilot was destroed by bullet
	pilot -> input_values						pilot cheks controled input values
	pilot -> laser -> log						ship was destoroed by laser
	pilot -> mega_laser -> log					pilot was destroed by mega_laser
	ship -> bonus								ship influens to bonus or take it
	ship -> input_values						ship chekes controled values
	ship -> input_values -> 
		laser -> bomb -> knife -> bullet		ship chekes controled values after that it shoots
	ship -> pilot -> log						ship kill pilot
	ship -> pilot								ship respawn pilot
	ship -> pilot -> bomb ->
					knife -> bonus -> log		ship was destroed by bomb after that ship's knifes destroed and pilot and bonus spawn
	ship -> pilot -> knife -> bonus -> log		ship was destroed by knife after that ship's knifes destroed and pilot and bonus spawn
	ship -> pilot -> bullet ->
					knife -> bonus -> log		ship was destroed by bullet after that ship's knifes destroed and pilot and bonus spawn
	ship -> pilot -> laser ->
					knife -> bonus -> log		ship was destroed by laser after that ship's knifes destroed and pilot and bonus spawn
	ship -> pilot -> mega_laser ->
					knife -> bonus -> log		ship was destroed by mega_laser after that ship's knifes destroed and pilot and bonus spawn
	turel -> bullet								turel create bullet
	*/

	//entity update events

	//mtx: bomb
	//t = (n - 1) / 2 * n
	void BombsChainReaction();

	//mtx: bomb -> asteroid -> bonus -> dynamic_particle
	//t = n * k
	void BombsDestroyAsteroids();

	//mtx: bomb -> bonus -> dynamic_particle
	//t = n * k
	void BombsDestroyBonuses();

	//mtx: bomb -> bullet
	//t = n * k
	void BombsCollisionsWithBullets();

	//mtx: bomb -> bullet
	//t = (n - 1) / 2 * n
	void BombsSpawnedByBulletsAnigilation();

	//mtx: boms -> knife
	//t = n * k
	void BombsDestroyKnifes();

	//mtx: bomb -> turel -> dynamic_particle
	//t = n * k
	void BombsDestroyTurels();

	//mtx: bullet -> asteroid -> bonus -> dynamic_particle
	//t = n * k
	void BulletsDestroyAsteroids();

	//mtx: bullet -> map
	//t = n * (m1 + m2 + m3)
	void BulletsDestroedByMap();

	//mtx: camera -> ship -> pilot
	//t = s + p
	void CameraFocusesOnPlayers();

	//mtx: knife -> asteroid -> bonus -> dynamic_particle
	//t = n * k
	void KnifesDestroyAsteroids();

	//mtx: knife -> bullet
	//t = n * k
	void KnifesDestroyBullets();

	//mtx: knife -> map -> dynamic_particle
	//t = n * (m1 + m2 + m3)
 	void KnifesDestroyMap();

 	//mtx: knife -> turel -> dynamic_particle
	//t = n * k
 	void KnifesDestroyTurels();

	//mtx: laser -> asteroid -> bonus -> dynamic_particle
	//t = n * k
	void LasersDestroyAsteroids();

	//mtx: laser -> bonus
	//t = n * k
	void LasersDestroyBonuses();

	//mtx: laser -> bomb
	//t = n * k
	void LasersDetonateBombs();

	//mtx: laser -> bullet
	//t = n * k
	void LasersDestroyBullets();

	//mtx: laser -> knife
	//t = n * k
	void LasersDestroyKnifes();

	//mtx: laser -> map -> dynamic_particle
	//t = n * (m1 + m2 + m3)
	void LasersDestroyMap();

	//mtx: laser -> turel -> dynamic_particle
	//t = n * k
	void LasersDestroyTurels();

	//mtx: mega_laser -> asteroid -> bonus -> dynamic_particle
	//t = n * k
	void MegaLasersDestroyAsteroids();

	//mtx: mega_laser -> bonus
	//t = n * k
	void MegaLasersDestroyBonuses();

	//mtx: mega_laser -> bomb
	//t = n * k
	void MegaLasersDetonateBombs();

	//mtx: mega_laser -> bullet
	//t = n * k
	void MegaLasersDestroyBullets();

	//mtx: mega_laser -> knife
	//t = n * k
	void MegaLasersDestroyKnifes();

	//mtx: mega_laser -> map -> dynamic_particle
	//t = n * (m1 + m2 + m3)
	void MegaLasersDestroyMap();

	//mtx: mega_laser -> turel -> dynamic_particle
	//t = n * k
	void MegaLasersDestroyTurels();

	//mtx: portal -> particle
	//t = n
	void PortalsCreateParticles();

	//mtx: portal -> asteroid -> particle
	//t = n * k
	void PortalsTPAsteroids();

	//mtx: portal -> bomb -> particle
	//t = n * k
	void PortalsTPBombs();

	//mtx: portal -> bonus -> particle
	//t = n * k
	void PortalsTPBonuses();

	//mtx: portal -> bullet -> particle
	//t = n * k
	void PortalsTPBullets();

	//mtx: portal -> particle -> dynamic_particles
	//t = n * k
	void PortalsTPDynamicParticles();

	//mtx: portal -> pilot -> particle
	//t = n * k
	void PortalsTPPilots();

	//mtx: portal -> ship -> particle
	//t = n * k
	void PortalsTPShips(); 

	//mtx: pilot -> bomb -> dynamic_particle -> log
	//t = n * k
	void PilotsKilledByBombs();

	//mtx: pilot -> bullet -> dynamic_particle -> log
	//t = n * k
	void PilotsKilledByBullet();

	//mtx: pilot -> input_values
	//t = n
	void PilotsCheckInput();

	//mtx: pilot -> knife -> dynamic_particle -> log
	//t = n * k
	void PilotsKilledByKnifes();

	//mtx: pilot -> laser -> dynamic_particle -> log
	//t = n * k
	void PilotsKilledByLasers();

	//mtx: pilot -> mega_laser -> dynamic_particle -> log
	//t = n * k
	void PilotsKilledByMegaLaser();

	//mtx: ship -> bonus
	//t = n * k
	void ShipsInfluenceToBonuses();

	//mtx: ship -> input_values
	//t = n
	void ShipsCheckInput();

	//mtx: ship -> input_values -> laser -> bomb -> knife -> bullet
	//t = n
	void ShipsShoot();

	//mtx: ship -> pilot -> dynamic_particle -> log
	//t = n * k
	void ShipsRespawnOrDestroyPilots();

	//mtx: ship -> pilot
	//t = n
	void PilotsRespawnAuto();

	//mtx: ship -> dynamic_particle
	//t = n * k
	void ShipsCreateExaust();

	//mtx: ship -> pilot -> bomb -> knife -> bonus -> dynamic_entity -> log
	//t = n * k
	void ShipsDestroedByBombsOrActivateBombs();

	//mtx: ship -> pilot -> bullet -> knife -> bonus -> dynamic_entity -> log
	//t = n * k
	void ShipsDestroedByBullets();

	//mtx: ship -> pilot -> knife -> bonus -> dynamic_entity -> log
	//t = n * k
	void ShipsDestroedByKnifes();

	//mtx: ship -> pilot -> laser -> knife -> bonus -> dynamic_entity -> log
	//t = n * k
	void ShipsDestroedByLasers();

	//mtx: ship -> pilot -> mega_laser -> knife -> bonus -> dynamic_entity -> log
	//t = n * k
	void ShipsDestroedByMegaLasers();

	//mtx: turel -> bullet
	//t = n
	void TurelsShoot();


	//entity update events

	//Update last_position of all map's elements.
	//mtx: map
	//t = m1 + m2 + m3
	void UpdateMapPhase2();

	//Update the position and velocity of entity.
	//mtx: asteroid
	//t = n
	void UpdateAsteroidsPhase2();

	//Update the position and velocity of entity.
	//mtx: bomb
	//t = n
	void UpdateBombsPhase2();

	//Update the position and velocity of entity.
	//mtx: bonus
	//t = n
	void UpdateBonusesPhase2();

	//Update the position and velocity of entity.
	//mtx: bullet
	//t = n
	void UpdateBulletsPhase2();

	//Update the position and velocity of entity.
	//mtx: deceler_area
	//t = n
	void UpdateDecelerAreasPhase2();

	//Update the position and velocity of entity.
	//mtx: particle
	//t = n
	void UpdateDynamicParticlesPhase2();

	//Update the position and velocity of entity.
	//mtx: grav_gen
	//t = n
	void UpdateGravGensPhase2();

	//Update the position and velocity of entity.
	//mtx: knife
	//t = n
	void UpdateKnifesPhase2();

	//Update the position and velocity of entity.
	//mtx: laser
	//t = n
	void UpdateLasersPhase2();

	//Update the position and velocity of entity.
	//mtx: mega_laser
	//t = n
	void UpdateMegaLasersPhase2();

	//Update the position and velocity of entity.
	//mtx: particle
	//t = n
	void UpdateParticlesPhase2();

	//Update the position and velocity of entity.
	//mtx: portal
	//t = n
	void UpdatePortalsPhase2();

	//Update the position and velocity of entity.
	//mtx: pilot
	//t = n
	void UpdatePilotsPhase2();

	//Update the position and velocity of entity.
	//mtx: ship
	//t = n
	void UpdateShipsPhase2();
	
	//Update the position and velocity of entity.
	//mtx: turel
	//t = n
	void UpdateTurelsPhase2();

	void MutexesLock();
	void MutexesUnlock();


	/*
	* In first phase entities infulence to other entities
	after that change their angular velosities and forces
	but not changed linear velocities, positions and angles.
	* In second phase entities not influence to other entities,
	they convert their force and angular velocityes values
	to new angle, velocity and position.
	*/

	/*
	The thread check map and match statuses.
	The thread can end or pause raund.
	The thread unpdate double click values.
	The thread set phases to default.
	The thread poll map's events.
	*/
	void PhysicThread0();
	//The thread calculte entities only.
	void PhysicThread1();
	//The thread calculte entities only.
	void PhysicThread2();
	//The thread calculte entities only.
	void PhysicThread3();

	void WaitPhase1();
	void WaitPhaseAllPhases();
	void WaitPhaseNotAll();

	void DestroyEntity(Bomb* destroyer, Asteroid* entity);
	void DestroyEntity(Bomb* destroyer, Bonus* entity);
	void DestroyEntity(Bomb* destroyer, Bullet* entity);
	void DestroyEntity(Bomb* destroyer, Knife* entity);
	void DestroyEntity(Bomb* destroyer, Ship* entity);
	void DestroyEntity(Bomb* destroyer, Particle* entity);
	void DestroyEntity(Bomb* destroyer, Pilot* entity);
	void DestroyEntity(Bomb* destroyer, Turel* entity);

	void DestroyEntity(Bullet* destroyer, Asteroid* entity);
	void DestroyEntity(Bullet* destroyer, Knife* entity);
	void DestroyEntity(Bullet* destroyer, Ship* entity);
	void DestroyEntity(Bullet* destroyer, Pilot* entity);

	void DestroyEntity(Knife* destroyer, Asteroid* entity);
	void DestroyEntity(Knife* destroyer, Ship* entity);
	void DestroyEntity(Knife* destroyer, Pilot* entity);
	void DestroyEntity(Knife* destroyer, Turel* entity);

	void DestroyEntity(Laser* destroyer, Asteroid* entity);
	void DestroyEntity(Laser* destroyer, Bonus* entity);
	void DestroyEntity(Laser* destroyer, Bullet* entity);
	void DestroyEntity(Laser* destroyer, Knife* entity);
	void DestroyEntity(Laser* destroyer, Ship* entity);
	void DestroyEntity(Laser* destroyer, Particle* entity);
	void DestroyEntity(Laser* destroyer, Pilot* entity);
	void DestroyEntity(Laser* destroyer, Turel* entity);

	void DestroyEntity(MegaLaser* destroyer, Asteroid* entity);
	void DestroyEntity(MegaLaser* destroyer, Bonus* entity);
	void DestroyEntity(MegaLaser* destroyer, Bullet* entity);
	void DestroyEntity(MegaLaser* destroyer, Knife* entity);
	void DestroyEntity(MegaLaser* destroyer, Ship* entity);
	void DestroyEntity(MegaLaser* destroyer, Particle* entity);
	void DestroyEntity(MegaLaser* destroyer, Pilot* entity);

	void DestroyEntity(Ship* destroyer, Pilot* entity);

	//Destroy entity by map.
	void DestroyEntity(Asteroid* entity);
	//Destroy entity by map.
	void DestroyEntity(Bonus* entity);
	//Destroy entity by map.
	void DestroyEntity(Bullet* entity);
	//Destroy entity by map.
	void DestroyEntity(Knife* entity);
	//Destroy entity by map.
	void DestroyEntity(Ship* entity);
	//Destroy entity by map.
	void DestroyEntity(Particle* entity);
	//Destroy entity by map.
	void DestroyEntity(Pilot* entity);
		
	void DestroySupportEntitiesBy(ControledEntity* produser);
	
	//The function spawn the ship from the pilot by spawner.
	//After that the function removes the pilot.
	void SpawnEntity(Ship* spawner, Pilot* pilot);

	//portal-> entyty_mtx -> particle
	template <typename EntityType>
	void TeleportEntity(Portal* portal, EntityType* entity);

	void AddBonuses(Ship* spawner);
		
	
	//Not checking nullprt!
	//mtx: laser -> bomb -> knife -> bullet
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

	//mtx: bomb
	void ShipShoot_BombKnife(Ship* ship);

	//mtx: knife
	void ShipShoot_Knife(Ship* ship);

	//mtx: bomb
	void ShipShoot_Bomb(Ship* ship);

	//mtx: bullet
	void ShipShoot_Loop(Ship* ship);

	//mtx: laser
	void ShipShoot_Laser(Ship* ship);

	//mtx: bullet
	void ShipShoot_NoBonus(Ship* ship);

	//loop by laser's host is created by asteroid
	//need mtx: laser -> bullet -> asteroid
	void CreateLoop(Laser* laser, Asteroid* asteroid);

	void IncrementScore(GameTypes::players_count_t team_number);
	void DecrementScore(GameTypes::players_count_t team_number);

	void DebugLog__CheckMutexeslLock();

	//The function returns a random bonus type.
	EngineTypes::Bonus::inventory_t GenerateRandomBonus();
	EngineTypes::Bonus::inventory_t GenerateRandomBonusAndRule();
	//The function returns a random id of map that is in the selected_maps_id_array.
	GameTypes::maps_count_t GenerateRandomMapId();
	//The function return the maximum score of all players.
	GameTypes::score_t GetMaxScore();

	//menu pointers
	MenuFunctions* object_p__menu_functions;
	//menu pointers
	uint8_t* menu_p__ships_select_buttons;
	//menu pointers
	EngineTypes::Bonus::inventory_t* menu_p__start_bonus;

	//open gl pointers
	OpenGL* object_p__open_gl_realisation;
};

