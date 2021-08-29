#include "MenuFunctions.h"
#include <GLFW/glfw3.h>

#pragma warning(disable : 26495)

MenuFunctions::MenuFunctions()
{
	ships_select_buttons = new GameTypes::players_count_t[GAME_PLAYERS_MAX_COUNT];
	for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
	{
		ships_select_buttons[i] = SHIPS_SELECT_BUTTONS_NO_TEAM;
	}
}

void::MenuFunctions::Back()
{
	if (*game_p__current_active_menu == game_p__main_menu)
	{
		Exit();
	}
	else if (*game_p__current_active_menu == game_p__option_menu || *game_p__current_active_menu == game_p__ships_select_menu)
	{
		OpenMainMenu();
	}
	else if (*game_p__current_active_menu == game_p__map_pull_select_menu || *game_p__current_active_menu == game_p__spawning_objects_select_menu)
	{
		OpenOptionsMenu();
	}
	else if (*game_p__current_active_menu == game_p__ships_control_menu)
	{
		*game_p__pause_game = true;
		OpenPauseMenu();
	}
	else if (*game_p__current_active_menu == game_p__pause_menu)
	{
		*game_p__pause_game = false;
		*game_p__current_active_menu = game_p__ships_control_menu;
	}
}

void MenuFunctions::OpenOptionsMenu()
{
	*game_p__current_active_menu = game_p__option_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenMainMenu()
{
	*game_p__current_active_menu = game_p__main_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenPauseMenu()
{
	*game_p__current_active_menu = game_p__pause_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenSheepsSelectMenu()
{
	*game_p__current_active_menu = game_p__ships_select_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenMapPullSelectMenu()
{
	*game_p__current_active_menu = game_p__map_pull_select_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenSpawnObjectsSelectMenu()
{
	*game_p__current_active_menu = game_p__spawning_objects_select_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}

void MenuFunctions::OpenSheepsControlMenu()
{
	*game_p__current_active_menu = game_p__ships_control_menu;
	Vec2F position = Vec2F();
	(*game_p__current_active_menu)->SetPosition(&position);
}



void MenuFunctions::StartGame()
{
	*game_p__start_game = true;
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

void MenuFunctions::SelectShip(GameTypes::players_count_t sheep_number, GameTypes::players_count_t team_number)
{
	ships_select_buttons[sheep_number - BUTTON_ID_SELECT_SHIP_1] = team_number;
	(*game_p__teams)[sheep_number - BUTTON_ID_SELECT_SHIP_1] = team_number;
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

void MenuFunctions::MainMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__main_menu->GetButtonsCount(); i++)
	{
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			game_p__main_menu->current_buttons[i].SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (game_p__main_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch(clk_status)
			{
			case GLFW_PRESS:
				game_p__main_menu->current_buttons[i].SetStatus(BUTTON_STATUS_SELECT, true);
				return;
			case GLFW_RELEASE:
				game_p__main_menu->current_buttons[i].SetStatus(BUTTON_STATUS_SELECT, false);
				switch (game_p__main_menu->current_buttons[i].GetId())
				{
				case BUTTON_ID_START_MATCH:
					OpenSheepsSelectMenu();
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
	CycleEnd:;
	}
}

void MenuFunctions::OptionMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__option_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__option_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->HavePoint(clk_pos))
		{
			switch (clk_status)
			{
			case GLFW_PRESS:
				last_select_button_index = i;
				current_button->SetStatus(BUTTON_STATUS_SELECT, true);
				return;
			case GLFW_RELEASE:
				if (last_select_button_index != i)
				{
					return;
				}
				current_button->SetStatus(BUTTON_STATUS_SELECT, false);
				switch (current_button->GetId())
				{
				case BUTTON_ID_SET_RANDOM_SPAWN:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE));
					return;
				case BUTTON_ID_SET_RANDOM_SPAWN_DIRECTION:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE));
					return;
				case BUTTON_ID_SET_SPAWN_THIS_BONUS:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_BONUS));
					return;
				case BUTTON_ID_SET_SPAWN_THIS_RANDOM_BONUS:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_START_BONUS_RANDOMIZE));
					return;
				case BUTTON_ID_SET_SPAWN_THIS_TRIPLE_BUFF:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS));
					return;
				case BUTTON_ID_SET_SPAWN_THIS_SHIELD_BAFF:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD));
					return;
				case BUTTON_ID_SET_ACTIVE_BALANCE:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_BALANCE_ACTIVE));
					return;
				case BUTTON_ID_SET_FRIEDLY_SHEEP_CAN_RESTORE:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE));
					return;
				case BUTTON_ID_SET_ACTIVE_FRIENDLY_FIRE:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_FRENDLY_FIRE));
					return;
				case BUTTON_ID_SET_PILOT_CAN_RESPAWN:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_PILOT_CAN_RESPAWN));
					return;
				case BUTTON_ID_SET_NEED_KILL_PILOT:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_NEED_KILL_PILOT));
					return;
				case BUTTON_ID_SET_KNIFES_CAN_DESTROY_BULLETS:
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, ChangeOption(GAME_RULE_KNIFES_CAN_DESTROY_BULLETS));
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
	CycleEnd:;
	}
}

void MenuFunctions::PauseMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__pause_menu->GetButtonsCount(); i++)
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

void MenuFunctions::ShipsSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__ships_select_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__ships_select_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->HavePoint(clk_pos))
		{
			unsigned id;
			switch (clk_status)
			{
			case GLFW_PRESS:
				last_select_button_index = i;
				current_button->SetStatus(BUTTON_STATUS_SELECT, true);
				return;
			case GLFW_RELEASE:
				current_button->SetStatus(BUTTON_STATUS_SELECT, false);
				if (last_select_button_index != i)
				{
					return;
				}
				id = current_button->GetId();
				switch (id)
				{
				case BUTTON_ID_SELECT_SHIP_1:
				case BUTTON_ID_SELECT_SHIP_2:
				case BUTTON_ID_SELECT_SHIP_3:
				case BUTTON_ID_SELECT_SHIP_4:
					switch (current_button->status & (BUTTON_STATUS_CUSTOM_RED | BUTTON_STATUS_CUSTOM_GREEN | BUTTON_STATUS_CUSTOM_BLUE | BUTTON_STATUS_CUSTOM_PURPURE))
					{
					case BUTTON_STATUS_INACTIVE:
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_RED);
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_RED);
						return;
					case BUTTON_STATUS_CUSTOM_RED:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_GREEN);
						return;
					case BUTTON_STATUS_CUSTOM_GREEN:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_BLUE);
						return;
					case BUTTON_STATUS_CUSTOM_BLUE:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_PURPURE);
						return;
					case BUTTON_STATUS_CUSTOM_PURPURE:
					default:
						SelectShip(id, SHIPS_SELECT_BUTTONS_NO_TEAM);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_INACTIVE);
						return;
					}
				case BUTTON_ID_START_GAME:
					for (GameTypes::players_count_t i = 0; i < GAME_PLAYERS_MAX_COUNT; i++)
					{
						if  ((*game_p__teams)[i] != SHIPS_SELECT_BUTTONS_NO_TEAM)
						{
							//std::cout << (*game_p__teams)[i] << std::endl;
							StartGame();
							return;
						}
					}
					SelectShip(BUTTON_ID_SELECT_SHIP_1, SHIPS_SELECT_BUTTONS_TEAM_RED);
					game_p__ships_select_menu->current_buttons[0].SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_RED);
				default:
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void MenuFunctions::MapPullSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__map_pull_select_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__map_pull_select_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (game_p__map_pull_select_menu->current_buttons[i].HavePoint(clk_pos))
		{
			switch (clk_status)
			{
			case GLFW_PRESS:
				last_select_button_index = i;
				current_button->SetStatus(BUTTON_STATUS_SELECT, true);
				return;
			case GLFW_RELEASE:
				current_button->SetStatus(BUTTON_STATUS_SELECT, false);
				if (last_select_button_index != i)
				{
					return;
				}
				EngineTypes::Button::button_id_t id = game_p__map_pull_select_menu->current_buttons[i].GetId() - BUTTON_ID_SELECT_MAP;
				if (id < GAME_MAPS_COUNT)
				{
					if ((*game_p__map_pull_array)[id])
					{
						(*game_p__map_pull_array)[id] = false;
					}
					else
					{
						(*game_p__map_pull_array)[id] = true;
					}
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, (*game_p__map_pull_array)[id]);
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void MenuFunctions::SpawnObjectsSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__spawning_objects_select_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__spawning_objects_select_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->HavePoint(clk_pos))
		{
			switch (clk_status)
			{
			case GLFW_PRESS:
				last_select_button_index = i;
				current_button->SetStatus(BUTTON_STATUS_SELECT, true);
				return;
			case GLFW_RELEASE:
				current_button->SetStatus(BUTTON_STATUS_SELECT, false);
				if (last_select_button_index != i)
				{
					return;
				}
				EngineTypes::Button::button_id_t id = current_button->GetId() - BUTTON_ID_SELECT_OBJECT;
				if (id < GAME_OBJECTS_COUNT)
				{
					if ((*game_p__object_pull_array)[id])
					{
						(*game_p__object_pull_array)[id] = false;
					}
					else
					{
						(*game_p__object_pull_array)[id] = true;
					}
					current_button->SetStatus(BUTTON_STATUS_ACTIVE, (*game_p__object_pull_array)[id]);
					return;
				}
			}
		}
	CycleEnd:;
	}
}


void MenuFunctions::ShipsControlMenuFunction(Vec2F* clk_pos, uint8_t clk_status)
{

}

void Scroll()
{

}

MenuFunctions::~MenuFunctions()
{}