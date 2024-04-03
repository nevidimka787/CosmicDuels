#include "MenuFunctions.h"
#include <GLFW/glfw3.h>

#include <vector>

#pragma warning(disable : 26495)

inline void SetStatusToButton(Menu* menu, EngineTypes::Button::id_t button_id, EngineTypes::Button::status_t status_mask, bool value);
inline void SetTextToButton(Menu* menu, EngineTypes::Button::id_t button_id, const char* text, EngineTypes::Button::text_size_t text_size = 0);
inline bool MinimalCountTeamsSelected(std::vector<GameTypes::players_count_t>& teams_array, GameTypes::players_count_t array_length, GameTypes::players_count_t players_count);

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
		return;
	}
	if (
		*game_p__current_active_menu == game_p__option_menu ||
		*game_p__current_active_menu == game_p__ships_select_menu ||
		*game_p__current_active_menu == game_p__credits_menu)
	{
		OpenMainMenu();
		return;
	}
	if (
		*game_p__current_active_menu == game_p__map_pull_select_menu ||
		*game_p__current_active_menu == game_p__objects_pull_menu ||
		*game_p__current_active_menu == game_p__bonus_pull_select_menu)
	{
		OpenOptionsMenu();
		return;
	}
	if (*game_p__current_active_menu == game_p__ships_control_menu)
	{
		*game_p__pause_round = true;
		OpenPauseMenu();
		return;
	}
	if (*game_p__current_active_menu == game_p__pause_menu)
	{
		*game_p__pause_round = false;
		*game_p__current_active_menu = game_p__ships_control_menu;
		return;
	}
}

void MenuFunctions::OpenCreditMenu()
{
	*game_p__current_active_menu = game_p__credits_menu;

	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenBonusPullSelectMenu()
{
	*game_p__current_active_menu = game_p__bonus_pull_select_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenOptionsMenu()
{
	UpdateOptionsMenu();
	*game_p__current_active_menu = game_p__option_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenMainMenu()
{
	*game_p__current_active_menu = game_p__main_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenPauseMenu()
{
	*game_p__current_active_menu = game_p__pause_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenSheepsSelectMenu()
{
	*game_p__current_active_menu = game_p__ships_select_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenMapPullSelectMenu()
{
	*game_p__current_active_menu = game_p__map_pull_select_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenSpawnObjectsSelectMenu()
{
	*game_p__current_active_menu = game_p__objects_pull_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::OpenSheepsControlMenu()
{
	*game_p__current_active_menu = game_p__ships_control_menu;
	
	(*game_p__current_active_menu)->SetPosition(std::move(Vec2F()));
}

void MenuFunctions::StartMatch()
{
	*game_p__play_match = true;
}

void MenuFunctions::PauseRaund()
{
	*game_p__pause_round = true;
	OpenPauseMenu();
}

void MenuFunctions::ResumeRaund()
{
	*game_p__pause_round = false;
	OpenSheepsControlMenu();
}

void MenuFunctions::EndRound()
{
	*game_p__play_round = false;
}

void MenuFunctions::EndMatch()
{
	*game_p__pause_round = false;
	*game_p__flag_end_match = true;
}

void MenuFunctions::SelectShip(GameTypes::players_count_t sheep_number, GameTypes::players_count_t team_number)
{
	ships_select_buttons[sheep_number - BUTTON_ID__SELECT_SHIP_1] = team_number;
	(*game_p__teams)[sheep_number - BUTTON_ID__SELECT_SHIP_1] = team_number;
}

void MenuFunctions::Exit()
{
	exit_game = true;
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

inline void SetStatusToButton(Menu* menu, EngineTypes::Button::id_t button_id, EngineTypes::Button::status_t status_mask, bool value)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < menu->GetButtonsCount(); i++)
	{
		if (menu->current_buttons[i].GetId() == button_id)
		{
			menu->current_buttons[i].SetStatus(status_mask, value);
			return;
		}
	}
}

inline void SetTextToButton(Menu* menu, EngineTypes::Button::id_t button_id, const char* text, EngineTypes::Button::text_size_t text_size)
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < menu->GetButtonsCount(); i++)
	{
		if (menu->current_buttons[i].GetId() == button_id)
		{
			menu->current_buttons[i].SetText(text);
			std::cout << "Set text: " << text << std::endl;
			if (text_size)
			{
				menu->current_buttons[i].SetTextSize(text_size);
			}
			return;
		}
	}
}

void MenuFunctions::BonusPullSelectMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__bonus_pull_select_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__bonus_pull_select_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
				EngineTypes::Button::id_t id = game_p__bonus_pull_select_menu->current_buttons[i].GetId() - BUTTON_ID__SELECT_BONUS;
				if (id < GAME_BONUS_INVENTORY_SIZE)
				{
					if ((*game_p__bonus_pull_array)[id])
					{
						(*game_p__bonus_pull_array)[id] = false;
					}
					else
					{
						(*game_p__bonus_pull_array)[id] = true;
					}
					current_button->SetStatus(BUTTON_STATUS_TRUE, (*game_p__bonus_pull_array)[id]);
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void CreditsMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{

}

void MenuFunctions::MainMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;

	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__main_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__main_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			break;
		}
		if (
			!current_button->GetStatus(BUTTON_STATUS_ACTIVE) || 
			!current_button->HavePoint(clk_pos))
		{
			continue;
		}
		switch(clk_status)
		{
		case GLFW_PRESS:
			current_button->SetStatus(BUTTON_STATUS_SELECT, true);
			return;
		case GLFW_RELEASE:
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			switch (current_button->GetId())
			{
			case BUTTON_ID__START_MATCH:
				OpenSheepsSelectMenu();
				return;
			case BUTTON_ID__GO_TO_OPTINS_MENU:
				OpenOptionsMenu();
				return;
			case BUTTON_ID__EXIT:
				Exit();
				return;
			case BUTTON_ID__GO_TO_CREDITS_MENU:
				OpenCreditMenu();
				return;
			default:
				return;
			}
		}
	}
}

void MenuFunctions::OptionMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;
	bool option;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__option_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__option_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
				case BUTTON_ID__SET_RANDOM_SPAWN:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE));
					return;
				case BUTTON_ID__SET_RANDOM_SPAWN_DIRECTION:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE));
					return;
				case BUTTON_ID__SET_SPAWN_THIS_BONUS:
					option = ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_BONUS);
					current_button->SetStatus(BUTTON_STATUS_TRUE, option);
					SetStatusToButton(game_p__option_menu, BUTTON_ID__SET_SPAWN_THIS_DIFFERENT_BONUSES, BUTTON_STATUS_ACTIVE, option);
					SetStatusToButton(game_p__option_menu, BUTTON_ID__GO_TO_SELECT_BONUSES_MENU, BUTTON_STATUS_ACTIVE, option);
					SetStatusToButton(game_p__option_menu, BUTTON_ID__SET_TRIPLE_BONUSES, BUTTON_STATUS_ACTIVE, option);
					return;
				case BUTTON_ID__SET_SPAWN_THIS_DIFFERENT_BONUSES:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS));
					return;
				case BUTTON_ID__SET_TRIPLE_BONUSES:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_TRIPLE_BONUSES));
					return;
				case BUTTON_ID__SET_SPAWN_THIS_TRIPLE_BAFF:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE));
					return;
				case BUTTON_ID__SET_SPAWN_THIS_SHIELD_BAFF:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD));
					return;
				case BUTTON_ID__SET_ACTIVE_BALANCE:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_BALANCE_ACTIVE));
					return;
				case BUTTON_ID__SET_FRIEDLY_SHEEP_CAN_RESTORE:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE));
					return;
				case BUTTON_ID__SET_ACTIVE_FRIENDLY_FIRE:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_FRIENDLY_FIRE));
					return;
				case BUTTON_ID__SET_NEED_KILL_PILOT:
					option = ChangeOption(GAME_RULE_NEED_KILL_PILOT);
					current_button->SetStatus(BUTTON_STATUS_TRUE, option);
					SetStatusToButton(game_p__option_menu, BUTTON_ID__SET_FRIEDLY_SHEEP_CAN_RESTORE, BUTTON_STATUS_ACTIVE, option);
					return;
				case BUTTON_ID__SET_KNIFES_CAN_DESTROY_BULLETS:
					current_button->SetStatus(BUTTON_STATUS_TRUE, ChangeOption(GAME_RULE_KNIFES_CAN_DESTROY_BULLETS));
					return;
				case BUTTON_ID__GO_TO_SELECT_MAP_MENU:
					OpenMapPullSelectMenu();
					return;
				case BUTTON_ID__GO_TO_SELECT_BONUSES_MENU:
					OpenBonusPullSelectMenu();
					return;
				case BUTTON_ID__GO_TO_SELECT_OBJECTS_MENU:
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

void MenuFunctions::PauseMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__pause_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__pause_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
					game_p__pause_menu->current_buttons[i].SetStatus(BUTTON_STATUS_SELECT, false);
					return;
				}
				current_button->SetStatus(BUTTON_STATUS_SELECT, false);
				switch (game_p__pause_menu->current_buttons[i].GetId())
				{
				case BUTTON_ID__GO_TO_MAIN_MENU:
					EndMatch();
					return;
				case BUTTON_ID__RESUME_MATCH:
				default:
					ResumeRaund();
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void MenuFunctions::ShipsSelectMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__ships_select_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__ships_select_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto end_of_button_cycle;
		}
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
				case BUTTON_ID__SELECT_SHIP_1:
				case BUTTON_ID__SELECT_SHIP_2:
				case BUTTON_ID__SELECT_SHIP_3:
				case BUTTON_ID__SELECT_SHIP_4:
					switch (current_button->GetStatus() & (BUTTON_STATUS_CUSTOM_RED | BUTTON_STATUS_CUSTOM_GREEN | BUTTON_STATUS_CUSTOM_BLUE | BUTTON_STATUS_CUSTOM_PURPURE))
					{
					case BUTTON_STATUS_FALSE:
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_RED | BUTTON_STATUS_ACTIVE);
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_RED);
						if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, true);
						}
						else
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, false);
						}
						return;
					case BUTTON_STATUS_CUSTOM_RED:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_GREEN);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_GREEN | BUTTON_STATUS_ACTIVE);
						if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, true);
						}
						else
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, false);
						}
						return;
					case BUTTON_STATUS_CUSTOM_GREEN:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_BLUE);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_BLUE | BUTTON_STATUS_ACTIVE);
						if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, true);
						}
						else
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, false);
						}
						return;
					case BUTTON_STATUS_CUSTOM_BLUE:
						SelectShip(id, SHIPS_SELECT_BUTTONS_TEAM_PURPURE);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_CUSTOM_PURPURE | BUTTON_STATUS_ACTIVE);
						if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, true);
						}
						else
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, false);
						}
						return;
					case BUTTON_STATUS_CUSTOM_PURPURE:
					default:
						SelectShip(id, SHIPS_SELECT_BUTTONS_NO_TEAM);
						current_button->SetOnlyCustomStatus(BUTTON_STATUS_FALSE | BUTTON_STATUS_ACTIVE);
						if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, true);
						}
						else
						{
							SetStatusToButton(game_p__ships_select_menu, BUTTON_ID__START_GAME, BUTTON_STATUS_ACTIVE, false);
						}
						return;
					}
				case BUTTON_ID__START_GAME:
					if (MinimalCountTeamsSelected(*game_p__teams, GAME_PLAYERS_MAX_COUNT, GAME_PLAYERS_MAX_COUNT))
					{
						StartMatch();
					}
				default:
					return;
				}
			}
		}
	end_of_button_cycle:;
	}
}

inline bool MinimalCountTeamsSelected(std::vector<GameTypes::players_count_t>& teams_array, GameTypes::players_count_t array_length, GameTypes::players_count_t players_count)
{
	bool* flags_arr = new bool[players_count];
	uint8_t found_teams = 0u;
	for (GameTypes::players_count_t i = 0; i < players_count; i++)
	{
		flags_arr[i] = false;
	}
	for (GameTypes::players_count_t i = 0; i < array_length; i++)
	{
		if (teams_array[i] != SHIPS_SELECT_BUTTONS_NO_TEAM)
		{
			flags_arr[teams_array[i] - 1] = true;
		}
	}
	for (GameTypes::players_count_t i = 0; i < players_count; i++)
	{
		found_teams += (flags_arr[i]) ? 1u : 0u;
	}
	delete[] flags_arr;
	return found_teams >= 2u;
}

void MenuFunctions::MapPullSelectMenuFunction(Vec2F clk_pos, uint8_t clk_status)
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
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
				EngineTypes::Button::id_t id = game_p__map_pull_select_menu->current_buttons[i].GetId() - BUTTON_ID__SELECT_MAP;
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
					current_button->SetStatus(BUTTON_STATUS_TRUE, (*game_p__map_pull_array)[id]);
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void MenuFunctions::SpawnObjectsSelectMenuFunction(Vec2F clk_pos, uint8_t clk_status)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__objects_pull_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__objects_pull_menu->current_buttons[i];
		if (clk_status == OPEN_GL_REALISATION_BUTTON_LOST)
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);
			goto CycleEnd;
		}
		if (current_button->GetStatus(BUTTON_STATUS_ACTIVE) && current_button->HavePoint(clk_pos))
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
				EngineTypes::Button::id_t id = current_button->GetId() - BUTTON_ID__SELECT_OBJECT;
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
					current_button->SetStatus(BUTTON_STATUS_TRUE, (*game_p__object_pull_array)[id]);
					return;
				}
			}
		}
	CycleEnd:;
	}
}

void MenuFunctions::ShipsControlMenuFunction(
	Vec2F clk_pos,
	uint8_t clk_status,
	std::vector<bool>& rotate_flags,
	std::vector<bool>& shoot_flags,
	std::vector<GameTypes::tic_t>& double_clk_timers,
	std::vector<bool>& burnout_flags)
{
	Button* current_button;
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__ships_control_menu->GetButtonsCount(); i++)
	{
		current_button = &game_p__ships_control_menu->current_buttons[i];
		EngineTypes::Button::id_t id = current_button->GetId();
		if (
			id < BUTTON_ID__SHIP1_SHOOT ||
			id > BUTTON_ID__SHIP4_ROTATE ||
			!current_button->GetStatus(BUTTON_STATUS_ACTIVE))
		{
			continue;
		}
		if (clk_status == GLFW_PRESS && current_button->HavePoint(clk_pos))
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, true);

			if (id % 2 == BUTTON_ID__SHIP1_ROTATE % 2)
			{
				id = (id - BUTTON_ID__SHIP1_ROTATE) / 2;
				if (!rotate_flags[id] && double_clk_timers[id] > 0)
				{
					double_clk_timers[id] = 0;
					burnout_flags[id] = true;
				}
				else
				{
					double_clk_timers[id] = GAME_DOUBLE_CLK_TIME;
				}
				rotate_flags[id] = true;
			}
			else
			{
				shoot_flags[(id - BUTTON_ID__SHIP1_SHOOT) / 2] = true;
			}
		}
		else
		{
			current_button->SetStatus(BUTTON_STATUS_SELECT, false);

			if (id % 2 == BUTTON_ID__SHIP1_ROTATE % 2)
			{
				rotate_flags[(id - BUTTON_ID__SHIP1_SHOOT) / 2] = false;
			}
			else
			{
				shoot_flags[(id - BUTTON_ID__SHIP1_SHOOT) / 2] = false;
			}
		}
	}
}

void MenuFunctions::ShipsControlMenuFunction(
	const std::vector<bool>& rotate_keys_pointers,
	const std::vector<bool>& shoot_keys_pointers,
	std::vector<bool>& rotate_flags,
	std::vector<bool>& shoot_flags,
	std::vector<GameTypes::tic_t>& double_clk_timers,
	std::vector<bool>& burnout_flags)
{
	for (GameTypes::players_count_t id = 0; id < GAME_PLAYERS_MAX_COUNT; id++)
	{
		shoot_flags[id] = shoot_keys_pointers[id];

		if (rotate_keys_pointers[id])
		{
			if (!rotate_flags[id] && double_clk_timers[id] > 0)
			{
				double_clk_timers[id] = 0;
				burnout_flags[id] = true;
			}
			else
			{
				double_clk_timers[id] = GAME_DOUBLE_CLK_TIME;
			}
			rotate_flags[id] = true;
		}
		else
		{
			rotate_flags[id] = false;
		}
	}
}

void MenuFunctions::UpdateOptionsMenu()
{
	for (EngineTypes::Menu::buttons_count_t i = 0; i < game_p__option_menu->GetButtonsCount(); i++)
	{
		Button& button = game_p__option_menu->current_buttons[i];

		switch (button.GetId())
		{
		case BUTTON_ID__GO_TO_SELECT_BONUSES_MENU:
			if (!(*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS))		button.SetStatus(BUTTON_STATUS_ACTIVE, false);
			break;
		case BUTTON_ID__SET_SPAWN_THIS_BONUS:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)			button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_SPAWN_THIS_DIFFERENT_BONUSES:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_DIFFERENT_BONUS) button.SetStatus(BUTTON_STATUS_TRUE, true);
			if (!(*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS))		button.SetStatus(BUTTON_STATUS_ACTIVE, false);
			break;
		case BUTTON_ID__SET_TRIPLE_BONUSES:
			if (*game_p__game_rules & GAME_RULE_TRIPLE_BONUSES)						button.SetStatus(BUTTON_STATUS_TRUE, true);
			if (!(*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS))		button.SetStatus(BUTTON_STATUS_ACTIVE, false);
			break;
		case BUTTON_ID__SET_SPAWN_THIS_TRIPLE_BAFF:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE)			button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_SPAWN_THIS_SHIELD_BAFF:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD)			button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_RANDOM_SPAWN:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)	button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_RANDOM_SPAWN_DIRECTION:
			if (*game_p__game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)	button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_KNIFES_CAN_DESTROY_BULLETS:
			if (*game_p__game_rules & GAME_RULE_KNIFES_CAN_DESTROY_BULLETS)			button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_NEED_KILL_PILOT:
			if (*game_p__game_rules & GAME_RULE_NEED_KILL_PILOT)					button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_FRIEDLY_SHEEP_CAN_RESTORE:
			if (*game_p__game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)			button.SetStatus(BUTTON_STATUS_TRUE, true);
			if (!(*game_p__game_rules & GAME_RULE_NEED_KILL_PILOT))					button.SetStatus(BUTTON_STATUS_ACTIVE, false);
			break;
		case BUTTON_ID__SET_ACTIVE_FRIENDLY_FIRE:
			if (*game_p__game_rules & GAME_RULE_FRIENDLY_FIRE)						button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		case BUTTON_ID__SET_ACTIVE_BALANCE:
			if (*game_p__game_rules & GAME_RULE_BALANCE_ACTIVE)						button.SetStatus(BUTTON_STATUS_TRUE, true);
			break;
		}
	}
}

bool MenuFunctions::ShouldExit()
{
	return exit_game;
}

MenuFunctions::~MenuFunctions()
{}