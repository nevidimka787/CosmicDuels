#include "GameRealisation.h"
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 4244)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 6200)
#pragma warning(disable : 6386)

void Game::GameInit()
{
	sheeps = (Sheep**)malloc(sizeof(Sheep*) * GAME_PLAYERS_MAX_COUNT);
	pilots = (Pilot**)malloc(sizeof(Pilot*) * GAME_PLAYERS_MAX_COUNT);

	bullets = (DynamicEntity**)malloc(sizeof(DynamicEntity*) * GAME_BULLETS_MAX_COUNT);
	bullets_count = 0;
	particles = (DynamicEntity**)malloc(sizeof(DynamicEntity*) * GAME_PARTICLES_MAX_COUNT);
	particles_count = 0;
	asteroids = (Asteroid**)malloc(sizeof(Asteroid*) * GAME_ASTEROIDS_MAX_COUNT);
	asterooids_count = 0;
	bonuses = (Bonus**)malloc(sizeof(Bonus*) * GAME_BONUSES_MAX_COUNT);
	bonuses_count = 0;

	srand((unsigned int)sheeps * (unsigned int)pilots);
}

void Game::MachInit(uint8_t players_count, uint32_t game_rules, uint16_t start_bonus)
{
	if (players_count > GAME_PLAYERS_MAX_COUNT)
	{
		Game::players_count = GAME_PLAYERS_MAX_COUNT;
	}
	else if (players_count < 2)
	{
		Game::players_count = 2;
	}
	else
	{
		Game::players_count = players_count;
	}
	shoot_keys = new bool[Game::players_count];
	rotate_keys = new bool[Game::players_count];
	scores = new uint8_t[Game::players_count];
	for (uint8_t i = 0; i < Game::players_count; i++)
	{
		scores[i] = 0;
	}


	Game::start_bonus = game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD ? BUFF_SHIELD : 0x0000;
	Game::start_bonus |= game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS ? BUFF_TRIPLE : 0x0000;
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)
	{
		if (game_rules & GAME_RULE_START_BONUS_RANDOMIZE)
		{
			Game::start_bonus |= GenerateRandomBonus();
		}
		else
		{
			Game::start_bonus |= start_bonus;
		}
	}

	current_map_id = 0;
}

void Game::LevelInit(uint8_t map_id)
{
	Vec2F* temp_points = new Vec2F[Game::players_count];;
	Rectangle* temp_rectangles;

	const uint8_t max_score = GetMaxScore();

	switch (map_id)
	{
	case MAP_TEST_MAP:
		/* Create map */
		temp_points = new Vec2F[2];
		temp_points[0] = Vec2F(1.0f, 1.0f);
		temp_points[1] = Vec2F(2.0f - 1.0f / (float)((uint32_t)1 << 23), 2.0f - 1.0f / (float)((uint32_t)1 << 23));
		temp_rectangles = new Rectangle[1];
		temp_rectangles[0] = Rectangle(&temp_points[0], &temp_points[1]);

		map = new Map(temp_rectangles, 1, nullptr, 0, nullptr, 0, &temp_points[0]);

		/*Spawn entities*/
		
		temp_points[0] = Vec2F(1.1f, 1.1f);
		temp_points[1] = Vec2F(1.9f, 1.9f);
		if (Game::players_count > 2)
		{
			temp_points[2] = Vec2F(1.1f, 1.9f);
		}
		if (Game::players_count > 3)
		{
			temp_points[3] = Vec2F(1.9f, 1.1f);
		}
		if (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)
		{
			for (uint8_t i = 0; i < RANDOM_ITERATIONS_COUNT; i++)
			{
				uint8_t temp1 = rand() % Game::players_count;
				uint8_t temp2 = rand() % Game::players_count;
				Vec2F temp3 = temp_points[temp1];
				temp_points[temp1] = temp_points[temp2];
				temp_points[temp2] = temp3;
			}
		}

		break;
	default:
		break;
	}


	float* temp_angles = new float[Game::players_count];
	temp_angles[0] = -(float)M_PI_4;
	temp_angles[1] = (float)M_PI - (float)M_PI_4;
	if (Game::players_count > 2)
	{
		temp_angles[2] = (float)M_PI_4;
	}
	if (Game::players_count > 3)
	{
		temp_angles[3] = (float)M_PI + (float)M_PI_4;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)
	{
		for (uint8_t i = 0; i < Game::players_count; i++)
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
		for (uint8_t i = 0; i < Game::players_count; i++)
		{
			Vec2F temp_vel = Vec2F();
			if (game_rules & GAME_RULE_BALANCE_ACTIVE && scores[i] <= max_score - BALANCE_ACTIVATE_DIFFERENCE_SCORES)
			{
				sheeps[i] = new Sheep(i, (void*)&rotate_keys[i], (void*)&shoot_keys[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus | BUFF_SHIELD);
			}
			else
			{
				sheeps[i] = new Sheep(i, (void*)&rotate_keys[i], (void*)&shoot_keys[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus);
			}
		}
	}

	current_map_id = map_id;
}

void Game::MenusInit()
{
	//main menu
	Button* buttons = new Button[3];
	Vec2F position = Vec2F(-0.5f, 0.5f);
	Vec2F size = Vec2F(1.0f, -0.5f);
	buttons[0] = Button(BUTTON_ID_START_MATCH, &position, &size, "Play", 16);
	position = Vec2F(-0.5f, 0.0f);
	size = Vec2F(0.5f, -0.5f);
	buttons[1] = Button(BUTTON_ID_GO_TO_OPTINS_MENU, &position, &size, "Options", 8);
	position = Vec2F(0.0f, 0.0f);
	buttons[2] = Button(BUTTON_ID_EXIT, &position, &size, "Exit", 8);
	Game::main_menu = new Menu(&position, buttons, 3, MenuFunctions::MainMenuFunction);
	delete[] buttons;

	//option menu
	buttons = new Button[14];
	size = Vec2F(1.0f, -0.1f);
	position = Vec2F(-0.5f, 0.5f - 0 * 0.1f);
	buttons[0] = Button(BUTTON_ID_SET_RANDOM_SPAWN, &position, &size, "Random spawn", 6);
	position = Vec2F(-0.5f, 0.5f - 1 * 0.1f);
	buttons[1] = Button(BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION, &position, &size, "Random spawn direction", 6);
	position = Vec2F(-0.5f, 0.5f - 2 * 0.1f);
	buttons[2] = Button(BUTTON_ID_SET_SPAWN_THIS_BONUS, &position, &size, "Spawn this bonus", 6);
	position = Vec2F(-0.5f, 0.5f - 3 * 0.1f);
	buttons[3] = Button(BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS, &position, &size, "Random start bonus", 6);
	position = Vec2F(-0.5f, 0.5f - 4 * 0.1f);
	buttons[4] = Button(BUTTON_ID_SET_SPAWN_WHIS_TRIPLE_BUFF, &position, &size, "Triple bonuses", 6);
	position = Vec2F(-0.5f, 0.5f - 5 * 0.1f);
	buttons[5] = Button(BUTTON_ID_SET_SPAWN_WHIT_SHIELD_BAFF, &position, &size, "Spawn whis shield", 6);
	position = Vec2F(-0.5f, 0.5f - 6 * 0.1f);
	buttons[6] = Button(BUTTON_ID_GO_TO_SELECT_MACH_TYPE_MENU, &position, &size, "Mach type", 6);
	position = Vec2F(-0.5f, 0.5f - 7 * 0.1f);
	buttons[7] = Button(BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, "Friendly sheep can restore", 6);
	position = Vec2F(-0.5f, 0.5f - 8 * 0.1f);
	buttons[8] = Button(BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE, &position, &size, "Frendly fire", 6);
	position = Vec2F(-0.5f, 0.5f - 9 * 0.1f);
	buttons[9] = Button(BUTTON_ID_SET_PILOT_CAN_RESPAWN, &position, &size, "Pilot can respawn", 6);
	position = Vec2F(-0.5f, 0.5f - 10 * 0.1f);
	buttons[10] = Button(BUTTON_ID_SET_NEED_KILL_PILOT, &position, &size, "Need kill pilot", 6);
	position = Vec2F(-0.5f, 0.5f - 11 * 0.1f);
	buttons[11] = Button(BUTTON_ID_GO_TO_SELECT_MAP_MENU, &position, &size, "Map", 6);
	position = Vec2F(-0.5f, 0.5f - 12 * 0.1f);
	buttons[12] = Button(BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU, &position, &size, "Spawning", 6);
	position = Vec2F(-0.5f, 0.5f - 13 * 0.1f);
	buttons[13] = Button(BUTTON_ID_SET_ACTIVE_BALANCE, &position, &size, "Balance", 6);
	position = Vec2F(0.0f, 0.0f);
	Game::option_menu = new Menu(&position, buttons, 14, MenuFunctions::OptionMenuFunction);
	delete[] buttons;
	
	//pause menu
	buttons = new Button[2];
	position = Vec2F(-0.2f, 0.2f);
	size = Vec2F(0.4f, -0.2f);
	buttons[0] = Button(BUTTON_ID_RESUME_MATCH, &position, &size, "Resume", 16);
	position = Vec2F(-0.2f, 0.0f);
	buttons[1] = Button(BUTTON_ID_GO_TO_MAIN_MENU , &position, &size, "Main menu", 8);
	position = Vec2F(0.0f, 0.0f);
	Game::pause_menu = new Menu(&position, buttons, 3, MenuFunctions::PauseMenuFunction);
	delete[] buttons;

	//sheeps select menu
	buttons = new Button[4];
	size = Vec2F(0.25f, -0.3f);
	position = Vec2F(-0.5f, 0.5f - 1.0f / 3.0f);
	buttons[0] = Button(BUTTON_ID_SELECT_SHEEP_1, &position, &size, "Red sheep", 6);
	position = Vec2F(-0.25f, 0.5f - 1.0f / 3.0f);
	buttons[1] = Button(BUTTON_ID_SELECT_SHEEP_2, &position, &size, "Green sheep", 6);
	position = Vec2F(0.0f, 0.5f - 1.0f / 3.0f);
	buttons[2] = Button(BUTTON_ID_SELECT_SHEEP_3, &position, &size, "Blue sheep", 6);
	position = Vec2F(0.25f, 0.5f - 1.0f / 3.0f);
	buttons[3] = Button(BUTTON_ID_SELECT_SHEEP_4, &position, &size, "Purpure sheep", 6);
	position = Vec2F();
	sheeps_select_menu = new Menu(&position, buttons, 4, MenuFunctions::SheepsSelectMenuFunction);
	delete[] buttons;

	//team sheeps select menu
	buttons = new Button[12];
size = Vec2F(0.25f, -0.3f);
position = Vec2F(-0.5f / 4.0f, 0.5f);
buttons[0] = Button(BUTTON_ID_SELECT_SHEEP_1_TO_TEAM_1, &position, &size, "Team 1", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 1.0f / 3.0f);
buttons[1] = Button(BUTTON_ID_SELECT_SHEEP_1_TO_NO_TEAM, &position, &size, "Red sheep", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 2.0f / 3.0f);
buttons[2] = Button(BUTTON_ID_SELECT_SHEEP_1_TO_TEAM_2, &position, &size, "Team 2", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f);
buttons[3] = Button(BUTTON_ID_SELECT_SHEEP_2_TO_TEAM_1, &position, &size, "Team 1", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 1.0f / 3.0f);
buttons[4] = Button(BUTTON_ID_SELECT_SHEEP_2_TO_NO_TEAM, &position, &size, "Green sheep", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 2.0f / 3.0f);
buttons[5] = Button(BUTTON_ID_SELECT_SHEEP_2_TO_TEAM_2, &position, &size, "Team 2", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f);
buttons[6] = Button(BUTTON_ID_SELECT_SHEEP_3_TO_TEAM_1, &position, &size, "Team 1", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 1.0f / 3.0f);
buttons[7] = Button(BUTTON_ID_SELECT_SHEEP_3_TO_NO_TEAM, &position, &size, "Blue sheep", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 2.0f / 3.0f);
buttons[8] = Button(BUTTON_ID_SELECT_SHEEP_3_TO_TEAM_2, &position, &size, "Team 2", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f);
buttons[9] = Button(BUTTON_ID_SELECT_SHEEP_4_TO_TEAM_1, &position, &size, "Team 1", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 1.0f / 3.0f);
buttons[10] = Button(BUTTON_ID_SELECT_SHEEP_4_TO_NO_TEAM, &position, &size, "Purpure sheep", 6);
position = Vec2F(-0.5f / 4.0f, 0.5f - 2.0f / 3.0f);
buttons[11] = Button(BUTTON_ID_SELECT_SHEEP_4_TO_TEAM_2, &position, &size, "Team 2", 6);
position = Vec2F();
team_sheeps_select_menu = new Menu(&position, buttons, 12, MenuFunctions::TeamsSelectMenuFunction);
delete[] buttons;

//maps select menu
buttons = new Button[MAPS_COUNT];
for (uint8_t i = 1; i <= MAPS_COUNT; i++)
{
	position = Vec2F(-0.5f + (float)(i % 4), 0.5f - (float)(i / 4));
	buttons[i] = Button(BUTTON_ID_SELECT_MAP + i - 1, &position, &size, "", 6);
}
position = Vec2F();
maps_select_menu = new Menu(&position, buttons, MAPS_COUNT, MenuFunctions::MapPullSelectMenuFunction);
delete[] buttons;

//spawning objects select menu
buttons = new Button[1];
position = Vec2F(-0.5f, 0.5f);
buttons[0] = Button(BUTTON_ID_SELECT_OBJECT_ASTEROID, &position, &size, "Asteroid", 6);
position = Vec2F();
spawning_objects_select_menu = new Menu(&position, buttons, 1, MenuFunctions::SpawnObjectsSelectMenuFunction);
delete[] buttons;
}

void Game::Recalculate()
{
	//shoots
	
	UpdateBullets();

	//shoots
	
	//collisions

	DynamicEntitiesCollisions((DynamicEntity**)sheeps, players_count);
	DynamicEntitiesCollisions((DynamicEntity**)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity**)asteroids, asterooids_count);

	DynamicEntitiesCollisions((DynamicEntity**)sheeps, players_count, (DynamicEntity**)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity**)sheeps, players_count, (DynamicEntity**)asteroids, asterooids_count);
	DynamicEntitiesCollisions((DynamicEntity**)asteroids, asterooids_count, (DynamicEntity**)pilots, players_count);

	DynamicEntitiesCollisions((DynamicEntity**)sheeps, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity**)pilots, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity**)asteroids, asterooids_count, map);

	DynamicEntitiesAddForce((DynamicEntity**)sheeps, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity**)pilots, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity**)asteroids, asterooids_count, grav_gens, grav_gens_count);

	//collisions
}

void Game::DynamicEntitiesCollisions(DynamicEntity** entities, uint16_t entities_count)
{
	for (uint16_t first = 0; first < entities_count; first++)
	{
		if (entities[first] != nullptr)
		{
			for (uint16_t second = first + 1; second < entities_count; second++)
			{
				if (entities[second] != nullptr)
				{
					entities[first]->Collision(entities[second]);
				}
			}
		}
	}
}

void Game::DynamicEntitiesCollisions(DynamicEntity** entities1, uint16_t entities1_count, DynamicEntity** entities2, uint16_t entities2_count)
{
	for (uint16_t first = 0; first < entities1_count; first++)
	{
		if (entities1[first] != nullptr)
		{
			for (uint16_t second = 0; second < entities2_count; second++)
			{
				if (entities2[second] != nullptr)
				{
					entities1[first]->Collision(entities2[second]);
				}
			}
		}
	}
}

void Game::DynamicEntitiesCollisions(DynamicEntity** entities, uint16_t entities_count, Map* map)
{
	for (uint16_t first = 0; first < entities_count; first++)
	{
		if (entities[first] != nullptr)
		{
			entities[first]->Collision(map);
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity** entities, uint16_t entities_count, Vec2F* force)
{
	for (uint16_t i = 0; i < entities_count; i++)
	{
		if (entities[i] != nullptr)
		{
			entities[i]->AddForce(force);
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity** entities, uint16_t entities_count, GravGen* grav_gen)
{
	Vec2F force;
	float len2;
	for (uint16_t i = 0; i < entities_count; i++)
	{
		if (entities[i] != nullptr)
		{
			force = entities[i]->GetPosition() - grav_gen->GetPosition();
			len2 = force.x * force.x + force.y * force.y;
			force /= len2 * sqrt(len2) / grav_gen->gravity;
			entities[i]->AddForce(&force);
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity** entities, uint16_t entities_count, GravGen** grav_gens, uint8_t grav_gens_count)
{
	Vec2F force;
	float len2;
	for (uint8_t grav_gen = 0; grav_gen < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen] != nullptr)
		{
			for (uint16_t i = 0; i < entities_count; i++)
			{
				if (entities[i] != nullptr)
				{
					force = entities[i]->GetPosition() - grav_gens[grav_gen]->GetPosition();
					len2 = force.x * force.x + force.y * force.y;
					force /= len2 * sqrt(len2) / grav_gens[grav_gen]->gravity;
					entities[i]->AddForce(&force);
				}
			}
		}
	}
}

void Game::UpdateBullets()
{
	//deleting bullets

	uint16_t iterations_count = bullets_count;
	for (uint16_t i = 0; i < iterations_count; i++)
	{
		if (bullets[i] != nullptr)
		{
			if (bullets[i]->IsCollision(map))
			{
				DeleteEntity(bullets[i]);
				goto end_cycle;
			}
			for (uint8_t sheep = 0; sheep < players_count; sheep++)
			{
				if (sheeps[sheep] != nullptr && bullets[i]->IsCollision(sheeps[sheep]))
				{
					if (sheeps[sheep]->GetActiveBaffsAndBonuses() & BUFF_SHIELD)
					{
						sheeps[sheep]->BreakShield();
					}
					else
					{
						Bonus* new_bonus = sheeps[sheep]->LoseBonus();
						Bonus* temp_bonus;
						while ((temp_bonus = new_bonus->Division()) != nullptr)
						{
							AddEntity(temp_bonus);
						}
						AddEntity(new_bonus);

						pilots[sheep] = sheeps[sheep]->Destroy();
						pilots_count++;
						delete sheeps[sheep];
						sheeps_count--;
					}

					DeleteEntity(bullets[i]);
					goto end_cycle;
				}
			}
			for (uint8_t pilot = 0; pilot < players_count; pilot++)
			{
				if (pilots[pilot] != nullptr && bullets[i]->IsCollision(pilots[pilot]))
				{
					DeleteEntity(pilots[pilot]);

					if (game_rules & BUTTON_ID_SET_NEED_KILL_PILOT)
					{
						DeleteEntity(bullets[i]);
						goto end_cycle;
					}
				}
			}
			for (uint16_t asteroid = 0; asteroid < asterooids_count; asteroid++)
			{
				if (asteroids[asteroid] != nullptr && bullets[i]->IsCollision(asteroids[asteroid]))
				{
					Bonus* new_bonus = asteroids[asteroid]->Destroy();
					if (new_bonus->bonus_type)
					{
						AddEntity(new_bonus);
					}
					DeleteEntity(asteroids[asteroid]);

					DeleteEntity(bullets[i]);
					goto end_cycle;
				}
			}
		}
	end_cycle:
	}

	//deleting bullets

	//add bullets



	//add bullets

	if (bullets_count < iterations_count)
	{
		DefragmentationArray((void**)bullets, iterations_count, bullets_count);
	}
}

void Game::AddEntity(Asteroid* new_asteroid)
{
	if (asterooids_count < GAME_ASTEROIDS_MAX_COUNT)
	{
		for (uint16_t asteroid = 0; asteroid < GAME_ASTEROIDS_MAX_COUNT; asteroid++)
		{
			if (asteroids[asteroid] == nullptr)
			{
				asteroids[asteroid] = new_asteroid;
				asterooids_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Bonus* new_bonus)
{
	if (bonuses_count < GAME_BONUSES_MAX_COUNT)
	{
		for (uint16_t bonus = 0; bonus < GAME_BONUSES_MAX_COUNT; bonus++)
		{
			if (bonuses[bonus] == nullptr)
			{
				bonuses[bonus] = new_bonus;
				bonuses_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(DynamicEntity* new_bullet)
{
	if (bullets_count < GAME_BULLETS_MAX_COUNT)
	{
		for (uint16_t bullet = 0; bullet < GAME_BULLETS_MAX_COUNT; bullet++)
		{
			if (bullets[bullet] == nullptr)
			{
				bullets[bullet] = new_bullet;
				bullets_count++;
				return;
			}
		}
	}
}

void Game::AddParticle(DynamicEntity* new_particle)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (uint16_t particle = 0; particle < GAME_BULLETS_MAX_COUNT; particle++)
		{
			if (particles[particle] == nullptr)
			{
				particles[particle] = new_particle;
				bullets_count++;
				return;
			}
		}
	}
}

void Game::DeleteEntity(Asteroid* deleting_asteroid)
{
	delete deleting_asteroid;
	asterooids_count--;
}

void Game::DeleteEntity(Bonus* deleting_bonus)
{
	delete deleting_bonus;
	bonuses_count--;
}

void Game::DeleteEntity(DynamicEntity* deleting_bullet)
{
	delete deleting_bullet;
	bullets_count--;
}

void Game::DeleteParticle(DynamicEntity* deleting_particle)
{
	delete deleting_particle;
	particles_count--;
}

uint8_t Game::GenerateRandomBonus()
{
	return 1 << (((rand() % BONUSES_COUNT)) * 2);
}

uint8_t Game::GetRandomMap()
{
	return rand() % MAPS_COUNT;
}

uint8_t Game::GetMaxScore()
{
	uint8_t max_score = scores[0];
	for (uint8_t i = 1; i < players_count; i++)
	{
		if (max_score < scores[i])
		{
			max_score = scores[i];
		}
	}
	return max_score;
}

void Game::DefragmentationArray(void** arr, uint16_t arr_len, uint16_t elements_count)
{
	uint16_t found_elements_count = 0;
	uint16_t to_cell = 0;
	bool optimal = true;
	for (uint16_t from_cell = 0; from_cell < arr_len && found_elements_count < elements_count; from_cell++)
	{
		if (arr[from_cell] == nullptr)
		{
			if (optimal == true)
			{
				optimal = false;
				to_cell = from_cell;
			}
		}
		else if (optimal == false)
		{
			arr[to_cell] = arr[from_cell];
			to_cell++;

			found_elements_count++;
		}
		else
		{
			found_elements_count++;
		}
	}
}