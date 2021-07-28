#include "GameRealisation.h"
#include "MenuFunctions.h"
#pragma warning (disable : 4244)
#pragma warning (disable : 4302)
#pragma warning (disable : 4311)
#pragma warning (disable : 6200)
#pragma warning (disable : 6294)
#pragma warning (disable : 6297)
#pragma warning (disable : 6386)
#pragma warning (disable : 26451)

inline void Game::Init::Game()
{
	game_rules = GAME_RULE_DEFAULT_GAME_RULES;
	global_timer = 0;

	MenuFunctions::ships_select_buttons = new players_count_t[GAME_PLAYERS_MAX_COUNT];
	for (players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		MenuFunctions::ships_select_buttons[i] = SHIPS_SELECT_BUTTONS_NO_TEAM;
	}

	srand((unsigned int)ships * (unsigned int)pilots);
}

inline void Game::Init::Mach()
{
	players_count = 0;
	for (uint8_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		if (MenuFunctions::ships_select_buttons[i] != SHIPS_SELECT_BUTTONS_NO_TEAM)
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
	scores = new players_count_t[players_count];
	for (players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
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
			start_bonus |= MenuFunctions::start_bonus;
		}
	}

	//create map pull array

	selected_maps_id_array_length = 0;
	for (maps_count_t map_id = 0; map_id < MAPS_COUNT; map_id++)
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
	selected_maps_id_array = new maps_count_t[selected_maps_id_array_length];

	for (maps_count_t map_id = 0, found = 0; found < selected_maps_id_array_length; map_id++)
	{
		if (map_pull_array[map_id] == true)
		{
			selected_maps_id_array[found] = map_id;
			found++;
		}
	}

	//create map pull array
}

inline void Game::Init::Level()
{

	current_map_id = selected_maps_id_array[rand() % selected_maps_id_array_length];


	Vec2F* temp_points = new Vec2F[players_count];;
	Rectangle* temp_rectangles;

	const score_t max_score = GetMaxScore();

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
				players_count_t temp1 = rand() % players_count;
				players_count_t temp2 = rand() % players_count;
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
		for (players_count_t i = 0; i < players_count; i++)
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
		for (players_count_t i = 0; i < players_count; i++)
		{
			Vec2F temp_vel = Vec2F();
			if (game_rules & GAME_RULE_BALANCE_ACTIVE && scores[i] <= max_score - BALANCE_ACTIVATE_DIFFERENCE_SCORES)
			{
				Add::Entity(Ship(i, (void*)&rotate_flags[i], (void*)&shoot_flags[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus | BUFF_SHIELD));
			}
			else
			{
				Add::Entity(Ship(i, (void*)&rotate_flags[i], (void*)&shoot_flags[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus));
			}
		}
	}
}

inline void Game::Init::Menus()
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
	main_menu = new Menu(&position, buttons, 3, MenuFunctions::MainMenuFunction);
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
	buttons[7] = Button(BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE, &position, &size, "Friendly sheep can restore", 6);
	position = Vec2F(-0.5f, 0.5f - 7 * 0.1f);
	buttons[8] = Button(BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE, &position, &size, "Frendly fire", 6);
	position = Vec2F(-0.5f, 0.5f - 8 * 0.1f);
	buttons[9] = Button(BUTTON_ID_SET_PILOT_CAN_RESPAWN, &position, &size, "Pilot can respawn", 6);
	position = Vec2F(-0.5f, 0.5f - 9 * 0.1f);
	buttons[10] = Button(BUTTON_ID_SET_NEED_KILL_PILOT, &position, &size, "Need kill pilot", 6);
	position = Vec2F(-0.5f, 0.5f - 10 * 0.1f);
	buttons[11] = Button(BUTTON_ID_GO_TO_SELECT_MAP_MENU, &position, &size, "Map", 6);
	position = Vec2F(-0.5f, 0.5f - 11 * 0.1f);
	buttons[12] = Button(BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU, &position, &size, "Spawning", 6);
	position = Vec2F(-0.5f, 0.5f - 12 * 0.1f);
	buttons[13] = Button(BUTTON_ID_SET_ACTIVE_BALANCE, &position, &size, "Balance", 6);
	position = Vec2F(0.0f, 0.0f);
	option_menu = new Menu(&position, buttons, 14, MenuFunctions::OptionMenuFunction);
	delete[] buttons;

	//pause menu
	buttons = new Button[2];
	position = Vec2F(-0.2f, 0.2f);
	size = Vec2F(0.4f, -0.2f);
	buttons[0] = Button(BUTTON_ID_RESUME_MATCH, &position, &size, "Resume", 16);
	position = Vec2F(-0.2f, 0.0f);
	buttons[1] = Button(BUTTON_ID_GO_TO_MAIN_MENU, &position, &size, "Main menu", 8);
	position = Vec2F(0.0f, 0.0f);
	pause_menu = new Menu(&position, buttons, 3, MenuFunctions::PauseMenuFunction);
	delete[] buttons;

	//sheeps select menu
	buttons = new Button[4];
	size = Vec2F(0.25f, -0.25f);
	position = Vec2F(-0.5f, 0.5f);
	buttons[0] = Button(BUTTON_ID_SELECT_SHIP_1, &position, &size, "Player 1", 6);
	position = Vec2F(0.0f, 0.5f);
	buttons[1] = Button(BUTTON_ID_SELECT_SHIP_2, &position, &size, "Player 2", 6);
	position = Vec2F(0.0f, 0.0f);
	buttons[2] = Button(BUTTON_ID_SELECT_SHIP_3, &position, &size, "Player 3", 6);
	position = Vec2F(-0.5f, 0.0f);
	buttons[3] = Button(BUTTON_ID_SELECT_SHIP_4, &position, &size, "Player 4", 6);
	position = Vec2F();
	ships_select_menu = new Menu(&position, buttons, 4, MenuFunctions::ShipsSelectMenuFunction);
	delete[] buttons;

	//maps select menu
	buttons = new Button[MAPS_COUNT];
	for (uint8_t i = 1; i < MAPS_COUNT; i++)
	{
		position = Vec2F(-0.5f + (float)(i % 4), 0.5f - (float)(i / 4));
		buttons[i] = Button(BUTTON_ID_SELECT_MAP + i - 1, &position, &size, "", 6);
	}
	position = Vec2F();
	map_pull_select_menu = new Menu(&position, buttons, MAPS_COUNT, MenuFunctions::MapPullSelectMenuFunction);
	delete[] buttons;

	//spawning objects select menu
	buttons = new Button[1];
	position = Vec2F(-0.5f, 0.5f);
	buttons[0] = Button(BUTTON_ID_SELECT_OBJECT_ASTEROID, &position, &size, "Asteroid", 6);
	position = Vec2F();
	spawning_objects_select_menu = new Menu(&position, buttons, 1, MenuFunctions::SpawnObjectsSelectMenuFunction);
	delete[] buttons;
}
