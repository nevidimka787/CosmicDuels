#include "Game.h"

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
	if (!new_knife.exist)
	{
		return;
	}
	for (GameTypes::entities_count_t knife = 0; knife < GAME_KNIFES_MAX_COUNT; knife++)
	{
		if (!knifes[knife].exist)
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
	if (!new_particle.exist)
	{
		return;
	}
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

void Game::AddEntity(DynamicParticle new_particle)
{
	for (GameTypes::entities_count_t particle = 0; particle < GAME_DYNAMIC_PARTICLES_MAX_COUNT; particle++)
	{
		if (dynamic_particles[particle].exist == false)
		{
			dynamic_particles[particle] = new_particle;
			dynamic_particles_count++;
			return;
		}
	}
}

void Game::AddEntity(Portal new_portal)
{
	for (GameTypes::entities_count_t portal = 0; portal < GAME_PORTALS_MAX_COUNT; portal++)
	{
		if (portals[portal].exist == false)
		{
			portals[portal] = new_portal;
			portals_count++;
			std::cout << "New Portal was added." << "Pos:" << new_portal.GetPosition() << std::endl;
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

void Game::RemoveEntity(DynamicParticle* deleting_dynamic_particle)
{
	if (deleting_dynamic_particle->exist)
	{
		deleting_dynamic_particle->exist = false;
		Game::dynamic_particles_count--;
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

void Game::RemoveEntity(Laser* deleting_lazer)
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

void Game::RemoveEntity(Turel* deleting_turel)
{
	if (deleting_turel->exist)
	{
		deleting_turel->exist = false;
		Game::turels_count--;
	}
}

void Game::RemoveEntity(Particle* deleting_particle)
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
	AddEntity(entity->CreateShards(global_timer));
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
	AddEntity(entity->CreateShards(global_timer));
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	AddEntity(entity->Destroy());
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
	RemoveEntity(entity);

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
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
		if (destroyer->CreatedByTeam(entity) || destroyer->GetHostTeamNumber() == AGGRESIVE_ENTITY_HOST_ID)
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else if (destroyer->GetHostTeamNumber() != AGGRESIVE_ENTITY_HOST_ID)
		{
			IncrementScore(destroyer->GetHostTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddEntity(entity->CreateShards(global_timer));
	AddBonuses(entity);
	AddEntity(entity->Destroy());
	RemoveEntity(entity);

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
}

void Game::DestroyEntity(Bullet* destroyer, Pilot* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	if (game_rules && GAME_RULE_NEED_KILL_PILOT)
	{
		if (destroyer->CreatedByTeam(entity) || destroyer->GetHostTeamNumber() == AGGRESIVE_ENTITY_HOST_ID)
		{
			DecrementScore(entity->GetTeamNumber());
		}
		else if (destroyer->GetHostTeamNumber() != AGGRESIVE_ENTITY_HOST_ID)
		{
			IncrementScore(destroyer->GetHostTeamNumber());
		}
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	AddEntity(entity->CreateShards(global_timer));
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
}

void Game::DestroyEntity(Knife* destroyer, Asteroid* entity)
{
	if (destroyer->exist == false || entity->exist == false)
	{
		return;
	}
	AddEntity(entity->CreateShards(global_timer));
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
	AddEntity(entity->CreateShards(global_timer));
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
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
	AddEntity(entity->CreateShards(global_timer));
	AddBonuses(entity);
	pilots[entity->GetPlayerNumber()] = entity->Destroy();
	pilots_count++;
	ships[entity->GetPlayerNumber()].exist = false;
	ships_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
	pilots[entity->GetPlayerNumber()].exist = false;
	pilots_count--;

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
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
	AddEntity(entity->CreateShards(global_timer));
	AddBonuses(entity);
	AddEntity(entity->Destroy());
	std::cout << "\nMegaLaser destroys ship\n";
	RemoveEntity(entity);

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
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
	AddEntity(entity->CreateShards(global_timer));
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
	AddEntity(entity->CreateShards(global_timer));
	RemoveEntity(entity);
}

void Game::DestroyEntity(Asteroid* entity)
{
	AddEntity(entity->CreateShards(global_timer));
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bonus* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Bullet* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Knife* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Ship* entity)
{
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		DecrementScore(entity->GetTeamNumber());
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	DestroySupportEntitiesBy(entity);
	AddEntity(entity->Destroy());
	RemoveEntity(entity);
}

void Game::DestroyEntity(Particle* entity)
{
	RemoveEntity(entity);
}

void Game::DestroyEntity(Pilot* entity)
{
	entity->exist = false;
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		DecrementScore(entity->GetTeamNumber());
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	AddEntity(entity->CreateShards(global_timer));
	pilots_count--;
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
		IncrementPlayersCountInTeam(spawner->GetTeamNumber());
		AddEntity(pilot->Respawn());
		RemoveEntity(pilot);

		camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
		camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
	}
}

void Game::AddBonuses(Ship* spawner)
{
	Bonus ship_bonus = spawner->LoseBonus();
	while (ship_bonus.CanDivision())
	{
		AddEntity(ship_bonus.Division());
	}
	AddEntity(ship_bonus);
}

//Memory functions.

void Game::MemoryLock()
{
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
	dynamic_particles = new DynamicParticle[GAME_PARTICLES_MAX_COUNT];
	dynamic_particles_count = 0;
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
	particles = new Particle[GAME_PARTICLES_MAX_COUNT];
	particles_count = 0;
	portals = new Portal[GAME_PORTALS_MAX_COUNT];
	portals_count = 0;
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
	stuning_timer = 0;
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
	dynamic_particles_count = 0;
	grav_gens_count = 0;
	knifes_count = 0;
	lasers_count = 0;
	mega_lasers_count = 0;
	particles_count = 0;
	portals_count = 0;
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
		if (entity < GAME_DYNAMIC_PARTICLES_MAX_COUNT && dynamic_particles[entity].exist)
		{
			dynamic_particles[entity].exist = false;
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
		if (entity < GAME_PORTALS_MAX_COUNT && portals[entity].exist)
		{
			portals[entity].exist = false;
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
	delete[] portals;
	portals_count = 0;
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


