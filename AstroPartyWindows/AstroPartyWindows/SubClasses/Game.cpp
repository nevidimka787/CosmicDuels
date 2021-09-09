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


void Game::Update()
{
	global_timer++;
	//update

	UpdateMap();

	UpdateBullets();
	UpdateShips();
	UpdatePilots();
	UpdateTurels();
	UpdateBombs();
	UpdateLazers();
	UpdateMegaLazers();
	UpdateKnifes();

	UpdateCamera();

	CheckEndMatch();

	//update

	//collisions
	
	DynamicEntitiesCollisions(ships, ships_count);
	DynamicEntitiesCollisions(pilots, pilots_count);
	DynamicEntitiesCollisions(asteroids, asteroids_count);

	DynamicEntitiesCollisions(ships, pilots, ships_count, pilots_count);
	DynamicEntitiesCollisions(ships,  asteroids, ships_count, asteroids_count);
	DynamicEntitiesCollisions(asteroids,  pilots, asteroids_count, pilots_count);
	
	DynamicEntitiesCollisions(&map, ships, ships_count);
	DynamicEntitiesCollisions(&map, pilots, pilots_count);
	DynamicEntitiesCollisions(&map, asteroids, asteroids_count);
	
	DynamicEntitiesAddForce(grav_gens, grav_gens_count, ships, ships_count);
	DynamicEntitiesAddForce(grav_gens, grav_gens_count, pilots, pilots_count);
	DynamicEntitiesAddForce(grav_gens, grav_gens_count, asteroids, asteroids_count);
	DynamicEntitiesAddForce(grav_gens, grav_gens_count, bullets, asteroids_count);
	
	//collisions

	//recalculation

	TransportAsteroids();
	TransportBombs();
	TransportBonuses();
	TransportBullets();
	TransportKnifes();
	TransportLasers();
	TransportMegaLasers();
	TransportPilots();
	TransportShips();
	//recalculation

	if (flag_end_match)
	{
		EndMatch();
	}

	if (camera.move_velocity_coefficient < 1000.0f)
	{
		camera.move_velocity_coefficient *= 1.01f;
		camera.resize_velocity_coefficient *= 1.01f;
	}
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
void Game::DynamicEntitiesAddForce(GravGen* grav_gen, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	float len2;
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			force = entities[i].GetPosition() - grav_gen->GetPosition();
			len2 = force.x * force.x + force.y * force.y;
			force /= len2 * sqrt(len2) / grav_gen->gravity;
			entities[i].AddForce(&force);

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

EngineTypes::Bonus::bonus_t Game::GenerateRandomBonus()
{
	return 1 << (((rand() % BONUS_BONUSES_COUNT)) * 2);
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
	if (asteroids_count < GAME_ASTEROIDS_MAX_COUNT)
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
}

void Game::AddEntity(Bonus new_bonus)
{
	if (bonuses_count < GAME_BONUSES_MAX_COUNT)
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
	if (new_bullet.exist && bullets_count < GAME_BULLETS_MAX_COUNT)
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

void Game::AddEntity(Knife new_knife)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (GameTypes::entities_count_t particle = 0; particle < GAME_KNIFES_MAX_COUNT; particle++)
		{
			if (knifes[particle].exist == false)
			{
				knifes[particle] = new_knife;
				knifes_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Bomb new_mine)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (GameTypes::entities_count_t particle = 0; particle < GAME_BOMBS_MAX_COUNT; particle++)
		{
			if (bombs[particle].exist == false)
			{
				bombs[particle] = new_mine;
				bombs_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Laser new_lazer)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (GameTypes::entities_count_t particle = 0; particle < GAME_LASERS_MAX_COUNT; particle++)
		{
			if (lasers[particle].exist == false)
			{
				lasers[particle] = new_lazer;
				lasers_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(DynamicEntity new_particle)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
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

void Game::RemoveEntity(Asteroid* deleting_asteroid)
{
	if (deleting_asteroid->exist)
	{
		deleting_asteroid->exist = false;
		Game::asteroids_count--;
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

void Game::RemoveEntity(Knife* deleting_knife)
{
	if (deleting_knife->exist)
	{
		deleting_knife->exist = false;
		Game::knifes_count--;
	}
}

void Game::RemoveEntity(Bomb* deleting_mine)
{
	if (deleting_mine->exist)
	{
		deleting_mine->exist = false;
		Game::bombs_count--;
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

void Game::RemoveEntity(Turel * deleting_turel)
{
	if (deleting_turel->exist)
	{
		deleting_turel->exist = false;
		Game::turels_count--;
	}
}

void Game::RemoveParticle(DynamicEntity * deleting_particle)
{
	if (deleting_particle->exist)
	{
		deleting_particle->exist = false;
		Game::particles_count--;
	}
}



template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::RemoveEntity(asteroid);
}

template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		GameTypes::score_t* current_score_p = &scores[pilot->GetTeamNumber()];
		if (*current_score_p > 0)
		{
			(*current_score_p)--;
		}
	}
	camera.SetCoefficients();
	pilot->exist = false;
	pilots_count--;
}

template <typename AggressiveEntityType>
void Game::DestroyEntityByAggressiveEntity(AggressiveEntityType* destried_entity, Ship* ship)
{
	if (destried_entity->exist == false || ship->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		GameTypes::score_t* current_score_p = &scores[ship->GetTeamNumber()];
		if (current_score_p > 0)
		{
			(*current_score_p)--;
		}
	}
	camera.SetCoefficients();
	ship->exist = false;
	ships_count--;
}



template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->KillerEntity::GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			DecrementScore(pilot->GetTeamNumber());
			std::cout << "Pilot auto kill by team: " << (int)pilot->GetTeamNumber() << std::endl;
		}
		else
		{
			IncrementScore(destried_entity->KillerEntity::GetPlayerMasterTeamNumber());
			std::cout << "Pilot from team: " << (int)pilot->GetTeamNumber() << " kill by team: " << (int)destried_entity->KillerEntity::GetPlayerMasterTeamNumber() << std::endl;
		}
		DecrementPlayersCountInTeam(pilot->GetTeamNumber());
	}
	camera.SetCoefficients();
	pilot->exist = false;
	pilots_count--;
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Ship* ship)
{
	if (destried_entity->exist == false || ship->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->KillerEntity::GetPlayerMasterTeamNumber() == ship->GetTeamNumber())
		{
			DecrementScore(ship->GetTeamNumber());
		}
		else
		{
			IncrementScore(destried_entity->KillerEntity::GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(ship->GetTeamNumber());
	}
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			if (temp__knife_p->GetPlayerMasterNumber() == ship->GetPlayerNumber())
			{
				RemoveEntity(temp__knife_p);
			}
			found_knifes++;
		}
	}
	temp__bonus = ship->LoseBonus();
	Bonus new_bonus = temp__bonus.Division();
	while (new_bonus.exist)
	{
		Game::AddEntity(new_bonus);
		new_bonus = temp__bonus.Division();
	}
	camera.SetCoefficients();
	Game::AddEntity(temp__bonus);
	pilots[ship->GetPlayerNumber()] = ship->Destroy();
	pilots_count++;
	ship->exist = false;
	ships_count--;
}

template <typename KillerEntityType>
void Game::DestroyEntityByKillerEntity(KillerEntityType* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::RemoveEntity(turel);
}



void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::RemoveEntity(asteroid);
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			DecrementScore(pilot->GetTeamNumber());
		}
		else
		{
			IncrementScore(destried_entity->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(pilot->GetTeamNumber());
	}
	camera.SetCoefficients();
	pilot->exist = false;
	pilots_count--;
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Ship* ship)
{
	if (destried_entity->exist == false || ship->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == ship->GetTeamNumber())
		{
			DecrementScore(ship->GetTeamNumber());
		}
		else
		{
			IncrementScore(destried_entity->GetPlayerMasterTeamNumber());
		}
		DecrementPlayersCountInTeam(ship->GetTeamNumber());
	}
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			if (temp__knife_p->GetPlayerMasterNumber() == ship->GetPlayerNumber())
			{
				RemoveEntity(temp__knife_p);
			}
			found_knifes++;
		}
	}
	temp__bonus = ship->LoseBonus();
	Bonus new_bonus = temp__bonus.Division();
	while (new_bonus.exist)
	{
		Game::AddEntity(new_bonus);
		new_bonus = temp__bonus.Division();
	}
	camera.SetCoefficients();
	Game::AddEntity(temp__bonus);
	pilots[ship->GetPlayerNumber()] = ship->Destroy();
	pilots_count++;
	ship->exist = false;
	ships_count--;
}

void Game::DestroyEntityByAggressiveEntity(Laser* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::RemoveEntity(turel);
}



void Game::DestroyEntityByAggressiveEntity(Ship* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetTeamNumber() == pilot->GetTeamNumber())
		{
			DecrementScore(pilot->GetTeamNumber());
		}
		else
		{
			IncrementScore(destried_entity->GetTeamNumber());
		}
		DecrementPlayersCountInTeam(pilot->GetTeamNumber());
	}
	camera.SetCoefficients();
	pilot->exist = false;
	pilots_count--;
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
		ships[pilot->GetPlayerNumber()] = pilot->Respawn();
		ships_count++;
		pilot->exist = false;
		pilots_count--;
	}
}

Game::Game()
{
	InitGame();
	InitMenus();
}

void Game::InitGame()
{
	game_rules = GAME_RULE_DEFAULT_GAME_RULES;
	global_timer = 0;

	teams = new GameTypes::entities_count_t[GAME_PLAYERS_MAX_COUNT];
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		teams[i] = 0;
	}
	object_pull_array = new bool[GAME_OBJECT_TYPES_COUNT];
	map_pull_array = new bool[GAME_MAPS_COUNT];
	for (GameTypes::objects_types_count_t i = 0; i < GAME_OBJECT_TYPES_COUNT; i++)
	{
		object_pull_array[i] = true;
	}
	srand((unsigned int)ships * (unsigned int)pilots);
}

void Game::InitMatch()
{
	players_count = 0;
	play_round = true;
	play_match = true;
	flag_end_match = false;
	for (uint8_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		if (menu_p__ships_select_buttons[i] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			players_count++;
		}
	}

	MemoryLock();
	
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		scores[player] = 0;
	}

	start_bonus = BONUS_NO_BONUS;
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD)
	{
		start_bonus |= game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD ? BUFF_SHIELD : 0x0000;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS)
	{
		start_bonus |= game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS ? BUFF_TRIPLE : 0x0000;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)
	{
		if (game_rules & GAME_RULE_START_BONUS_RANDOMIZE)
		{
			start_bonus |= GenerateRandomBonus();
		}
		else
		{
			start_bonus |= *menu_p__start_bonus;
		}
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

	end_match_score = 4;
}

void Game::InitLevel()
{
	play_round = true;

	//std::cout << "Memory set default." << std::endl;

	MemorySetDefault();

	//std::cout << "Creating map." << std::endl;

	current_map_id = selected_maps_id_array[rand() % selected_maps_id_array_length];

	Vec2F temp_positions[GAME_PLAYERS_MAX_COUNT];

	const GameTypes::score_t max_score = GetMaxScore();

	Segment new_segment;
	Rectangle rectangles[2];

	switch (current_map_id)
	{
	case MAP_SQUARE_ON_CENTER:
		/* Create map */

		new_segment.Set(Vec2F(-2.0f, -2.0f), Vec2F(2.0f, 2.0f), true);
		rectangles[0].Set(&new_segment);
		new_segment.Set(Vec2F(-0.5f, -0.5f), Vec2F(0.5f, 0.5f), true);
		rectangles[1].Set(&new_segment);

		map.Set(rectangles, 2);
		cyrcles_count = 0;
		polygons_count = 0;
		rectangles_count = 2;


		/*Spawn entities*/
		temp_positions[0].Set(-1.9f, 1.9f);
		temp_positions[1].Set(1.9f, 1.9f);
		temp_positions[2].Set(1.9f, -1.9f);
		temp_positions[3].Set(-1.9f, -1.9f);

		break;
	case MAP_TEST_MAP:
	default:
		/* Create map */

		new_segment.Set(Vec2F(-2.0f, -2.0f), Vec2F(2.0f, 2.0f), true);
		rectangles[0].Set(&new_segment);

		map.Set(rectangles, 1);
		cyrcles_count = 0;
		polygons_count = 0;
		rectangles_count = 1;
		

		/*Spawn entities*/
		temp_positions[0].Set(-1.9f, 1.9f);
		temp_positions[1].Set(1.9f, 1.9f);
		temp_positions[2].Set(1.9f, -1.9f);
		temp_positions[3].Set(-1.9f, -1.9f);

		break;
	}

	/* Spawn players */

	//std::cout << "Spawning players" << std::endl;

	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		ships_can_shoot_flags[player] = SHIP_UNBRAKABLE_PERIOD;
		players_in_team[player] = 0;
	}

	float temp_angles[GAME_PLAYERS_MAX_COUNT];
	temp_angles[0] = -(float)M_PI_4;
	temp_angles[1] = -(float)M_PI_2 - (float)M_PI_4;
	temp_angles[2] = (float)M_PI_2 + (float)M_PI_4;
	temp_angles[3] = (float)M_PI_4;

	if (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)
	{
		for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
		{
			temp_angles[i] += (float)rand() / (float)RAND_MAX;
		}
	}

	if (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)
	{
		for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
		{
			GameTypes::players_count_t temp1 = rand() % GAME_PLAYERS_MAX_COUNT;
			Vec2F temp2 = temp_positions[temp1];
			temp_positions[temp1] = temp_positions[i];
			temp_positions[i] = temp2;
		}
	}


	Vec2F zero_velocity;
	players_count = 0;
	for (GameTypes::players_count_t player = 0; player < GAME_PLAYERS_MAX_COUNT; player++)
	{
		if (teams[player] > 0)
		{
			ships[player].Set(
				&temp_positions[player], &zero_velocity,
				player, teams[player],
				(void*)&rotate_flags[player], (void*)&shoot_flags[player],
				nullptr, 0,
				temp_angles[player], start_bonus);

			players_count++;
			IncrementPlayersCountInTeam(teams[player]);
		}
	}
	
	ships_count = players_count;
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
	position.Set(-0.5f, 0.3f);
	size.Set(0.475f, -0.25f);
	buttons[1].Set(BUTTON_ID_GO_TO_OPTINS_MENU, &position, &size, area, "Options", 6);
	position.Set(0.025f, 0.3f);
	buttons[2].Set(BUTTON_ID_EXIT, &position, &size, area, "Exit", 6);
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	main_menu.Set(&position, &size, buttons, 3);
	delete[] buttons;

	//option menu
	buttons = new Button[14];
	size.Set(1.5f, -0.1f);
#define GAME_OPTION_MENU_BORD	0.11f
#define GAME_OPTION_MENU_UP_Y	0.9f
#define GAME_OPTIONS_STAT_X		-0.75f
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 0 * GAME_OPTION_MENU_BORD);
	buttons[0].Set(BUTTON_ID_SET_RANDOM_SPAWN, &position, &size, area, "Random spawn", 5);
	buttons[0].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 1 * GAME_OPTION_MENU_BORD);
	buttons[1].Set(BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION, &position, &size, area, "Random spawn direction", 5);
	buttons[1].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 2 * GAME_OPTION_MENU_BORD);
	buttons[2].Set(BUTTON_ID_SET_SPAWN_THIS_BONUS, &position, &size, area, "Spawn this bonus", 5);
	buttons[2].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 3 * GAME_OPTION_MENU_BORD);
	buttons[3].Set(BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS, &position, &size, area, "Random start bonus", 5);
	buttons[3].status = (game_rules & GAME_RULE_START_BONUS_RANDOMIZE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 4 * GAME_OPTION_MENU_BORD);
	buttons[4].Set(BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BUFF, &position, &size, area, "Triple bonuses", 5);
	buttons[4].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 5 * GAME_OPTION_MENU_BORD);
	buttons[5].Set(BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF, &position, &size, area, "Spawn whis shield", 5);
	buttons[5].status = (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 6 * GAME_OPTION_MENU_BORD);
	buttons[6].Set(BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS, &position, &size, area, "Knifes can destroy bullets", 5);
	buttons[6].status = (game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 7 * GAME_OPTION_MENU_BORD);
	buttons[7].Set(BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, area, "Friendly sheep can restore", 5);
	buttons[7].status = (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 8 * GAME_OPTION_MENU_BORD);
	buttons[8].Set(BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE, &position, &size, area, "Frendly fire", 5);
	buttons[8].status = (game_rules & GAME_RULE_FRENDLY_FIRE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 9 * GAME_OPTION_MENU_BORD);
	buttons[9].Set(BUTTON_ID_SET_PILOT_CAN_RESPAWN, &position, &size, area, "Pilot can respawn", 5);
	buttons[9].status = (game_rules & GAME_RULE_PILOT_CAN_RESPAWN) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 10 * GAME_OPTION_MENU_BORD);
	buttons[10].Set(BUTTON_ID_SET_NEED_KILL_PILOT, &position, &size, area, "Need kill pilot", 5);
	buttons[10].status = (game_rules & GAME_RULE_NEED_KILL_PILOT) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 11 * GAME_OPTION_MENU_BORD);
	buttons[11].Set(BUTTON_ID_GO_TO_SELECT_MAP_MENU, &position, &size, area, "Map pull menu", 5);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 12 * GAME_OPTION_MENU_BORD);
	buttons[12].Set(BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU, &position, &size, area, "Spawning objects menu", 5);
	position.Set(GAME_OPTIONS_STAT_X, GAME_OPTION_MENU_UP_Y - 13 * GAME_OPTION_MENU_BORD);
	buttons[13].Set(BUTTON_ID_SET_ACTIVE_BALANCE, &position, &size, area, "Auto balance", 5);
	buttons[13].status = (game_rules & GAME_RULE_BALANCE_ACTIVE) ? BUTTON_STATUS_ACTIVE : BUTTON_STATUS_INACTIVE;
	position.Set(0.0f, 0.0f);
	size.Set(1.5f, GAME_OPTION_MENU_UP_Y - 1.0f - 15.0f * GAME_OPTION_MENU_BORD);
	option_menu.Set(&position, &size, buttons, 14);
	option_menu.HardRecalculate();
	delete[] buttons;

	//pause menu
	buttons = new Button[2];
	position.Set(-0.4f, 0.825f);
	size.Set(0.8f, -0.2f);
	buttons[0].Set(BUTTON_ID_RESUME_MATCH, &position, &size, area, "Resume", 6);
	position.Set(-0.4f, 0.6f);
	buttons[1].Set(BUTTON_ID_GO_TO_MAIN_MENU, &position, &size, area, "Main menu", 6);
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	pause_menu.Set(&position, &size, buttons, 2);
	delete[] buttons;

	//sheeps select menu
	buttons = new Button[5];;
	size.Set(0.475f, -0.25f);
	position.Set(-0.5f, 0.9f);
	buttons[0].Set(BUTTON_ID_SELECT_SHIP_1, &position, &size, area, "Player 1", 6);
	position.Set(0.025f, 0.9f);
	buttons[1].Set(BUTTON_ID_SELECT_SHIP_2, &position, &size, area, "Player 2", 6);
	position.Set(0.025f, 0.6f);
	buttons[2].Set(BUTTON_ID_SELECT_SHIP_3, &position, &size, area, "Player 3", 6);
	position.Set(-0.5f, 0.6f);
	buttons[3].Set(BUTTON_ID_SELECT_SHIP_4, &position, &size, area, "Player 4", 6);
	position.Set(-0.5f, 0.3f);
	size.Set(1.0f, -0.25f);
	buttons[4].Set(BUTTON_ID_START_GAME, &position, &size, area, "Start game", 7);
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, -1.0f);
	ships_select_menu.Set(&position, &size, buttons, 5);
	delete[] buttons;

	//maps select menu
#define GAME_MAPPULL_MENU_UP_Y		0.9f
#define GAME_MAPPULL_MENU_BORDER	0.55f
	buttons = new Button[GAME_MAPS_COUNT];
	size.Set(0.5f, -0.25f);
	for (uint8_t i = 0; i < GAME_MAPS_COUNT; i++)
	{
		position.Set(-0.5f + (float)(i % 2) * GAME_MAPPULL_MENU_BORDER, GAME_MAPPULL_MENU_UP_Y - (float)(i / 2) * GAME_MAPPULL_MENU_BORDER);
		buttons[i].Set(BUTTON_ID_SELECT_MAP + i, &position, &size, area, "", 5, BUTTON_STATUS_ACTIVE);
	}
	buttons[0].SetText("Test");
	buttons[1].SetText("Square");
	position.Set(0.0f, 0.0f);
	size.Set(1.0f, GAME_MAPPULL_MENU_BORDER * (float)(((GAME_MAPS_COUNT + 1) / 2) + 1));
	map_pull_select_menu.Set(&position, &size, buttons, GAME_MAPS_COUNT);
	delete[] buttons;

	//spawning objects select menu
	buttons = new Button[1];
	position.Set(-0.5f, 0.9f);
	size.Set(0.475f, -0.475f);
	buttons[0].Set(BUTTON_ID_SELECT_OBJECT_ASTEROID, &position, &size, area, "Asteroid", 6);
	position.Set(0.0f, 0.0f);
	size.Set(0.5f, -0.5f);
	spawning_objects_select_menu.Set(&position, &size, buttons, 1);
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
	std::cout << "Next level." << std::endl;
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
	std::cout << "NextLevel:: mx_score: " << (int)max_score << " win_found: " << (bool)winner_found << std::endl;
	if (max_score >= end_match_score && winner_found)
	{
		flag_end_match = true;
		std::cout << "End match." << std::endl;
	}
	else
	{
		play_round = false;
		std::cout << "Complete match." << std::endl;
	}
}

void Game::EndMatch()
{
	play_match = false;
	play_round = false;
	flag_all_entities_initialisate = false;

	MemoryFree();

	flag_update_end_match = false;
	object_p__menu_functions->OpenMainMenu();
}

void Game::DecrementPlayersCountInTeam(GameTypes::players_count_t team_number)
{
	if (team_number > 0 && players_in_team[team_number - 1])
	{
		std::cout << "Team number: " << (unsigned)team_number << " lost player." << std::endl;
		players_in_team[team_number - 1]--;
	}
	flag_update_end_match = true;
}

void Game::IncrementPlayersCountInTeam(GameTypes::players_count_t team_number)
{
	if (team_number > 0)
	{
		std::cout << "Team number: " << (unsigned)team_number << " add player." << std::endl;
		players_in_team[team_number - 1]++;
	}
	flag_update_end_match = true;
}

void Game::CheckEndMatch()
{
	if (end_match_tik)
	{
		if (global_timer > end_match_tik)
		{
			NextLevel();
		}
	}
	else if(flag_update_end_match)
	{
		std::cout << "Update CheckEndMatch" << std::endl;
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
		std::cout << "Round finishing." << std::endl;
		end_match_tik = global_timer + GAME_END_MATCH_DELLAY;
		flag_update_end_match = false;
	}
}

//Memory functions.

void Game::MemoryLock()
{
	particles = new DynamicEntity[GAME_PARTICLES_MAX_COUNT];
	particles_count = 0;
	asteroids = new Asteroid[GAME_ASTEROIDS_MAX_COUNT];
	asteroids_count = 0;
	bombs = new Bomb[GAME_BOMBS_MAX_COUNT];
	bombs_count = 0;
	bonuses = new Bonus[GAME_BONUSES_MAX_COUNT];
	bonuses_count = 0;
	bullets = new Bullet[GAME_BULLETS_MAX_COUNT];
	bullets_count = 0;
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

	shoot_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	rotate_flags = new bool[GAME_PLAYERS_MAX_COUNT];
	ships_can_shoot_flags = new GameTypes::tic_t[GAME_PLAYERS_MAX_COUNT];
	scores = new GameTypes::score_t[GAME_PLAYERS_MAX_COUNT];
	players_in_team = new GameTypes::players_count_t[GAME_PLAYERS_MAX_COUNT];
}

void Game::MemorySetDefault()
{
	global_timer = 0;
	end_match_tik = 0;

	camera.SetCoefficients();
	camera.SetHightLimits();
	camera.SetLowLimits(0.4f * GAME_ENGINE_AREA_SIZE, 0.4f * GAME_ENGINE_AREA_SIZE);
	camera.SetScale(object_p__open_gl_realisation->GetScale());

	asteroids_count = 0;
	bombs_count = 0;
	bonuses_count = 0;
	bullets_count = 0;
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
}

//Memory functions.

void Game::ShipShoot(Ship* ship)
{
	if (ships_can_shoot_flags[ship->GetPlayerNumber()] > 0)
	{
		return;
	}

	//Magicka and Magicka 2 are the best games I've seen.

	ships_can_shoot_flags[ship->GetPlayerNumber()] = GAME_DELLAY_BETWEEN_SHOOTS;

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
		Laser(ship);
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
		Bomb(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_KNIFE))
	{
		Knife(ship);
		return;
	}
	ShipShoot_NoBonus(ship);
	return;
}

void Game::ShipShoot_LaserLoopBombKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserLoopBomb(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserLoopKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserBombKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LoopBombKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserLoop(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserBomb(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LoopBomb(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LaserKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_LoopKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_BombKnife(Ship* ship)
{
	ShipShoot_NoBonus(ship);
}

void Game::ShipShoot_Knife(Ship* ship)
{
	Game::AddEntity(ship->CreateKnife(0));
	Game::AddEntity(ship->CreateKnife(1));
}

void Game::ShipShoot_Bomb(Ship* ship)
{
	Game::AddEntity(ship->CreateBomb());
}

void Game::ShipShoot_Loop(Ship* ship)
{
	for (GameTypes::entities_count_t bullet = 0; bullet < BULLETS_IN_LOOP; bullet++)
	{
		Game::AddEntity(ship->CreateLoop(bullet));
	}
}

void Game::ShipShoot_Laser(Ship* ship)
{
	Game::AddEntity(ship->CreateLaser());
}

void Game::ShipShoot_NoBonus(Ship* ship)
{
	Game::AddEntity(ship->CreateBullet());
}



void Game::IncrementScore(GameTypes::players_count_t team_number)
{
	scores[team_number - 1]++;
}

void Game::DecrementScore(GameTypes::players_count_t team_number)
{
	scores[team_number - 1]--;
}



void Game::TransportAsteroids()
{
	for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
	{
		temp__asteroid_p = &asteroids[asteroid];
		if (temp__asteroid_p->exist)
		{
			temp__asteroid_p->Update();
			found_asteroids++;
		}
	}
}

void Game::TransportBombs()
{
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			temp__bomb_p->Recalculate();
			found_bombs++;
		}
	}
}

void Game::TransportBonuses()
{
	for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
	{
		temp__bonus_p = &bonuses[bonus];
		if (temp__bonus_p->exist)
		{
			temp__bonus_p->Update();
			found_bonuses++;
		}
	}
}

void Game::TransportBullets()
{
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			temp__bullet_p->Update();
			found_bullets++;
		}
	}
}

void Game::TransportKnifes()
{
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			temp__knife_p->Update();
			found_knifes++;
		}
	}
}

void Game::TransportLasers()
{
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__lazer_p = &lasers[laser];
		if (temp__lazer_p->exist)
		{
			temp__lazer_p->Recalculate();
			found_lasers++;
		}
	}
}

void Game::TransportMegaLasers()
{
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_lazer_p = &mega_lasers[mega_laser];
		if (temp__mega_lazer_p->exist)
		{
			temp__mega_lazer_p->Recalculate();
			found_mega_lasers++;
		}
	}
}

void Game::TransportPilots()
{
	for (GameTypes::entities_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			temp__pilot_p->Update();
			temp__pilot_p->UpdateMatrix();
		}
	}
}

void Game::TransportShips()
{
	for (GameTypes::entities_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			temp__ship_p->Update();
			temp__ship_p->UpdateMatrix();
		}
	}
}

void Game::UpdateAsteroids()
{}

void Game::UpdateBombs()
{
	BombsChainReaction();

	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count && bomb < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
				{
					temp__ship_p = &ships[ship];
					if (temp__ship_p->exist && temp__ship_p->Entity::IsCollision(temp__bomb_p))
					{

						DestroyEntityByKillerEntity(temp__bomb_p, temp__ship_p);
					}
				}
				for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
				{
					temp__pilot_p = &pilots[pilot];
					if (temp__pilot_p->exist && temp__pilot_p->Entity::IsCollision(temp__bomb_p))
					{
						DestroyEntityByKillerEntity(temp__bomb_p, temp__pilot_p);
					}
				}
				for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__bonus_p->exist)
					{
						if (temp__asteroid_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntityByKillerEntity(temp__bomb_p, temp__asteroid_p);
						}
						else
						{
							found_asteroids++;
						}
					}
				}
				for (GameTypes::entities_count_t bonus = 0, found_bonus = 0; found_bonus < bonuses_count; bonus++)
				{
					temp__bonus_p = &bonuses[bonus];
					if (temp__bonus_p->exist)
					{
						if (temp__bonus_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntityByKillerEntity(temp__bomb_p, temp__bonus_p);
						}
						else
						{
							found_bonus++;
						}
					}
				}
				for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
				{
					temp__bullet_p = &bullets[bullet];
					if (temp__bullet_p->exist)
					{
						if (temp__bullet_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntityByKillerEntity(temp__bomb_p, temp__bullet_p);
						}
						else
						{
							found_bullets++;
						}
					}
				}
				for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__bullet_p->exist)
					{
						if (temp__knife_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntityByKillerEntity(temp__bomb_p, temp__knife_p);
						}
						else
						{
							found_knifes++;
						}
					}
				}
				for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
				{
					temp__turel_p = &turels[turel];
					if (temp__turel_p->exist)
					{
						if (temp__turel_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntityByKillerEntity(temp__bomb_p, temp__turel_p);
						}
						found_turels++;
					}
				}
			}
			else if (temp__bomb_p->IsActive() == false)
			{
				for (GameTypes::players_count_t sheep = 0; sheep < GAME_PLAYERS_MAX_COUNT; sheep++)
				{
					temp__ship_p = &ships[sheep];
					if (temp__ship_p->exist && temp__ship_p->GetDistance(temp__bomb_p) == 0.0f)
					{
						temp__bomb_p->Activate();
					}
				}
			}
		}
		found_bombs++;
	}
}

void Game::UpdateBonuses()
{}

void Game::UpdateBullets()
{
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (bullets[bullet].exist)
		{
			temp__vector = temp__bullet_p->GetPosition();
			if (temp__bullet_p->Entity::IsCollision(&map) || temp__vector > area_size || temp__vector < -area_size)
			{
				RemoveEntity(temp__bullet_p);
				goto end_of_cycle;
			}
			for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist && temp__ship_p->IsCollision(temp__bullet_p))
				{
					//temp__ship_p->exist&& temp__ship_p->IsCollision(temp__bullet_p);
					if (temp__ship_p->GetActiveBaffs() & BUFF_SHIELD)
					{
						temp__ship_p->BreakShield();
					}
					else
					{
						DestroyEntityByKillerEntity(temp__bullet_p, temp__ship_p);
					}
					RemoveEntity(temp__bullet_p);
					goto end_of_cycle;
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist && temp__pilot_p->IsCollision(temp__bullet_p))
				{
					DestroyEntityByKillerEntity(temp__bullet_p, temp__pilot_p);
					if (game_rules & GAME_RULE_NEED_KILL_PILOT)
					{
						RemoveEntity(temp__bullet_p);
						goto end_of_cycle;
					}
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(temp__bullet_p))
					{
						DestroyEntityByKillerEntity(temp__bullet_p, temp__asteroid_p);
						RemoveEntity(temp__bullet_p);
						goto end_of_cycle;
					}
					found_asteroids++;
				}
			}
			if (game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS)
			{
				for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__bullet_p->exist)
					{
						if (temp__knife_p->Entity::IsCollision(temp__bullet_p))
						{
							DestroyEntityByKillerEntity(temp__bullet_p, temp__knife_p);
							RemoveEntity(temp__bullet_p);
							goto end_of_cycle;
						}
						found_knifes++;
					}
				}
			}
			for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				temp__bomb_p = &bombs[bomb];
				if (temp__bullet_p->exist)
				{
					if (temp__bomb_p->IsActive() == false && temp__bomb_p->IsBoom() == false && temp__bomb_p->Entity::IsCollision(temp__bullet_p))
					{
						temp__bomb_p->Activate();
						RemoveEntity(temp__bullet_p);
						goto end_of_cycle;
					}
					found_bombs++;
				}
			}
			found_bullets++;
		}
	end_of_cycle:;
	}
}
void Game::UpdateCamera()
{
	camera.Focus(ships, pilots, GAME_PLAYERS_MAX_COUNT);
}

void Game::UpdateKnifes()
{
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist && temp__ship_p->GetPlayerNumber() != temp__knife_p->GetPlayerMasterNumber() && temp__ship_p->Entity::IsCollision(temp__knife_p))
				{
					DestroyEntityByKillerEntity(temp__knife_p, temp__ship_p);
					RemoveEntity(temp__knife_p);
					goto end_of_cycle;
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist && temp__pilot_p->Entity::IsCollision(temp__knife_p))
				{
					DestroyEntityByKillerEntity(temp__knife_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(temp__knife_p))
					{
						DestroyEntityByKillerEntity(temp__knife_p, temp__asteroid_p);
						if (temp__knife_p->LoseHealth() == false)
						{
							RemoveEntity(temp__knife_p);
							goto end_of_cycle;
						}
					}
					found_asteroids++;
				}
			}
			found_knifes++;
		end_of_cycle:;
		}
	}
}

void Game::UpdateLazers()
{
	for (GameTypes::entities_count_t laser = 0, found_laser = 0; found_laser < lasers_count; laser++)
	{
		temp__lazer_p = &lasers[laser];
		if (temp__lazer_p->exist)
		{
			temp__beam = temp__lazer_p->GetBeam();
			for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist && temp__ship_p->Entity::IsCollision(&temp__beam))
				{
					DestroyEntityByAggressiveEntity(temp__lazer_p, temp__ship_p);
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist && temp__pilot_p->Entity::IsCollision(&temp__beam))
				{
					DestroyEntityByAggressiveEntity(temp__lazer_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__bonus_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntityByAggressiveEntity(temp__lazer_p, temp__asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &asteroids[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntityByAggressiveEntity(temp__lazer_p, temp__bonus_p);
					}
					else
					{
						found_bonuses++;
					}
				}
			}
			for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bonus_p->exist)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntityByAggressiveEntity(temp__lazer_p, temp__bullet_p);
					}
					else
					{
						found_bullets++;
					}
				}
			}
			for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < bullets_count; turel++)
			{
				temp__turel_p = &turels[turel];
				if (temp__bonus_p->exist)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntityByAggressiveEntity(temp__lazer_p, temp__bullet_p);
					}
					else
					{
						found_turels++;
					}
				}
			}
			found_laser++;
		}
	}
}

void Game::UpdateMap()
{
	void* element_pointer;
	for (EngineTypes::Map::elements_array_length_t element = 0, found_elements = 0; found_elements < rectangles_count; element++)
	{
		element_pointer = (void*)map.GetRectanglePointer(element);
		if (((Rectangle*)element_pointer)->exist)
		{
			((Rectangle*)element_pointer)->Update();
			found_elements++;
		}
	}
}

void Game::UpdateMegaLazers()
{
	return;
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_lazer_p = &mega_lasers[mega_laser];
		if (temp__mega_lazer_p->exist)
		{
			temp__segment = temp__mega_lazer_p->GetSegment();
			for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist && temp__ship_p->Entity::IsCollision(&temp__segment))
				{
					DestroyEntityByAggressiveEntity(temp__mega_lazer_p, temp__ship_p);
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist && temp__pilot_p->Entity::IsCollision(&temp__segment))
				{
					DestroyEntityByAggressiveEntity(temp__mega_lazer_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(&temp__segment))
					{
						//DestroyEntityByAggressiveEntity(temp__mega_lazer_p, temp__asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->Entity::IsCollision(&temp__segment))
					{
						DestroyEntityByAggressiveEntity(temp__mega_lazer_p, temp__bonus_p);
					}
					else
					{
						found_bonuses++;
					}
				}
			}
			for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__segment))
					{
						DestroyEntityByKillerEntity(temp__mega_lazer_p, temp__bullet_p);
					}
					else
					{
						found_bullets++;
					}
				}
			}
			found_mega_lasers++;
		}
	}
}

void Game::UpdatePilots()
{
	for (GameTypes::players_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			if (shoot_flags[pilot] == true)
			{
				temp__pilot_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_PILOT);
			}
			if (rotate_flags[pilot] == true)
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
		}
	}
}

void Game::UpdateShips()
{
	for (GameTypes::players_count_t ship = 0; ship < GAME_PLAYERS_MAX_COUNT; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			temp__ship_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
			for (GameTypes::entities_count_t pilot = 0; pilot < GAME_PLAYERS_MAX_COUNT; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist && temp__pilot_p->Entity::IsCollision(temp__ship_p) == true)
				{
					if (temp__pilot_p->SameTeams(temp__ship_p) && game_rules & GAME_RULE_PILOT_CAN_RESPAWN && game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)
					{
						SpawnEntity(temp__ship_p, temp__pilot_p);
					}
					else
					{
						DestroyEntityByAggressiveEntity(temp__ship_p, temp__pilot_p);
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->GetDistance(temp__ship_p) < temp__ship_p->radius * GAME_SHIP_INFLUENCE_RADIUS_COEFFISIENT)
					{
						if (temp__bonus_p->GetDistance(temp__ship_p) < 0.0f)
						{
							temp__ship_p->TakeBonus(temp__bonus_p);
							RemoveEntity(temp__bonus_p);
						}
						temp__bonus_p->AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, temp__ship_p->GetPosition());
					}
					found_bonuses++;
				}
			}
			if (ships_can_shoot_flags[ship] > 0)
			{
				ships_can_shoot_flags[ship]--;
			}
			if (shoot_flags[ship] == true && ships_can_shoot_flags[ship] == 0)
			{
				ShipShoot(temp__ship_p);
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
		}
	}
}

void Game::UpdateTurels()
{
	for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
	{
		temp__turel_p = &turels[turel];
		if (temp__turel_p->exist)
		{
			if (temp__turel_p->CanShoot(global_timer))
			{
				Game::AddEntity(temp__turel_p->Shoot());
			}
			found_turels++;
		}
	}
}

void Game::BombsChainReaction()
{
	bool recursion = true;
	while (recursion == true)
	{
		recursion = false;
		for (GameTypes::entities_count_t bomb = 0, found_bomb = 0; found_bomb < bombs_count && bomb < GAME_BOMBS_MAX_COUNT; bomb++)
		{
			temp__bomb_p = &bombs[bomb];
			if (temp__bomb_p->exist)
			{
				if (temp__bomb_p->IsBoom() == true)
				{
					Bomb* temp_bomb;
					for (GameTypes::entities_count_t new_bomb = 0, found_new_bomb = 0; found_new_bomb < bombs_count && new_bomb < GAME_BOMBS_MAX_COUNT; new_bomb++)
					{
						temp_bomb = &bombs[new_bomb];
						if (temp_bomb->exist)
						{
							if (new_bomb != bomb && temp_bomb->IsBoom() == false && temp__bomb_p->GetDistance(temp_bomb) == 0.0f)
							{
								recursion = true;
								temp_bomb->Boom();
							}
							found_new_bomb++;
						}
					}
				}
				found_bomb++;
			}
		}
	}
}