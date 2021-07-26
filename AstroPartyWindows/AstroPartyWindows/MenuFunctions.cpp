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

void MenuFunctions::SelectSheep(uint8_t sheep_number)
{
	
}

void MenuFunctions::SelectSheep(uint8_t sheep_number, uint8_t team_number)
{

}

void MenuFunctions::Exit()
{

}

void MenuFunctions::ChangeOption(uint32_t option_number)
{

}

void MenuFunctions::MainMenuFunction(Vec2F* clk_pos)
{

}

void MenuFunctions::OptionMenuFunction(Vec2F* clk_pos)
{

}

void MenuFunctions::PauseMenuFunction(Vec2F* clk_pos)
{

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
					MenuFunctions::sheeps_select_buttons[0] = SHEEPS_SELECT_BUTTONS_TEAM_RED;
					return;
				case BUTTON_STATUS_CUSTOM_0:
					MenuFunctions::sheeps_select_buttons[0] = SHEEPS_SELECT_BUTTONS_TEAM_BLUE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					MenuFunctions::sheeps_select_buttons[0] = SHEEPS_SELECT_BUTTONS_TEAM_GREEN;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					MenuFunctions::sheeps_select_buttons[0] = SHEEPS_SELECT_BUTTONS_TEAM_PURPURE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					MenuFunctions::sheeps_select_buttons[0] = SHEEPS_SELECT_BUTTONS_NO_TEAM;
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_2:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					MenuFunctions::sheeps_select_buttons[1] = SHEEPS_SELECT_BUTTONS_TEAM_RED;
					return;
				case BUTTON_STATUS_CUSTOM_0:
					MenuFunctions::sheeps_select_buttons[1] = SHEEPS_SELECT_BUTTONS_TEAM_BLUE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					MenuFunctions::sheeps_select_buttons[1] = SHEEPS_SELECT_BUTTONS_TEAM_GREEN;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					MenuFunctions::sheeps_select_buttons[1] = SHEEPS_SELECT_BUTTONS_TEAM_PURPURE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					MenuFunctions::sheeps_select_buttons[1] = SHEEPS_SELECT_BUTTONS_NO_TEAM;
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_3:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					MenuFunctions::sheeps_select_buttons[2] = SHEEPS_SELECT_BUTTONS_TEAM_RED;
					return;
				case BUTTON_STATUS_CUSTOM_0:
					MenuFunctions::sheeps_select_buttons[2] = SHEEPS_SELECT_BUTTONS_TEAM_BLUE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					MenuFunctions::sheeps_select_buttons[2] = SHEEPS_SELECT_BUTTONS_TEAM_GREEN;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					MenuFunctions::sheeps_select_buttons[2] = SHEEPS_SELECT_BUTTONS_TEAM_PURPURE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					MenuFunctions::sheeps_select_buttons[2] = SHEEPS_SELECT_BUTTONS_NO_TEAM;
					Game::current_active_menu->current_buttons[i].status = BUTTOM_STATUS_INACTIVE;
					return;
				}
			case BUTTON_ID_SELECT_SHEEP_4:
				switch (Game::current_active_menu->current_buttons[i].status)
				{
				case BUTTOM_STATUS_INACTIVE:
					Game::current_active_menu->current_buttons[i].status = BUTTON_STATUS_CUSTOM_0;
					MenuFunctions::sheeps_select_buttons[3] = SHEEPS_SELECT_BUTTONS_TEAM_RED;
					return;
				case BUTTON_STATUS_CUSTOM_0:
					MenuFunctions::sheeps_select_buttons[3] = SHEEPS_SELECT_BUTTONS_TEAM_BLUE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_1:
					MenuFunctions::sheeps_select_buttons[3] = SHEEPS_SELECT_BUTTONS_TEAM_GREEN;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_2:
					MenuFunctions::sheeps_select_buttons[3] = SHEEPS_SELECT_BUTTONS_TEAM_PURPURE;
					Game::current_active_menu->current_buttons[i].status++;
					return;
				case BUTTON_STATUS_CUSTOM_3:
				default:
					MenuFunctions::sheeps_select_buttons[3] = SHEEPS_SELECT_BUTTONS_NO_TEAM;
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

}

void MenuFunctions::SpawnObjectsSelectMenuFunction(Vec2F* clk_pos)
{

}


void MenuFunctions::SheepsControlMenuFunction(Vec2F* clk_pos)
{

}