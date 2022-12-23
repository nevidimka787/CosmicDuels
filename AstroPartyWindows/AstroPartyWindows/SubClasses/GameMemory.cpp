#include "Game.h"



void Game::AddEntity(AnnihAreaGen new_annih_area_gen)
{
	if (new_annih_area_gen.exist)
	{
		for (GameTypes::entities_count_t annih_area_gen = 0; annih_area_gen < GAME_ANNIH_AREAS_GENS_MAX_COUNT; annih_area_gen++)
		{
			if (annih_area_gens[annih_area_gen].exist == false)
			{
				annih_area_gens[annih_area_gen] = new_annih_area_gen;
				annih_area_gens_count++;
				return;
				//printf("void Game::AddEntity(AnnihAreaGen)::annih_area_gens_count:%3u\n", annih_area_gens_count);
			}
		}
	}
}

void Game::AddEntity(Asteroid new_asteroid)
{
	if (new_asteroid.exist)
	{
		for (GameTypes::entities_count_t asteroid = 0; asteroid < GAME_ASTEROIDS_MAX_COUNT; asteroid++)
		{
			if (asteroids[asteroid].exist == false)
			{
				asteroids[asteroid] = new_asteroid;
				asteroids_count++;
				//printf("Add asteroid: %u.\tAsteroids count: %u.\n", asteroid, asteroids_count);
				return;
			}
		}
	}

	std::cout << "Game::AddEntyty::asteroids_count" << asteroids_count << std::endl;
}

void Game::AddEntity(Bonus new_bonus)
{
	if (new_bonus.exist)
	{
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
}

void Game::AddEntity(Bullet new_bullet)
{
	if (new_bullet.exist)
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
}

void Game::AddEntity(DecelerationArea new_deceler_area)
{
	if (new_deceler_area.exist)
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
}

void Game::AddEntity(GravGen new_grav_gen)
{
	if (new_grav_gen.exist)
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
}

void Game::AddEntity(Knife new_knife)
{
	if (new_knife.exist)
	{
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
}

void Game::AddEntity(Bomb new_bomb)
{
	if (new_bomb.exist)
	{
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
}

void Game::AddEntity(Laser new_laser)
{
	if (new_laser.exist)
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
}

void Game::AddEntity(MegaLaser new_mega_laser)
{
	if (new_mega_laser.exist)
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
}

void Game::AddEntity(Turret new_turret)
{
	if (new_turret.exist)
	{
		for (GameTypes::map_elements_count_t turret = 0; turret < GAME_TURRETS_MAX_COUNT; turret++)
		{
			if (turrets[turret].exist == false)
			{
				turrets[turret] = new_turret;
				turrets_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Particle new_particle)
{
	if (new_particle.exist)
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
}

void Game::AddEntity(DynamicParticle new_particle)
{
	if (new_particle.exist)
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
}

void Game::AddEntity(Portal new_portal)
{
	if (new_portal.exist)
	{
		for (GameTypes::entities_count_t portal = 0; portal < GAME_PORTALS_MAX_COUNT; portal++)
		{
			if (portals[portal].exist == false)
			{
				portals[portal] = new_portal;
				portals_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Pilot new_pilot)
{
	if (new_pilot.exist)
	{
		GameTypes::players_count_t pilot_number = new_pilot.GetPlayerNumber();
		if (pilot_number < GAME_PLAYERS_MAX_COUNT && !pilots[pilot_number].exist)
		{
			pilots[pilot_number] = new_pilot;
			pilots_count++;
		}
	}
}

void Game::AddEntity(Ship new_ship)
{
	if (new_ship.exist)
	{
		GameTypes::players_count_t ship_number = new_ship.GetPlayerNumber();
		if (ship_number < GAME_PLAYERS_MAX_COUNT && !ships[ship_number].exist)
		{
			ships[ship_number] = new_ship;
			ships_count++;
		}
	}
}

void Game::RemoveEntity(AnnihAreaGen* deleting_annih_area_gen)
{
	if (deleting_annih_area_gen->exist)
	{
		deleting_annih_area_gen->exist = false;
		annih_area_gens_count--;
	}
}

void Game::RemoveEntity(Asteroid* deleting_asteroid)
{
	if (deleting_asteroid->exist)
	{
		deleting_asteroid->exist = false;
		asteroids_count--;
	}
}

void Game::RemoveEntity(Bomb* deleting_bomb)
{
	if (deleting_bomb->exist)
	{
		deleting_bomb->exist = false;
		bombs_count--;
	}
}

void Game::RemoveEntity(Bonus* deleting_bonus)
{
	if (deleting_bonus->exist)
	{
		deleting_bonus->exist = false;
		bonuses_count--;
	}
}

void Game::RemoveEntity(Bullet* deleting_bullet)
{
	if (deleting_bullet->exist)
	{
		deleting_bullet->exist = false;
		bullets_count--;
	}
}

void Game::RemoveEntity(DecelerationArea* deleting_deceler_area)
{
	if (deleting_deceler_area->exist)
	{
		deleting_deceler_area->exist = false;
		deceler_areas_count--;
	}
}

void Game::RemoveEntity(DynamicParticle* deleting_dynamic_particle)
{
	if (deleting_dynamic_particle->exist)
	{
		deleting_dynamic_particle->exist = false;
		dynamic_particles_count--;
	}
}

void Game::RemoveEntity(GravGen* deleting_grav_gen)
{
	if (deleting_grav_gen->exist)
	{
		deleting_grav_gen->exist = false;
		grav_gens_count--;
	}
}

void Game::RemoveEntity(Knife* deleting_knife)
{
	if (deleting_knife->exist)
	{
		deleting_knife->exist = false;
		knifes_count--;
	}
}

void Game::RemoveEntity(Laser* deleting_lazer)
{
	if (deleting_lazer->exist)
	{
		deleting_lazer->exist = false;
		lasers_count--;
	}
}

void Game::RemoveEntity(MegaLaser* deleting_mega_lazer)
{
	if (deleting_mega_lazer->exist)
	{
		deleting_mega_lazer->exist = false;
		mega_lasers_count--;
	}
}

void Game::RemoveEntity(Turret* deleting_turret)
{
	if (deleting_turret->exist)
	{
		deleting_turret->exist = false;
		turrets_count--;
	}
}

void Game::RemoveEntity(Particle* deleting_particle)
{
	if (deleting_particle->exist)
	{
		deleting_particle->exist = false;
		particles_count--;
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

void Game::DestroyEntity(Bomb* destroyer, Turret* entity)
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
		if (destroyer->IsCreatedByTeam(entity))
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
		if (destroyer->IsCreatedByTeam(entity))
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

void Game::DestroyEntity(Knife* destroyer, Turret* entity)
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
		if (destroyer->IsCreatedByTeam(entity))
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
		if (destroyer->IsCreatedByTeam(entity))
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

void Game::DestroyEntity(Laser* destroyer, Turret* entity)
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
		if (destroyer->IsSameTeams(entity))
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
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		DecrementScore(entity->GetTeamNumber());
		DecrementPlayersCountInTeam(entity->GetTeamNumber());
	}
	AddEntity(entity->CreateShards(global_timer));
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
			if (knifes[knife].IsCreatedBy(produser))
			{
				RemoveEntity(&knifes[knife]);
			}
			else
			{
				found_knifes++;
			}
		}
	}
	for (GameTypes::entities_count_t annih_area_gen = 0, found_annih_area_gens = 0; found_annih_area_gens < annih_area_gens_count; annih_area_gen++)
	{
		if (annih_area_gens[annih_area_gen].exist)
		{
			if (annih_area_gens[annih_area_gen].IsCreatedBy(produser))
			{
				//found_annih_area_gens++;
				RemoveEntity(&annih_area_gens[annih_area_gen]);
			}
			else
			{
				found_annih_area_gens++;
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
	annih_area_gens = new AnnihAreaGen[GAME_ANNIH_AREAS_GENS_MAX_COUNT];
	annih_area_gens_count = 0;
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
	turrets = new Turret[GAME_TURRETS_MAX_COUNT];
	turrets_count = 0;

	burnout_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	double_clk_timers = new GameTypes::tic_t[GAME_PLAYERS_MAX_COUNT];
	ships_shooting_flags = new bool[GAME_PLAYERS_MAX_COUNT];
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

	annih_area_gens_count = 0;
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
	turrets_count = 0;

	for (GameTypes::entities_count_t entity = 0; entity < UINT16_MAX - 1; entity++)
	{
		if (entity < GAME_ANNIH_AREAS_GENS_MAX_COUNT && annih_area_gens[entity].exist)
		{
			annih_area_gens[entity].exist = false;
		}
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
		if (entity < GAME_TURRETS_MAX_COUNT && turrets[entity].exist)
		{
			turrets[entity].exist = false;
		}
	}

	current_active_menu = &ships_control_menu;
	flag_all_entities_initialisate = true;
	pause_round = false;
}

void Game::MemoryFree()
{
	delete[] annih_area_gens;
	annih_area_gens_count = 0;
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
	delete[] turrets;
	turrets_count = 0;

	players_count = 0;
	delete[] players_in_team;

	delete[] burnout_flags;
	delete[] double_clk_timers;
	delete[] ships_shooting_flags;
	delete[] shoot_flags;
	delete[] rotate_flags;
	delete[] ships_can_shoot_flags;
	delete[] scores;
}

void Game::ClearShipsShootingFlags()
{
	for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
	{
		ships_shooting_flags[ship] = false;
	}
}

//Memory functions.


