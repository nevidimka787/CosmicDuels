#include "MenuFunctions.h"

#pragma warning(disable : 26495)

MenuFunctions::MenuFunctions()
{}

void::MenuFunctions::Back()
{
	if (*game_p__current_active_menu == game_p__main_menu)
	{
		Exit();
	}
	else if (*game_p__current_active_menu == game_p__option_menu)
	{
		*game_p__current_active_menu = game_p__main_menu;
	}
	else if (*game_p__current_active_menu == game_p__ships_select_menu || *game_p__current_active_menu == game_p__team_sheeps_select_menu || *game_p__current_active_menu == game_p__map_pull_select_menu || *game_p__current_active_menu == game_p__spawning_objects_select_menu)
	{
		*game_p__current_active_menu = game_p__option_menu;
	}
}

void MenuFunctions::OpenOptionsMenu()
{
	*game_p__current_active_menu = game_p__option_menu;
}

void MenuFunctions::OpenMainMenu()
{
	*game_p__current_active_menu = game_p__main_menu;
}

void MenuFunctions::OpenPauseMenu()
{
	*game_p__current_active_menu = game_p__pause_menu;
}

void MenuFunctions::OpenSheepsSelectMenu()
{
	*game_p__current_active_menu = game_p__ships_select_menu;
}

void MenuFunctions::OpenTeamsSelectMenu()
{
	*game_p__current_active_menu = game_p__team_sheeps_select_menu;
}

void MenuFunctions::OpenMapPullSelectMenu()
{
	*game_p__current_active_menu = game_p__map_pull_select_menu;
}

void MenuFunctions::OpenSpawnObjectsSelectMenu()
{
	*game_p__current_active_menu = game_p__spawning_objects_select_menu;
}

void MenuFunctions::OpenSheepsControlMenu()
{
	*game_p__current_active_menu = game_p__ships_control_menu;
}



void MenuFunctions::StartGame()
{
	*game_p__start_game = true;
	OpenSheepsControlMenu();
}

void MenuFunctions::PauseGame()
{
	*game_p__pause_game = true;
	OpenPauseMenu();
}

void MenuFunctions::ResumeGame()
{
	*game_p__pause_game = false;
	OpenSheepsControlMenu();
}

void MenuFunctions::SelectSheep(GameTypes::players_count_t sheep_number, GameTypes::players_count_t team_number)
{
	ships_select_buttons[sheep_number] = team_number;
	game_p__teams[sheep_number] = team_number;
}

void MenuFunctions::Exit()
{
	exit(0);
}

//Function changes and returns value of option.
bool MenuFunctions::ChangeOption(GameTypes::game_rules_t option_id)
{
	if (*game_p__game_rules & option_id)
	{
		*game_p__game_rules &= GAME_RULES_T_MAX - option_id;
		return false;
	}
	*game_p__game_rules |= option_id;
	return true;
}

void MenuFunctions::MainMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < game_p__main_menu->GetButtonsCount(); i++)
	{
		if (game_p__main_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (game_p__main_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_START_MATCH:
				StartGame();
				return;
			case BUTTON_ID_GO_TO_OPTINS_MENU:
				OpenOptionsMenu();
				return;
			case BUTTON_ID_EXIT:
				Exit();
			default:
				return;
			}
		}
	}
}

void MenuFunctions::OptionMenuFunction(Vec2F* clk_pos)
{
	Button* current_button;
	for (uint8_t i = 0; i < game_p__option_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__option_menu->current_buttons[i];
		if (current_button->HavePoint(clk_pos))
		{
			switch (current_button->GetId())
			{
			case BUTTON_ID_SET_RANDOM_SPAWN:
				std::cout << clk_pos->x << ' ' << clk_pos->y << std::endl;
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE);
				return;
			case BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_BONUS:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_BONUS);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_START_BONUS_RANDOMIZE);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BUFF:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS);
				return;
			case BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD);
				return;
			case BUTTON_ID_SET_ACTIVE_BALANCE:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_BALANCE_ACTIVE);
				return;
			case BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE);
				return;
			case BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_FRENDLY_FIRE);
				return;
			case BUTTON_ID_SET_PILOT_CAN_RESPAWN:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_PILOT_CAN_RESPAWN);
				return;
			case BUTTON_ID_SET_NEED_KILL_PILOT:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_NEED_KILL_PILOT);
				return;
			case BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS:
				current_button->status = (ClassTypes::Button::button_status_t)ChangeOption(GAME_RULE_KNIFES_CAN_BREAKE_BULLETS);
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
	for (uint8_t i = 0; i < game_p__pause_menu->GetButtonsCount(); i++)
	{
		if (game_p__pause_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (game_p__pause_menu->current_buttons[i].GetId())
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

void MenuFunctions::ShipsSelectMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < game_p__team_sheeps_select_menu->GetButtonsCount(); i++)
	{
		if (game_p__team_sheeps_select_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (game_p__team_sheeps_select_menu->current_buttons[i].GetId())
			{
			case BUTTON_ID_SELECT_SHIP_1:
				switch (game_p__team_sheeps_select_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(0, SHIPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(0, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(0, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(0, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(0, SHIPS_SELECT_BUTTONS_NO_TEAM);
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHIP_2:
				switch (game_p__team_sheeps_select_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_NO_TEAM);
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHIP_3:
				switch (game_p__team_sheeps_select_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_NO_TEAM);
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHIP_4:
				switch (game_p__team_sheeps_select_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_RED);
					return;
				case BUTTON_STATUS_CUSTOM_0:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
					game_p__team_sheeps_select_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					SelectSheep(1, SHIPS_SELECT_BUTTONS_NO_TEAM);
					game_p__team_sheeps_select_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
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
	for (uint8_t i = 0; i < game_p__map_pull_select_menu->GetButtonsCount(); i++)
	{
		if (game_p__map_pull_select_menu->current_buttons[i].HavePoint(clk_pos))
		{
			uint32_t id = game_p__map_pull_select_menu->current_buttons[i].GetId() - BUTTON_ID_SELECT_MAP;
			if (id < MAPS_COUNT)
			{
				game_p__map_pull_array[id]++;
				return;
			}
		}
	}
}

void MenuFunctions::SpawnObjectsSelectMenuFunction(Vec2F* clk_pos)
{
	for (uint8_t i = 0; i < game_p__spawning_objects_select_menu->GetButtonsCount(); i++)
	{
		if (game_p__spawning_objects_select_menu->current_buttons[i].HavePoint(clk_pos))
		{
			uint32_t id = game_p__spawning_objects_select_menu->current_buttons[i].GetId() - BUTTON_ID_SELECT_OBJECT;
			if (id < OBJECTS_COUNT)
			{
				game_p__object_pull_array[id]++;
				return;
			}
		}
	}
}


void MenuFunctions::ShipsControlMenuFunction(Vec2F* clk_pos)
{

}

MenuFunctions::~MenuFunctions()
{}