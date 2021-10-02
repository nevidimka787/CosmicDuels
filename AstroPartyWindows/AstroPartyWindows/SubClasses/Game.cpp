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
	//LasersDetonateBombs(); -- was not realisated
	//LasersDestroyKnifes(); -- was not ralisated
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
	//MegaLasersDetonateBombs(); -- not realisated
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
	//KnifesDestroyMap(); -- not realisated
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
	//KnifesDestroyTurels(); -- not added
	//LasersDestroyMap(); -- not added
	LasersDestroyTurels();
	//MegaLasersDestroyKnifes(); -- not realisated
	//MegaLasersDestroyTurels(); -- not realisated
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

void Game::AddEntity(Asteroid new_asteroid)
{
	for (GameTypes::entities_count_t asteroid = 0; asteroid < GAME_ASTEROIDS_MAX_COUNT; asteroid++)
	{
		if (asteroids[asteroid].exist == false)
		{
			asteroids[asteroid] = new_asteroid;
			asteroids_count++;
			return;
		}
	}
}

void Game::AddEntity(Bonus new_bonus)
{
	if (new_bonus.exist == false)
	{
		return;
	}
	for (GameTypes::entities_count_t bonus = 0; bonus < GAME_BONUSES_MAX_COUNT; bonus++)
	{
		if (bonuses[bonus].exist == false)
		{
			bonuses[bonus] = new_bonus;
			bonuses_count++;
			return;
		}
	}
}

void Game::AddEntity(Bullet new_bullet)
{
	for (GameTypes::entities_count_t bullet = 0; bullet < GAME_BULLETS_MAX_COUNT; bullet++)
	{
		if (bullets[bullet].exist == false)
		{
			bullets[bullet] = new_bullet;
			bullets_count++;
			return;
		}
	}
}

void Game::AddEntity(DecelerationArea new_deceler_area)
{
	for (GameTypes::map_elements_count_t deceler_area = 0; deceler_area < GAME_DECEL_AREAS_MAX_COUNT; deceler_area++)
	{
		if (deceler_areas[deceler_area].exist == false)
		{
			deceler_areas[deceler_area] = new_deceler_area;
			deceler_areas_count++;
			return;
		}
	}
}

void Game::AddEntity(GravGen new_grav_gen)
{
	for (GameTypes::map_elements_count_t grav_gen = 0; grav_gen < GAME_GRAV_GENS_MAX_COUNT; grav_gen++)
	{
		if (grav_gens[grav_gen].exist == false)
		{
			grav_gens[grav_gen] = new_grav_gen;
			grav_gens_count++;
			return;
		}
	}
}

void Game::AddEntity(Knife new_knife)
{
	for (GameTypes::entities_count_t knife = 0; knife < GAME_KNIFES_MAX_COUNT; knife++)
	{
		if (knifes[knife].exist == false)
		{
			knifes[knife] = new_knife;
			knifes_count++;
			return;
		}
	}
}

void Game::AddEntity(Bomb new_bomb)
{
	if (bombs_count == GAME_BOMBS_MAX_COUNT)
	{
		return;
	}
	for (GameTypes::entities_count_t bomb = 0; bomb < GAME_BOMBS_MAX_COUNT; bomb++)
	{
		if (bombs[bomb].exist == false)
		{
			bombs[bomb] = new_bomb;
			bombs_count++;
			return;
		}
	}
}

void Game::AddEntity(Laser new_laser)
{
	for (GameTypes::entities_count_t laser = 0; laser < GAME_LASERS_MAX_COUNT; laser++)
	{
		if (lasers[laser].exist == false)
		{
			lasers[laser] = new_laser;
			lasers_count++;
			return;
		}
	}
}

void Game::AddEntity(MegaLaser new_mega_laser)
{
	for (GameTypes::map_elements_count_t mega_laser = 0; mega_laser < GAME_MEGA_LASERS_MAX_COUNT; mega_laser++)
	{
		if (mega_lasers[mega_laser].exist == false)
		{
			mega_lasers[mega_laser] = new_mega_laser;
			mega_lasers_count++;
			return;
		}
	}
}

void Game::AddEntity(Turel new_turel)
{
	for (GameTypes::map_elements_count_t turel = 0; turel < GAME_TURELS_MAX_COUNT; turel++)
	{
		if (turels[turel].exist == false)
		{
			turels[turel] = new_turel;
			turels_count++;
			return;
		}
	}
}

void Game::AddEntity(Particle new_particle)
{
	for (GameTypes::entities_count_t particle = 0; particle < GAME_PARTICLES_MAX_COUNT; particle++)
	{
		if (particles[particle].exist == false)
		{
			particles[particle] = new_particle;
			particles_count++;
			return;
		}
	}
}


void Game::AddEntity(Pilot new_pilot)
{
	if (!new_pilot.exist)
	{
		return;
	}
	GameTypes::players_count_t pilot_number = new_pilot.GetPlayerNumber();
	if (pilot_number < GAME_PLAYERS_MAX_COUNT && !pilots[pilot_number].exist)
	{
		pilots[pilot_number] = new_pilot;
		pilots_count++;
	}
	printf("\nPilots count: %i\n", pilots_count);
}


void Game::AddEntity(Ship new_ship)
{
	if (!new_ship.exist)
	{
		return;
	}
	GameTypes::players_count_t ship_number = new_ship.GetPlayerNumber();
	if (ship_number < GAME_PLAYERS_MAX_COUNT && !ships[ship_number].exist)
	{
		ships[ship_number] = new_ship;
		ships_count++;
	}
}

void Game::RemoveEntity(Asteroid* deleting_asteroid)
{
	if (deleting_asteroid->exist)
	{
		deleting_asteroid->exist = false;
		Game::asteroids_count--;
	}
}

void Game::RemoveEntity(Bomb* deleting_bomb)
{
	if (deleting_bomb->exist)
	{
		deleting_bomb->exist = false;
		Game::bombs_count--;
	}
}

void Game::RemoveEntity(Bonus* deleting_bonus)
{
	if (deleting_bonus->exist)
	{
		deleting_bonus->exist = false;
		Game::bonuses_count--;
	}
}

void Game::RemoveEntity(Bullet* deleting_bullet)
{
	if (deleting_bullet->exist)
	{
		deleting_bullet->exist = false;
		Game::bullets_count--;
	}
}

void Game::RemoveEntity(DecelerationArea* deleting_deceler_area)
{
	if (deleting_deceler_area->exist)
	{
		deleting_deceler_area->exist = false;
		Game::deceler_areas_count--;
	}
}

void Game::RemoveEntity(GravGen* deleting_grav_gen)
{
	if (deleting_grav_gen->exist)
	{
		deleting_grav_gen->exist = false;
		Game::grav_gens_count--;
	}
}

void Game::RemoveEntity(Knife* deleting_knife)
{
	if (deleting_knife->exist)
	{
		deleting_knife->exist = false;
		Game::knifes_count--;
	}
}

void Game::RemoveEntity(Laser * deleting_lazer)
{
	if (deleting_lazer->exist)
	{
		deleting_lazer->exist = false;
		Game::lasers_count--;
	}
}

void Game::RemoveEntity(MegaLaser* deleting_mega_lazer)
{
	if (deleting_mega_lazer->exist)
	{
		deleting_mega_lazer->exist = false;
		Game::mega_lasers_count--;
	}
}

void Game::RemoveEntity(Turel * deleting_turel)
{
	if (deleting_turel->exist)
	{
		deleting_turel->exist = false;
		Game::turels_count--;
	}
}

void Game::RemoveEntity(Particle * deleting_particle)
{
	if (deleting_particle->exist)
	{
		deleting_particle->exist = false;
		Game::particles_count--;
	}
}

void Game::RemoveEntity(Pilot* deleting_pilot)
{
	if (deleting_pilot->exist)
	{
		deleting_pilot->exist = false;
		pilots_count--;
	}
}

void Game::RemoveEntity(Ship* deleting_ship)
{
	if (deleting_ship->exist)
	{
		deleting_ship->exist = false;
		ships_count--;
	}
}



void Game::DestroyEntity(Bomb* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (entity->GetSize() > ASTEROID_SIZE_MEDIUM)
	{
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else if (entity->GetSize() > ASTEROID_SIZE_SMALL)
	{
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else
	{
		AddEntity(entity->Destroy());
		RemoveEntity(entity);
	}
}

void Game::DestroyEntity(Bomb* destroyer, Bonus* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bomb* destroyer, Bullet* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bomb* destroyer, Knife* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bomb* destroyer, Ship* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (!(game_rules && GAME_RULE_NEED_KILL_PILOT))
	{
		if (destroyer->IsAggressiveFor(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetTeamNumber(entity));
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	AddEntity(entity->Destroy());
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Bomb* destroyer, Particle* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bomb* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->IsAggressiveFor(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetTeamNumber(entity));
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	RemoveEntity(entity);
	camera.SetCoefficients();
}

void Game::DestroyEntity(Bomb* destroyer, Turel* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bullet* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (entity->GetSize() > ASTEROID_SIZE_MEDIUM)
	{
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else if (entity->GetSize() > ASTEROID_SIZE_SMALL)
	{
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else
	{
		AddEntity(entity->Destroy());
		RemoveEntity(entity);
	}
}

void Game::DestroyEntity(Bullet* destroyer, Knife* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bullet* destroyer, Ship* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (!(game_rules && GAME_RULE_NEED_KILL_PILOT))
	{
		if (destroyer->CreatedByTeam(entity) || destroyer->GetPlayerMasterTeamNumber() == AGGRESIVE_ENTITY_HOST_ID)
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else if(destroyer->GetPlayerMasterTeamNumber() != AGGRESIVE_ENTITY_HOST_ID)
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddBonuses(entity);
	AddEntity(entity->Destroy());
	RemoveEntity(entity);
	camera.SetCoefficients();
}

void Game::DestroyEntity(Bullet* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->CreatedByTeam(entity) || destroyer->GetPlayerMasterTeamNumber() == AGGRESIVE_ENTITY_HOST_ID)
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else if(destroyer->GetPlayerMasterTeamNumber() != AGGRESIVE_ENTITY_HOST_ID)
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Knife* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (entity->GetSize() > ASTEROID_SIZE_MEDIUM)
	{
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else if (entity->GetSize() > ASTEROID_SIZE_SMALL)
	{
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else
	{
		AddEntity(entity->Destroy());
		RemoveEntity(entity);
	}
}

void Game::DestroyEntity(Knife* destroyer, Ship* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (!(game_rules && GAME_RULE_NEED_KILL_PILOT))
	{
		if (destroyer->CreatedByTeam(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Knife* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->CreatedByTeam(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Knife* destroyer, Turel* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Laser* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (entity->GetSize() > ASTEROID_SIZE_MEDIUM)
	{
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else if (entity->GetSize() > ASTEROID_SIZE_SMALL)
	{
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else
	{
		AddEntity(entity->Destroy());
		RemoveEntity(entity);
	}
}

void Game::DestroyEntity(Laser* destroyer, Bonus* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Laser* destroyer, Bullet* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Laser* destroyer, Knife* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Laser* destroyer, Ship* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (!(game_rules && GAME_RULE_NEED_KILL_PILOT))
	{
		if (destroyer->CreatedByTeam(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Laser* destroyer, Particle* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Laser* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->CreatedByTeam(entity))
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;
	camera.SetCoefficients();
}

void Game::DestroyEntity(Laser* destroyer, Turel* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(MegaLaser* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (entity->GetSize() > ASTEROID_SIZE_MEDIUM)
	{
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else if (entity->GetSize() > ASTEROID_SIZE_SMALL)
	{
		AddEntity(entity->Division());
		entity->DecrementSize();
	}
	else
	{
		AddEntity(entity->Destroy());
		RemoveEntity(entity);
	}
}

void Game::DestroyEntity(MegaLaser* destroyer, Bonus* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(MegaLaser* destroyer, Bullet* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(MegaLaser* destroyer, Knife* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(MegaLaser* destroyer, Ship* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (!(game_rules && GAME_RULE_NEED_KILL_PILOT))
	{
		DecrementScore(entity->GetTeamNumber());
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddBonuses(entity);
	AddEntity(entity->Destroy());
	std::cout << "\nMegaLaser destroys ship\n";
	RemoveEntity(entity);
	camera.SetCoefficients();
}

void Game::DestroyEntity(MegaLaser* destroyer, Particle* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(MegaLaser* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		DecrementScore(entity->GetTeamNumber());
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	RemoveEntity(entity);
}

void Game::DestroyEntity(Ship* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->SameTeams(entity))
		{
			DecrementScore(destroyer->GetTeamNumber());
		}
		else
		{
			IncrementScore(destroyer->GetTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	RemoveEntity(entity);
}

void Game::DestroySupportEntitiesBy(ControledEntity* produser)
{
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		if (lasers[laser].exist)
		{
			if (lasers[laser].CreatedBy(produser))
			{
				RemoveEntity(&lasers[laser]);
			}
			else
			{
				found_lasers++;
			}
		}
	}
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		if (knifes[knife].exist)
		{
			if (knifes[knife].CreatedBy(produser))
			{
				RemoveEntity(&knifes[knife]);
			}
			else
			{
				found_knifes++;
			}
		}
	}
}



void Game::SpawnEntity(Ship* spawner, Pilot* pilot)
{
	if (spawner->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)
	{
		camera.SetCoefficients();
		IncrementPlayersCountInTeam(spawner->GetTeamNumber());
		AddEntity(pilot->Respawn());
		RemoveEntity(pilot);
	}
}

void Game::AddBonuses(Ship* spawner)
{
	Bonus ship_bonus = spawner->LoseBonus();
	AddEntity(ship_bonus);
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
			buttons[buttons_count].Set(BUTTON_ID_SHIP1_SHOOT + 2 * player, &positions[player * 2], &size1, &area, "", 0, BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1));
			buttons[buttons_count + 1].Set(BUTTON_ID_SHIP1_ROTATE + 2 * player, &positions[player * 2 + 1], &size2, &area, "", 0, BUTTON_STATUS_CUSTOM_RED << (teams[player] - 1));
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

#define EVENT5__PERIOD	1000lu
#define EVENT5_PHASE	200lu

	Vec2F position = Vec2F();
	float local_position;

	local_position = (float)((global_timer + EVENT5_PHASE) % EVENT5__PERIOD) / (float)EVENT5__PERIOD * EVENT5__SQUARE_SIZE * 2.0f - EVENT5__SQUARE_SIZE;

	map_data_mtx.lock();

	Rectangle* rectangle_p = map.GetRectanglePointer(EVENT5__UP_RECTANGLE);
	rectangle_p->SetCenterPosition(Vec2F(EVENT5__CENTER_POSITION + local_position, EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
	rectangle_p = map.GetRectanglePointer(EVENT5__RIGHT_RECTANGLE);
	rectangle_p->SetCenterPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION - local_position));
	rectangle_p = map.GetRectanglePointer(EVENT5__DONW_RECTANGLE);
	rectangle_p->SetCenterPosition(Vec2F(EVENT5__CENTER_POSITION - local_position, EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
	rectangle_p = map.GetRectanglePointer(EVENT5__LEFT_RECTANGLE);
	rectangle_p->SetCenterPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION + local_position));

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

#define MAP_TUREL_ON_CENTER__FRAME_SIZE				2.0f
#define MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE		0.1f
#define MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION	1.5f
#define MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION	1.0f
#define MAP_TUREL_ON_CENTER__CENTER					0.0f
#define MAP_TUREL_ON_CENTER__RECTANGLE_COUNT		6

	/* Create map */

	Rectangle* rectangles = new Rectangle[MAP_TUREL_ON_CENTER__RECTANGLE_COUNT];

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__FRAME_SIZE, -MAP_TUREL_ON_CENTER__FRAME_SIZE),
		Vec2F(MAP_TUREL_ON_CENTER__FRAME_SIZE, MAP_TUREL_ON_CENTER__FRAME_SIZE),
		true);
	rectangles[0].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE, -MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE),
		Vec2F(MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE, MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE),
		true);
	rectangles[1].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION),
		true);
	rectangles[2].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION),
		true);
	rectangles[3].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION),
		true);
	rectangles[4].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION),
		true);
	rectangles[5].Set(&new_segment);

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
		Vec2F(MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE, MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE),
		Vec2F(MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE, MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__FRAME_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(-MAP_DYNAMICAL_RECTANGLE_SIZE, MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE),
		Vec2F(MAP_DYNAMICAL_RECTANGLE_SIZE, MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].Normalise();

	diagonal.Set(
		Vec2F(MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE, -MAP_DYNAMICAL_RECTANGLE_SIZE),
		Vec2F(MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE, MAP_DYNAMICAL_RECTANGLE_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].Normalise();

	diagonal.Set(
		Vec2F(-MAP_DYNAMICAL_RECTANGLE_SIZE, -MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE),
		Vec2F(MAP_DYNAMICAL_RECTANGLE_SIZE, -MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].Normalise();

	diagonal.Set(
		Vec2F(-MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE, -MAP_DYNAMICAL_RECTANGLE_SIZE),
		Vec2F(-MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE, MAP_DYNAMICAL_RECTANGLE_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].Normalise();

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
	buttons[0].Set(BUTTON_ID_START_MATCH, &position, &size, area, "PLAY", 20);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.3f);
	size.Set(0.475f, -0.25f);
	buttons[1].Set(BUTTON_ID_GO_TO_OPTINS_MENU, &position, &size, area, "Options", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.3f);
	buttons[2].Set(BUTTON_ID_EXIT, &position, &size, area, "Exit", 6);
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
	buttons[0].Set(BUTTON_ID_SET_RANDOM_SPAWN, &position, &size, area, "Random spawn", 5);
	buttons[0].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 1 * GAME_OPTION_MENU_BORD);
	buttons[1].Set(BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION, &position, &size, area, "Random spawn direction", 5);
	buttons[1].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 2 * GAME_OPTION_MENU_BORD);
	buttons[2].Set(BUTTON_ID_SET_SPAWN_THIS_BONUS, &position, &size, area, "Spawn this bonus", 5);
	buttons[2].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 3 * GAME_OPTION_MENU_BORD);
	buttons[3].Set(BUTTON_ID_SET_SPAWN_THIS_DIFFERENT_BONUSES, &position, &size, area, "Can spawn this different bonuses", 4);
	buttons[3].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 4 * GAME_OPTION_MENU_BORD);
	buttons[4].Set(BUTTON_ID_SET_TRIPLE_BONUSES, &position, &size, area, "Triple bonuses", 5);
	buttons[4].status = ((game_rules & GAME_RULE_TRIPLE_BONUSES) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 5 * GAME_OPTION_MENU_BORD);
	buttons[5].Set(BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BAFF, &position, &size, area, "Spawn this triple", 5);
	buttons[5].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 6 * GAME_OPTION_MENU_BORD);
	buttons[6].Set(BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF, &position, &size, area, "Spawn this shield", 5);
	buttons[6].status = ((game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 7 * GAME_OPTION_MENU_BORD);
	buttons[7].Set(BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS, &position, &size, area, "Knifes can destroy bullets", 5);
	buttons[7].status = ((game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 8 * GAME_OPTION_MENU_BORD);
	buttons[8].Set(BUTTON_ID_SET_NEED_KILL_PILOT, &position, &size, area, "Need kill pilot", 5);
	buttons[8].status = ((game_rules & GAME_RULE_NEED_KILL_PILOT) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | ((game_rules & GAME_RULE_NEED_KILL_PILOT) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 9 * GAME_OPTION_MENU_BORD);
	buttons[9].Set(BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, area, "Friendly sheep can restore", 5);
	buttons[9].status = ((game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 10 * GAME_OPTION_MENU_BORD);
	buttons[10].Set(BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE, &position, &size, area, "Frendly fire", 5);
	buttons[10].status = ((game_rules & GAME_RULE_FRENDLY_FIRE) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE) | BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 11 * GAME_OPTION_MENU_BORD);
	buttons[11].Set(BUTTON_ID_GO_TO_SELECT_BONUSES_MENU, &position, &size, area, "Bonus pull menu", 5);
	buttons[11].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_FALSE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 12 * GAME_OPTION_MENU_BORD);
	buttons[12].Set(BUTTON_ID_GO_TO_SELECT_MAP_MENU, &position, &size, area, "Map pull menu", 5);
	buttons[12].status |= BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 13 * GAME_OPTION_MENU_BORD);
	buttons[13].Set(BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU, &position, &size, area, "Spawning objects menu", 5);
	buttons[13].status |= BUTTON_STATUS_ACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 14 * GAME_OPTION_MENU_BORD);
	buttons[14].Set(BUTTON_ID_SET_ACTIVE_BALANCE, &position, &size, area, "Auto balance", 5);
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
	buttons[0].Set(BUTTON_ID_RESUME_MATCH, &position, &size, area, "Resume", 6);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.4f, 0.6f);
	buttons[1].Set(BUTTON_ID_GO_TO_MAIN_MENU, &position, &size, area, "Main menu", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	pause_menu.Set(&position, &size, buttons, 2);
	delete[] buttons;

	//sheeps select menu
	buttons = new Button[5];;
	size.Set(0.475f, -0.25f);
	position.Set(-0.5f, 0.9f);
	buttons[0].Set(BUTTON_ID_SELECT_SHIP_1, &position, &size, area, "Player 1", 6);
	buttons[0].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.9f);
	buttons[1].Set(BUTTON_ID_SELECT_SHIP_2, &position, &size, area, "Player 2", 6);
	buttons[1].status |= BUTTON_STATUS_ACTIVE;
	position.Set(0.025f, 0.6f);
	buttons[2].Set(BUTTON_ID_SELECT_SHIP_3, &position, &size, area, "Player 3", 6);
	buttons[2].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.6f);
	buttons[3].Set(BUTTON_ID_SELECT_SHIP_4, &position, &size, area, "Player 4", 6);
	buttons[3].status |= BUTTON_STATUS_ACTIVE;
	position.Set(-0.5f, 0.3f);
	size.Set(1.0f, -0.25f);
	buttons[4].Set(BUTTON_ID_START_GAME, &position, &size, area, "Start game", 7);
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
		buttons[i].Set(BUTTON_ID_SELECT_MAP + i, &position, &size, area, "", 5, BUTTON_STATUS_TRUE);
		buttons[i].status |= BUTTON_STATUS_ACTIVE;
	}
	buttons[MAP_TEST_MAP].SetText("Empty");
	buttons[MAP_TUREL_ON_CENTER].SetText("Turel");
	buttons[MAP_CYRCLE_ON_CENTER].SetText("Grav gen");
	buttons[MAP_DECELERATION_AREA].SetText("Deceler");
	buttons[MAP_MEGA_LASERS].SetText("Mega laser");
	buttons[MAP_DYNAMICAL].SetText("Dynamical");
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, GAME_PULL_MENU_RIGHT_BORDER * (float)(((GAME_MAPS_COUNT + 1) / 2) + 1));
	map_pull_select_menu.Set(&position, &size, buttons, GAME_MAPS_COUNT);
	delete[] buttons;

	//spawning objects select menu
	buttons = new Button[GAME_OBJECTS_COUNT];
	position.Set(-0.5f, 0.9f);
	size.Set(0.475f, -0.475f);
	buttons[0].Set(BUTTON_ID_SELECT_OBJECT_ASTEROID, &position, &size, area, "Asteroid", 6, (object_pull_array[GAME_OBJECT_ASTEROID] == true) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE);
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
		buttons[bonus].Set(BUTTON_ID_SELECT_BONUS + bonus, &position, &size, area, "", 7, (bonus_pull_array[bonus] == true) ? BUTTON_STATUS_TRUE : BUTTON_STATUS_FALSE);
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

//Memory functions.

void Game::MemoryLock()
{
	particles = new Particle[GAME_PARTICLES_MAX_COUNT];
	particles_count = 0;
	asteroids = new Asteroid[GAME_ASTEROIDS_MAX_COUNT];
	asteroids_count = 0;
	bombs = new Bomb[GAME_BOMBS_MAX_COUNT];
	bombs_count = 0;
	bonuses = new Bonus[GAME_BONUSES_MAX_COUNT];
	bonuses_count = 0;
	bullets = new Bullet[GAME_BULLETS_MAX_COUNT];
	bullets_count = 0;
	deceler_areas = new DecelerationArea[GAME_DECEL_AREAS_MAX_COUNT];
	deceler_areas_count = 0;
	grav_gens = new GravGen[GAME_GRAV_GENS_MAX_COUNT];
	grav_gens_count = 0;
	knifes = new Knife[GAME_KNIFES_MAX_COUNT];
	knifes_count = 0;
	lasers = new Laser[GAME_LASERS_MAX_COUNT];
	lasers_count = 0;
	mega_lasers = new MegaLaser[GAME_MEGA_LASERS_MAX_COUNT];
	mega_lasers_count = 0;
	pilots = new Pilot[GAME_PLAYERS_MAX_COUNT];
	pilots_count = 0;
	ships = new Ship[GAME_PLAYERS_MAX_COUNT];
	ships_count = 0;
	turels = new Turel[GAME_TURELS_MAX_COUNT];
	turels_count = 0;

	burnout_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	burnout_double_clk_timer = new int32_t[GAME_PLAYERS_MAX_COUNT];
	shoot_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	rotate_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	ships_can_shoot_flags = new GameTypes::tic_t[GAME_PLAYERS_MAX_COUNT];
	scores = new GameTypes::score_t[GAME_PLAYERS_MAX_COUNT];
	players_in_team = new GameTypes::players_count_t[GAME_PLAYERS_MAX_COUNT];
}

void Game::MemorySetDefault()
{
	global_timer = 0;
	end_match_tic = 0;

	camera.SetCoefficients();
	camera.SetHightLimits();
	camera.SetLowLimits();
	camera.SetScale(object_p__open_gl_realisation->GetScale());

	asteroids_count = 0;
	bombs_count = 0;
	bonuses_count = 0;
	bullets_count = 0;
	deceler_areas_count = 0;
	grav_gens_count = 0;
	knifes_count = 0;
	lasers_count = 0;
	mega_lasers_count = 0;
	particles_count = 0;
	pilots_count = 0;
	ships_count = 0;
	turels_count = 0;

	for (GameTypes::entities_count_t entity = 0; entity < UINT16_MAX - 1; entity++)
	{
		if (entity < GAME_ASTEROIDS_MAX_COUNT && asteroids[entity].exist)
		{
			asteroids[entity].exist = false;
		}
		if (entity < GAME_BOMBS_MAX_COUNT && bombs[entity].exist)
		{
			bombs[entity].exist = false;
		}
		if (entity < GAME_BONUSES_MAX_COUNT && bonuses[entity].exist)
		{
			bonuses[entity].exist = false;
		}
		if (entity < GAME_BULLETS_MAX_COUNT && bullets[entity].exist)
		{
			bullets[entity].exist = false;
		}
		if (entity < GAME_DECEL_AREAS_MAX_COUNT && deceler_areas[entity].exist)
		{
			deceler_areas->exist = false;
		}
		if (entity < GAME_GRAV_GENS_MAX_COUNT && grav_gens[entity].exist)
		{
			grav_gens[entity].exist = false;
		}
		if (entity < GAME_KNIFES_MAX_COUNT && knifes[entity].exist)
		{
			knifes[entity].exist = false;
		}
		if (entity < GAME_LASERS_MAX_COUNT && lasers[entity].exist)
		{
			lasers[entity].exist = false;
		}
		if (entity < GAME_MEGA_LASERS_MAX_COUNT && mega_lasers[entity].exist)
		{
			mega_lasers[entity].exist = false;
		}
		if (entity < GAME_PARTICLES_MAX_COUNT && particles[entity].exist)
		{
			particles[entity].exist = false;
		}
		if (entity < GAME_PLAYERS_MAX_COUNT)
		{
			if (pilots[entity].exist)
			{
				pilots[entity].exist = false;
			}
			if (ships[entity].exist)
			{
				ships[entity].exist = false;
			}
		}
		if (entity < GAME_TURELS_MAX_COUNT && turels[entity].exist)
		{
			turels[entity].exist = false;
		}
	}

	current_active_menu = &ships_control_menu;
	flag_all_entities_initialisate = true;
	pause_round = false;
}

void Game::MemoryFree()
{
	delete[] asteroids;
	asteroids_count = 0;
	delete[] bombs;
	bombs_count = 0;
	delete[] bonuses;
	bonuses_count = 0;
	delete[] bullets;
	bullets_count = 0;
	delete[] deceler_areas;
	deceler_areas_count = 0;
	delete[] grav_gens;
	grav_gens_count = 0;
	delete[] knifes;
	knifes_count = 0;
	delete[] lasers;
	lasers_count = 0;
	delete[] mega_lasers;
	mega_lasers_count = 0;
	delete[] particles;
	particles_count = 0;
	delete[] pilots;
	pilots_count = 0;
	delete[] ships;
	ships_count = 0;
	delete[] turels;
	turels_count = 0;

	players_count = 0;
	delete[] players_in_team;

	delete[] burnout_flags;
	delete[] burnout_double_clk_timer;
	delete[] shoot_flags;
	delete[] rotate_flags;
	delete[] ships_can_shoot_flags;
	delete[] scores;
}

//Memory functions.

void Game::ShipShoot(Ship* ship)
{
	if (ships_can_shoot_flags[ship->GetPlayerNumber()] > 0)
	{
		return;
	}
	ships_can_shoot_flags[ship->GetPlayerNumber()] = GAME_DELLAY_BETWEEN_SHOOTS;

	if (ship->ActivateAvailableBuffs())
	{
		return;
	}

	//Magicka and Magicka 2 are the best games I've seen.

	if (ship->SpendBonus(BONUS_LASER))
	{
		if (ship->SpendBonus(BONUS_LOOP))
		{
			if (ship->SpendBonus(BONUS_BOMB))
			{
				if (ship->SpendBonus(BONUS_KNIFE))
				{
					ShipShoot_LaserLoopBombKnife(ship);
					return;
				}
				ShipShoot_LaserLoopBomb(ship);
				return;
			}
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserLoopKnife(ship);
				return;
			}
			ShipShoot_LaserLoop(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserBombKnife(ship);
				return;
			}
			ShipShoot_LaserBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LaserKnife(ship);
			return;
		}
		ShipShoot_Laser(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_LOOP))
	{
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LoopBombKnife(ship);
				return;
			}
			ShipShoot_LoopBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LoopKnife(ship);
			return;
		}
		ShipShoot_Loop(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_BOMB))
	{
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_BombKnife(ship);
			return;
		}
		ShipShoot_Bomb(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_KNIFE))
	{
		ShipShoot_Knife(ship);
		return;
	}
	ShipShoot_NoBonus(ship);
	return;
}

void Game::ShipShoot_LaserLoopBombKnife(Ship* ship)
{
	std::cout << "Laser Loop Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE * 3;
}

void Game::ShipShoot_LaserLoopBomb(Ship* ship)
{
	std::cout << "Laser Loop Bomb" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE * 2;
}

void Game::ShipShoot_LaserLoopKnife(Ship* ship)
{
	std::cout << "Laser Loop Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE * 2;
}

void Game::ShipShoot_LaserBombKnife(Ship* ship)
{
	std::cout << "Laser Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE * 2;
}

void Game::ShipShoot_LoopBombKnife(Ship* ship)
{
	std::cout << "Loop Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE * 2;
}

void Game::ShipShoot_LaserLoop(Ship* ship)
{
	std::cout << "Laser Loop" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_LaserBomb(Ship* ship)
{
	std::cout << "Laser Bomb" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_LoopBomb(Ship* ship)
{
	std::cout << "Loop Bomb" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_LaserKnife(Ship* ship)
{
	std::cout << "Laser Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_LoopKnife(Ship* ship)
{
	std::cout << "Loop Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_BombKnife(Ship* ship)
{
	std::cout << "Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_CONBO_USE;
}

void Game::ShipShoot_Knife(Ship* ship)
{
	knifes_array_mtx.lock();
	Game::AddEntity(ship->CreateKnife(0));
	Game::AddEntity(ship->CreateKnife(1));
	knifes_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Bomb(Ship* ship)
{
	bombs_array_mtx.lock();
	Game::AddEntity(ship->CreateBomb());
	bombs_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Loop(Ship* ship)
{
	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0; bullet < SHIP_BULLETS_IN_LOOP; bullet++)
	{
		Game::AddEntity(ship->CreateLoop(bullet));
	}
	bullets_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Laser(Ship* ship)
{
	lasers_array_mtx.lock();
	Game::AddEntity(ship->CreateLaser());
	lasers_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_NoBonus(Ship* ship)
{
	bullets_array_mtx.lock();
	if (ship->HaveBuff(SHIP_BUFF_TRIPLE))
	{
		AddEntity(ship->CreateTriple(0));
		AddEntity(ship->CreateTriple(1));
		AddEntity(ship->CreateTriple(2));
		ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BETWEEN_TRIPLES;
	}
	else
	{
		AddEntity(ship->CreateBullet());
	}
	bullets_array_mtx.unlock();
}



void Game::IncrementScore(GameTypes::players_count_t team_number)
{
	logs.PushToEnd((LOG_CHANGE_SCORE << LOG_HEAD) | (LOG_INCREMENT << LOG_DATA_SCORE) | (((team_number - 1) & LOG_MASK_BITS) << LOG_DATA_TEAM));
}

void Game::DecrementScore(GameTypes::players_count_t team_number)
{
	logs.PushToEnd((LOG_CHANGE_SCORE << LOG_HEAD) | (LOG_DECREMENT << LOG_DATA_SCORE) | (((team_number - 1) & LOG_MASK_BITS) << LOG_DATA_TEAM));
}



void Game::UpdateAsteroidsPhase2()
{
	Asteroid* temp__asteroid_p;
	asteroids_array_mtx.lock();
	for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
	{
		temp__asteroid_p = &asteroids[asteroid];
		if (temp__asteroid_p->exist)
		{
			temp__asteroid_p->Update();
			found_asteroids++;
		}
	}
	asteroids_array_mtx.unlock();
}

void Game::UpdateBombsPhase2()
{
	Bomb* temp__bomb_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			temp__bomb_p->Update();
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::UpdateBonusesPhase2()
{
	Bonus* temp__bonus_p;
	bonuses_array_mtx.lock();
	for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
	{
		temp__bonus_p = &bonuses[bonus];
		if (temp__bonus_p->exist)
		{
			temp__bonus_p->Update();
			found_bonuses++;
		}
	}
	bonuses_array_mtx.unlock();
}

void Game::UpdateBulletsPhase2()
{
	Bullet* temp__bullet_p;
	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			temp__bullet_p->Update();
			found_bullets++;
		}
	}
	bullets_array_mtx.unlock();
}

void Game::UpdateDecelerAreasPhase2()
{
	DecelerationArea* temp__deceler_area_p;
	deceler_areas_array_mtx.lock();
	for (GameTypes::map_elements_count_t deceler_area = 0, found_deceler_areas = 0; found_deceler_areas < deceler_areas_count; deceler_area++)
	{
		temp__deceler_area_p = &deceler_areas[deceler_area];
		if (temp__deceler_area_p->exist)
		{
			temp__deceler_area_p->Update();
			found_deceler_areas++;
		}
	}
	deceler_areas_array_mtx.unlock();
}

void Game::UpdateGravGensPhase2()
{
	GravGen* temp__grav_gen_p;
	grav_gens_array_mtx.lock();
	for (GameTypes::map_elements_count_t grav_gen = 0, found_grav_gens = 0; found_grav_gens < grav_gens_count; grav_gen++)
	{
		temp__grav_gen_p = &grav_gens[grav_gen];
		if (temp__grav_gen_p->exist)
		{
			temp__grav_gen_p->Update();
			found_grav_gens++;
		}
	}
	grav_gens_array_mtx.unlock();
}

void Game::UpdateKnifesPhase2()
{
	Knife* temp__knife_p;
	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			temp__knife_p->Update();
			found_knifes++;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::UpdateLasersPhase2()
{
	Laser* temp__laser_p;
	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			temp__laser_p->Update();
			if (!temp__laser_p->CanShoot())
			{
				RemoveEntity(temp__laser_p);
				goto end_of_cycle;
			}
			found_lasers++;
		}
	end_of_cycle:;
	}
	lasers_array_mtx.unlock();
}

void Game::UpdateMegaLasersPhase2()
{
	MegaLaser* temp__mega_laser_p;
	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			temp__mega_laser_p->Update();
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::UpdateParticlesPhase2()
{
	Particle* temp__particle_p;
	particles_array_mtx.lock();
	for (GameTypes::entities_count_t particle = 0, found_particles = 0; found_particles < particles_count; particle++)
	{
		temp__particle_p = &particles[particle];
		if (temp__particle_p->exist)
		{
			temp__particle_p->Update();
			found_particles++;
		}
	}
	particles_array_mtx.unlock();
}

void Game::UpdatePilotsPhase2()
{
	Pilot* temp__pilot_p;
	pilots_array_mtx.lock();
	for (GameTypes::entities_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			temp__pilot_p->Update();
			temp__pilot_p->UpdateMatrix();
		}
	}
	pilots_array_mtx.unlock();
}

void Game::UpdateShipsPhase2()
{
	Ship* temp__ship_p;
	ships_array_mtx.lock();
	for (GameTypes::entities_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			temp__ship_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
			temp__ship_p->Update();
			temp__ship_p->UpdateMatrix();
		}
	}
	ships_array_mtx.unlock();
}

void Game::UpdateTurelsPhase2()
{
	Turel* temp__turel_p;
	turels_array_mtx.lock();
	for (GameTypes::map_elements_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
	{
		temp__turel_p = &turels[turel];
		if (temp__turel_p->exist)
		{
			temp__turel_p->Update();
			found_turels++;
		}
	}
	turels_array_mtx.unlock();
}

void Game::UpdateMapPhase2()
{
	void* element_pointer;
	map_data_mtx.lock();
	for (EngineTypes::Map::elements_array_length_t element = 0, found_elements = 0; found_elements < map.rectangles_array_length; element++)
	{
		element_pointer = (void*)map.GetRectanglePointer(element);
		if (((Rectangle*)element_pointer)->exist)
		{
			((Rectangle*)element_pointer)->Update();
			found_elements++;
		}
	}
	map_data_mtx.unlock();
}

void Game::BombsChainReaction()
{
	Bomb* temp__bomb1_p;
	Bomb* temp__bomb2_p;
	GameTypes::entities_count_t second_bomb, found_second_bombs;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb1_p = &bombs[bomb];
		if (temp__bomb1_p->exist)
		{
			if (temp__bomb1_p->CanRemove())
			{
				RemoveEntity(temp__bomb1_p);
				goto end_of_bomb_cycle;
			}
			if (temp__bomb1_p->IsBoom())
			{
				for (second_bomb = bomb + 1, found_second_bombs = found_bombs + 1; found_second_bombs < bombs_count; second_bomb++)
				{
					temp__bomb2_p = &bombs[second_bomb];
					if (bombs[second_bomb].exist)
					{
						if (!temp__bomb2_p->IsBoom() && temp__bomb2_p->Entity::IsCollision(temp__bomb1_p))
						{
							temp__bomb2_p->Boom();
						}
						found_second_bombs++;
					}
				}
			}
			found_bombs++;
		}
	end_of_bomb_cycle:;
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Bomb* temp__bomb_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				asteroids_array_mtx.lock();
				for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__asteroid_p->exist)
					{
						if (temp__asteroid_p->Entity::IsCollision(temp__bomb_p))
						{
							bonuses_array_mtx.lock();
							DestroyEntity(temp__bomb_p, temp__asteroid_p);
							bonuses_array_mtx.unlock();
							goto end_of_asteroid_cycle;
						}
						found_asteroids++;
					}
				end_of_asteroid_cycle:;
				}
				asteroids_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bomb* temp__bomb_p;
	Bonus* temp__bonus_p;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				bonuses_array_mtx.lock();
				for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
				{
					temp__bonus_p = &bonuses[bonus];
					if (temp__bonus_p->exist)
					{
						if (temp__bonus_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						found_bonuses++;
					}
				end_of_bonus_cycle:;
				}
				bonuses_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsCollisionsWithBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bomb* temp__bomb_p;
	Bullet* temp__bullet_p;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bomb_p->Entity::IsCollision(temp__bullet_p))
					{
						if (temp__bomb_p->IsBoom())
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						else if (!temp__bomb_p->IsActive())
						{
							temp__bomb_p->Activate();
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
					}
					found_bullets++;
				}
			}
		end_of_bullet_cycle:
			bullets_array_mtx.unlock();
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsSpawnedByBulletsAnigilation()
{
	GameTypes::entities_count_t second_bullet, found_second_bullets;
	Bullet* temp__bullet1_p;
	Bullet* temp__bullet2_p;

	bombs_array_mtx.lock();
	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t first_bullet = 0, found_first_bullets = 0; found_first_bullets < bullets_count; first_bullet++)
	{
		temp__bullet1_p = &bullets[first_bullet];
		if (temp__bullet1_p->exist)
		{
			if (temp__bullet1_p->is_ignore == BULLET_IGNORE_NOTHING)
			{
				for (second_bullet = first_bullet + 1, found_second_bullets = found_first_bullets + 1; found_second_bullets < bullets_count; second_bullet++)
				{
					temp__bullet2_p = &bullets[second_bullet];
					if (temp__bullet2_p->exist)
					{
						if (temp__bullet2_p->is_ignore == BULLET_IGNORE_NOTHING)
						{
							if (temp__bullet2_p->Entity::IsCollision(temp__bullet1_p))
							{
#define BOMB_BY_BULLET__BOOM_PERIOD	75
								Vec2F bomb_position = temp__bullet1_p->GetPosition();
								Vec2F bomb_velocity = (temp__bullet1_p->GetVelocity() + temp__bullet2_p->GetVelocity()) / 2.0f;
								AddEntity(
									Bomb(
										&bomb_position,
										&bomb_velocity,
										temp__bullet1_p->GetPlayerMasterTeamNumber(),
										temp__bullet2_p->GetPlayerMasterTeamNumber(),
										BOMB_BY_BULLET__BOOM_PERIOD,
										0.0f,
										0.0f,
										DEFAULT_FORCE_COLLISION_COEFFICIENT,
										BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
										BOMB_BOOM_RADIUS / 3.0f,
										BOMB_BOOM,
										true));
								RemoveEntity(temp__bullet1_p);
								RemoveEntity(temp__bullet2_p);
								goto end_of_bullet1_cycle;
							}
						}
						found_second_bullets++;
					}
				}
			}
			found_first_bullets++;
		end_of_bullet1_cycle:;
		}
	}
	bullets_array_mtx.unlock();
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Bomb* temp__bomb_p;
	Knife* temp__knife_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				knifes_array_mtx.lock();
				for ( knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__knife_p->exist)
					{
						if (temp__knife_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__knife_p);
							goto end_of_knife_cycle;
						}
						found_knifes++;
					}
				end_of_knife_cycle:;
				}
				knifes_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyTurels()
{
	GameTypes::entities_count_t turel, found_turels;
	Bomb* temp__bomb_p;
	Turel* temp__turel_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				turels_array_mtx.lock();
				for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
				{
					temp__turel_p = &turels[turel];
					if (temp__turel_p->exist)
					{
						if (temp__turel_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__turel_p);
							goto end_of_turel_cycle;
						}
						found_turels++;
					}
				end_of_turel_cycle:;
				}
				turels_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BulletsDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Bullet* temp__bullet_p;

	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(temp__bullet_p))
					{
						bonuses_array_mtx.lock();
						DestroyEntity(temp__bullet_p, temp__asteroid_p);
						bonuses_array_mtx.unlock();
						RemoveEntity(temp__bullet_p);
						goto end_of_asteroid_cycle;
					}
					found_asteroids++;
				}
			end_of_asteroid_cycle:;
			}
			asteroids_array_mtx.unlock();
			found_bullets++;
		}
	}
	bullets_array_mtx.unlock();
}

void Game::BulletsDestroedByMap()
{
	Bullet* temp__bullet_p;

	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			map_data_mtx.lock();
			if (temp__bullet_p->Entity::IsCollision(&map))
			{
				map_data_mtx.unlock();
				RemoveEntity(temp__bullet_p);
				goto end_of_map_cycle;
			}
			map_data_mtx.unlock();
			found_bullets++;
		}
	end_of_map_cycle:;
	}
	bullets_array_mtx.unlock();
}

void Game::CameraFocusesOnPlayers()
{
	camera_data_mtx.lock();
	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	camera.Focus(ships, pilots, ships_count, pilots_count);
	pilots_array_mtx.unlock();
	ships_array_mtx.unlock();
	camera_data_mtx.unlock();
}

void Game::KnifesDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Knife* temp__knife_p;
	Segment temp__segment;
	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					temp__segment = temp__knife_p->GetSegment();
					if (temp__asteroid_p->Entity::IsCollision(&temp__segment))
					{
						bonuses_array_mtx.lock();
						DestroyEntity(temp__knife_p, temp__asteroid_p);
						bonuses_array_mtx.unlock();
						if (temp__knife_p->LoseHealth() == false)
						{
							RemoveEntity(temp__knife_p);
							asteroids_array_mtx.unlock();
							goto end_of_knife_cycle;
						}
					}
					found_asteroids++;
				}
			}
			asteroids_array_mtx.unlock();
			found_knifes++;
		end_of_knife_cycle:;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Knife* temp__knife_p;

	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->IsCollision(temp__knife_p->GetSegment()))
					{
						DestroyEntity(temp__bullet_p, temp__knife_p);
						RemoveEntity(temp__bullet_p);
						bullets_array_mtx.unlock();
						goto end_of_knife_cycle;
					}
					found_knifes++;
				}
			}
			bullets_array_mtx.unlock();
			found_bullets++;
		}
	end_of_knife_cycle:;
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyMap()
{

}

void Game::KnifesDestroyTurels()
{

}

void Game::LasersDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Beam temp__beam;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					temp__beam = temp__laser_p->GetBeam();
					if (temp__asteroid_p->Entity::IsCollision(&temp__beam))
					{
						bonuses_array_mtx.lock();
						DestroyEntity(temp__laser_p, temp__asteroid_p);
						bonuses_array_mtx.unlock();
						goto end_of_asteroid_cycle;
					}
					found_asteroids++;
				}
			end_of_asteroid_cycle:;
			}
			asteroids_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bonus* temp__bonus_p;
	Beam temp__beam;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			bonuses_array_mtx.lock();
			for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntity(temp__laser_p, temp__bonus_p);
						goto end_of_bonus_cycle;
					}
					found_bonuses++;
				}
			end_of_bonus_cycle:;
			}
			bonuses_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDetonateBombs()
{

}

void Game::LasersDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Beam temp__beam;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntity(temp__laser_p, temp__bullet_p);
						goto end_of_bullet_cycle;
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			bullets_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyKnifes()
{

}

void Game::LasersDestroyMap()
{

}

void Game::LasersDestroyTurels()
{
	GameTypes::entities_count_t turel, found_turels;
	Beam temp__beam;
	Laser* temp__laser_p;
	Turel* temp__turel_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			turels_array_mtx.lock();
			for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
			{
				temp__turel_p = &turels[turel];
				if (temp__turel_p->exist)
				{
					if (temp__turel_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntity(temp__laser_p, temp__turel_p);
						goto end_of_turel_cycle;
					}
					found_turels++;
				}
			end_of_turel_cycle:;
			}
			turels_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	MegaLaser* temp__mega_laser_p;
	Segment temp__segment;

	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				asteroids_array_mtx.lock();
				for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__asteroid_p->exist)
					{
						if (temp__asteroid_p->IsCollision(&temp__segment))
						{
							bonuses_array_mtx.lock();
							DestroyEntity(temp__mega_laser_p, temp__asteroid_p);
							bonuses_array_mtx.unlock();
							goto end_of_asteroid_cycle;
						}
						found_asteroids++;
					}
				end_of_asteroid_cycle:;
				}
				asteroids_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bonus* temp__bonus_p;
	MegaLaser* temp__mega_laser_p;
	Segment temp__segment;

	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				bonuses_array_mtx.lock();
				for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
				{
					temp__bonus_p = &bonuses[bonus];
					if (temp__bonus_p->exist)
					{
						if (temp__bonus_p->IsCollision(&temp__segment))
						{
							DestroyEntity(temp__mega_laser_p, temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						found_bonuses++;
					}
				end_of_bonus_cycle:;
				}
				bonuses_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDetonateBombs()
{

}

void Game::MegaLasersDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	MegaLaser* temp__mega_laser_p;
	Segment temp__segment;

	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				temp__segment = temp__mega_laser_p->GetSegment();
				bullets_array_mtx.lock();
				for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
				{
					temp__bullet_p = &bullets[bullet];
					if (temp__bullet_p->exist)
					{
						if (temp__bullet_p->IsCollision(&temp__segment))
						{
							DestroyEntity(temp__mega_laser_p, temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						found_bullets++;
					}
				end_of_bullet_cycle:;
				}
				bullets_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyKnifes()
{

}

void Game::MegaLasersDestroyMap()
{

}

void Game::MegaLasersDestroyTurels()
{

}

void Game::PilotsKilledByBombs()
{
	GameTypes::players_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			bombs_array_mtx.lock();
			for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				temp__bomb_p = &bombs[bomb];
				if (temp__bomb_p->exist)
				{
					if (temp__bomb_p->IsBoom() && temp__pilot_p->IsCollision(temp__bomb_p))
					{
						log_data_mtx.lock();
						DestroyEntity(temp__bomb_p, temp__pilot_p);
						log_data_mtx.unlock();
						goto end_of_bomb_cycle;

					}
					found_bombs++;
				}
			}
			found_pilots++;
		end_of_bomb_cycle:
			bombs_array_mtx.unlock();
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByBullet()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::entities_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->is_ignore == BULLET_IGNORE_NOTHING &&
						temp__pilot_p->IsCollision(temp__bullet_p))
					{
						if (game_rules & GAME_RULE_FRENDLY_FIRE && temp__bullet_p->CreatedByTeam(temp__pilot_p))
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						log_data_mtx.lock();
						DestroyEntity(temp__bullet_p, temp__pilot_p);
						log_data_mtx.unlock();
						if (game_rules & GAME_RULE_NEED_KILL_PILOT)
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			bullets_array_mtx.unlock();
			found_pilots++;
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsCheckInput()
{
	Pilot* temp__pilot_p;
	GameTypes::players_count_t pilot_player_number;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			pilot_player_number = temp__pilot_p->GetPlayerNumber();
			input_values_mtx.lock();
			if (shoot_flags[pilot_player_number] == true)
			{
				temp__pilot_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_PILOT);
			}
			if (rotate_flags[pilot_player_number] == true)
			{
				if (rotation_inverse == true)
				{
					temp__pilot_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					temp__pilot_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				temp__pilot_p->SetAngularVelocity(0.0f);
			}
			input_values_mtx.unlock();
			found_pilots++;
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	Pilot* temp__pilot_p;
	Segment temp__segment;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			knifes_array_mtx.lock();
			for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
			{
				temp__knife_p = &knifes[knife];
				if (temp__knife_p->exist)
				{
					temp__segment = temp__knife_p->GetSegment();
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__knife_p->CreatedByTeam(temp__pilot_p)) &&
						temp__pilot_p->IsCollision(&temp__segment))
					{
						log_data_mtx.lock();
						DestroyEntity(temp__knife_p, temp__pilot_p);
						log_data_mtx.unlock();
						knifes_array_mtx.unlock();
						goto end_of_pilot_cycle;
					}
					found_knifes++;
				}
			}
			knifes_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByLasers()
{
	GameTypes::entities_count_t laser, found_lasers;
	Beam temp__beam;
	Laser* temp__laser_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			lasers_array_mtx.lock();
			for (laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
			{
				temp__laser_p = &lasers[laser];
				if (temp__laser_p->exist)
				{
					temp__beam = temp__laser_p->GetBeam();
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__laser_p->CreatedByTeam(temp__pilot_p)) &&
						temp__pilot_p->IsCollision(&temp__beam))
					{
						log_data_mtx.lock();
						DestroyEntity(temp__laser_p, temp__pilot_p);
						log_data_mtx.unlock();
						lasers_array_mtx.unlock();
						goto end_of_pilots_cycle;
					}
					found_lasers++;
				}
			}
			lasers_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilots_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsRespawnAuto()
{
	GameTypes::players_count_t pilot, found_pilots;
	Ship* temp__ship_p;
	Pilot* temp__pilot_p;

	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	for (pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			if (temp__pilot_p->CanRespawn())
			{
				AddEntity(temp__pilot_p->Respawn());
				ships_can_shoot_flags[pilot] = SHIP_DEFAULT_UNBRAKABLE_PERIOD;
				RemoveEntity(temp__pilot_p);
				goto end_of_pilot_cycle;
			}
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	ships_array_mtx.unlock();
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByMegaLaser()
{
	GameTypes::entities_count_t mega_laser, found_mega_lasers;
	MegaLaser* temp__mega_laser_p;
	Pilot* temp__pilot_p;
	Segment temp__segment;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			mega_lasers_array_mtx.lock();
			for (mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
			{
				temp__mega_laser_p = &mega_lasers[mega_laser];
				if (temp__mega_laser_p->exist)
				{
					if (temp__mega_laser_p->IsShooting(global_timer))
					{
						temp__segment = temp__mega_laser_p->GetSegment();
						if (temp__pilot_p->Entity::IsCollision(&temp__segment))
						{
							log_data_mtx.lock();
							DestroyEntity(temp__mega_laser_p, temp__pilot_p);
							log_data_mtx.unlock();
							mega_lasers_array_mtx.unlock();
							goto end_of_pilot_cycle;
						}
					}
					found_mega_lasers++;
				}
			}
			mega_lasers_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::ShipsInfluenceToBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Ship* temp__ship_p;
	Bonus* temp__bonus_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			bonuses_array_mtx.lock();
			for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->GetDistance(temp__ship_p) < temp__ship_p->radius * GAME_SHIP_INFLUENCE_RADIUS_COEFFISIENT)
					{
						if (temp__bonus_p->IsCollision(temp__ship_p))
						{
							if (temp__bonus_p->bonus_inventory & BONUS_RULE_REVERSE)
							{
								if (rotation_inverse)
								{
									rotation_inverse = false;
								}
								else
								{
									rotation_inverse = true;
								}
								rotate_inverse_chage_tic = global_timer;
								temp__bonus_p->bonus_inventory &= BONUS_ALL - BONUS_RULE_REVERSE;
							}
							temp__ship_p->TakeBonus(temp__bonus_p, game_rules & GAME_RULE_TRIPLE_BONUSES);
							RemoveEntity(temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						temp__bonus_p->AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, temp__ship_p->GetPosition());
					}
					found_bonuses++;
				}
			end_of_bonus_cycle:;
			}
			bonuses_array_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsCheckInput()
{
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			input_values_mtx.lock();
			if (ships_can_shoot_flags[ship] > 0)
			{
				ships_can_shoot_flags[ship]--;
			}
			if (rotate_flags[ship] == true)
			{
				if (rotation_inverse == true)
				{
					temp__ship_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					temp__ship_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				temp__ship_p->SetAngularVelocity(0.0f);
			}
			if (burnout_flags[ship] == true)
			{
				burnout_flags[ship] = false;
				temp__ship_p->Burnout(BULLET_DEFAULT_VELOCITY, !rotation_inverse);
			}
			input_values_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsShoot()
{
	Ship* temp__ship_p;
	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			input_values_mtx.lock();
			if (shoot_flags[ship] == true && ships_can_shoot_flags[ship] == 0)
			{
				ShipShoot(temp__ship_p);
			}
			input_values_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsRespawnOrDestroyPilots()
{
	GameTypes::players_count_t pilot, found_pilots;
	Ship* temp__ship_p;
	Pilot* temp__pilot_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			for (pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist)
				{
					if (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE && temp__pilot_p->SameTeams(temp__ship_p))
					{
						SpawnEntity(temp__ship_p, temp__pilot_p);
						goto end_of_pilot_cycle;
					}
					else if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__pilot_p->SameTeams(temp__ship_p)) &&
						temp__ship_p->DynamicEntity::IsCollision(temp__pilot_p))
					{
						DestroyEntity(temp__ship_p, temp__pilot_p);
						goto end_of_pilot_cycle;
					}
					found_pilots++;
				}
			end_of_pilot_cycle:;
			}
			pilots_array_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBombsOrActivateBombs()
{
	GameTypes::players_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				temp__bomb_p = &bombs[bomb];
				if (temp__bomb_p->exist)
				{
					if (temp__bomb_p->IsBoom() && temp__ship_p->IsCollision(temp__bomb_p))
					{
						bonuses_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__bomb_p, temp__ship_p);
						log_data_mtx.unlock();
						bonuses_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					else if (!temp__bomb_p->IsActive() && !temp__bomb_p->CreatedByTeam(temp__ship_p) && temp__ship_p->GetDistance(temp__bomb_p) < BOMB_ACTIVATION_RADIUS)
					{
						temp__bomb_p->Activate();
					}
					found_bombs++;
				}
			}
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if ((temp__bullet_p->is_ignore & BULLET_IGNORE_MUSTER) &&
						temp__bullet_p->CreatedBy(temp__ship_p) &&
						!temp__ship_p->IsColectEntity(temp__bullet_p))
					{
						temp__bullet_p->is_ignore = BULLET_IGNORE_NOTHING;
					}
					if (!temp__ship_p->IsUnbrakable() && 
						temp__bullet_p->is_ignore == BULLET_IGNORE_NOTHING && 
						temp__ship_p->IsCollision(temp__bullet_p))
					{
						if (game_rules & GAME_RULE_FRENDLY_FIRE && temp__bullet_p->CreatedByTeam(temp__ship_p))
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						if (temp__ship_p->HaveBuff(SHIP_BUFF_SHIELD))
						{
							temp__ship_p->SetUnbrakablePeriod(SHIP_DEFAULT_UNBRAKABLE_PERIOD);
							temp__ship_p->SpendBuffNoCheck(SHIP_BUFF_SHIELD);
							goto end_of_bullet_cycle;
						}
						else
						{
							temp__ship_p->AddVelocity(temp__bullet_p->GetVelocity());
							bonuses_array_mtx.lock();
							log_data_mtx.lock();
							DestroyEntity(temp__bullet_p, temp__ship_p);
							log_data_mtx.unlock();
							bonuses_array_mtx.unlock();
							RemoveEntity(temp__bullet_p);
							goto end_of_ship_cycle;
						}
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			found_ships++;
		end_of_ship_cycle:
			bullets_array_mtx.unlock();
			pilots_array_mtx.unlock();
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	Ship* temp__ship_p;
	Segment temp__segment;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			knifes_array_mtx.lock();
			for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
			{
				temp__knife_p = &knifes[knife];
				temp__segment = temp__knife_p->GetSegment();
				if (temp__knife_p->exist)
				{
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__knife_p->CreatedByTeam(temp__ship_p)) &&
						!temp__ship_p->IsUnbrakable() &&
						!temp__knife_p->CreatedBy(temp__ship_p) &&
						temp__ship_p->IsCollision(&temp__segment))
					{
						bonuses_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__knife_p, temp__ship_p);
						RemoveEntity(temp__knife_p);
						log_data_mtx.unlock();
						bonuses_array_mtx.unlock();
						knifes_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					found_knifes++;
				}
			}
			knifes_array_mtx.unlock();
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByLasers()
{
	GameTypes::entities_count_t laser, found_lasers;
	Beam temp__beam;
	Laser* temp__laser_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			lasers_array_mtx.lock();
			for (laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
			{
				temp__laser_p = &lasers[laser];
				if (temp__laser_p->exist)
				{
					temp__beam = temp__laser_p->GetBeam();
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__laser_p->CreatedByTeam(temp__ship_p)) &&
						!temp__ship_p->IsUnbrakable() &&
						!temp__laser_p->CreatedBy(temp__ship_p) &&
						temp__ship_p->IsCollision(&temp__beam))
					{
						bonuses_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__laser_p, temp__ship_p);
						log_data_mtx.unlock();
						bonuses_array_mtx.unlock();
						lasers_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					found_lasers++;
				}
			}
			lasers_array_mtx.unlock();
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByMegaLasers()
{
	GameTypes::entities_count_t mega_laser, found_mega_lasers;
	MegaLaser* temp__mega_laser_p;
	Ship* temp__ship_p;
	Segment temp__segment;

	ships_array_mtx.lock();
	for (GameTypes::entities_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			mega_lasers_array_mtx.lock();
			for (mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
			{
				temp__mega_laser_p = &mega_lasers[mega_laser];
				if (temp__mega_laser_p->exist)
				{
					if (temp__mega_laser_p->IsShooting(global_timer))
					{
						temp__segment = temp__mega_laser_p->GetSegment();
						if (temp__ship_p->IsCollision(&temp__segment))
						{
							bonuses_array_mtx.lock();
							log_data_mtx.lock();
							DestroyEntity(temp__mega_laser_p, temp__ship_p);
							log_data_mtx.unlock();
							bonuses_array_mtx.unlock();
							goto end_of_ship_cycle;
						}
					}
					found_mega_lasers++;
				}
			}
			found_ships++;
		end_of_ship_cycle:
			mega_lasers_array_mtx.unlock();
		}
	}
	ships_array_mtx.unlock();
}

void Game::TurelsShoot()
{
	Turel* temp__turel_p;

	turels_array_mtx.lock();
	for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
	{
		temp__turel_p = &turels[turel];
		if (temp__turel_p->exist)
		{
			if (temp__turel_p->CanShoot(global_timer))
			{
				bullets_array_mtx.lock();
				AddEntity(temp__turel_p->Shoot());
				bullets_array_mtx.unlock();
			}
			found_turels++;
		}
	}
	turels_array_mtx.unlock();
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