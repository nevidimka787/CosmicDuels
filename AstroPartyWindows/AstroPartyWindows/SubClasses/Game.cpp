#include "Game.h"
#include <math.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 6001)
#pragma warning(disable : 6011)
#pragma warning(disable : 6200)
#pragma warning(disable : 6294)
#pragma warning(disable : 6297)
#pragma warning(disable : 6385)
#pragma warning(disable : 6386)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 26495)

#define M_PI	3.14159265358979323846	// pi
#define M_PI_2	1.5707796326794897		// pi/2
#define M_PI_4  0.785398163397448309616	// pi/4

void Game::PhysicThread0()
{
	global_timer++;

	BombsChainReaction();
	BulletsDestroyAsteroids();
	KnifesDestroyAsteroids();
	LasersDestroyBonuses();
	MegaLasersDestroyAsteroids();
	MegaLasersDestroyBonuses();
	PilotsKilledByBombs();
	ShipsInfluenceToBonuses();
	ShipsRespawnOrDestroyPilots();
	ShipsDestroedByBombsOrActivateBombs();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_0;
	threads_statuses_mtx.unlock();

	WaitPhase1();

	UpdateGravGensPhase2();
	UpdateDecelerAreasPhase2();
	UpdateMapPhase2();
	UpdateMegaLasersPhase2();
	UpdateTurelsPhase2();

	camera_data_mtx.lock();
	if (camera.move_velocity_coefficient < 1000.0f)
	{
		camera.move_velocity_coefficient *= 1.01f;
		camera.resize_velocity_coefficient *= 1.01f;
	}
	camera_data_mtx.unlock();
	input_values_mtx.lock();
	if (burnout_double_clk_timer[0] > 0)
		burnout_double_clk_timer[0]--;
	else if (burnout_double_clk_timer[0] < 0)
		burnout_double_clk_timer[0]++;
	if (burnout_double_clk_timer[1] > 0)
		burnout_double_clk_timer[1]--;
	else if (burnout_double_clk_timer[1] < 0)
		burnout_double_clk_timer[1]++;
	if (burnout_double_clk_timer[2] > 0)
		burnout_double_clk_timer[2]--;
	else if (burnout_double_clk_timer[2] < 0)
		burnout_double_clk_timer[2]++;
	if (burnout_double_clk_timer[3] > 0)
		burnout_double_clk_timer[3]--;
	else if (burnout_double_clk_timer[3] < 0)
		burnout_double_clk_timer[3]++;
	input_values_mtx.unlock();

	PollEvents();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_0;
	threads_statuses_mtx.unlock();

	WaitPhaseAllPhases();

	thread_1_update.lock();
	thread_2_update.lock();
	thread_3_update.lock();

	CheckEndMatch();
	if (flag_end_match)
	{
		play_round = false;
	}

	threads_statuses_mtx.lock();
	threads_statuses &= THREAD_MASK_NULL;
	threads_statuses_mtx.unlock();

	thread_1_update.unlock();
	thread_2_update.unlock();
	thread_3_update.unlock();
}

void Game::PhysicThread1()
{
	PilotsKilledByBullet();
	ShipsCheckInput();
	ShipsShoot();

	BombsDestroyAsteroids();
	BombsDestroyBonuses();
	BulletsDestroedByMap();
	if (game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS)
	{
		KnifesDestroyBullets();
	}
	LasersDetonateBombs();
	LasersDestroyKnifes();
	MegaLasersDestroyBullets();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_1;
	threads_statuses_mtx.unlock();

	WaitPhase1();

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

	UpdateBulletsPhase2();
	UpdateBonusesPhase2();
	UpdatePilotsPhase2();

	thread_1_update.lock();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_1;
	threads_statuses_mtx.unlock();

	WaitPhaseAllPhases();

	thread_1_update.unlock();

	WaitPhaseNotAll();
}

void Game::PhysicThread2()
{
	MegaLasersDetonateBombs();
	//MegaLasersDestroyMap(); -- not realisated
	CameraFocusesOnPlayers();
	PilotsCheckInput();
	PilotsKilledByKnifes();
	PilotsRespawnAuto();
	ShipsDestroedByBullets();
	ShipsDestroedByKnifes();
	TurelsShoot();

	BombsCollisionsWithBullets();
	BombsSpawnedByBulletsAnigilation();
	KnifesDestroyMap();
	LasersDestroyAsteroids();
	LasersDestroyBullets();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_2;
	threads_statuses_mtx.unlock();

	WaitPhase1();

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

	//UpdateParticlesPhase2();
	UpdateShipsPhase2();
	UpdateKnifesPhase2();
	UpdateLasersPhase2();

	thread_2_update.lock();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_2;
	threads_statuses_mtx.unlock();

	WaitPhaseAllPhases();

	thread_2_update.unlock();

	WaitPhaseNotAll();
}

void Game::PhysicThread3()
{
	KnifesDestroyTurels();
	LasersDestroyMap();
	LasersDestroyTurels();
	MegaLasersDestroyKnifes();
	MegaLasersDestroyTurels();
	PilotsKilledByLasers();
	PilotsKilledByMegaLaser();
	ShipsDestroedByLasers();
	ShipsDestroedByMegaLasers();

	BombsDestroyKnifes();
	BombsDestroyTurels();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_1 << THREAD_3;
	threads_statuses_mtx.unlock();

	WaitPhase1();

	pilots_array_mtx.lock();
	map_data_mtx.lock();
	DynamicEntitiesCollisions(&map, pilots, pilots_count);
	map_data_mtx.unlock();
	pilots_array_mtx.unlock();
	ships_array_mtx.lock();
	map_data_mtx.lock();
	DynamicEntitiesCollisions(&map, ships, ships_count);
	map_data_mtx.unlock();
	ships_array_mtx.unlock();
	asteroids_array_mtx.lock();
	map_data_mtx.lock();
	DynamicEntitiesCollisions(&map, asteroids, asteroids_count);
	map_data_mtx.unlock();
	asteroids_array_mtx.unlock();
	bombs_array_mtx.lock();
	map_data_mtx.lock();
	DynamicEntitiesCollisions(&map, bombs, bombs_count);
	map_data_mtx.unlock();
	bombs_array_mtx.unlock();

	UpdateBombsPhase2();
	UpdateAsteroidsPhase2();

	thread_3_update.lock();

	threads_statuses_mtx.lock();
	threads_statuses |= THREAD_COMPLETE << THREAD_PHASE_2 << THREAD_3;
	threads_statuses_mtx.unlock();

	WaitPhaseAllPhases();

	thread_3_update.unlock();

	WaitPhaseNotAll();
}
template<typename EntityType>
void Game::DynamicEntitiesCollisions(EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities_count; first++)
	{
		if (entities[first].exist)
		{
			for (GameTypes::entities_count_t second = first + 1, found_entites_count2 = found_entites_count1 + 1; found_entites_count2 < entities_count; second++)
			{
				if (entities[second].exist)
				{
					entities[first].Collision(&entities[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}

template<typename Entity1Type, typename Entity2Type>
void Game::DynamicEntitiesCollisions(Entity1Type* entities1, Entity2Type* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities1_count; first++)
	{
		if (entities1[first].exist)
		{
			for (GameTypes::entities_count_t second = 0, found_entites_count2 = 0; found_entites_count2 < entities2_count; second++)
			{
				if (entities2[second].exist)
				{
					entities1[first].Collision(&entities2[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesCollisions(Map* map, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			entities[i].Collision(map);
			found++;
		}
	}
}

void Game::DynamicEntitiesCollisions(Map* map, Bomb* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			if (!entities[i].IsBoom())
			{
				entities[i].Collision(map);
			}
			found++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(Vec2F* force, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			entities[i].AddForce(force);
			found++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	float len2;
	for (GameTypes::map_elements_count_t grav_gen = 0; grav_gen < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					force = entities[i].GetPosition() - grav_gens[grav_gen].GetPosition();
					len2 = force.x * force.x + force.y * force.y;
					force /= len2 * sqrt(len2) / grav_gens[grav_gen].gravity;
					entities[i].AddForce(&force);

					found++;
				}
			}
		}
	}
}

void Game::DynamicEntitiesAddForce(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bomb* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	float len2;
	for (GameTypes::map_elements_count_t grav_gen = 0, found_grav_gens = 0; found_grav_gens < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					if (!entities[i].IsBoom())
					{
						force = entities[i].GetPosition() - grav_gens[grav_gen].GetPosition();
						len2 = force.x * force.x + force.y * force.y;
						force /= len2 * sqrt(len2) / grav_gens[grav_gen].gravity;
						entities[i].AddForce(&force);
					}
					found++;
				}
			}
			found_grav_gens++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(DecelerationArea* deceler_areas, GameTypes::map_elements_count_t deceler_areas_count, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	for (GameTypes::map_elements_count_t deceler_area = 0, found_deceler_areas = 0; found_deceler_areas < deceler_areas_count; deceler_area++)
	{
		if (deceler_areas[deceler_area].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					if (deceler_areas[deceler_area].IsCollision(&entities[i]))
					{
						force = -entities[i].GetVelocity() * deceler_areas[deceler_area].deceleration_parameter;
						entities[i].AddForce(force);
					}
					found++;
				}
			}
			found_deceler_areas++;
		}
	}
}

EngineTypes::Bonus::inventory_t Game::GenerateRandomBonus()
{
	unsigned random = rand() % GAME_BONUSES_COUNT + 1;
	GameTypes::objects_types_count_t bonus = 0;
	GameTypes::objects_types_count_t select_bonuses_count = 0;
	for (; random > 0; bonus++)
	{
		if (bonus == GAME_BONUSES_COUNT)
		{
			bonus = 0;
		}
		if (bonus_pull_array[bonus])
		{
			random--;
		}
	}
	return 1 << ((bonus - 1) * 2);
}


EngineTypes::Bonus::inventory_t Game::GenerateRandomBonusAndRule()
{
	unsigned random = rand() % GAME_BONUSES_COUNT + 2;
	if (random == GAME_BONUSES_COUNT + 1)
	{
		return BONUS_RULE_REVERSE;
	}
	GameTypes::objects_types_count_t bonus = 0;
	GameTypes::objects_types_count_t select_bonuses_count = 0;
	for (; random > 0; bonus++)
	{
		if (bonus == GAME_BONUSES_COUNT)
		{
			bonus = 0;
		}
		if (bonus_pull_array[bonus])
		{
			random--;
		}
	}
	return 1 << ((bonus - 1) * 2);
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
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		teams[i] = 0;
	}
	object_pull_array = new bool[GAME_OBJECT_TYPES_COUNT];
	map_pull_array = new bool[GAME_MAPS_COUNT];
	bonus_pull_array = new bool[GAME_BONUSES_COUNT];
	for (GameTypes::objects_types_count_t i = 0; i < GAME_OBJECT_TYPES_COUNT; i++)
	{
		object_pull_array[i] = true;
	}
	for (GameTypes::objects_types_count_t i = 0; i < GAME_MAPS_COUNT; i++)
	{
		map_pull_array[i] = true;
	}
	for (GameTypes::objects_types_count_t i = 0; i < GAME_BONUSES_COUNT; i++)
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

	for (GameTypes::entities_types_count_t bonus = 0; bonus < GAME_BONUSES_COUNT; bonus++)
	{
		if (bonus_pull_array[bonus])
		{
			goto skip_bonus_pull_set;//skip second cycle
		}
	}
	for (GameTypes::entities_types_count_t bonus = 0; bonus < GAME_BONUSES_COUNT; bonus++)
	{
		bonus_pull_array[bonus] = true;
	}
skip_bonus_pull_set:

	//Set bonus pull

	MemoryLock();
	
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		scores[player] = 0;
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

	//create buttons
	Vec2F* points = new Vec2F[3];
	points[0].Set(0.0f, 0.0f);
	points[1].Set(1.0f, 0.0f);
	points[2].Set(0.5f, 0.5f);
	Area area;
	area.Set(points, 3);
	Vec2F* positions = new Vec2F[GAME_PLAYERS_MAX_COUNT * 2];
	positions[0].Set(-1.0f, 0.9f);
	positions[1].Set(-0.9f, 1.0f);
	positions[2].Set(0.9f, 1.0f);
	positions[3].Set(1.0f, 0.9f);
	positions[4].Set(1.0f, -0.9f);
	positions[5].Set(0.9f, -1.0f);
	positions[6].Set(-0.9f, -1.0f);
	positions[7].Set(-1.0f, -0.9f);
	Vec2F size1, size2;
	EngineTypes::Menu::buttons_count_t buttons_count = 0;
	size1.Set(0.095f, -0.095f);
	size2.Set(-0.095f, 0.095f);
	Button* buttons = new Button[GAME_PLAYERS_MAX_COUNT * 2];
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (teams[player] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			buttons[buttons_count].Set(BUTTON_ID__SHIP1_SHOOT + 2 * player, &positions[player * 2], &size1, &area, "", 0, BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1));
			buttons[buttons_count + 1].Set(BUTTON_ID__SHIP1_ROTATE + 2 * player, &positions[player * 2 + 1], &size2, &area, "", 0, BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1));
			buttons_count += 2;
		}
	}
	positions[0].Set(0.0f, 0.0f);
	size1.Set(1.0f, -1.0f);
	ships_control_menu.Set(&positions[0], &size1, buttons, buttons_count);
	delete[] positions;
	delete[] buttons;

	//create map pull array

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
	}
}

void Game::Event0()
{
	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 500) && asteroids_count == 0)
	{
		asteroids_array_mtx.unlock();
		Vec2F positions[1];
#define EVENT0__CENTER_POSITION	6.0f
		positions[0].Set(EVENT0__CENTER_POSITION, EVENT0__CENTER_POSITION);
		Vec2F zero_velocity;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(&positions[0], &zero_velocity, BONUS_RULE_REVERSE, ASTEROID_SIZE_SMALL));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event1()
{
	turels_array_mtx.lock();
	if (turels[0].exist)
	{
		turels[0].Rotate(0.01f);
	}
	turels_array_mtx.unlock();
	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count == 0)
	{
		asteroids_array_mtx.unlock();
		Vec2F positions[4];
#define EVENT1_SQUARE_SIZE 0.7f
		positions[0].Set(-EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE);
		positions[1].Set(-EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE);
		positions[2].Set(EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE);
		positions[3].Set(EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE);
		Vec2F zero_velocity;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(&positions[0], &zero_velocity, GenerateRandomBonus() + GenerateRandomBonus(), ASTEROID_DEFAULT_SIZE));
		AddEntity(Asteroid(&positions[1], &zero_velocity, GenerateRandomBonus() + GenerateRandomBonus(), ASTEROID_DEFAULT_SIZE));
		AddEntity(Asteroid(&positions[2], &zero_velocity, GenerateRandomBonus() + GenerateRandomBonus(), ASTEROID_DEFAULT_SIZE));
		AddEntity(Asteroid(&positions[3], &zero_velocity, GenerateRandomBonus() + GenerateRandomBonus(), ASTEROID_DEFAULT_SIZE));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event2()
{
	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count == 0)
	{
		asteroids_array_mtx.unlock();
		Vec2F positions[4];
#define EVENT2_SQUARE_SIZE 1.5f
		positions[0].Set(-EVENT2_SQUARE_SIZE, 0.0f);
		positions[1].Set(EVENT2_SQUARE_SIZE, 0.0f);
		positions[2].Set(0.0f, -EVENT2_SQUARE_SIZE);
		positions[3].Set(0.0f, EVENT2_SQUARE_SIZE);
		Vec2F zero_velocity;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(&positions[0], &zero_velocity, GenerateRandomBonus(), ASTEROID_MAX_SIZE));
		AddEntity(Asteroid(&positions[1], &zero_velocity, GenerateRandomBonus(), ASTEROID_MAX_SIZE));
		AddEntity(Asteroid(&positions[2], &zero_velocity, GenerateRandomBonus(), ASTEROID_MAX_SIZE));
		AddEntity(Asteroid(&positions[3], &zero_velocity, GenerateRandomBonus(), ASTEROID_MAX_SIZE));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event3()
{
	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count < 2)
	{
		asteroids_array_mtx.unlock();
		Vec2F asteroid_velocityes[4];
		Vec2F asteroid_position;
#define EVENT3_ASTEROID_VELOCIRY		0.015f
#define EVENT3_ASTEROID_SPAWN_RADIUS	0.2f
		asteroid_velocityes[0].Set(EVENT3_ASTEROID_VELOCIRY, EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[1].Set(EVENT3_ASTEROID_VELOCIRY, -EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[2].Set(-EVENT3_ASTEROID_VELOCIRY, -EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[3].Set(-EVENT3_ASTEROID_VELOCIRY, EVENT3_ASTEROID_VELOCIRY);
		uint8_t dir = rand() % 4;
		asteroid_position = -asteroid_velocityes[dir].Normalize() * EVENT3_ASTEROID_SPAWN_RADIUS;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(&asteroid_position, &asteroid_velocityes[dir], GenerateRandomBonusAndRule(), ASTEROID_SIZE_MEDIUM));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event4()
{
#define EVENT4__CENTER_POSITION	6.0f

	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count < 2)
	{
		asteroids_array_mtx.unlock();
		Vec2F asteroid_position;
		asteroid_position.Set(EVENT4__CENTER_POSITION, EVENT4__CENTER_POSITION);
		Vec2F zero_velocity;
		asteroids_array_mtx.lock();
		AddEntity(
			Asteroid(
				&asteroid_position,
				&zero_velocity,
				GenerateRandomBonus() + GenerateRandomBonus() + GenerateRandomBonus() + GenerateRandomBonusAndRule(),
				ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event5()
{
#define EVENT5__CENTER_POSITION	6.0f
#define EVENT5__SQUARE_SIZE		1.0f
#define EVENT5__UP_RECTANGLE	1u
#define EVENT5__RIGHT_RECTANGLE	2u
#define EVENT5__DONW_RECTANGLE	3u
#define EVENT5__LEFT_RECTANGLE	4u

#define EVENT5__RECTANGLE_SPEED	(0.005f * EVENT5__SQUARE_SIZE)

#define EVENT5__PERIOD	1000lu
#define EVENT5_PHASE	200lu

	map_data_mtx.lock();

	if (rotation_inverse)
	{
		Rectangle* rectangle_p = map.GetRectanglePointer(EVENT5__UP_RECTANGLE);
		rectangle_p->Move(Vec2F(-EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.GetRectanglePointer(EVENT5__RIGHT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, EVENT5__RECTANGLE_SPEED));
		rectangle_p = map.GetRectanglePointer(EVENT5__DONW_RECTANGLE);
		rectangle_p->Move(Vec2F(EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.GetRectanglePointer(EVENT5__LEFT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, -EVENT5__RECTANGLE_SPEED));

		if (rectangle_p->Position().y < EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE)
		{
			//left rctangle
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));

			rectangle_p = map.GetRectanglePointer(EVENT5__UP_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.GetRectanglePointer(EVENT5__RIGHT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
			rectangle_p = map.GetRectanglePointer(EVENT5__DONW_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
		}
	}
	else
	{
		Rectangle* rectangle_p = map.GetRectanglePointer(EVENT5__UP_RECTANGLE);
		rectangle_p->Move(Vec2F(EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.GetRectanglePointer(EVENT5__RIGHT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, -EVENT5__RECTANGLE_SPEED));
		rectangle_p = map.GetRectanglePointer(EVENT5__DONW_RECTANGLE);
		rectangle_p->Move(Vec2F(-EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.GetRectanglePointer(EVENT5__LEFT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, EVENT5__RECTANGLE_SPEED));

		if (rectangle_p->Position().y > EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE)
		{
			//left rctangle
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));

			rectangle_p = map.GetRectanglePointer(EVENT5__UP_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.GetRectanglePointer(EVENT5__RIGHT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.GetRectanglePointer(EVENT5__DONW_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
		}
	}

	map_data_mtx.unlock();

	if (!(global_timer % EVENT5__PERIOD))
	{
		Vec2F position = Vec2F(EVENT5__CENTER_POSITION, EVENT5__CENTER_POSITION);
		Vec2F zero_velosity;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(&position, &zero_velosity, GenerateRandomBonusAndRule()));
		asteroids_array_mtx.unlock();
	}
}

void Game::InitLevel()
{
	MutexesLock();
	threads_statuses = THREAD_MASK_NULL;
	srand(time(0));
	play_round = true;
	flag_round_results = false;

	logs.Clear();

	MemorySetDefault();

	current_map_id = GenerateRandomMapId();

	Vec2F ships_positions[GAME_PLAYERS_MAX_COUNT];
	float ships_angles[GAME_PLAYERS_MAX_COUNT];

	const GameTypes::score_t max_score = GetMaxScore();
	
	current_event = current_map_id;

	/* Create maps */
	
	switch (current_map_id)
	{
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
	case MAP_TEST_MAP:
	default:
		CreateMap0(ships_positions, ships_angles);
		break;
	}

	/* Spawn players */

	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		ships_can_shoot_flags[player] = SHIP_DEFAULT_UNBRAKABLE_PERIOD;
		players_in_team[player] = 0;
		burnout_double_clk_timer[player] = 0;
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
		start_bonus |= (game_rules & GAME_RULE_TRIPLE_BONUSES) ? BONUS_BUFF_TRIPLE * 3 : BONUS_BUFF_TRIPLE;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD)
	{
		start_bonus |= (game_rules & GAME_RULE_TRIPLE_BONUSES) ? BONUS_BUFF_SHIELD * 3 : BONUS_BUFF_SHIELD;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS && !(game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS))
	{
		start_bonus |= GenerateRandomBonus() * ((game_rules & GAME_RULE_TRIPLE_BONUSES) ? 3 : 1);
	}

	//Set start bonus

	Vec2F zero_velocity;
	players_count = 0;
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (teams[player] > 0)
		{
			if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS && game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS)
			{
				start_bonus &= BONUS_ALL - BONUS_BONUS;
				start_bonus |= GenerateRandomBonus();
			}
			ships[player].Set(
				&ships_positions[player], &zero_velocity,
				player, teams[player],
				(void*)&rotate_flags[player], (void*)&burnout_flags[player], (void*)&shoot_flags[player],
				nullptr, 0,
				ships_angles[player], start_bonus);

			players_count++;
			IncrementPlayersCountInTeam(teams[player]);
		}
	}
	
	ships_count = players_count;

	MutexesUnlock();

	threads_statuses_mtx.lock();
	threads_statuses = THREAD_NEW_CYCLE;
	threads_statuses_mtx.unlock();
}

void Game::CreateMap0(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_TEST_MAP__CENTER_POSITION	EVENT0__CENTER_POSITION
#define MAP_TEST_MAP__FRAME_SIZE		2.0f
#define MAP_TEST_MAP__RECTANGLES_COUNT	1
#define MAP_TEST_MAP__CAMERA_SIZE		10.0f

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_TEST_MAP__RECTANGLES_COUNT];

	new_segment.Set(
		Vec2F(MAP_TEST_MAP__CENTER_POSITION - MAP_TEST_MAP__FRAME_SIZE, MAP_TEST_MAP__CENTER_POSITION - MAP_TEST_MAP__FRAME_SIZE),
		Vec2F(MAP_TEST_MAP__CENTER_POSITION + MAP_TEST_MAP__FRAME_SIZE, MAP_TEST_MAP__CENTER_POSITION + MAP_TEST_MAP__FRAME_SIZE),
		true);
	rectangles[0].Set(&new_segment);

	map.Set(rectangles, MAP_TEST_MAP__RECTANGLES_COUNT);


	/* Spawn entities */

	ships_positions[0].Set(MAP_TEST_MAP__CENTER_POSITION - 1.85f, MAP_TEST_MAP__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_TEST_MAP__CENTER_POSITION + 1.9f, MAP_TEST_MAP__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_TEST_MAP__CENTER_POSITION + 1.85f, MAP_TEST_MAP__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_TEST_MAP__CENTER_POSITION - 1.9f, MAP_TEST_MAP__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	camera.SetHightLimits(
		MAP_TEST_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_TEST_MAP__CENTER_POSITION, MAP_TEST_MAP__CENTER_POSITION));
	camera.SetSize(MAP_TEST_MAP__CAMERA_SIZE);

	delete[] rectangles;
}

void Game::CreateMap1(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_TUREL_ON_CENTER__CENTER_POSITION		0.0f
#define MAP_TUREL_ON_CENTER__FRAME_SIZE				2.0f
#define MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE		0.1f
#define MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION	1.5f
#define MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION	1.0f
#define MAP_TUREL_ON_CENTER__CENTER					0.0f
#define MAP_TUREL_ON_CENTER__RECTANGLE_COUNT		6u

#define MAP_TUREL_ON_CENTER__FRAME_RECTANGLE		0u
#define MAP_TUREL_ON_CENTER__CENTER_RECTANGLE		1u
#define MAP_TUREL_ON_CENTER__DOWN_LEFT_RECTANGLE	2u
#define MAP_TUREL_ON_CENTER__UP_RIGHT_RECTANGLE		3u
#define MAP_TUREL_ON_CENTER__UP_LEFT_RECTANGLE		4u
#define MAP_TUREL_ON_CENTER__DOWN_RIGHT_RECTANGLE	5u

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_TUREL_ON_CENTER__RECTANGLE_COUNT];

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__FRAME_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__FRAME_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__FRAME_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__CENTER_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__DOWN_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__UP_RIGHT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__UP_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__DOWN_RIGHT_RECTANGLE].Set(&new_segment);

	map.Set(rectangles, MAP_TUREL_ON_CENTER__RECTANGLE_COUNT);

	new_position.Set(MAP_TUREL_ON_CENTER__CENTER, MAP_TUREL_ON_CENTER__CENTER);
	AddEntity(Turel(&new_position));


	/* Spawn entities */

	ships_positions[0].Set(-1.85f, 1.9f);
	ships_positions[1].Set(1.9f, 1.85f);
	ships_positions[2].Set(1.85f, -1.9f);
	ships_positions[3].Set(-1.9f, -1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	delete[] rectangles;
}

void Game::CreateMap2(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_CYRCLE_ON_CENTER__RECTANGLES_COUNT	5
#define MAP_CYRCLE_ON_CENTER__CYRCLES_COUNT		1

#define MAP_CYRCLE_ON_CENTER__FRAME_SIZE		2.0f
#define MAP_CYRCLE_ON_CENTER__SQUARE_SIZE		0.5f
#define MAP_CYRCLE_ON_CENTER__SQUARE_POSITION	1.5f
#define MAP_CYRCLE_ON_CENTER__CYRCLE_SIZE		(0.1f / 0.8f)
#define MAP_CYRCLE_ON_CENTER__CENTER			0.0f

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_CYRCLE_ON_CENTER__RECTANGLES_COUNT];
	Cyrcle* cyrcles = new Cyrcle[MAP_CYRCLE_ON_CENTER__CYRCLES_COUNT];

	new_segment.Set(
		Vec2F(-MAP_CYRCLE_ON_CENTER__FRAME_SIZE, -MAP_CYRCLE_ON_CENTER__FRAME_SIZE),
		Vec2F(MAP_CYRCLE_ON_CENTER__FRAME_SIZE, MAP_CYRCLE_ON_CENTER__FRAME_SIZE),
		true);
	rectangles[0].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_CYRCLE_ON_CENTER__SQUARE_POSITION, -MAP_CYRCLE_ON_CENTER__SQUARE_POSITION),
		Vec2F(MAP_CYRCLE_ON_CENTER__SQUARE_SIZE, MAP_CYRCLE_ON_CENTER__SQUARE_SIZE));
	rectangles[1].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_CYRCLE_ON_CENTER__SQUARE_POSITION, MAP_CYRCLE_ON_CENTER__SQUARE_POSITION),
		Vec2F(-MAP_CYRCLE_ON_CENTER__SQUARE_SIZE, -MAP_CYRCLE_ON_CENTER__SQUARE_SIZE));
	rectangles[2].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_CYRCLE_ON_CENTER__SQUARE_POSITION, MAP_CYRCLE_ON_CENTER__SQUARE_POSITION),
		Vec2F(MAP_CYRCLE_ON_CENTER__SQUARE_SIZE, -MAP_CYRCLE_ON_CENTER__SQUARE_SIZE));
	rectangles[3].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_CYRCLE_ON_CENTER__SQUARE_POSITION, -MAP_CYRCLE_ON_CENTER__SQUARE_POSITION),
		Vec2F(-MAP_CYRCLE_ON_CENTER__SQUARE_SIZE, MAP_CYRCLE_ON_CENTER__SQUARE_SIZE));
	rectangles[4].Set(&new_segment);


	new_position.Set(MAP_CYRCLE_ON_CENTER__CENTER, MAP_CYRCLE_ON_CENTER__CENTER);
	cyrcles[0].Set(&new_position, MAP_CYRCLE_ON_CENTER__CYRCLE_SIZE);

	map.Set(rectangles, MAP_CYRCLE_ON_CENTER__RECTANGLES_COUNT, cyrcles, MAP_CYRCLE_ON_CENTER__CYRCLES_COUNT);

	new_position.Set(MAP_CYRCLE_ON_CENTER__CENTER, MAP_CYRCLE_ON_CENTER__CENTER);
	AddEntity(GravGen(&new_position));

	/* Spawn entities */

	ships_positions[0].Set(-1.85f, 1.9f);
	ships_positions[1].Set(1.9f, 1.85f);
	ships_positions[2].Set(1.85f, -1.9f);
	ships_positions[3].Set(-1.9f, -1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	delete[] rectangles;
	delete[] cyrcles;
}

void Game::CreateMap3(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_DECELERATION_AREA__FRAME_SIZE				2.0f
#define MAP_DECELERATION_AREA__IN_SEGMENT_HIGH			0.75f
#define MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH			1.25f
#define MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH		((MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH - MAP_DECELERATION_AREA__IN_SEGMENT_HIGH) / 2.0f)
#define MAP_DECELERATION_AREA__DECELERATION_AREA_POS	0.5f
#define MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS	0.25f

#define MAP_DECELERATION_AREA__FRAME_RECTANGEL_ID	0
#define MAP_DECELERATION_AREA__DOWN_RECTANGEL_ID	1
#define MAP_DECELERATION_AREA__RIGHT_RECTANGEL_ID	2
#define MAP_DECELERATION_AREA__UP_RECTANGEL_ID		3
#define MAP_DECELERATION_AREA__LEFT_RECTANGEL_ID	4
#define MAP_DECELERATION_AREA__RECTANGLES_COUNT		5

	Rectangle* rectangles = new Rectangle[MAP_DECELERATION_AREA__RECTANGLES_COUNT];

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__FRAME_SIZE, -MAP_DECELERATION_AREA__FRAME_SIZE),
		Vec2F(MAP_DECELERATION_AREA__FRAME_SIZE, MAP_DECELERATION_AREA__FRAME_SIZE),
		true);
	rectangles[MAP_DECELERATION_AREA__FRAME_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, -MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH),
		Vec2F(MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, -MAP_DECELERATION_AREA__IN_SEGMENT_HIGH),
		true);
	rectangles[MAP_DECELERATION_AREA__DOWN_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH, -MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		Vec2F(MAP_DECELERATION_AREA__IN_SEGMENT_HIGH, MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		true);
	rectangles[MAP_DECELERATION_AREA__RIGHT_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH),
		Vec2F(-MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, MAP_DECELERATION_AREA__IN_SEGMENT_HIGH),
		true);
	rectangles[MAP_DECELERATION_AREA__UP_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH, MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		Vec2F(-MAP_DECELERATION_AREA__IN_SEGMENT_HIGH, -MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		true);
	rectangles[MAP_DECELERATION_AREA__LEFT_RECTANGEL_ID].Set(&new_segment);

	map.Set(rectangles, MAP_DECELERATION_AREA__RECTANGLES_COUNT);

	/* Spawning entities */

	new_position.Set(MAP_DECELERATION_AREA__DECELERATION_AREA_POS, MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	ships_positions[0].Set(-1.85f, 1.9f);
	ships_positions[1].Set(1.9f, 1.85f);
	ships_positions[2].Set(1.85f, -1.9f);
	ships_positions[3].Set(-1.9f, -1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	delete[] rectangles;
}

void Game::CreateMap4(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_MEGA_LASERS__RECTANGLES_COUNT	5u
#define MAP_MEGA_LASERS__FRAME_RECTANGLE	0u
#define MAP_MEGA_LASERS__UP_RECTANGLE		1u
#define MAP_MEGA_LASERS__RIGHT_RECTANGLE	2u
#define MAP_MEGA_LASERS__DOWN_RECTANGLE		3u
#define MAP_MEGA_LASERS__LEFT_RECTANGLE		4u

#define MAP_MEGA_LASERS__FRAME_SIZE						2.0f
#define MAP_MEGA_LASERS__CENTER_POSITION				EVENT4__CENTER_POSITION
#define MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X	1.5f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y	1.5f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X	0.0f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y	1.3f

#define MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X	0.0f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y	1.4f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X	-1.3f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y	1.4f

#define MAP_MEGA_LASERS__CAMERA_SIZE	10.0f

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_MEGA_LASERS__RECTANGLES_COUNT];

	Segment diagonal = Segment(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE),
		true);
	rectangles[MAP_MEGA_LASERS__FRAME_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y),
		true);
	rectangles[MAP_MEGA_LASERS__UP_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X),
		true);
	rectangles[MAP_MEGA_LASERS__RIGHT_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y),
		true);
	rectangles[MAP_MEGA_LASERS__DOWN_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X),
		true);
	rectangles[MAP_MEGA_LASERS__LEFT_RECTANGLE].Set(&diagonal);

	map.Set(rectangles, MAP_MEGA_LASERS__RECTANGLES_COUNT);

	/* Spawn entities */

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y),
		true);
	AddEntity(MegaLaser(&diagonal, 0, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X),
		true);
	AddEntity(MegaLaser(&diagonal, 500, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y),
		true);
	AddEntity(MegaLaser(&diagonal, 0, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X),
		true);
	AddEntity(MegaLaser(&diagonal, 500, 1000, 500));

	ships_positions[0].Set(MAP_MEGA_LASERS__CENTER_POSITION - 1.85f, MAP_MEGA_LASERS__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_MEGA_LASERS__CENTER_POSITION + 1.9f, MAP_MEGA_LASERS__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_MEGA_LASERS__CENTER_POSITION + 1.85f, MAP_MEGA_LASERS__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_MEGA_LASERS__CENTER_POSITION - 1.9f, MAP_MEGA_LASERS__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	camera.SetHightLimits(
		MAP_MEGA_LASERS__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_MEGA_LASERS__CENTER_POSITION, MAP_MEGA_LASERS__CENTER_POSITION));
	camera.SetSize(MAP_MEGA_LASERS__CAMERA_SIZE);
}

void Game::CreateMap5(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_DYNAMICAL__RECTANGLES_COUNT	5u
#define MAP_DYNAMICAL__FRAME_RECTANGLE	0u
#define MAP_DYNAMICAL__UP_RECTANGLE		EVENT5__UP_RECTANGLE
#define MAP_DYNAMICAL__RIGHT_RECTANGLE	EVENT5__RIGHT_RECTANGLE
#define MAP_DYNAMICAL__DOWN_RECTANGLE	EVENT5__DONW_RECTANGLE
#define MAP_DYNAMICAL__LEFT_RECTANGLE	EVENT5__LEFT_RECTANGLE

#define MAP_DYNAMICAL__FRAME_SIZE		2.0f
#define MAP_DYNAMICAL__CENTER_POSITION	EVENT5__CENTER_POSITION

#define MAP_DYNAMICAL_RECTANGLE_POSITION	EVENT5__SQUARE_SIZE
#define MAP_DYNAMICAL_RECTANGLE_SIZE		0.25f

#define MAP_DYNAMICAL__CAMERA_SIZE	10.0f

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_DYNAMICAL__RECTANGLES_COUNT];

	Segment diagonal = Segment(
		Vec2F(
			MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE, 
			MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE),
		Vec2F(
			MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE, 
			MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__FRAME_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION, 
			MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION, 
			MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION, 
			-MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION, 
			-MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].NormaliseThis();

	map.Set(rectangles, MAP_DYNAMICAL__RECTANGLES_COUNT);

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.85f, MAP_DYNAMICAL__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMap6(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_DESTRUCTIBLE__CENTER_POSTION	6.0f
#define MAP_DESTRUCTIBLE__FRAME_SIZE		2.0f

#define MAP_DESTRUCTIBLE__RECTANGLES_SIZE			(MAP_DESTRUCTIBLE__FRAME_SIZE / 5.0f)
#define MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X	(1.0f / 2.5f * MAP_DESTRUCTIBLE__FRAME_SIZE)
#define MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y	(MAP_DESTRUCTIBLE__FRAME_SIZE - MAP_DESTRUCTIBLE__RECTANGLES_SIZE)
#define MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION		MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X

#define MAP_DESTRUCTIBLE__RECTANGLES_COUNT			17u
#define MAP_DESTRUCTIBLE__FRAME_RECTANGLE			0u
#define MAP_DESTRUCTABLE__OUT_UP_RIGHT_RECTANGLE	1u
#define MAP_DESTRUCTABLE__OUT_RIGHT_UP_RECTANGLE	2u
#define MAP_DESTRUCTABLE__OUT_RIGHT_DOWN_RECTANGLE	3u
#define MAP_DESTRUCTABLE__OUT_DOWN_RIGHT_RECTANGLE	4u
#define MAP_DESTRUCTABLE__OUT_DOWN_LEFT_RECTANGLE	5u
#define MAP_DESTRUCTABLE__OUT_LEFT_DOWN_RECTANGLE	6u
#define MAP_DESTRUCTABLE__OUT_LEFT_UP_RECTANGLE		7u
#define MAP_DESTRUCTABLE__OUT_UP_LEFT_RECTANGLE		8u
#define MAP_DESTRUCTABLE__UP_RIGHT_RECTANGLE		9u
#define MAP_DESTRUCTABLE__DOWN_RIGHT_RECTANGLE		10u
#define MAP_DESTRUCTABLE__DOWN_LEFT_RECTANGLE		11u
#define MAP_DESTRUCTABLE__UP_LEFT_RECTANGLE			12u
#define MAP_DESTRUCTABLE__UP_RECTANGLE				13u
#define MAP_DESTRUCTABLE__RIGHT_RECTANGLE			14u
#define MAP_DESTRUCTABLE__DOWN_RECTANGLE			15u
#define MAP_DESTRUCTABLE__LEFT_RECTANGLE			16u

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_DESTRUCTIBLE__RECTANGLES_COUNT];
	Segment diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTIBLE__FRAME_RECTANGLE].Set(&diagonal, true);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_UP_RIGHT_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_RIGHT_UP_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_RIGHT_DOWN_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_DOWN_RIGHT_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_DOWN_LEFT_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_LEFT_DOWN_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_LEFT_UP_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_UP_LEFT_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_RIGHT_RECTANGLE].Set(&diagonal, true);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_RIGHT_RECTANGLE].Set(&diagonal, true);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_LEFT_RECTANGLE].Set(&diagonal, true);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_LEFT_RECTANGLE].Set(&diagonal, true);
	
	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__RIGHT_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_RECTANGLE].Set(&diagonal, false);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__LEFT_RECTANGLE].Set(&diagonal, false);

	map.Set(rectangles, MAP_DESTRUCTIBLE__RECTANGLES_COUNT);

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.85f, MAP_DYNAMICAL__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	ships_angles[3] = (float)M_PI_4;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
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
	Area* area = new Area();
	area->Set(points, 4);

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
	buttons[4].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	ships_select_menu.Set(&position, &size, buttons, 5);
	delete[] buttons;

	//maps select menu
#define GAME_PULL_MENU_UP_Y		0.9f
#define GAME_PULL_MENU_RIGHT_BORDER	0.55f
#define GAME_PULL_MENU_DOWN_BORDER	0.3f
	buttons = new Button[GAME_MAPS_COUNT];
	size.Set(0.5f, -0.25f);
	for (uint8_t i = 0; i < GAME_MAPS_COUNT; i++)
	{
		position.Set(-0.5f + (float)(i % 2) * GAME_PULL_MENU_RIGHT_BORDER, GAME_PULL_MENU_UP_Y - (float)(i / 2) * GAME_PULL_MENU_DOWN_BORDER);
		buttons[i].Set(BUTTON_ID__SELECT_MAP + i, &position, &size, area, "", 5, BUTTON_STATUS_TRUE);
		buttons[i].status |= BUTTON_STATUS_ACTIVE;
	}
	buttons[MAP_TEST_MAP].SetText("Empty");
	buttons[MAP_TEST_MAP].text_size = 6u;
	buttons[MAP_TUREL_ON_CENTER].SetText("Turel");
	buttons[MAP_TUREL_ON_CENTER].text_size = 6u;
	buttons[MAP_CYRCLE_ON_CENTER].SetText("Grav gen");
	buttons[MAP_DECELERATION_AREA].SetText("Deceler");
	buttons[MAP_MEGA_LASERS].SetText("Mega laser");
	buttons[MAP_DYNAMICAL].SetText("Dynamical");
	buttons[MAP_DESTROYABLE].SetText("Destructible");
	buttons[MAP_DESTROYABLE].text_size = 4u;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, GAME_PULL_MENU_RIGHT_BORDER * (float)(((GAME_MAPS_COUNT + 1) / 2) + 1));
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
	buttons = new Button[GAME_BONUSES_COUNT];
	size.Set(0.5f, -0.25f);
	for (uint8_t bonus = 0; bonus < GAME_BONUSES_COUNT; bonus++)
	{
		position.Set(-0.5f + (float)(bonus % 2) * GAME_PULL_MENU_RIGHT_BORDER, GAME_PULL_MENU_UP_Y -(float)(bonus / 2) * GAME_PULL_MENU_DOWN_BORDER);
		buttons[bonus].Set(BUTTON_ID__SELECT_BONUS + bonus, &position, &size, area, "", 7, (bonus_pull_array[bonus] == true) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE);
		buttons[bonus].status |= BUTTON_STATUS_ACTIVE;
	}
	buttons[GAME_BONUS_LOOP].SetText("Loop");
	buttons[GAME_BONUS_LASER].SetText("Laser");
	buttons[GAME_BONUS_BOMB].SetText("Bomb");
	buttons[GAME_BONUS_KNIFE].SetText("Knife");
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, GAME_PULL_MENU_RIGHT_BORDER * (float)(((GAME_BONUSES_COUNT + 1) / 2) + 1));
	bonus_pull_select_menu.Set(&position, &size, buttons, GAME_BONUSES_COUNT);
	delete[] buttons;

	//ship control menu
	position.Set(-1.0f, -1.0f);
	size.Set(2.0f, 2.0f);
	ships_control_menu.Set(&position, &size);

	current_active_menu = &main_menu;

	delete area;
}

void Game::NextLevel()
{
	//check draw
	GameTypes::score_t max_score = 0;
	bool winner_found = false;
	for (GameTypes::players_count_t team = 0; team < GAME_PLAYERS_MAX_COUNT; team++)
	{
		if (max_score < scores[team])
		{
			max_score = scores[team];
			winner_found = true;
		}
		else if (max_score == scores[team])
		{
			winner_found = false;
		}
	}
	if (max_score >= end_match_score && winner_found)
	{
		flag_end_match = true;
	}
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
			//NextLevel();
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

	GameTypes::players_count_t lines_count = players_count;

	GameTypes::score_t step = end_match_score * 2 + 1;

	Segment segment;
	Rectangle* rectangles = new Rectangle[lines_count + step];

	float y_border = (float)lines_count / (float)step * ROUND_RESULTS_INIT__MAP_SIZE;
	

	for (EngineTypes::Map::elements_array_length_t element = 0; element < step; element++)//vertical lines
	{
		segment.Set(
			Vec2F(
				-ROUND_RESULTS_INIT__MAP_SIZE + (float)element / (float)step * ROUND_RESULTS_INIT__MAP_SIZE_X2,
				-y_border),
			Vec2F(
				-ROUND_RESULTS_INIT__MAP_SIZE + (float)(element + 1) / (float)step * ROUND_RESULTS_INIT__MAP_SIZE_X2,
				y_border),
			true);

		rectangles[element].Set(&segment);
	}
	for (EngineTypes::Map::elements_array_length_t element = 0; element < lines_count; element++)//horizontal lines
	{
		segment.Set(
			Vec2F(ROUND_RESULTS_INIT__MAP_SIZE,
				-y_border + (float)element / (float)step * ROUND_RESULTS_INIT__MAP_SIZE_X2),
			Vec2F(-ROUND_RESULTS_INIT__MAP_SIZE,
				-y_border + (float)(element + 1) / (float)step * ROUND_RESULTS_INIT__MAP_SIZE_X2),
			true);

		rectangles[element + step].Set(&segment);
	}

	map.Set(rectangles, lines_count + step);

	map_data_mtx.unlock();
	ships_array_mtx.unlock();

	delete[] rectangles;

	GameTypes::players_count_t y_pos = 0;

	MutexesLock();
	MemorySetDefault();
	MutexesUnlock();

	ships_array_mtx.lock();

	for (GameTypes::players_count_t team = 1; team <= GAME_PLAYERS_MAX_COUNT; team++)
	{
		for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
		{
			if (ships[ship].GetTeamNumber() == team)
			{
				ships[ship].exist = true;
				ships[ship].SetPosition(
					Vec2F(
						((float)scores[team - 1] / ((float)end_match_score + 0.5f) - 0.015f) * ROUND_RESULTS_INIT__MAP_SIZE,
						-y_border + (((float)y_pos + 0.5f) / (float)step) * ROUND_RESULTS_INIT__MAP_SIZE_X2));
				ships[ship].radius = SHIP_DEFAULT_RADIUS / (float)step * 5.0f * ROUND_RESULTS_INIT__MAP_SIZE;
				ships[ship].SetAngle(0.0f);
				ships[ship].UpdateMatrix();
				ships_count++;
				y_pos++;
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

	GameTypes::score_t step = end_match_score;

	EngineTypes::Log::data_t data = logs.PopFromStart();

	if (((data >> LOG_HEAD) & LOG_MASK_BITS) == LOG_CHANGE_SCORE)
	{
		for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
		{
			if (ships[ship].exist && ships[ship].GetTeamNumber() == ((data >> LOG_DATA_TEAM) & LOG_MASK_BITS) + 1)
			{
				if (((data >> LOG_DATA_SCORE) & LOG_MASK_BITS) == LOG_INCREMENT)
				{
					ships[ship].Move(Vec2F(1.0f / ((float)step + ROUND_RESULTS__MAP_SIZE_X025) * ROUND_RESULTS__MAP_SIZE, 0.0f));
					ships[ship].UpdateMatrix();
					scores[ships[ship].GetTeamNumber() - 1]++;
				}
				else if (((data >> LOG_DATA_SCORE) & LOG_MASK_BITS) == LOG_DECREMENT)
				{
					ships[ship].Move(Vec2F(-1.0f / ((float)step + ROUND_RESULTS__MAP_SIZE_X025) * ROUND_RESULTS__MAP_SIZE, 0.0f));
					ships[ship].UpdateMatrix();
					scores[ships[ship].GetTeamNumber() - 1]--;
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



//deceler_area -> grav_gen -> camera -> ship -> pilot -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turel -> bullet -> asteroid -> bonus -> map -> particle -> log
void Game::MutexesLock()
{
	deceler_areas_array_mtx.lock();
	grav_gens_array_mtx.lock();
	camera_data_mtx.lock();
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
	log_data_mtx.lock();
}

//deceler_area -> grav_gen -> camera -> ship -> pilot -> input_values ->  mega_laser -> laser ->  bomb -> knife -> turel -> bullet -> asteroid -> bonus -> map -> particle -> log
void Game::MutexesUnlock()
{
	deceler_areas_array_mtx.unlock();
	grav_gens_array_mtx.unlock();
	camera_data_mtx.unlock();
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
	log_data_mtx.unlock();
}

void Game::WaitPhase1()
{
	while ((threads_statuses & THREAD_PHASE_1_COMPLETE) != THREAD_PHASE_1_COMPLETE);
}

void Game::WaitPhaseAllPhases()
{
	while ((threads_statuses & THREAD_ALL_PHASE_COMPLETE) != THREAD_ALL_PHASE_COMPLETE);
}

void Game::WaitPhaseNotAll()
{
	while ((threads_statuses & THREAD_ALL_PHASE_COMPLETE) == THREAD_ALL_PHASE_COMPLETE);
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

void Game::DebugLog__CheckMutexeslLock()
{
	std::shared_mutex* mtx_00 = &deceler_areas_array_mtx;
	std::shared_mutex* mtx_01 = &grav_gens_array_mtx;
	std::shared_mutex* mtx_02 = &camera_data_mtx;
	std::shared_mutex* mtx_03 = &ships_array_mtx;
	std::shared_mutex* mtx_04 = &pilots_array_mtx;
	std::shared_mutex* mtx_05 = &input_values_mtx;
	std::shared_mutex* mtx_06 = &mega_lasers_array_mtx;
	std::shared_mutex* mtx_07 = &lasers_array_mtx;
	std::shared_mutex* mtx_08 = &bombs_array_mtx;
	std::shared_mutex* mtx_09 = &knifes_array_mtx;
	std::shared_mutex* mtx_0A = &turels_array_mtx;
	std::shared_mutex* mtx_0B = &bullets_array_mtx;
	std::shared_mutex* mtx_0C = &asteroids_array_mtx;
	std::shared_mutex* mtx_0D = &bonuses_array_mtx;
	std::shared_mutex* mtx_0E = &map_data_mtx;
	std::shared_mutex* mtx_0F = &particles_array_mtx;
	std::shared_mutex* mtx_10 = &log_data_mtx;

	bool data[17];
	for (uint8_t i = 0; i < 17; i++)
	{
		data[i] = true;
	}

	std::thread th_00(Th_00, mtx_00, &data[0x00]);
	std::thread th_01(Th_01, mtx_01, &data[0x01]);
	std::thread th_02(Th_02, mtx_02, &data[0x02]);
	std::thread th_03(Th_03, mtx_03, &data[0x03]);
	std::thread th_04(Th_04, mtx_04, &data[0x04]);
	std::thread th_05(Th_05, mtx_05, &data[0x05]);
	std::thread th_06(Th_06, mtx_06, &data[0x06]);
	std::thread th_07(Th_07, mtx_07, &data[0x07]);
	std::thread th_08(Th_08, mtx_08, &data[0x08]);
	std::thread th_09(Th_09, mtx_09, &data[0x09]);
	std::thread th_0A(Th_0A, mtx_0A, &data[0x0A]);
	std::thread th_0B(Th_0B, mtx_0B, &data[0x0B]);
	std::thread th_0C(Th_0C, mtx_0C, &data[0x0C]);
	std::thread th_0D(Th_0D, mtx_0D, &data[0x0D]);
	std::thread th_0E(Th_0E, mtx_0E, &data[0x0E]);
	std::thread th_0F(Th_0F, mtx_0F, &data[0x0F]);
	std::thread th_10(Th_10, mtx_10, &data[0x10]);

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
	printf(" 5. Input values:  ");
	if (data[0x05])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 6. Mega lasers:   ");
	if (data[0x06])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 7. Lasers:        ");
	if (data[0x07])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 8. Bombs:         ");
	if (data[0x08])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf(" 9. Knifes:        ");
	if (data[0x09])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("10. Turels:        ");
	if (data[0x0A])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("11. Bullets:       ");
	if (data[0x0B])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("12. Asteroids:     ");
	if (data[0x0C])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("13. Bonuses:       ");
	if (data[0x0D])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("14. Map:           ");
	if (data[0x0E])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("15. Particles:     ");
	if (data[0x0F])
	{
		printf("lock\n");
	}
	else
	{
		printf("unlock\n");
	}
	printf("16. Logs:          ");
	if (data[0x10])
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
}