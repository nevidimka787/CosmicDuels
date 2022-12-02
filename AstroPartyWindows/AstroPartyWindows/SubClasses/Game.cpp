#include "Game.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 6001)
#pragma warning(disable : 6011)
#pragma warning(disable : 6200)
#pragma warning(disable : 6294)
#pragma warning(disable : 6297)
#pragma warning(disable : 6308)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 26495)

#define M_PI	3.14159265358979323846	// pi
#define M_PI_2	1.5707796326794897		// pi/2
#define M_PI_4  0.785398163397448309616	// pi/4

void Game::PhysicThread0()
{
	if (ships_count > 0)
	{
		ShipsInfluenceToBonuses();
		ShipsRespawnOrDestroyPilots();
		ShipsDestroedByBombsOrActivateBombs();
		ShipsCheckInput();
		ShipsCreateExaust();
		ShipsDestroedByBullets();
		ShipsDestroedByKnifes();
		ShipsDestroedByLasers();
		ShipsDestroedByMegaLasers();
	}
	
	if (pilots_count > 0)
	{
		PilotsKilledByBombs();
		PilotsKilledByBullet();
		PilotsCheckInput();
		PilotsKilledByKnifes();
		PilotsRespawnAuto();
		PilotsKilledByLasers();
		PilotsKilledByMegaLaser();
	}
	
	if (mega_lasers_count > 0)
	{
		MegaLasersDestroyAsteroids();
		MegaLasersDestroyBonuses();
		MegaLasersDetonateBombs();
		MegaLasersDestroyBullets();
		//MegaLasersDestroyMap(); -- not realisated
		MegaLasersDestroyKnifes();
		MegaLasersDestroyTurels();
	}
	
	if (lasers_count > 0)
	{
		LasersDestroyBonuses();
		LasersDetonateBombs();
		LasersDestroyKnifes();
		LasersDestroyAsteroids();
		LasersDestroyBullets();
		LasersDestroyMap();
		LasersDestroyTurels();
	}

	if (knifes_count > 0)
	{
		if (game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS)
		{
			KnifesDestroyBullets();
		}
		KnifesDestroyAsteroids();
		KnifesDestroyMap();
		KnifesDestroyTurels();
	}
	
	if (bullets_count > 0)
	{
		BombsSpawnedByBulletsAnigilation();
		BulletsDestroyAsteroids();
		BulletsDestroedByMap();
	}

	if (bombs_count > 0)
	{
		BombsChainReaction();
		BombsDestroyAsteroids();
		BombsDestroyBonuses();
		BombsCollisionsWithBullets();
		BombsDestroyKnifes();
		BombsDestroyTurels();
	}
	
	if (turels_count > 0)
	{
		TurelsShoot();
	}

	if (portals_count > 0)
	{
		if (!(global_timer % PARTICLE_PERIOD_PORTAL_OUT))
		{
			PortalsCreateParticles();
		}
		PortalsTPShips();
		PortalsTPAsteroids();
		PortalsTPBombs();
		PortalsTPBonuses();
		PortalsTPBullets();
		PortalsTPDynamicParticles();
		PortalsTPPilots();
	}

	CameraFocusesOnPlayers();

	ShipsShoot(); // Some bullets must be updated before making impact.

	camera_data_mtx.lock();
	if (camera.move_velocity_coefficient < CAMERA_HIGH_MOVE_VELOCITY)
	{
		camera.move_velocity_coefficient *= CAMERA_UPDATE_COEFF_PARAM;
	}
	if (camera.resize_velocity_coefficient < CAMERA_HIGH_RESIZE_VELOCITY)
	{
		camera.resize_velocity_coefficient *= CAMERA_UPDATE_COEFF_PARAM;
	}
	camera_data_mtx.unlock();

	input_values_mtx.lock();
	for (uint8_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (double_clk_timers[player] > 0)
		{
			double_clk_timers[player]--;
		}
	}
	input_values_mtx.unlock();

	PollEvents();

	CheckEndMatch();

	ships_array_mtx.lock();
	asteroids_array_mtx.lock();
	DynamicEntitiesCollisions(ships, asteroids, ships_count, asteroids_count);
	asteroids_array_mtx.unlock();
	DynamicEntitiesCollisions(ships, ships_count);
	ships_array_mtx.unlock();

	if (deceler_areas_count > 0)
	{
		deceler_areas_array_mtx.lock();
		asteroids_array_mtx.lock();
		DynamicEntitiesAddForce(deceler_areas, deceler_areas_count, asteroids, asteroids_count);
		asteroids_array_mtx.unlock();
		bombs_array_mtx.lock();
		DynamicEntitiesAddForce(deceler_areas, deceler_areas_count, bombs, bombs_count);
		bombs_array_mtx.unlock();
		bullets_array_mtx.lock();
		DynamicEntitiesAddForce(deceler_areas, deceler_areas_count, bullets, bullets_count);
		bullets_array_mtx.unlock();
		pilots_array_mtx.lock();
		DynamicEntitiesAddForce(deceler_areas, deceler_areas_count, pilots, pilots_count);
		pilots_array_mtx.unlock();
		ships_array_mtx.lock();
		DynamicEntitiesAddForce(deceler_areas, deceler_areas_count, ships, ships_count);
		ships_array_mtx.unlock();
		deceler_areas_array_mtx.unlock();
	}


	pilots_array_mtx.lock();
	DynamicEntitiesCollisions(pilots, pilots_count);
	asteroids_array_mtx.lock();
	DynamicEntitiesCollisions(asteroids, pilots, asteroids_count, pilots_count);
	pilots_array_mtx.unlock();
	DynamicEntitiesCollisions(asteroids, asteroids_count);
	asteroids_array_mtx.unlock();

	if (grav_gens_count > 0)
	{
		grav_gens_array_mtx.lock();
		bombs_array_mtx.lock();
		DynamicEntitiesAddForce(grav_gens, grav_gens_count, bombs, bombs_count);
		bombs_array_mtx.unlock();
		pilots_array_mtx.lock();
		DynamicEntitiesAddForce(grav_gens, grav_gens_count, pilots, pilots_count);
		pilots_array_mtx.unlock();
		ships_array_mtx.lock();
		DynamicEntitiesAddForce(grav_gens, grav_gens_count, ships, ships_count);
		ships_array_mtx.unlock();
		asteroids_array_mtx.lock();
		DynamicEntitiesAddForce(grav_gens, grav_gens_count, asteroids, asteroids_count);
		asteroids_array_mtx.unlock();
		bullets_array_mtx.lock();
		DynamicEntitiesAddForce(grav_gens, grav_gens_count, bullets, bullets_count);
		bullets_array_mtx.unlock();
		grav_gens_array_mtx.unlock();
	}	

	if (ships_count > 0)
	{
		ships_array_mtx.lock();
		map_data_mtx.lock();
		dynamic_particles_array_mtx.lock();
		DynamicEntitiesCollisions(&map, ships, ships_count);
		dynamic_particles_array_mtx.unlock();
		map_data_mtx.unlock();
		ships_array_mtx.unlock();
	}

	if (pilots_count > 0)
	{
		pilots_array_mtx.lock();
		map_data_mtx.lock();
		dynamic_particles_array_mtx.lock();
		DynamicEntitiesCollisions(&map, pilots, pilots_count);
		dynamic_particles_array_mtx.unlock();
		map_data_mtx.unlock();
		pilots_array_mtx.unlock();
	}

	if (asteroids_count > 0)
	{
		asteroids_array_mtx.lock();
		bonuses_array_mtx.lock();
		map_data_mtx.lock();
		dynamic_particles_array_mtx.lock();
		DynamicEntitiesCollisions(&map, asteroids, asteroids_count);
		dynamic_particles_array_mtx.unlock();
		bonuses_array_mtx.unlock();
		map_data_mtx.unlock();
		asteroids_array_mtx.unlock();
	}
	
	if (bombs_count > 0)
	{
		bombs_array_mtx.lock();
		map_data_mtx.lock();
		DynamicEntitiesCollisions(&map, bombs, bombs_count);
		map_data_mtx.unlock();
		bombs_array_mtx.unlock();
	}
	
	UpdateDecelerAreasPhase2();
	UpdateAnigAreaGensPhase2();
	UpdateShipsPhase2();
	UpdatePilotsPhase2();
	UpdateAsteroidsPhase2();
	UpdateLasersPhase2();
	UpdateKnifesPhase2();
	UpdateBulletsPhase2();
	UpdateMegaLasersPhase2();
	UpdateBonusesPhase2();
	UpdateBombsPhase2();
	UpdateTurelsPhase2();
	UpdateGravGensPhase2();
	UpdateMapPhase2();
	UpdateDynamicParticlesPhase2();
	UpdateParticlesPhase2();

	if (flag_end_match)
	{
		play_round = false;
	}

	global_timer++;
	if (stuning_timer > 0)
	{
		stuning_timer--;
	}
}

void Game::UpdateInventoryTemplateMask()
{
	inventory_template_mask =
		(bonus_pull_array[GAME_BONUS_ID_BOMB] ? BONUS_BOMB * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_KNIFE] ? BONUS_KNIFE * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_LASER] ? BONUS_LASER * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_LOOP] ? BONUS_LOOP * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_REVERS] ? BONUS_RULE_REVERSE * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_SHIELD] ? BONUS_BUFF_SHIELD * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_STREAM] ? BONUS_BUFF_STREAM * 0x3 : 0x0) |
		(bonus_pull_array[GAME_BONUS_ID_TRIPLE] ? BONUS_BUFF_TRIPLE * 0x3 : 0x0);
}

EngineTypes::Bonus::inventory_t Game::CheckBonusPoolMenu(EngineTypes::Bonus::inventory_t inventory_template)
{
	return inventory_template & inventory_template_mask;
}

EngineTypes::Bonus::inventory_t Game::GenerateRandomBonus()
{
	printf("Warning::GenerateRandomBonus()::Function outdated. Use GenerateRandomInventory()\n");

	unsigned random = (rand() % GAME_BONUS_INVENTORY_SIZE) + 1;//{1; 2; 3; 4}
	unsigned last_rand = random;
	GameTypes::objects_types_count_t bonus = 0;
	GameTypes::objects_types_count_t select_bonuses_count = 0;

	while (true)
	{
		if (bonus_pull_array[bonus])
		{
			if (random == 0)
			{
				return 1 << (bonus * 2);
			}
			random--;
		}
		bonus++;
		if (bonus >= GAME_BONUS_INVENTORY_SIZE)
		{
			if (last_rand == random)
			{
				return 0;
			}
			bonus = 0;
		}
	}
	/*
	0000 0000 0000 0001		0x0001	LOOP
	0000 0000 0000 0100		0x0004	LASER
	0000 0000 0001 0000		0x0010	BOMB
	0000 0000 0100 0000		0x0040	KNIFE
	*/
}


EngineTypes::Bonus::inventory_t Game::GenerateRandomBonusAndRule()
{
	printf("Warning::GenerateRandomBonusAndRule()::Function outdated. Use GenerateRandomInventory()\n");

	unsigned random = (rand() % (GAME_BONUS_INVENTORY_SIZE + 1)) + 1;//{1; 2; 3; 4; 5}
	if (random == GAME_BONUS_INVENTORY_SIZE + 1)
	{
		return BONUS_RULE_REVERSE;
	}
	unsigned last_rand = random;
	GameTypes::objects_types_count_t bonus = 0;
	GameTypes::objects_types_count_t select_bonuses_count = 0;
	while (true)
	{
		if (bonus_pull_array[bonus])
		{
			if (random == 0)
			{
				return 1 << (bonus * 2);
			}
			random--;
		}
		bonus++;
		if (bonus >= GAME_BONUS_INVENTORY_SIZE)
		{
			if (last_rand == random)
			{
				return 0;
			}
			bonus = 0;
		}
	}
	/*
	0000 0000 0000 0001		0x0001	LOOP
	0000 0000 0000 0100		0x0004	LASER
	0000 0000 0001 0000		0x0010	BOMB
	0000 0000 0100 0000		0x0040	KNIFE
	*/
}

EngineTypes::Bonus::inventory_t Game::GenerateRandomInventory(
	EngineTypes::Bonus::inventory_t inventory_template,
	GameTypes::objects_types_count_t min_objects_count,
	GameTypes::objects_types_count_t max_objects_count,
	GameTypes::objects_types_count_t min_objects_types_count,
	GameTypes::objects_types_count_t max_objects_types_count)
{

	//Bonus loop analys

	inventory_template = CheckBonusPoolMenu(inventory_template);

	//Objec list analys

	GameTypes::objects_types_count_t objects_types_count_in_list = 0;

	for (EngineTypes::Bonus::inventory_t cell = 0; cell < 8; cell++)
	{
		if (inventory_template & (BONUS_CELL << (cell * 2)))
		{
			objects_types_count_in_list++;
		}
	}

	if (min_objects_count > max_objects_count)
	{
		GameTypes::objects_types_count_t buff = min_objects_count;
		min_objects_count = max_objects_count;
		max_objects_count = buff;
	}
	if (min_objects_types_count > max_objects_types_count)
	{
		GameTypes::objects_types_count_t buff = min_objects_types_count;
		min_objects_types_count = max_objects_types_count;
		max_objects_types_count = buff;
	}

	if (min_objects_types_count > objects_types_count_in_list)
	{
		min_objects_types_count = objects_types_count_in_list;
		max_objects_types_count = objects_types_count_in_list;
	}

	if (min_objects_count < 1)
	{
		min_objects_count = 1;
	}
	if (max_objects_count > 3)
	{
		max_objects_count = 3;
	}

	
	//Inventory generation

	GameTypes::objects_types_count_t objects_types_count = min_objects_types_count + rand() % (max_objects_types_count - min_objects_types_count + 1);

	GameTypes::objects_types_count_t checknig_objects_types_count = 0;
	GameTypes::objects_types_count_t filling_objects_types_count = 0;

	EngineTypes::Bonus::inventory_t inventory = BONUS_NOTHING;

	for (EngineTypes::Bonus::inventory_t cell = 0; cell < 8; cell++)
	{
		if (inventory_template & (BONUS_CELL << (cell * 2)))
		{
			if (objects_types_count_in_list <= checknig_objects_types_count || objects_types_count_in_list <= filling_objects_types_count)
			{
				break;
			}

			if (rand() % (objects_types_count_in_list - checknig_objects_types_count) < objects_types_count - filling_objects_types_count)
			{
				filling_objects_types_count++;

				inventory += (min_objects_count + rand() % (max_objects_count - min_objects_count + 1)) << (cell * 2);
			}
			checknig_objects_types_count++;
		}
	}

	return inventory;
}

GameTypes::maps_count_t Game::GenerateRandomMapId()
{
	return selected_maps_id_array[rand() % selected_maps_id_array_length];
}

GameTypes::score_t Game::GetMaxScore()
{
	GameTypes::score_t max_score = scores[0];
	for (GameTypes::score_t i = 1; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		if (max_score < scores[i])
		{
			max_score = scores[i];
		}
	}
	return max_score;
}

Game::Game()
{
	InitGame();
	InitMenus();
}

void Game::InitGame()
{
	game_rules = GAME_RULE_DEFAULT_GAME_RULES;
	flag_all_entities_initialisate = false;
	flag_round_results = false;
	global_timer = 0;

	teams = new GameTypes::entities_count_t[GAME_PLAYERS_MAX_COUNT];
	playing_teams = new GameTypes::entities_count_t[GAME_PLAYERS_MAX_COUNT];
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		teams[i] = 0;
		playing_teams[i] = 0;
	}
	object_pull_array = new bool[GAME_OBJECT_TYPES_COUNT];
	map_pull_array = new bool[GAME_MAPS_COUNT];
	bonus_pull_array = new bool[GAME_BONUS_INVENTORY_SIZE];
	for (GameTypes::objects_types_count_t i = 0; i < GAME_OBJECT_TYPES_COUNT; i++)
	{
		object_pull_array[i] = true;
	}
	for (GameTypes::objects_types_count_t i = 0; i < GAME_MAPS_COUNT; i++)
	{
		map_pull_array[i] = true;
	}
	for (GameTypes::objects_types_count_t i = 0; i < GAME_BONUS_INVENTORY_SIZE; i++)
	{
		bonus_pull_array[i] = true;
	}
}

void Game::InitMatch()
{
	players_count = 0;
	play_round = true;
	play_match = true;
	flag_end_match = false;
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (menu_p__ships_select_buttons[player] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			players_count++;
		}
	}

	//Set bonus pull

	for (GameTypes::entities_types_count_t bonus = 0; bonus < GAME_BONUS_INVENTORY_SIZE; bonus++)
	{
		if (bonus_pull_array[bonus])
		{
			goto skip_bonus_pull_set;//skip second cycle
		}
	}
	for (GameTypes::entities_types_count_t bonus = 0; bonus < GAME_BONUS_INVENTORY_SIZE; bonus++)
	{
		bonus_pull_array[bonus] = true;
	}

skip_bonus_pull_set:

	UpdateInventoryTemplateMask();

	//Set bonus pull

	MemoryLock();
	
	for (GameTypes::players_count_t team = 1; team <= GAME_PLAYERS_MAX_COUNT; team++)
	{
		scores[team - 1] = -2;
	}

	selected_maps_id_array_length = 0;
	for (GameTypes::maps_count_t map_id = 0; map_id < GAME_MAPS_COUNT; map_id++)
	{
		if (map_pull_array[map_id] == true)
		{
			selected_maps_id_array_length++;
		}
	}

	if (selected_maps_id_array_length == 0)
	{
		selected_maps_id_array_length = GAME_MAPS_COUNT;
		selected_maps_id_array = new GameTypes::maps_count_t[GAME_MAPS_COUNT];
		for (GameTypes::maps_count_t map_id = 0; map_id < GAME_MAPS_COUNT; map_id++)
		{
			selected_maps_id_array[map_id] = map_id;
		}
	}
	else
	{
		selected_maps_id_array = new GameTypes::maps_count_t[selected_maps_id_array_length];

		for (GameTypes::maps_count_t map_id = 0, found = 0; found < selected_maps_id_array_length; map_id++)
		{
			if (map_pull_array[map_id] == true)
			{
				selected_maps_id_array[found] = map_id;
				found++;
			}
		}
	}

	//set players pull
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		playing_teams[player] = teams[player];
	}

	end_match_score = 5;
}

void Game::PollEvents()
{
	switch (current_event)
	{
	case MAP_TEST_MAP:			Event0();	return;
	case MAP_TUREL_ON_CENTER:	Event1();	return;
	case MAP_CYRCLE_ON_CENTER:	Event2();	return;
	case MAP_DECELERATION_AREA:	Event3();	return;
	case MAP_MEGA_LASERS:		Event4();	return;
	case MAP_DYNAMICAL:			Event5();	return;
	case MAP_DESTROYABLE:		Event6();	return;
	case MAP_AGGRESSIVE:		Event7();	return;
	case MAP_BROKEN:			Event8();	return;
	case MAP_PORTAL:			Event9();	return;
	case MAP_NO_CENTER:			Event10();	return;
	}
}

void Game::InitLevel()
{
	MutexesLock();
	//ResetAllThreadEvents();
	srand(time(0));
	play_round = true;
	flag_round_results = false;

	logs.Clear();

	MemorySetDefault();

	current_map_id = GenerateRandomMapId();

	//current_map_id = MAP_PORTAL;
	//std::cout << "Only map Portal selected." << std::endl;

	Vec2F ships_positions[GAME_PLAYERS_MAX_COUNT];
	float ships_angles[GAME_PLAYERS_MAX_COUNT];

	const GameTypes::score_t max_score = GetMaxScore();
	
	current_event = current_map_id;

	/* Create maps */

	//current_map_id = MAP_NO_CENTER;
	//current_event = current_map_id;
	//std::cout << "Game::InitLevel::Only map 10 created." << std::endl;
	
	switch (current_map_id)
	{
	case MAP_NO_CENTER:
		CreateMap10(ships_positions, ships_angles);
		break;
	case MAP_PORTAL:
		CreateMap9(ships_positions, ships_angles);
		break;
	case MAP_BROKEN:
		CreateMap8(ships_positions, ships_angles);
		break;
	case MAP_AGGRESSIVE:
		CreateMap7(ships_positions, ships_angles);
		break;
	case MAP_DESTROYABLE:
		CreateMap6(ships_positions, ships_angles);
		break;
	case MAP_DYNAMICAL:
		CreateMap5(ships_positions, ships_angles);
		break;
	case MAP_MEGA_LASERS:
		CreateMap4(ships_positions, ships_angles);
		break;
	case MAP_DECELERATION_AREA:
		CreateMap3(ships_positions, ships_angles);
		break;
	case MAP_CYRCLE_ON_CENTER:
		CreateMap2(ships_positions, ships_angles);
		break;
	case MAP_TUREL_ON_CENTER:
		CreateMap1(ships_positions, ships_angles);
		break;
	default:
		std::cout << "WARNING::Map value is undeclarated. Defaul map is created." << std::endl;
	case MAP_TEST_MAP:
		CreateMap0(ships_positions, ships_angles);
		break;
	}

	AddEntity(
		Particle(
			0,								//current tic
			Vec2F(0.5f),					//position
			0.0f,							//angle
			101.0f,							//radius
			PARTICLE_PERIOD_BACKGROUND,		//type
			Color3F(67.0f / 256.0f, 45.0f / 256.0f, 93.0f / 256.0f) / 2.0f,
			1000,							//period
			0,								//postpone
			PARTICLE_ANIMATION_NOT_FINISH)	//finish tic
	);

	AddEntity(
		Particle(
			0,								//current tic
			Vec2F(-0.5f),					//position
			0.0f,							//angle
			21.0f,							//radius
			PARTICLE_PERIOD_BACKGROUND,		//type
			Color3F(57.0f / 256.0f, 40.0f / 256.0f, 93.0f / 256.0f) / 2.0f,
			1000,							//period
			0,								//postpone
			PARTICLE_ANIMATION_NOT_FINISH)	//finish tic
	);

	AddEntity(
		Particle(
			0,								//current tic
			Vec2F(-0.5f),					//position
			0.0f,							//angle
			7.0f,							//radius
			PARTICLE_PERIOD_BACKGROUND,		//type
			Color3F(57.0f / 256.0f, 40.0f / 256.0f, 93.0f / 256.0f) / 2.0f,
			1000,							//period
			0,								//postpone
			PARTICLE_ANIMATION_NOT_FINISH)	//finish tic
	);

	printf("Particles count %lu\n", particles_count);

	/* Spawn players */

	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		ships_can_shoot_flags[player] = SHIP_DEFAULT_UNBRAKABLE_PERIOD;
		players_in_team[player] = 0;
		double_clk_timers[player] = 0;
		burnout_flags[player] = false;
	}

	if (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)
	{
		for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
		{
			ships_angles[i] += (float)rand() / (float)RAND_MAX;
		}
	}

	if (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)
	{
		for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
		{
			GameTypes::players_count_t temp1 = rand() % GAME_PLAYERS_MAX_COUNT;
			Vec2F temp2 = ships_positions[temp1];
			ships_positions[temp1] = ships_positions[i];
			ships_positions[i] = temp2;
		}
	}

	//Set start bonus

	start_bonus = BONUS_NOTHING;
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE)
	{
		start_bonus |= BONUS_BUFF_TRIPLE;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD)
	{
		start_bonus |= (game_rules & GAME_RULE_TRIPLE_BONUSES) ? BONUS_BUFF_SHIELD * 3 : BONUS_BUFF_SHIELD;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS && !(game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS))
	{
		start_bonus |= GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) * ((game_rules & GAME_RULE_TRIPLE_BONUSES) ? BONUS_CELL : 1);
	}

	//Set start bonus

	ships_control_menu.Clear();

	Vec2F buttons_positions[4] =
	{
		Vec2F(-1.0f, 1.0f),
		Vec2F(1.0f, 1.0f),
		Vec2F(1.0f, -1.0f),
		Vec2F(-1.0f, -1.0f)
	};

	Vec2F zero_velocity;
	players_count = 0;
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (playing_teams[player] > 0)
		{
			if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS && game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS)
			{
				start_bonus &= BONUS_ALL - BONUS_BONUS;
				start_bonus |= GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) * ((game_rules & GAME_RULE_TRIPLE_BONUSES) ? BONUS_CELL : 1);
			}

			AddEntity(
				Ship(
					ships_positions[player],		//position
					Vec2F(),						//velocity
					player,							//player number
					playing_teams[player],			//team number
					(void*)&burnout_flags[player],	//burnout flag pointer
					(void*)&rotate_flags[player],	//rotate flag pointer
					(void*)&shoot_flags[player],	//shoot flag pointer
					nullptr,						//heatbox vertexs array
					0,								//heatbox vertexs count
					ships_angles[player],			//angle
					start_bonus));					//bonus inventory

			players_count++;
			IncrementPlayersCountInTeam(playing_teams[player]);

#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true

			Vec2F 
				horisontal_points[3] = { Vec2F(0.0f, 0.0f), Vec2F(1.0f, 0.0f), Vec2F(1.0f / 2.0f) },
				vertical_points[3] = { Vec2F(0.0f, 0.0f), Vec2F(0.0f, 1.0f), Vec2F(1.0f / 2.0f) };
			Area
				shoot_button_area = (player % 1) ? Area(vertical_points, 3) : Area(horisontal_points, 3),
				rotate_button_area = (player % 1) ? Area(horisontal_points, 3) : Area(vertical_points, 3);

			Button
				button_shoot = Button(
					BUTTON_ID__SHIP1_ROTATE + player * 2,
					buttons_positions[player],
					Vec2F((player == 0 || player == 3) ? 1.0f : -1.0f, (player == 2 || player == 3) ? 1.0f : -1.0f) * BUTTON_CONTROLER_SIZE,
					&shoot_button_area,
					"",
					0,
					BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1) | BUTTON_STATUS_ACTIVE),
				button_rotate = Button(
					BUTTON_ID__SHIP1_SHOOT + player * 2,
					buttons_positions[player],
					Vec2F((player == 0 || player == 3) ? 1.0f : -1.0f, (player == 2 || player == 3) ? 1.0f : -1.0f) * BUTTON_CONTROLER_SIZE,
					&rotate_button_area,
					"",
					0,
					BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1) | BUTTON_STATUS_ACTIVE);

			ships_control_menu.AddButton(player * 2, &button_shoot);
			ships_control_menu.AddButton(player * 2, &button_rotate);
#endif
		}
	}
	
	ships_count = players_count;

	MutexesUnlock();

	ships_control_menu.Recalculate();
	current_active_menu = &ships_control_menu;

	//ResetAllThreadEvents();
}

void Game::InitMenus()
{
	//arrays

	//main menu
	Vec2F* points = new Vec2F[4];
	points[0].Set(0.0f, 0.0f);
	points[1].Set(0.0f, 1.0f);
	points[2].Set(1.0f, 1.0f);
	points[3].Set(1.0f, 0.0f);
	Area* area = new Area(points, 4);

	Button* buttons = new Button[3];
	Vec2F position;
	position.Set(-0.5f, 0.95f);
	Vec2F size;
	size.Set(1.0f, -0.60f);
	buttons[0].Set(BUTTON_ID__START_MATCH, &position, &size, area, "PLAY", 20);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.3f);
	size.Set(0.475f, -0.25f);
	buttons[1].Set(BUTTON_ID__GO_TO_OPTINS_MENU, &position, &size, area, "Options", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.3f);
	buttons[2].Set(BUTTON_ID__EXIT, &position, &size, area, "Exit", 6);
	buttons[2].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	main_menu.Set(&position, &size, buttons, 3);
	delete[] buttons;

	//option menu
	buttons = new Button[15];
	size.Set(1.5f, -0.1f);
#define GAME_OPTION_MENU_BORD	0.11f
#define GAME_OPTION_MENU_UP_Y	0.9f
#define GAME_OPTIONS_STAT_X		-0.75f
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 0 * GAME_OPTION_MENU_BORD);
	buttons[0].Set(BUTTON_ID__SET_RANDOM_SPAWN, &position, &size, area, "Random spawn", 5);
	buttons[0].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 1 * GAME_OPTION_MENU_BORD);
	buttons[1].Set(BUTTON_ID__SET_RANDOM_SPAWN_DIRECTION, &position, &size, area, "Random spawn direction", 5);
	buttons[1].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 2 * GAME_OPTION_MENU_BORD);
	buttons[2].Set(BUTTON_ID__SET_SPAWN_THIS_BONUS, &position, &size, area, "Spawn this bonus", 5);
	buttons[2].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 3 * GAME_OPTION_MENU_BORD);
	buttons[3].Set(BUTTON_ID__SET_SPAWN_THIS_DIFFERENT_BONUSES, &position, &size, area, "Can spawn this different bonuses", 4);
	buttons[3].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 4 * GAME_OPTION_MENU_BORD);
	buttons[4].Set(BUTTON_ID__SET_TRIPLE_BONUSES, &position, &size, area, "Triple bonuses", 5);
	buttons[4].status = ((game_rules & GAME_RULE_TRIPLE_BONUSES) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 5 * GAME_OPTION_MENU_BORD);
	buttons[5].Set(BUTTON_ID__SET_SPAWN_THIS_TRIPLE_BAFF, &position, &size, area, "Spawn this triple", 5);
	buttons[5].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 6 * GAME_OPTION_MENU_BORD);
	buttons[6].Set(BUTTON_ID__SET_SPAWN_THIS_SHIELD_BAFF, &position, &size, area, "Spawn this shield", 5);
	buttons[6].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 7 * GAME_OPTION_MENU_BORD);
	buttons[7].Set(BUTTON_ID__SET_KNIFES_CAN_DESTROY_BULLETS, &position, &size, area, "Knifes can destroy bullets", 5);
	buttons[7].status = ((game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 8 * GAME_OPTION_MENU_BORD);
	buttons[8].Set(BUTTON_ID__SET_NEED_KILL_PILOT, &position, &size, area, "Need kill pilot", 5);
	buttons[8].status = ((game_rules & GAME_RULE_NEED_KILL_PILOT) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | ((game_rules & GAME_RULE_NEED_KILL_PILOT) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 9 * GAME_OPTION_MENU_BORD);
	buttons[9].Set(BUTTON_ID__SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, area, "Friendly sheep can restore", 5);
	buttons[9].status = ((game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 10 * GAME_OPTION_MENU_BORD);
	buttons[10].Set(BUTTON_ID__SET_ACTIVE_FRIENDLY_FIRE, &position, &size, area, "Frendly fire", 5);
	buttons[10].status = ((game_rules & GAME_RULE_FRENDLY_FIRE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 11 * GAME_OPTION_MENU_BORD);
	buttons[11].Set(BUTTON_ID__GO_TO_SELECT_BONUSES_MENU, &position, &size, area, "Bonus pull menu", 5);
	buttons[11].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 12 * GAME_OPTION_MENU_BORD);
	buttons[12].Set(BUTTON_ID__GO_TO_SELECT_MAP_MENU, &position, &size, area, "Map pull menu", 5);
	buttons[12].status |= BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 13 * GAME_OPTION_MENU_BORD);
	buttons[13].Set(BUTTON_ID__GO_TO_SELECT_OBJECTS_MENU, &position, &size, area, "Spawning objects menu", 5);
	buttons[13].status |= BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 14 * GAME_OPTION_MENU_BORD);
	buttons[14].Set(BUTTON_ID__SET_ACTIVE_BALANCE, &position, &size, area, "Auto balance", 5);
	buttons[14].status = ((game_rules & GAME_RULE_BALANCE_ACTIVE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.5f, GAME_OPTION_MENU_UP_Y - 1.0f - 15.0f * GAME_OPTION_MENU_BORD);
	option_menu.Set(&position, &size, buttons, 15);
	option_menu.HardRecalculate();
	delete[] buttons;

	//pause menu
	buttons = new Button[2];
	position.Set(-0.4f, 0.825f);
	size.Set(0.8f, -0.2f);
	buttons[0].Set(BUTTON_ID__RESUME_MATCH, &position, &size, area, "Resume", 6);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.4f, 0.6f);
	buttons[1].Set(BUTTON_ID__GO_TO_MAIN_MENU, &position, &size, area, "Main menu", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	pause_menu.Set(&position, &size, buttons, 2);
	delete[] buttons;

	//sheeps select menu
	buttons = new Button[5];;
	size.Set(0.475f, -0.25f);
	position.Set(-0.5f, 0.9f);
	buttons[0].Set(BUTTON_ID__SELECT_SHIP_1, &position, &size, area, "Player 1", 6);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.9f);
	buttons[1].Set(BUTTON_ID__SELECT_SHIP_2, &position, &size, area, "Player 2", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.6f);
	buttons[2].Set(BUTTON_ID__SELECT_SHIP_3, &position, &size, area, "Player 3", 6);
	buttons[2].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.6f);
	buttons[3].Set(BUTTON_ID__SELECT_SHIP_4, &position, &size, area, "Player 4", 6);
	buttons[3].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.3f);
	size.Set(1.0f, -0.25f);
	buttons[4].Set(BUTTON_ID__START_GAME, &position, &size, area, "Start game", 7);
	buttons[4].status |= BUTTON_STATUS_FALSE;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	ships_select_menu.Set(&position, &size, buttons, 5);
	delete[] buttons;

	//maps select menu
#define GAME_PULL_MENU_UP_Y				0.9f
#define GAME_PULL_MENU_RIGHT_BORDER		0.55f
#define GAME_PULL_MENU_DOWN_BORDER		0.3f
#define GAME_PULL_MENU_BUTTON_SIZE_Y	-0.25f
	buttons = new Button[GAME_MAPS_COUNT];
	size.Set(0.5f, GAME_PULL_MENU_BUTTON_SIZE_Y);
	for (uint8_t i = 0; i < GAME_MAPS_COUNT; i++)
	{
		position.Set(-0.5f + (float)(i % 2) * GAME_PULL_MENU_RIGHT_BORDER, GAME_PULL_MENU_UP_Y - (float)(i / 2) * GAME_PULL_MENU_DOWN_BORDER);
		buttons[i].Set(BUTTON_ID__SELECT_MAP + i, &position, &size, area, "", 5, BUTTON_STATUS_TRUE);
		buttons[i].status |= BUTTON_STATUS_ACTIVE;
	}
	buttons[MAP_TEST_MAP].SetText("Test");
	buttons[MAP_TEST_MAP].text_size = 6u;
	buttons[MAP_TUREL_ON_CENTER].SetText("Turel");
	buttons[MAP_TUREL_ON_CENTER].text_size = 6u;
	buttons[MAP_CYRCLE_ON_CENTER].SetText("Grav gen");
	buttons[MAP_DECELERATION_AREA].SetText("Deceler");
	buttons[MAP_MEGA_LASERS].SetText("Mega laser");
	buttons[MAP_DYNAMICAL].SetText("Dynamical");
	buttons[MAP_DESTROYABLE].SetText("Destructible");
	buttons[MAP_DESTROYABLE].text_size = 4u;
	buttons[MAP_AGGRESSIVE].SetText("Aggressive");
	buttons[MAP_AGGRESSIVE].text_size = 4u;
	buttons[MAP_BROKEN].SetText("Broken");
	buttons[MAP_PORTAL].SetText("Portal");
	buttons[MAP_NO_CENTER].SetText("No Center");
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -GAME_PULL_MENU_DOWN_BORDER * (float)(((GAME_MAPS_COUNT + 1) / 2) + 1));
	map_pull_select_menu.Set(&position, &size, buttons, GAME_MAPS_COUNT);
	delete[] buttons;

	//spawning objects select menu
	buttons = new Button[GAME_OBJECTS_COUNT];
	position.Set(-0.5f, 0.9f);
	size.Set(0.475f, -0.475f);
	buttons[0].Set(BUTTON_ID__SELECT_OBJECT_ASTEROID, &position, &size, area, "Asteroid", 6, (object_pull_array[GAME_OBJECT_ASTEROID] == true) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(0.5f, -0.5f);
	spawning_objects_select_menu.Set(&position, &size, buttons, 1);
	delete[] buttons;

	//spawning bonuses select menu
	buttons = new Button[GAME_BONUS_INVENTORY_SIZE];
	size.Set(0.5f, -0.25f);
	for (uint8_t bonus = 0; bonus < GAME_BONUS_INVENTORY_SIZE; bonus++)
	{
		position.Set(-0.5f + (float)(bonus % 2) * GAME_PULL_MENU_RIGHT_BORDER, GAME_PULL_MENU_UP_Y -(float)(bonus / 2) * GAME_PULL_MENU_DOWN_BORDER);
		buttons[bonus].Set(BUTTON_ID__SELECT_BONUS + bonus, &position, &size, area, "", 7, (bonus_pull_array[bonus] == true) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE);
		buttons[bonus].status |= BUTTON_STATUS_ACTIVE;
	}
	buttons[GAME_BONUS_ID_LOOP].SetText("Loop");
	buttons[GAME_BONUS_ID_LASER].SetText("Laser");
	buttons[GAME_BONUS_ID_BOMB].SetText("Bomb");
	buttons[GAME_BONUS_ID_KNIFE].SetText("Knife");
	buttons[GAME_BONUS_ID_TRIPLE].SetText("Triple");
	buttons[GAME_BONUS_ID_SHIELD].SetText("Shield");
	buttons[GAME_BONUS_ID_STREAM].SetText("Stream");
	buttons[GAME_BONUS_ID_REVERS].SetText("Revers");
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -GAME_PULL_MENU_DOWN_BORDER * (float)(((GAME_BONUS_INVENTORY_SIZE + 1) / 2) + 1));
	bonus_pull_select_menu.Set(&position, &size, buttons, GAME_BONUS_INVENTORY_SIZE);
	delete[] buttons;

#if OPEN_GL_REALISATION__SHIPS_CONTROLED_BY_SCREEN_BUTTONS == true
	//ship control menu
	position.Set(-1.0f, -1.0f);
	size.Set(2.0f, 2.0f);
	ships_control_menu.Set(&position, &size);
#endif

	current_active_menu = &main_menu;

	delete area;
}

void Game::NextLevel()
{
	/*
	If team has 5 or more points this team is seted as potential winner team.
	If count of potential winners more than 1, then math is completed else mach is completed.

	If team has -5 or less points this team is seted as not existing. All players from that team will be not playing in all next rounds.
	If counts of playing teams is 1 or less then the mach is completed.
	*/

	bool potential_winner_detected = false;

	//Checking winners
	for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
	{
		if (scores[team] >= end_match_score)
		{
			potential_winner_detected = true;
		}

		if (scores[team] <= -end_match_score)
		{
			for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
			{
				if (playing_teams[player] == team + 1)
				{
					playing_teams[player] = SHIPS_SELECT_BUTTONS_NO_TEAM;
				}
			}
		}
	}

	
	GameTypes::players_count_t found_players_count = 0;

	if (potential_winner_detected)
	{
		GameTypes::players_count_t winners_count = 0;
		for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
		{
			if (scores[team] >= end_match_score)
			{
				scores[team] = 0;
				winners_count++;
			}
			else
			{
				scores[team] = -end_match_score;
				for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
				{
					if (playing_teams[player] == team + 1)
					{
						playing_teams[player] = SHIPS_SELECT_BUTTONS_NO_TEAM;
					}
				}
			}
		}

		flag_end_match = (winners_count <= 1);
		end_match_score = 1;
		return;
	}

	GameTypes::players_count_t existing_players_count = 0;

	//Checking loosers.
 	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (playing_teams[player] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			existing_players_count++;
		}
	}

	flag_end_match = (existing_players_count <= 1);
	return;
}

void Game::EndMatch()
{
	play_match = false;
	flag_all_entities_initialisate = false;

	MemoryFree();

	flag_update_end_match = false;
	object_p__menu_functions->OpenMainMenu();
}

void Game::DecrementPlayersCountInTeam(GameTypes::players_count_t team_number)
{
	if (team_number > 0 && players_in_team[team_number - 1])
	{
		players_in_team[team_number - 1]--;
	}
	flag_update_end_match = true;
}

void Game::IncrementPlayersCountInTeam(GameTypes::players_count_t team_number)
{
	if (team_number > 0)
	{
		players_in_team[team_number - 1]++;
	}
	flag_update_end_match = true;
}

void Game::CheckEndMatch()
{
	if (end_match_tic)
	{
		if (global_timer > end_match_tic)
		{
			RoundResultsInit();
		}
	}
	else if(flag_update_end_match)
	{
		for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
		{
			if (players_in_team[team])
			{
				if (flag_update_end_match)
				{
					flag_update_end_match = false;
				}
				else
				{
					return;
				}
			}
		}
		end_match_tic = global_timer + GAME_END_MATCH_DELLAY;
		flag_update_end_match = false;
	}
}

void Game::RoundResultsInit()
{
#define ROUND_RESULTS_INIT__MAP_SIZE			3.0f
#define ROUND_RESULTS_INIT__MAP_SIZE_X05		(ROUND_RESULTS_INIT__MAP_SIZE * 0.5f)
#define ROUND_RESULTS_INIT__MAP_SIZE_X2			(ROUND_RESULTS_INIT__MAP_SIZE * 2.0f)
#define ROUND_RESULTS_INIT__MAP_SIZE_X4			(ROUND_RESULTS_INIT__MAP_SIZE * 4.0f)

	camera_data_mtx.lock();
	ships_array_mtx.lock();
	map_data_mtx.lock();

	camera.SetPosition(Vec2F(0.0f, 0.0f));
	camera.SetSize(ROUND_RESULTS_INIT__MAP_SIZE);

	camera_data_mtx.unlock();

	flag_round_results = true;
	play_round = false;
	flag_round_results = true;

	CreateMapRoundResults(players_count, end_match_score, GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE);

	map_data_mtx.unlock();
	ships_array_mtx.unlock();

	MutexesLock();
	MemorySetDefault();
	MutexesUnlock();

	ships_array_mtx.lock();

	GameTypes::players_count_t y_pos = 0;
	float up_y = (float)(players_count - 1) / 2.0f;

	for (GameTypes::players_count_t team = 1; team <= GAME_PLAYERS_MAX_COUNT; team++)
	{
		if (scores[team - 1] > -end_match_score)
		{
			for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
			{
				if (ships[ship].GetTeamNumber() == team)
				{
					ships[ship].exist = true;
					ships[ship].SetPosition(
						Vec2F(
							(float)scores[ships[ship].GetTeamNumber() - 1],
							up_y - (float)y_pos) * GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE);
					ships[ship].radius = GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE / 5.0f;
					ships[ship].SetAngle(0.0f);
					ships[ship].UpdateMatrix();
					ships[ship].ClearInventory();
					ships_count++;
					y_pos++;
				}
			}
		}
	}

	ships_array_mtx.unlock();
}

bool Game::RoundResults()
{
#define ROUND_RESULTS__MAP_SIZE				ROUND_RESULTS_INIT__MAP_SIZE
#define ROUND_RESULTS__MAP_SIZE_X05			(ROUND_RESULTS__MAP_SIZE * 0.5f)
#define ROUND_RESULTS__MAP_SIZE_X025		(ROUND_RESULTS__MAP_SIZE * 0.25f)
#define ROUND_RESULTS__MAP_SIZE_X2			(ROUND_RESULTS__MAP_SIZE * 2.0f)

	if (!logs.HaveData())
	{
		return false;
	}

	GameTypes::score_t step = GAME_FULL_MATCH_ROUNDS * 2 + 1;

	float size_coeff = (end_match_score == GAME_FULL_MATCH_ROUNDS) ? 1.0f : 1.0f * (float)step / (float)GAME_FULL_MATCH_ROUNDS;

	EngineTypes::Log::data_t data = logs.PopFromStart();

	if (((data >> LOG_HEAD) & LOG_MASK_BITS) == LOG_CHANGE_SCORE)
	{
		for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
		{
			if (ships[ship].exist && ships[ship].GetTeamNumber() == ((data >> LOG_DATA_TEAM) & LOG_MASK_BITS) + 1)
			{
				if (((data >> LOG_DATA_SCORE) & LOG_MASK_BITS) == LOG_INCREMENT)
				{
					if (scores[ships[ship].GetTeamNumber() - 1] < end_match_score)
					{
						ships[ship].Move(Vec2F(GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE, 0.0f));
						ships[ship].UpdateMatrix();
						scores[ships[ship].GetTeamNumber() - 1]++;
					}
				}
				else if (((data >> LOG_DATA_SCORE) & LOG_MASK_BITS) == LOG_DECREMENT)
				{
					if (scores[ships[ship].GetTeamNumber() - 1] > -end_match_score)
					{
						ships[ship].Move(Vec2F(-GAME_POUND_RESULTS_MAP_DEFAUL_CELL_SIZE, 0.0f));
						ships[ship].UpdateMatrix();
						scores[ships[ship].GetTeamNumber() - 1]--;
					}
				}
			}
		}
	}

	if (!logs.HaveData() && !play_match)
	{
		delete[] ships;
	}
	return  logs.HaveData();
}



void Game::IncrementScore(GameTypes::players_count_t team_number)
{
	logs.PushToEnd((LOG_CHANGE_SCORE << LOG_HEAD) | (LOG_INCREMENT << LOG_DATA_SCORE) | (((team_number - 1) & LOG_MASK_BITS) << LOG_DATA_TEAM));
}

void Game::DecrementScore(GameTypes::players_count_t team_number)
{
	logs.PushToEnd((LOG_CHANGE_SCORE << LOG_HEAD) | (LOG_DECREMENT << LOG_DATA_SCORE) | (((team_number - 1) & LOG_MASK_BITS) << LOG_DATA_TEAM));
}



void Game::ResetThreadDurations()
{
	for (uint8_t th = 0; th < 4; th++)
	{
		thread_durations[th][0] = std::chrono::microseconds(0);
		thread_durations[th][1] = std::chrono::microseconds(0);
	}
}



//deceler_area -> grav_gen -> camera -> portal -> ship -> pilot -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turel -> bullet -> asteroid -> bonus -> map -> particle -> dynamic_particle -> log
void Game::MutexesLock()
{
	deceler_areas_array_mtx.lock();
	grav_gens_array_mtx.lock();
	camera_data_mtx.lock();
	portals_array_mtx.lock();
	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	input_values_mtx.lock();
	mega_lasers_array_mtx.lock();
	lasers_array_mtx.lock();
	bombs_array_mtx.lock();
	knifes_array_mtx.lock();
	turels_array_mtx.lock();
	bullets_array_mtx.lock();
	asteroids_array_mtx.lock();
	bonuses_array_mtx.lock();
	map_data_mtx.lock();
	particles_array_mtx.lock();
	dynamic_particles_array_mtx.lock();
	log_data_mtx.lock();
}

//deceler_area -> grav_gen -> camera -> ship -> pilot -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turel -> bullet -> asteroid -> bonus -> map -> particle -> log
void Game::MutexesUnlock()
{
	deceler_areas_array_mtx.unlock();
	grav_gens_array_mtx.unlock();
	camera_data_mtx.unlock();
	portals_array_mtx.unlock();
	ships_array_mtx.unlock();
	pilots_array_mtx.unlock();
	input_values_mtx.unlock();
	mega_lasers_array_mtx.unlock();
	lasers_array_mtx.unlock();
	bombs_array_mtx.unlock();
	knifes_array_mtx.unlock();
	turels_array_mtx.unlock();
	bullets_array_mtx.unlock();
	asteroids_array_mtx.unlock();
	bonuses_array_mtx.unlock();
	map_data_mtx.unlock();
	particles_array_mtx.unlock();
	dynamic_particles_array_mtx.unlock();
	log_data_mtx.unlock();
}

void Th_00(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_01(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_02(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_03(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_04(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_05(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_06(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_07(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_08(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_09(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0A(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0B(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0C(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0D(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0E(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_0F(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_10(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_11(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Th_12(std::shared_mutex* mtx_p, bool* return_data)
{
	mtx_p->lock();
	mtx_p->unlock();
	*return_data = false;
}

void Game::DebugLog__CheckMutexeslLock()
{
	std::shared_mutex* mtx_00 = &deceler_areas_array_mtx;
	std::shared_mutex* mtx_01 = &grav_gens_array_mtx;
	std::shared_mutex* mtx_02 = &camera_data_mtx;
	std::shared_mutex* mtx_03 = &ships_array_mtx;
	std::shared_mutex* mtx_04 = &pilots_array_mtx;
	std::shared_mutex* mtx_05 = &anig_area_gens_array_mtx;
	std::shared_mutex* mtx_06 = &input_values_mtx;
	std::shared_mutex* mtx_07 = &mega_lasers_array_mtx;
	std::shared_mutex* mtx_08 = &lasers_array_mtx;
	std::shared_mutex* mtx_09 = &bombs_array_mtx;
	std::shared_mutex* mtx_0A = &knifes_array_mtx;
	std::shared_mutex* mtx_0B = &turels_array_mtx;
	std::shared_mutex* mtx_0C = &bullets_array_mtx;
	std::shared_mutex* mtx_0D = &asteroids_array_mtx;
	std::shared_mutex* mtx_0E = &bonuses_array_mtx;
	std::shared_mutex* mtx_0F = &map_data_mtx;
	std::shared_mutex* mtx_10 = &particles_array_mtx;
	std::shared_mutex* mtx_11 = &dynamic_particles_array_mtx;
	std::shared_mutex* mtx_12 = &log_data_mtx;

	bool data[19];
	for (uint8_t i = 0; i < 18; i++)
	{
		data[i] = true;
	}

	std::thread th_00(Th_00, mtx_00, &data[0x00]);//deceler area
	std::thread th_01(Th_01, mtx_01, &data[0x01]);//grav gen
	std::thread th_02(Th_02, mtx_02, &data[0x02]);//camera
	std::thread th_03(Th_03, mtx_03, &data[0x03]);//ship
	std::thread th_04(Th_04, mtx_04, &data[0x04]);//pilot
	std::thread th_05(Th_05, mtx_11, &data[0x05]);//anig_area_gen
	std::thread th_06(Th_06, mtx_05, &data[0x06]);//input
	std::thread th_07(Th_07, mtx_06, &data[0x07]);//mega laser
	std::thread th_08(Th_08, mtx_07, &data[0x08]);//laser
	std::thread th_09(Th_09, mtx_08, &data[0x09]);//bomb
	std::thread th_0A(Th_0A, mtx_09, &data[0x0A]);//knife
	std::thread th_0B(Th_0B, mtx_0A, &data[0x0B]);//turel
	std::thread th_0C(Th_0C, mtx_0B, &data[0x0C]);//bullet
	std::thread th_0D(Th_0D, mtx_0C, &data[0x0D]);//asteroid
	std::thread th_0E(Th_0E, mtx_0D, &data[0x0E]);//bonus
	std::thread th_0F(Th_0F, mtx_0E, &data[0x0F]);//map
	std::thread th_10(Th_10, mtx_0F, &data[0x10]);//particle
	std::thread th_11(Th_11, mtx_10, &data[0x11]);//dynamic particle
	std::thread th_12(Th_12, mtx_11, &data[0x12]);//log

	std::this_thread::sleep_for(std::chrono::seconds(1));

	if (data[0x00] == false)
	{
		for (uint8_t i = 1; i < 17; i++)
		{
			if (data[i] != false)
			{
				data[0x00] = true;
				goto end_of_cycle;
			}
		}
	}
end_of_cycle:
	if (data[0x00] == false)
	{
		th_00.join();
		th_01.join();
		th_02.join();
		th_03.join();
		th_04.join();
		th_05.join();
		th_06.join();
		th_07.join();
		th_08.join();
		th_09.join();
		th_0A.join();
		th_0B.join();
		th_0C.join();
		th_0D.join();
		th_0E.join();
		th_0F.join();
		th_10.join();
		th_11.join();
		th_12.join();

		std::cout << "All mutexes are not locked." << std::endl;
		return;
	}

	printf(" 0. Deceler areas: ");
	if (data[0x00])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 1. Grav gens:     ");
	if (data[0x01])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 2. Camera:        ");
	if (data[0x02])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 3. Ships:         ");
	if (data[0x03])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 4. Pilots:        ");
	if (data[0x04])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 5. AnigAreaGens:  ");
	if (data[0x05])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 6. Input values:  ");
	if (data[0x06])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 7. Mega lasers:   ");
	if (data[0x07])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 8. Lasers:        ");
	if (data[0x08])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 9. Bombs:         ");
	if (data[0x09])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("10. Knifes:        ");
	if (data[0x0A])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("11. Turels:        ");
	if (data[0x0B])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("12. Bullets:       ");
	if (data[0x0C])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("13. Asteroids:     ");
	if (data[0x0D])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("14. Bonuses:       ");
	if (data[0x0E])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("15. Map:           ");
	if (data[0x0F])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("16. Particles:     ");
	if (data[0x10])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("17. Dynam part-es: ");
	if (data[0x11])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("18. Logs:          ");
	if (data[0x12])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}

	th_00.join();
	th_01.join();
	th_02.join();
	th_03.join();
	th_04.join();
	th_05.join();
	th_06.join();
	th_07.join();
	th_08.join();
	th_09.join();
	th_0A.join();
	th_0B.join();
	th_0C.join();
	th_0D.join();
	th_0E.join();
	th_0F.join();
	th_10.join();
	th_11.join();
	th_12.join();
}