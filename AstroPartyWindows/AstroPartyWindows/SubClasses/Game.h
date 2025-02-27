#pragma once

#include "MenuFunctions.h"
#include "OpenGLRealisation.h"

#include "../Classes/GameEngine.h"
#include "../DebugClasses/loged_shared_mutex.h"

#include <assert.h>
#include <math.h>
#include <shared_mutex>
#include <thread>
#include <vector>

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
	std::vector<GameTypes::players_count_t> teams;
	//Index of array is ship's player number. Values of array are numbers of teams by every ship.
	//If player not play in current level, then alue is zero.
	std::vector<GameTypes::players_count_t> playing_teams;
	//Index is team number
	//Value is count of exist players in team
	std::vector<GameTypes::players_count_t> players_in_team;

	GameTypes::maps_count_t selected_maps_id_array_length;

	//Controle is inversing
	bool rotation_inverse;
	//Buttons positions in inveres.
	bool buttons_inverse;
	//This flag will be active after the completion of the MatchInit function and inactive after the end of the match.
	bool flag_all_entities_initialisate;
	//If true match will finish.
	bool flag_end_match;
	//If true results of round are drawind.
	bool flag_round_results;

	GameTypes::maps_count_t current_event;

	//Current scores of players.
	std::vector<GameTypes::score_t> scores;
	//Last scores of plauers. 
	std::vector<GameTypes::score_t> last_match_scores;
	//Id of the current map of the level.
	GameTypes::maps_count_t current_map_id;
	

	//If player will get end_match_score or high scor then this people will become potencial winner.
	//This variable is setted by InitMatch function.
	//This variable chaned in NextLevel function.
	GameTypes::score_t end_match_score;

	//Count of annihilation area generators on the map.
	GameTypes::entities_count_t annih_area_gens_count;
	//Count of deceler areas on the map.
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
	//Count of turrets on the map.
	GameTypes::map_elements_count_t turrets_count;
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

	//game variables

	//game lists

	GameTypes::control_flags_t control_flags;

	//Tumer for check double clk for ships' burnout.
	std::vector<GameTypes::tic_t> double_clk_timers;
	//Array of flags store data about shoots of ships in current tic.
	std::vector<bool> ships_shooting_flags;
	//If value in the array then ship can shoot.
	std::vector<GameTypes::tic_t> ships_can_shoot_flags;
	//This array contains flags for maps that will be used in the current match.
	std::vector<bool> map_pull_array;
	//This array contains maps' ids that will be used in the current match.
	std::vector<GameTypes::maps_count_t> selected_maps_id_array;
	//This array contains objects that will be spawned in the current match.
	std::vector<bool> object_pull_array;
	//This array contains bonuses, buffs and rules that will be spawned in the current match.
	std::vector<bool> bonus_pull_array;
	EngineTypes::Bonus::inventory_t inventory_template_mask;

	//game lists

	//game objects

	Logs logs;

	//Array of shiips.
	std::vector<Ship> ships;
	//Array of pilots.
	std::vector<Pilot> pilots;

	std::vector<AnnihAreaGen> annih_area_gens;
	//Array of asteroids.
	std::vector<Asteroid> asteroids;
	//Array of bonuses.
	std::vector<Bonus> bonuses;
	//Array of bombs.
	std::vector<Bomb> bombs;
	//Array of bullets.
	std::vector<Bullet> bullets;
	//Array of dynamic particles.
	std::vector<DynamicParticle> dynamic_particles;
	//Array of knifes.
	std::vector<Knife> knifes;
	//Array of lazers.
	std::vector<Laser> lasers;
	//Array of particles.
	std::vector<Particle> particles;

	//Object stores data about map on current level.
	Map::MapData map;
	//Array of deceleration areas.
	std::vector <DecelerationArea> deceler_areas;
	//Array of gravity generators.
	std::vector <GravGen> grav_gens;
	//Array of turrets.
	std::vector <Turret> turrets;
	//Array of porttals
	std::vector <Portal> portals;

	Camera camera;

	//game objects

	//game mutexes

#ifdef _DEBUG
	loged_shared_mutex annih_area_gens_array_mtx = loged_shared_mutex("annih_area_gens_array_mtx");
	loged_shared_mutex asteroids_array_mtx = loged_shared_mutex("asteroids_array_mtx");
	loged_shared_mutex bombs_array_mtx = loged_shared_mutex("bombs_array_mtx");
	loged_shared_mutex bonuses_array_mtx = loged_shared_mutex("bonuses_array_mtx");
	loged_shared_mutex bullets_array_mtx = loged_shared_mutex("bullets_array_mtx");
	loged_shared_mutex camera_data_mtx = loged_shared_mutex("camera_data_mtx");
	loged_shared_mutex deceler_areas_array_mtx = loged_shared_mutex("deceler_areas_array_mtx");
	loged_shared_mutex dynamic_particles_array_mtx = loged_shared_mutex("dynamic_particles_array_mtx");
	loged_shared_mutex grav_gens_array_mtx = loged_shared_mutex("grav_gens_array_mtx");
	loged_shared_mutex input_values_mtx = loged_shared_mutex("input_values_mtx");
	loged_shared_mutex knifes_array_mtx = loged_shared_mutex("knifes_array_mtx");
	loged_shared_mutex lasers_array_mtx = loged_shared_mutex("lasers_array_mtx");
	loged_shared_mutex log_data_mtx = loged_shared_mutex("log_data_mtx");
	loged_shared_mutex map_data_mtx = loged_shared_mutex("map_data_mtx");
	loged_shared_mutex particles_array_mtx = loged_shared_mutex("particles_array_mtx");
	loged_shared_mutex pilots_array_mtx = loged_shared_mutex("pilots_array_mtx");
	loged_shared_mutex portals_array_mtx = loged_shared_mutex("portals_array_mtx");
	loged_shared_mutex ships_array_mtx = loged_shared_mutex("ships_array_mtx");
	loged_shared_mutex turrets_array_mtx = loged_shared_mutex("turrets_array_mtx");
#else // _DEBUG
	std::shared_mutex annih_area_gens_array_mtx;
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
	std::shared_mutex particles_array_mtx;
	std::shared_mutex pilots_array_mtx;
	std::shared_mutex portals_array_mtx;
	std::shared_mutex ships_array_mtx;
	std::shared_mutex turrets_array_mtx;
#endif // _DEBUG

	//The array store time of calculation indicated phase of indicated thread.
	//first index -- number of thread
	//second index -- number of phase
	std::chrono::system_clock::duration thread_durations[4][2];
	void ResetThreadDurations();

	//game mutexes

	//menu objects

	//Pointer to current active menu. This that pointer work all drawing functions.
	Menu* current_active_menu;

	//Buttons on the game screen to controle ships and players.
	Menu ships_control_menu;

//	main_menu
//	|---options_menu
// 	|	|---bonus_pull_select_menu
// 	|	|---map_pull_select_menu
// 	|	|---spawning_objects_select_menu
//	|---credit_menu
//	|---ships_select_menu
//		|---pause_menu

	Menu main_menu;

	Menu option_menu;
	Menu credits_menu;
	Menu ships_select_menu;
	Menu pause_menu;

	Menu bonus_pull_select_menu;
	Menu map_pull_select_menu;
	Menu objects_pull_menu;

	//menu objects

	//Function adds entity to array that store entities of the same type.
	void AddEntity(const AnnihAreaGen& annih_area_gen);

	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Asteroid& new_asteroid);

	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Bonus& new_bonus);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Bullet& new_bullet);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const DecelerationArea& new_deceleration_area);

	//Function adds entity to array that store entities of the same type.
	void AddEntity(const DynamicParticle& new_particle);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const GravGen& new_grav_gen);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Knife& new_knife);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Bomb& new_bomb);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Laser& new_lazsr);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Particle& new_particle);

	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Portal& new_portal);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Pilot& new_pilot);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Ship& new_ship);
	
	//Function adds entity to array that store entities of the same type.
	void AddEntity(const Turret& new_lazer);


	//Function removes the specified entity from the array.
	void RemoveEntity(AnnihAreaGen& annih_area_gen);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Asteroid& deleting_asteroid);

	//Function removes the specified entity from the array.
	void RemoveEntity(Bonus& deleting_bonus);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Bomb& deleting_bomb);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Bullet& deleting_bullet);
	
	//Function adds entity to array that store entities of the same type.
	void RemoveEntity(DecelerationArea& new_deceleration_area);

	//Function adds entity to array that store entities of the same type.
	void RemoveEntity(DynamicParticle& new_deceleration_area);
	
	//Function adds entity to array that store entities of the same type.
	void RemoveEntity(GravGen& new_grav_gen);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Knife& deleting_knife);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Laser& deleting_laser);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Particle& deleting_particle);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Pilot& deleting_particle);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Ship& deleting_particle);
	
	//Function removes the specified entity from the array.
	void RemoveEntity(Turret& deleting_turret);


	
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
	
	//"Orbit"
	void Event0();
	
	//"turret"
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

	//"No Center"
	void Event10();

	//"Collaider"
	void Event11();

	//"Kaleidoscope"
	void Event12();

	//"Orbit"
	void CreateMap0(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);
	
	//"turret"
	void CreateMap1(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);
	
	//"Grav Gen"
	void CreateMap2(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);
	
	//"Deceleration area"
	void CreateMap3(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);
	
	//"Mega lasers"
	void CreateMap4(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);
	
	//"Dynamical"
	void CreateMap5(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Destryable"
	void CreateMap6(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Aggressive"
	void CreateMap7(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Broken"
	void CreateMap8(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Portal"
	void CreateMap9(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"No center"
	void CreateMap10(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Collaider"
	void CreateMap11(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	//"Kaleidoscope"
	void CreateMap12(std::vector<Vec2F>& ships_positions, std::vector<float>& ships_angles);

	void CreateMapRoundResults(GameTypes::players_count_t players_count, GameTypes::score_t max_score, float cell_size);

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

	void ClearShipsShootingFlags();

	//Memory functions
	

	//The function calculates forces of collisions between entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesCollisions(
		std::vector<EntityType>& entities,
		GameTypes::entities_count_t entities_count);
	
	//The function calculates forces of collisions between entities from two arrays.
	//Use temp_p1 temp_p2
	template<typename Entity1Type, typename Entity2Type>
	void DynamicEntitiesCollisions(
		std::vector<Entity1Type>& entities1,
		std::vector<Entity2Type>& entities2,
		GameTypes::entities_count_t entities1_count, 
		GameTypes::entities_count_t entities2_count);
	
	//The function calculates forces of collisions between entities in the array and elements of the map.
	//Use temp_p1
	template<typename EntityType>
	void DynamicEntitiesCollisions(
		Map::MapData& map,
		std::vector<EntityType>& entities,
		GameTypes::entities_count_t entities_count);

	void DynamicEntitiesCollisions(
		Map::MapData& map,
		std::vector<Asteroid>& asteroids,
		GameTypes::entities_count_t asteroid_count);
	
	//The function calculates forces of collisions between entities in the array and elements of the map.
	void DynamicEntitiesCollisions(
		Map::MapData& map,
		std::vector<Bomb>& bombs,
		GameTypes::entities_count_t entities_count);

	//The function calculates forces of collisions between entities in the array and elements of the map.
	void DynamicEntitiesCollisions(
		Map::MapData& map,
		std::vector<DynamicParticle>& entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds the specified force to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		const Vec2F& force,
		std::vector<EntityType>& entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		std::vector<GravGen>& grav_gens,
		GameTypes::map_elements_count_t grav_gens_count,
		std::vector<EntityType>& entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	void DynamicEntitiesAddForce(
		std::vector<GravGen>& grav_gens,
		GameTypes::map_elements_count_t grav_gens_count,
		std::vector<Bomb>& entities,
		GameTypes::entities_count_t entities_count);
	
	//The function adds forces of the all gravity generators to all entities in the array.
	//Use temp_p1 temp_p2
	template<typename EntityType>
	void DynamicEntitiesAddForce(
		std::vector<DecelerationArea>& deceler_area,
		GameTypes::map_elements_count_t deceler_area_count,
		std::vector<EntityType>& entities,
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
	 6. annih_area_gen
	 7. input_values
	 8. mega_laser
	 9. laser
	10. bomb
	11. knife
	12. turret
	13. bullet
	14. asteroid
	15. bonus
	16. map
	17. particle
	18. dynamic_particle
	19. log
	
	//deceler_area -> grav_gen -> camera -> portal -> ship -> pilot -> annih_area_generator -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turret -> bullet -> asteroid -> bonus -> map -> particle -> dynamic_particle -> log

	bomb										bomb chain reaction
	bomb -> asteroid -> bonus					bomb destroys asteroid after that bonus spawns
	bomb -> bonus								bomb destroys bonus
	bomb -> bullet								bomb destroys bullet
	bomb -> bullet								bomb was activated by bullet
	bomb -> bullet								bomb was spawned by two annihilated bullets
	bomb -> knife								bomb destroys knife
	bomb -> turret								bomb destroys turret
	bullet -> asteroid -> bonus					bullet destroys asteroid after that bonus spawns
	bullet -> map								bullet destroys map
	camera -> ship -> pilot						camera focused on ships and pilots
	knife -> asteroid -> bonus					knife destroys asteroid after that bonus spawns
	knife -> bullet								knife destroys bullet
	knife -> map								knife destroys map
	knife -> turret								knife destroys turret
	laser										laser checks that it can be removed
	laser -> asteroid -> bonus					laser destroys asteroid after that bonus spawns
	laser -> bomb								laser detonates bomb
	laser -> bonus								laser destroys bonus
	laser -> bullet								laser destroys bullet
	laser -> knife								laser destroys knife
	laser -> map								laser destroys map
	laser -> turret								laser destroys turret
	mega_laser -> asteroid -> bonus				mega_laser destroys asteroid after that bonus spawns
	mega_laser -> bomb							mega_laser detonates bomb
	mega_laser -> bonus							mega_laser destroys bonus
	mega_laser -> bullet						mega_laser destroys bullet
	mega_laser -> knife							mega_laser destroys knife
	mega_laser -> map							mega_laser destroys map
	mega_laser -> turret							mega_laser destroys turret
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
	turret -> bullet								turret create bullet
	*/

	//entity update events

	//mtx: annih_area_generator
	//t = n
	void AnnihAreaGensShoot();

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
	void BombsSpawnedByBulletsAnnihilation();

	//mtx: boms -> knife
	//t = n * k
	void BombsDestroyKnifes();

	//mtx: bomb -> turret -> dynamic_particle
	//t = n * k
	void BombsDestroyTurrets();

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

 	//mtx: knife -> turret -> dynamic_particle
	//t = n * k
 	void KnifesDestroyTurrets();

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
	void LasersCollisionWthMap();

	//mtx: laser -> turret -> dynamic_particle
	//t = n * k
	void LasersDestroyTurrets();

	//mtx: portal -> particle
	//t = n
	void PortalsCreateParticles();

	//mtx: portal -> asteroid -> particle
	//t = n * k
	template<typename Entity_T>
	void PortalsTPEntityes(std::vector<Entity_T>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);

	//mtx: pilot -> input_values
	//t = n
	void PilotsCheckInput();

	//mtx: pilot -> bomb -> dynamic_particle -> log
	//t = n * k
	template<typename Entity_T>
	void PilotsKilledBy(std::vector<Entity_T>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx);

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

	//mtx: ship -> pilot -> annih_area_gen -> bomb -> bonus -> dynamic_entity -> log
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

	//mtx: turret -> bullet
	//t = n
	void TurretsShoot();


	//entity update events

	//Update last_position of all map's elements.
	//mtx: map
	//t = m1 + m2 + m3
	void UpdateMapPhase2();

	//Update the position and velocity of entity.
	//mtx: ship- > annih_areaa_gen
	//t = n
	void UpdateAnnihAreaGensPhase2();

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
	//mtx: turret
	//t = n
	void UpdateTurretsPhase2();

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

	void ResetAllThreadEvents();

	void DestroyEntity(const Bomb& destroyer, Asteroid& entity);
	void DestroyEntity(const Bomb& destroyer, Bonus& entity);
	void DestroyEntity(const Bomb& destroyer, Bullet& entity);
	void DestroyEntity(const Bomb& destroyer, Knife& entity);
	void DestroyEntity(const Bomb& destroyer, Ship& entity);
	void DestroyEntity(const Bomb& destroyer, Particle& entity);
	void DestroyEntity(const Bomb& destroyer, Pilot& entity);
	void DestroyEntity(const Bomb& destroyer, Turret& entity);

	void DestroyEntity(const Bullet& destroyer, Asteroid& entity);
	void DestroyEntity(const Bullet& destroyer, Knife& entity);
	void DestroyEntity(const Bullet& destroyer, Ship& entity);
	void DestroyEntity(const Bullet& destroyer, Pilot& entity);

	void DestroyEntity(const Knife& destroyer, Asteroid& entity);
	void DestroyEntity(const Knife& destroyer, Ship& entity);
	void DestroyEntity(const Knife& destroyer, Pilot& entity);
	void DestroyEntity(const Knife& destroyer, Turret& entity);

	void DestroyEntity(const Laser& destroyer, Asteroid& entity);
	void DestroyEntity(const Laser& destroyer, Bonus& entity);
	void DestroyEntity(const Laser& destroyer, Bullet& entity);
	void DestroyEntity(const Laser& destroyer, Knife& entity);
	void DestroyEntity(const Laser& destroyer, Ship& entity);
	void DestroyEntity(const Laser& destroyer, Particle& entity);
	void DestroyEntity(const Laser& destroyer, Pilot& entity);
	void DestroyEntity(const Laser& destroyer, Turret& entity);

	void DestroyEntity(const Ship& destroyer, Pilot& entity);

	// Destroy an asteroid
	// Divide it if this one enouth big.
	void DestroyEntity(Asteroid& entity);
	//Destroy entity by map.
	void DestroyEntity(Bonus& entity);
	//Destroy entity by map.
	void DestroyEntity(Bullet& entity);
	//Destroy entity by map.
	void DestroyEntity(Knife& entity);
	//Destroy entity by map.
	void DestroyEntity(Ship& entity);
	//Destroy entity by map.
	void DestroyEntity(Particle& entity);
	//Destroy entity by map.
	void DestroyEntity(Pilot& entity);
		
	void DestroySupportEntitiesBy(ControledEntity& produser);
	
	//The function spawn the ship from the pilot by spawner.
	//After that the function removes the pilot.
	void SpawnEntity(const Ship& spawner, Pilot& pilot);

	//portal-> entyty_mtx -> particle
	template <typename EntityType>
	void TeleportEntity(const Portal& portal, EntityType& entity);

	void AddBonuses(Ship& spawner);
		
	//mtx: annih_area_gen -> bomb
	void AnnihAreaGenShoot(const AnnihAreaGen& annih_area_gen);

	//Not checking nullprt!
	//mtx: laser -> bomb -> knife -> bullet -> dynamic_particle
	void ShipShoot(Ship& ship);

	void ShipShoot_LaserLoopBombKnife(const Ship& ship);
	void ShipShoot_LaserLoopBomb(Ship& ship);
	void ShipShoot_LaserLoopKnife(Ship& ship);
	void ShipShoot_LaserBombKnife(Ship& ship);
	void ShipShoot_LoopBombKnife(Ship& ship);
	void ShipShoot_LaserLoop(Ship& ship);
	void ShipShoot_LaserBomb(Ship& ship);
	void ShipShoot_LoopBomb(Ship& ship);
	void ShipShoot_LaserKnife(Ship& ship);
	void ShipShoot_LoopKnife(Ship& ship);

	//mtx: bomb
	void ShipShoot_BombKnife(Ship& ship);

	//mtx: knife
	void ShipShoot_Knife(Ship& ship);

	//mtx: bomb
	void ShipShoot_Bomb(Ship& ship);

	//mtx: bullet
	void ShipShoot_Loop(Ship& ship);

	//mtx: laser
	void ShipShoot_Laser(Ship& ship);

	//mtx: bullet
	void ShipShoot_NoBonus(Ship& ship);

	//bomb by laser's host is created by asteroid
	//need mtx: laser -> bomb -> asteroid
	void CreateBomb(const Laser& creator, const Asteroid& producer);

	//loop by laser's host is created by asteroid
	//need mtx: laser -> bullet -> asteroid
	void CreateLoop(const Laser& creator, const Asteroid& producer);

	void IncrementScore(GameTypes::players_count_t team_number);
	void DecrementScore(GameTypes::players_count_t team_number);

	void DebugLog__CheckMutexeslLock();

	// The function update inventory template mask using bonus pool menu.
	void UpdateInventoryTemplateMask();

	// The function return new inventory template 
	// that doesn't collect bonuses thats weren't in the bonus pool menu.
	EngineTypes::Bonus::inventory_t CheckBonusPoolMenu(EngineTypes::Bonus::inventory_t inventory_template);

	//The function returns a random bonus type.
	EngineTypes::Bonus::inventory_t GenerateRandomBonus();
	EngineTypes::Bonus::inventory_t GenerateRandomBonusAndRule();

	/*
	The function return random inventory.
	There are no objects in inventory that not be indicated in the objects list.
	The inventory will have a objects' types count that large than minimun objects' types count and
	less than maximun objects' types count.
	The inventory will have a objects count of every type large than minimun objects count and
	less than maximun objects count.
	*/
	EngineTypes::Bonus::inventory_t GenerateRandomInventory(
		EngineTypes::Bonus::inventory_t objects_list,
		GameTypes::objects_types_count_t min_objects_count,
		GameTypes::objects_types_count_t max_objects_count,
		GameTypes::objects_types_count_t min_objects_types_count,
		GameTypes::objects_types_count_t max_objects_types_count);

	//The function returns a random id of map that is in the selected_maps_id_array.
	GameTypes::maps_count_t GenerateRandomMapId();
	//The function return the maximum score of all players.
	GameTypes::score_t GetMaxScore();

	//menu pointers
	uint8_t* menu_p__ships_select_buttons;
	//menu pointers
	EngineTypes::Bonus::inventory_t* menu_p__start_bonus;

	//open gl pointers
	OpenGL* object_p__open_gl_realisation;
	//menu pointers
	MenuFunctions* object_p__menu_functions;
};

