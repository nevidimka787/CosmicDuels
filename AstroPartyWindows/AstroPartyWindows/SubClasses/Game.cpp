#include "Game.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 6011)
#pragma warning(disable : 6200)
#pragma warning(disable : 6294)
#pragma warning(disable : 6297)
#pragma warning(disable : 6386)
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 26495)


void Game::Update()
{
	//update
	UpdateAll();

	//update

	//collisions

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count);

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, (DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, (DynamicEntity*)asteroids, asteroids_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count, (DynamicEntity*)pilots, players_count);

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count, map);

	DynamicEntitiesAddForce((DynamicEntity*)ships, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)pilots, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)asteroids, asteroids_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)bullets, asteroids_count, grav_gens, grav_gens_count);

	//collisions

	//recalculation

	TransportAllObjects();

	//recalculation
}

void Game::DynamicEntitiesCollisions(DynamicEntity* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities_count; first++)
	{
		if (entities[first].exist)
		{
			for (GameTypes::entities_count_t second = first + 1, found_entites_count2 = found_entites_count1; found_entites_count2 < entities_count; second++)
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

void Game::DynamicEntitiesCollisions(DynamicEntity* entities1, GameTypes::entities_count_t entities1_count, DynamicEntity* entities2, GameTypes::entities_count_t entities2_count)
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

void Game::DynamicEntitiesCollisions(DynamicEntity* entities, GameTypes::entities_count_t entities_count, Map* map)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count = 0; found_entites_count < entities_count; first++)
	{
		if (entities[first].exist)
		{
			entities[first].Collision(map);
			found_entites_count++;
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, GameTypes::entities_count_t entities_count, Vec2F* force)
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

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, GameTypes::entities_count_t entities_count, GravGen* grav_gen)
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

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, GameTypes::entities_count_t entities_count, GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count)
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

ClassTypes::Bonus::bonus_t Game::GenerateRandomBonus()
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
	for (GameTypes::score_t i = 1; i < players_count; i++)
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
	if (bullets_count < GAME_BULLETS_MAX_COUNT)
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
				bullets_count++;
				return;
			}
		}
	}
}

void Game::RemoveEntity(Asteroid* deleting_asteroid)
{
	deleting_asteroid->exist = false;
	Game::asteroids_count--;
}

void Game::RemoveEntity(Bonus* deleting_bonus)
{
	deleting_bonus->exist = false;
	Game::bonuses_count--;
}

void Game::RemoveEntity(Bullet* deleting_bullet)
{
	deleting_bullet->exist = false;
	Game::bullets_count--;
}

void Game::RemoveEntity(Knife* deleting_knife)
{
	deleting_knife->exist = false;
	Game::knifes_count--;
}

void Game::RemoveEntity(Bomb* deleting_mine)
{
	deleting_mine->exist = false;
	Game::bombs_count--;
}

void Game::RemoveEntity(Laser* deleting_lazer)
{
	deleting_lazer->exist = false;
	Game::lasers_count--;
}

void Game::RemoveEntity(Turel* deleting_turel)
{
	deleting_turel->exist = false;
	Game::turels_count--;
}

void Game::RemoveParticle(DynamicEntity* deleting_particle)
{
	deleting_particle->exist = false;
	Game::particles_count--;
}



void Game::DestroyEntity(AggressiveEntity* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::RemoveEntity(asteroid);
}

void Game::DestroyEntity(AggressiveEntity* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

void Game::DestroyEntity(AggressiveEntity* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

void Game::DestroyEntity(AggressiveEntity* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

void Game::DestroyEntity(AggressiveEntity* destried_entity, Pilot* pilot)
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
	pilot->exist = false;
	pilots_count--;
}

void Game::DestroyEntity(AggressiveEntity* destried_entity, Ship* ship)
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
	ship->exist = false;
	ships_count--;
}



void Game::DestroyEntity(KillerEntity* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
}

void Game::DestroyEntity(KillerEntity* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

void Game::DestroyEntity(KillerEntity* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

void Game::DestroyEntity(KillerEntity* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

void Game::DestroyEntity(KillerEntity* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	pilot->exist = false;
	pilots_count--;
}

void Game::DestroyEntity(KillerEntity* destried_entity, Ship* sheep)
{
	if (destried_entity->exist == false || sheep->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == sheep->GetTeamNumber())
		{
			scores[sheep->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist == true)
		{
			if (temp__knife_p->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
			{
				RemoveEntity(temp__knife_p);
			}
			found_knifes++;
		}
	}
	temp__bonus = sheep->LoseBonus();
	Bonus new_bonus = temp__bonus.Division();
	while (new_bonus.exist == true)
	{
		Game::AddEntity(new_bonus);
		new_bonus = temp__bonus.Division();
	}
	Game::AddEntity(temp__bonus);
	pilots[sheep->GetPlayerNumber()] = sheep->Destroy();
	pilots_count++;
	sheep->exist = false;
	ships_count--;
}

void Game::DestroyEntity(KillerEntity* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::RemoveEntity(turel);
}



void Game::DestroyEntity(Laser* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::RemoveEntity(asteroid);
}

void Game::DestroyEntity(Laser* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bonus);
}

void Game::DestroyEntity(Laser* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::RemoveEntity(bullet);
}

void Game::DestroyEntity(Laser* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::RemoveEntity(knife);
}

void Game::DestroyEntity(Laser* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	pilot->exist = false;
	pilots_count--;
}

void Game::DestroyEntity(Laser* destried_entity, Ship* sheep)
{
	if (destried_entity->exist == false || sheep->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == sheep->GetTeamNumber())
		{
			scores[sheep->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist == true)
		{
			if (temp__knife_p->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
			{
				RemoveEntity(temp__knife_p);
			}
			found_knifes++;
		}
	}
	temp__bonus = sheep->LoseBonus();
	Bonus new_bonus = temp__bonus.Division();
	while (new_bonus.exist == true)
	{
		Game::AddEntity(new_bonus);
		new_bonus = temp__bonus.Division();
	}
	Game::AddEntity(temp__bonus);
	pilots[sheep->GetPlayerNumber()] = sheep->Destroy();
	pilots_count++;
	sheep->exist = false;
	ships_count--;
}

void Game::DestroyEntity(Laser* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::RemoveEntity(turel);
}



void Game::DestroyEntity(Ship* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetTeamNumber()]++;
		}
	}
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
		Game::AddEntity(pilot->Respawn());
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

	menu_p__ships_select_buttons = new GameTypes::players_count_t[GAME_PLAYERS_MAX_COUNT];
	teams = new GameTypes::entities_count_t[GAME_PLAYERS_MAX_COUNT];
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		menu_p__ships_select_buttons[i] = SHIPS_SELECT_BUTTONS_NO_TEAM;
	}

	srand((unsigned int)ships * (unsigned int)pilots);
}

void Game::InitMach()
{
	players_count = 0;
	for (uint8_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		if (menu_p__ships_select_buttons[i] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			players_count++;
		}
	}
	ships_count = players_count;

	ships = new Ship[players_count];
	pilots = new Pilot[players_count];

	knifes = new Knife[players_count << 1];
	knifes_count = 0;
	lasers = new Laser[players_count];
	lasers_count = 0;
	bombs = new Bomb[GAME_BOMBS_MAX_COUNT];
	bombs_count = 0;

	bullets = new Bullet[GAME_BULLETS_MAX_COUNT];
	bullets_count = 0;
	particles = new DynamicEntity[GAME_PARTICLES_MAX_COUNT];
	particles_count = 0;
	asteroids = new Asteroid[GAME_ASTEROIDS_MAX_COUNT];
	asteroids_count = 0;
	bonuses = new Bonus[GAME_BONUSES_MAX_COUNT];
	bonuses_count = 0;

	shoot_flags = new bool[players_count];
	rotate_flags = new bool[players_count];
	scores = new GameTypes::players_count_t[players_count];
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		scores[i] = 0;
	}


	start_bonus = game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD ? BUFF_SHIELD : 0x0000;
	start_bonus |= game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS ? BUFF_TRIPLE : 0x0000;
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

	//create map pull array

	selected_maps_id_array_length = 0;
	for (GameTypes::maps_count_t map_id = 0; map_id < MAPS_COUNT; map_id++)
	{
		if (map_pull_array[map_id] == true)
		{
			selected_maps_id_array_length++;
		}
	}

	if (selected_maps_id_array_length == 0)
	{
		selected_maps_id_array_length = MAPS_COUNT;
	}
	selected_maps_id_array = new GameTypes::maps_count_t[selected_maps_id_array_length];

	for (GameTypes::maps_count_t map_id = 0, found = 0; found < selected_maps_id_array_length; map_id++)
	{
		if (map_pull_array[map_id] == true)
		{
			selected_maps_id_array[found] = map_id;
			found++;
		}
	}

	//create map pull array
}

void Game::InitLevel()
{

	current_map_id = selected_maps_id_array[rand() % selected_maps_id_array_length];


	Vec2F* temp_points = new Vec2F[players_count];;
	Rectangle* temp_rectangles;

	const GameTypes::score_t max_score = GetMaxScore();

	switch (current_map_id)
	{
	case MAP_TEST_MAP:
	default:
		/* Create map */
		temp_points = new Vec2F[players_count];
		temp_points[0] = Vec2F(1.0f, 1.0f);
		temp_points[1] = Vec2F(2.0f - 1.0f / (float)((uint32_t)1 << 23), 2.0f - 1.0f / (float)((uint32_t)1 << 23));
		temp_rectangles = new Rectangle[1];
		temp_rectangles[0] = Rectangle(&temp_points[0], &temp_points[1], true);

		map = new Map(temp_rectangles, 1, nullptr, 0, nullptr, 0, &temp_points[0]);

		/*Spawn entities*/

		temp_points[0] = Vec2F(1.1f, 1.1f);
		temp_points[1] = Vec2F(1.9f, 1.9f);
		if (players_count > 2)
		{
			temp_points[2] = Vec2F(1.1f, 1.9f);
		}
		if (players_count > 3)
		{
			temp_points[3] = Vec2F(1.9f, 1.1f);
		}
		if (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)
		{
			for (uint8_t i = 0; i < RANDOM_ITERATIONS_COUNT; i++)
			{
				GameTypes::players_count_t temp1 = rand() % players_count;
				GameTypes::players_count_t temp2 = rand() % players_count;
				Vec2F temp3 = temp_points[temp1];
				temp_points[temp1] = temp_points[temp2];
				temp_points[temp2] = temp3;
			}
		}

		break;
	}


	float* temp_angles = new float[players_count];
	temp_angles[0] = -(float)M_PI_4;
	temp_angles[1] = (float)M_PI - (float)M_PI_4;
	if (players_count > 2)
	{
		temp_angles[2] = (float)M_PI_4;
	}
	if (players_count > 3)
	{
		temp_angles[3] = (float)M_PI + (float)M_PI_4;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)
	{
		for (GameTypes::players_count_t i = 0; i < players_count; i++)
		{
			int temp1 = rand();
			float temp2 = 1.0f;
			float* temp1_pointer = (float*)&temp1;
			int* temp2_pointer = (int*)&temp2;
			temp1 &= 0x007FFFFF;					//0000 0000 0111 1111 1111 1111 1111 1111 - mantisa
			temp1 |= *temp2_pointer & 0xFF800000;	//1111 1111 1000 0000 0000 0000 0000 0000
			temp_angles[i] += (*temp1_pointer - 1.0f) * 2.0 * (float)M_PI;
		}
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)
	{
		for (GameTypes::players_count_t i = 0; i < players_count; i++)
		{
			Vec2F temp_vel = Vec2F();
			if (game_rules & GAME_RULE_BALANCE_ACTIVE && scores[i] <= max_score - BALANCE_ACTIVATE_DIFFERENCE_SCORES)
			{
				temp__ship = Ship(i, teams[i], (void*)&rotate_flags[i], (void*)&shoot_flags[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus | BUFF_SHIELD);
				Game::AddEntity(temp__ship);
			}
			else
			{
				temp__ship = Ship(i, teams[i], (void*)&rotate_flags[i], (void*)&shoot_flags[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus);
				Game::AddEntity(temp__ship);
			}
		}
	}
}

void Game::InitMenus()
{
	//arrays

	//main menu
	Button* buttons = new Button[3];
	Vec2F position = Vec2F(-0.5f, 0.5f);
	Vec2F size = Vec2F(1.0f, -0.60f);
	buttons[0].Set(BUTTON_ID_START_MATCH, &position, &size, "Play", 16);
	position = Vec2F(-0.5f, -0.15f);
	size = Vec2F(0.475f, -0.25f);
	buttons[1].Set(BUTTON_ID_GO_TO_OPTINS_MENU, &position, &size, "Options", 8);
	position = Vec2F(0.025f, -0.15f);
	buttons[2].Set(BUTTON_ID_EXIT, &position, &size, "Exit", 8);
	main_menu = new Menu(&position, buttons, 3);
	delete[] buttons;

	//option menu
	buttons = new Button[14];
	size = Vec2F(1.0f, -0.1f);
	position = Vec2F(-0.5f, 0.5f - 0 * 0.1f);
	buttons[0].Set(BUTTON_ID_SET_RANDOM_SPAWN, &position, &size, "Random spawn", 6);
	position = Vec2F(-0.5f, 0.5f - 1 * 0.1f);
	buttons[1].Set(BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION, &position, &size, "Random spawn direction", 6);
	position = Vec2F(-0.5f, 0.5f - 2 * 0.1f);
	buttons[2].Set(BUTTON_ID_SET_SPAWN_THIS_BONUS, &position, &size, "Spawn this bonus", 6);
	position = Vec2F(-0.5f, 0.5f - 3 * 0.1f);
	buttons[3].Set(BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS, &position, &size, "Random start bonus", 6);
	position = Vec2F(-0.5f, 0.5f - 4 * 0.1f);
	buttons[4].Set(BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BUFF, &position, &size, "Triple bonuses", 6);
	position = Vec2F(-0.5f, 0.5f - 5 * 0.1f);
	buttons[5].Set(BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF, &position, &size, "Spawn whis shield", 6);
	position = Vec2F(-0.5f, 0.5f - 7 * 0.1f);
	buttons[6].Set(BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS, &position, &size, "Knifes can destroy bullets", 6);
	position = Vec2F(-0.5f, 0.5f - 7 * 0.1f);
	buttons[7].Set(BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, "Friendly sheep can restore", 6);
	position = Vec2F(-0.5f, 0.5f - 8 * 0.1f);
	buttons[8].Set(BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE, &position, &size, "Frendly fire", 6);
	position = Vec2F(-0.5f, 0.5f - 9 * 0.1f);
	buttons[9].Set(BUTTON_ID_SET_PILOT_CAN_RESPAWN, &position, &size, "Pilot can respawn", 6);
	position = Vec2F(-0.5f, 0.5f - 10 * 0.1f);
	buttons[10].Set(BUTTON_ID_SET_NEED_KILL_PILOT, &position, &size, "Need kill pilot", 6);
	position = Vec2F(-0.5f, 0.5f - 11 * 0.1f);
	buttons[11].Set(BUTTON_ID_GO_TO_SELECT_MAP_MENU, &position, &size, "Map", 6);
	position = Vec2F(-0.5f, 0.5f - 12 * 0.1f);
	buttons[12].Set(BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU, &position, &size, "Spawning", 6);
	position = Vec2F(-0.5f, 0.5f - 13 * 0.1f);
	buttons[13].Set(BUTTON_ID_SET_ACTIVE_BALANCE, &position, &size, "Balance", 6);
	position = Vec2F(0.0f, 0.0f);
	option_menu = new Menu(&position, buttons, 14);
	delete[] buttons;

	//pause menu
	buttons = buttons = new Button[2];
	position = Vec2F(-0.2f, 0.2f);
	size = Vec2F(0.4f, -0.2f);
	buttons[0].Set(BUTTON_ID_RESUME_MATCH, &position, &size, "Resume", 16);
	position = Vec2F(-0.2f, 0.0f);
	buttons[1].Set(BUTTON_ID_GO_TO_MAIN_MENU, &position, &size, "Main menu", 8);
	position = Vec2F(0.0f, 0.0f);
	pause_menu = new Menu(&position, buttons, 2);
	delete[] buttons;

	//sheeps select menu
	buttons = buttons = new Button[4];;
	size = Vec2F(0.25f, -0.25f);
	position = Vec2F(-0.5f, 0.5f);
	buttons[0].Set(BUTTON_ID_SELECT_SHIP_1, &position, &size, "Player 1", 6);
	position = Vec2F(0.0f, 0.5f);
	buttons[1].Set(BUTTON_ID_SELECT_SHIP_2, &position, &size, "Player 2", 6);
	position = Vec2F(0.0f, 0.0f);
	buttons[2].Set(BUTTON_ID_SELECT_SHIP_3, &position, &size, "Player 3", 6);
	position = Vec2F(-0.5f, 0.0f);
	buttons[3].Set(BUTTON_ID_SELECT_SHIP_4, &position, &size, "Player 4", 6);
	position = Vec2F();
	ships_select_menu = new Menu(&position, buttons, 4);
	delete[] buttons;

	//maps select menu
	buttons = buttons = new Button[MAPS_COUNT];;
	for (uint8_t i = 0; i < MAPS_COUNT; i++)
	{
		position = Vec2F(-0.5f + (float)(i % 4), 0.5f - (float)(i / 4));
		buttons[i].Set(BUTTON_ID_SELECT_MAP + i - 1, &position, &size, "", 6);
	}
	position = Vec2F();
	map_pull_select_menu = new Menu(&position, buttons, MAPS_COUNT);
	delete[] buttons;

	//spawning objects select menu
	buttons = buttons = new Button[1];
	position = Vec2F(-0.5f, 0.5f);
	buttons[0].Set(BUTTON_ID_SELECT_OBJECT_ASTEROID, &position, &size, "Asteroid", 6);
	position = Vec2F();
	spawning_objects_select_menu = new Menu(&position, buttons, 1);
	delete[] buttons;

	current_active_menu = main_menu;
}



void Game::ShipShoot(Ship* ship)
{
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

}

void Game::ShipShoot_LaserLoopBomb(Ship* ship)
{

}

void Game::ShipShoot_LaserLoopKnife(Ship* ship)
{

}

void Game::ShipShoot_LaserBombKnife(Ship* ship)
{

}

void Game::ShipShoot_LoopBombKnife(Ship* ship)
{

}

void Game::ShipShoot_LaserLoop(Ship* ship)
{

}

void Game::ShipShoot_LaserBomb(Ship* ship)
{

}

void Game::ShipShoot_LoopBomb(Ship* ship)
{

}

void Game::ShipShoot_LaserKnife(Ship* ship)
{

}

void Game::ShipShoot_LoopKnife(Ship* ship)
{

}

void Game::ShipShoot_BombKnife(Ship* ship)
{

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
	Game::AddEntity(ship->CreateLazer());
}

void Game::ShipShoot_NoBonus(Ship* ship)
{
	Game::AddEntity(ship->CreateBullet());
}



void Game::UpdateAll()
{
	UpdateBullets();
	UpdateShips();
	UpdateTurels();
	UpdateBombs();
	UpdateLazers();
	UpdateMegaLazers();
	UpdateKnifes();
}

void Game::TransportAllObjects()
{
	TransportAsteroids();
	TransportBombs();
	TransportBonuses();
	TransportBullets();
	TransportKnifes();
	TransportLasers();
	TransportMegaLasers();
	TransportPilots();
	TransportShips();
}

void Game::TransportAsteroids()
{
	for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
	{
		temp__asteroid_p = &asteroids[asteroid];
		if (temp__asteroid_p->exist == true)
		{
			temp__asteroid_p->Recalculate();
			found_asteroids++;
		}
	}
}

void Game::TransportBombs()
{
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < asteroids_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist == true)
		{
			temp__bomb_p->Recalculate();
			found_bombs++;
		}
	}
}

void Game::TransportBonuses()
{
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < asteroids_count; mega_laser++)
	{
		temp__mega_lazer_p = &mega_lasers[mega_laser];
		if (temp__mega_lazer_p->exist == true)
		{
			temp__mega_lazer_p->Recalculate();
			found_mega_lasers++;
		}
	}
}

void Game::TransportBullets()
{
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < asteroids_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist == true)
		{
			temp__bullet_p->Recalculate();
			found_bullets++;
		}
	}
}

void Game::TransportKnifes()
{
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < asteroids_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist == true)
		{
			temp__knife_p->Recalculate();
			found_knifes++;
		}
	}
}

void Game::TransportLasers()
{
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < asteroids_count; laser++)
	{
		temp__lazer_p = &lasers[laser];
		if (temp__lazer_p->exist == true)
		{
			temp__lazer_p->Recalculate();
			found_lasers++;
		}
	}
}

void Game::TransportMegaLasers()
{
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < asteroids_count; mega_laser++)
	{
		temp__mega_lazer_p = &mega_lasers[mega_laser];
		if (temp__mega_lazer_p->exist == true)
		{
			temp__mega_lazer_p->Recalculate();
			found_mega_lasers++;
		}
	}
}

void Game::TransportPilots()
{
	for (GameTypes::entities_count_t pilot = 0, found_pilots = 0; found_pilots < asteroids_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist == true)
		{
			temp__pilot_p->Recalculate();
			found_pilots++;
		}
	}
}

void Game::TransportShips()
{
	for (GameTypes::entities_count_t ship = 0, found_ships = 0; found_ships < asteroids_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist == true)
		{
			temp__ship_p->Recalculate();
			found_ships++;
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
		if (temp__bomb_p->exist == true)
		{
			if (temp__bomb_p->IsBoom())
			{
				for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
				{
					temp__ship_p = &ships[ship];
					if (temp__ship_p->exist == true && temp__ship_p->IsCollision(temp__bomb_p))
					{

						DestroyEntity(temp__bomb_p, temp__ship_p);
					}
				}
				for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
				{
					temp__pilot_p = &pilots[pilot];
					if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(temp__bomb_p))
					{
						DestroyEntity(temp__bomb_p, temp__pilot_p);
					}
				}
				for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__bonus_p->exist == true)
					{
						if (temp__asteroid_p->IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__asteroid_p);
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
					if (temp__bonus_p->exist == true)
					{
						if (temp__bonus_p->IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__bonus_p);
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
					if (temp__bullet_p->exist == true)
					{
						if (temp__bullet_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__bullet_p);
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
					if (temp__bullet_p->exist == true)
					{
						if (temp__knife_p->IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__knife_p);
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
					if (temp__turel_p->exist == true)
					{
						if (temp__turel_p->IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__turel_p);
						}
						found_turels++;
					}
				}
			}
			else if (temp__bomb_p->IsActive() == false)
			{
				for (GameTypes::players_count_t sheep = 0; sheep < players_count; sheep++)
				{
					temp__ship_p = &ships[sheep];
					if (temp__ship_p->exist == true && temp__ship_p->GetDistance(temp__bomb_p) == 0.0f)
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
		if (temp__bullet_p->exist == true)
		{
			if (temp__bullet_p->IsCollision(map))
			{
				RemoveEntity(temp__bullet_p);
				goto end_of_cycle;
			}
			for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist == true && temp__ship_p->IsCollision(temp__bullet_p))
				{
					if (temp__ship_p->GetActiveBaffs() & BUFF_SHIELD)
					{
						temp__ship_p->BreakShield();
					}
					else
					{
						DestroyEntity(temp__bullet_p, temp__ship_p);
					}
					RemoveEntity(temp__bullet_p);
					goto end_of_cycle;
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(temp__bullet_p))
				{
					DestroyEntity(temp__bullet_p, temp__pilot_p);
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
				if (temp__asteroid_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(temp__bullet_p))
					{
						DestroyEntity(temp__bullet_p, temp__asteroid_p);
						RemoveEntity(temp__bullet_p);
						goto end_of_cycle;
					}
					found_asteroids++;
				}
			}
			if (game_rules & GAME_RULE_KNIFES_CAN_BREAKE_BULLETS)
			{
				for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__bullet_p->exist == true)
					{
						if (temp__knife_p->IsCollision(temp__bullet_p))
						{
							DestroyEntity(temp__bullet_p, temp__knife_p);
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
				if (temp__bullet_p->exist == true)
				{
					if (temp__bomb_p->IsActive() == false && temp__bomb_p->IsBoom() == false && temp__bomb_p->IsCollision(temp__bullet_p))
					{
						temp__bomb_p->Activate();
						RemoveEntity(temp__bullet_p);
						goto end_of_cycle;
					}
					found_bombs++;
				}
			}
		}

		found_bullets++;
	end_of_cycle:;
	}
}

void Game::UpdateKnifes()
{
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist == true)
		{
			for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist == true && temp__ship_p->GetPlayerNumber() != temp__knife_p->GetPlayerMasterNumber() && temp__ship_p->IsCollision(temp__knife_p))
				{
					DestroyEntity(temp__knife_p, temp__ship_p);
					RemoveEntity(temp__knife_p);
					goto end_of_cycle;
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(temp__knife_p))
				{
					DestroyEntity(temp__knife_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(temp__knife_p))
					{
						DestroyEntity(temp__knife_p, temp__asteroid_p);
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
		if (temp__lazer_p->exist == true)
		{
			temp__beam = temp__lazer_p->GetBeam();
			for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist == true && temp__ship_p->IsCollision(&temp__beam))
				{
					DestroyEntity(temp__lazer_p, temp__ship_p);
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(&temp__beam))
				{
					DestroyEntity(temp__lazer_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__bonus_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(&temp__beam))
					{
						DestroyEntity(temp__lazer_p, temp__asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__asteroid_p = &asteroids[bonus];
				if (temp__bonus_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(&temp__beam))
					{
						DestroyEntity(temp__lazer_p, temp__asteroid_p);
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
				if (temp__bonus_p->exist == true)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntity(temp__lazer_p, temp__bullet_p);
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
				if (temp__bonus_p->exist == true)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__beam))
					{
						DestroyEntity(temp__lazer_p, temp__bullet_p);
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

void Game::UpdateMegaLazers()
{
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lazers_count; mega_laser++)
	{
		temp__mega_lazer_p = &mega_lasers[mega_laser];
		if (temp__mega_lazer_p->exist == true)
		{
			temp__segment = temp__mega_lazer_p->GetSegment();
			for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
			{
				temp__ship_p = &ships[ship];
				if (temp__ship_p->exist == true && temp__ship_p->IsCollision(&temp__segment))
				{
					DestroyEntity(temp__mega_lazer_p, temp__ship_p);
				}
			}
			for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(&temp__segment))
				{
					DestroyEntity(temp__mega_lazer_p, temp__pilot_p);
				}
			}
			for (GameTypes::entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__bonus_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(&temp__segment))
					{
						DestroyEntity(temp__mega_lazer_p, temp__asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__asteroid_p = &asteroids[bonus];
				if (temp__bonus_p->exist == true)
				{
					if (temp__asteroid_p->IsCollision(&temp__segment))
					{
						DestroyEntity(temp__mega_lazer_p, temp__asteroid_p);
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
				if (temp__bonus_p->exist == true)
				{
					if (temp__bullet_p->Entity::IsCollision(&temp__segment))
					{
						DestroyEntity(temp__mega_lazer_p, temp__bullet_p);
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
	for (GameTypes::players_count_t pilot = 0; pilot < players_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist == true)
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
	for (GameTypes::players_count_t ship = 0; ship < players_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist == true)
		{
			temp__ship_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
			for (GameTypes::entities_count_t pilot = 0; pilot < players_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist == true && temp__pilot_p->IsCollision(temp__ship_p) == true)
				{
					if (game_rules & GAME_RULE_PILOT_CAN_RESPAWN && game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)
					{
						SpawnEntity(temp__ship_p, temp__pilot_p);
					}
					else
					{
						DestroyEntity(temp__ship_p, temp__pilot_p);
					}
				}
			}
			for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist == true)
				{
					if (temp__bonus_p->GetDistance(temp__ship_p) < GAME_SHIP_INFLUENCE_RADIUS)
					{
						temp__bonus_p->AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, temp__ship_p);
					}
					found_bonuses++;
				}
			}
			if (shoot_flags[ship] == true && sheeps_can_shoot_flags[ship] == true)
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
		if (temp__turel_p->exist == true)
		{
			if (temp__turel_p->CanShoot(current_tic))
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
			if (temp__bomb_p->exist == true)
			{
				if (temp__bomb_p->IsBoom() == true)
				{
					Bomb* temp_bomb;
					for (GameTypes::entities_count_t new_bomb = 0, found_new_bomb = 0; found_new_bomb < bombs_count && new_bomb < GAME_BOMBS_MAX_COUNT; new_bomb++)
					{
						temp_bomb = &bombs[new_bomb];
						if (temp_bomb->exist == true)
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