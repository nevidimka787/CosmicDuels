#include "MenuFunctions.h"

void::MenuFunctions::Back()
{
	if (Game::current_active_menu == Game::main_menu)
	{
		Exit();
	}
	else if (Game::current_active_menu == Game::option_menu)
	{
		Game::current_active_menu = Game::main_menu;
	}
	else if (Game::current_active_menu == Game::sheeps_select_menu || Game::current_active_menu == Game::team_sheeps_select_menu || Game::current_active_menu == Game::maps_select_menu || Game::current_active_menu == Game::spawning_objects_select_menu)
	{
		Game::current_active_menu = Game::option_menu;
	}
}

void MenuFunctions::OpenOptionsMenu()
{
	Game::current_active_menu = Game::option_menu;
}

void MenuFunctions::OpenMainMenu()
{
	Game::current_active_menu = Game::main_menu;
}

void MenuFunctions::OpenPauseMenu()
{
	Game::current_active_menu = Game::pause_menu;
}

void MenuFunctions::OpenSheepsSelectMenu()
{
	Game::current_active_menu = Game::sheeps_select_menu;
}

void MenuFunctions::OpenTeamsSelectMenu()
{
	Game::current_active_menu = Game::team_sheeps_select_menu;
}

void MenuFunctions::OpenMapPullSelectMenu()
{
	Game::current_active_menu = Game::maps_select_menu;
}

void MenuFunctions::OpenSpawnObjectsSelectMenu()
{
	Game::current_active_menu = Game::spawning_objects_select_menu;
}

void MenuFunctions::OpenSheepsControlMenu()
{
	Game::current_active_menu = Game::sheeps_control_menu;
}



void MenuFunctions::StartGame()
{
	Game::start_game = true;
	OpenSheepsControlMenu();
}

void MenuFunctions::PauseGame()
{
	Game::pause_game = true;
	OpenPauseMenu();
}

void MenuFunctions::ResumeGame()
{
	Game::pause_game = false;
	OpenSheepsControlMenu();
}

void MenuFunctions::SelectSheep(Game::players_count_t sheep_number, uint8_t team_number)
{
	sheeps_select_buttons[sheep_number] = team_number;
}

void MenuFunctions::Exit()
{
	exit(0);
}

void MenuFunctions::ChangeOption(Game::game_rules_t option_id)
{
	if (Game::game_rules & option_id)
	{
		Game::game_rules &= GAME_RULES_T_MAX - option_id;
		return;
	}
	Game::game_rules |= option_id;
}

void MenuFunctions::MainMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (Game::current_active_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_START_MATCH:
				StartGame();
				return;
			case BUTTON_ID_GO_TO_OPTINS_MENU:
				OpenOptionsMenu();
				return;
			default:
				return;
			}
		}
	}
}

void MenuFunctions::OptionMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (Game::current_active_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_SET_RANDOM_SPAWN:
				ChangeOption(GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE);
				return;
			case BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION:
				ChangeOption(GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_BONUS:
				ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_BONUS);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS:
				ChangeOption(GAME_RULE_START_BONUS_RANDOMIZE);
				return;
			case BUTTON_ID_SET_SPAWN_WHIS_TRIPLE_BUFF:
				ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS);
				return;
			case BUTTON_ID_SET_SPAWN_WHIT_SHIELD_BAFF:
				ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD);
				return;
			case BUTTON_ID_SET_ACTIVE_BALANCE:
				ChangeOption(GAME_RULE_BALANCE_ACTIVE);
				return;
			case BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE:
				ChangeOption(GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE);
				return;
			case BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE:
				ChangeOption(GAME_RULE_FRENDLY_FIRE);
				return;
			case BUTTON_ID_SET_PILOT_CAN_RESPAWN:
				ChangeOption(GAME_RULE_PILOT_CAN_RESPAWN);
				return;
			case BUTTON_ID_SET_NEED_KILL_PILOT:
				ChangeOption(GAME_RULE_NEED_KILL_PILOT);
				return;
			case BUTTON_ID_GO_TO_SELECT_MAP_MENU:
				OpenMapPullSelectMenu();
				return;
			case BUTTON_ID_GO_TO_SELECT_OBJECTS_MENU:
				OpenSpawnObjectsSelectMenu();
				return;
			default:
				return;
			}
		}
	}
}

void MenuFunctions::PauseMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (Game::current_active_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_GO_TO_MAIN_MENU:
				OpenMainMenu();
				return;
			case BUTTON_ID_RESUME_MATCH:
			default:
				ResumeGame();
				return;
			}
		}
	}
}

void MenuFunctions::SheepsSelectMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (Game::current_active_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_SELECT_SHEEP_1:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(0, SHEEPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(0, SHEEPS_SELECT_BUTTONS_TEAM_BLUE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(0, SHEEPS_SELECT_BUTTONS_TEAM_GREEN);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(0, SHEEPS_SELECT_BUTTONS_TEAM_PURPURE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(0, SHEEPS_SELECT_BUTTONS_NO_TEAM);
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_2:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_BLUE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_GREEN);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_PURPURE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_NO_TEAM);
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_3:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_BLUE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_GREEN);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_PURPURE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_NO_TEAM);
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_4:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_BLUE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_GREEN);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_TEAM_PURPURE);
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHEEPS_SELECT_BUTTONS_NO_TEAM);
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			default:
				return;
			}
		}
	}
}

void MenuFunctions::MapPullSelectMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			uint32_t id = Game::current_active_menu->current_buttons[i].GetId() - BUTTON_ID_SELECT_MAP;
			if (id < MAPS_COUNT)
			{
				Game::map_pull_array[id]++;
				return;
			}
		}
	}
}

void MenuFunctions::SpawnObjectsSelectMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < Game::current_active_menu->GetButtonsCount(); i++)
	{
		if (Game::current_active_menu->current_buttons[i].HavePoint(clk_pos))
		{
			uint32_t id = Game::current_active_menu->current_buttons[i].GetId() - BUTTON_ID_SELECT_OBJECT;
			if (id < OBJECTS_COUNT)
			{
				Game::object_pull_array[id]++;
				return;
			}
		}
	}
}


void MenuFunctions::SheepsControlMenuFunction(Vec2F* clk_pos)
{

}