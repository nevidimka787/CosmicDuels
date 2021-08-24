#pragma once

#include <iostream>
#include "../Classes/Vec.h"
#include "../Classes/GameEngine.h"
#include "../Classes/Menu.h"

#include "../Constants/MenuRealisationConstants.h"
#include "../Constants/GameRealisationConstants.h"
#include "../Constants/OpenGLRealisationConstants.h"

#include "../Types/ClassesTypes.h"

class MenuFunctions
{
private:
	ClassTypes::Menu::buttons_count_t last_select_button_index;
public:
	uint8_t* ships_select_buttons;

	ClassTypes::Bonus::bonus_t start_bonus;

	MenuFunctions();

	void Back();

	void OpenOptionsMenu();
	void OpenMainMenu();
	void OpenPauseMenu();
	void OpenSheepsSelectMenu();
	void OpenMapPullSelectMenu();
	void OpenSpawnObjectsSelectMenu();
	void OpenSheepsControlMenu();

	void StartGame();
	void PauseGame();
	void ResumeGame();
	void SelectShip(GameTypes::players_count_t sheep_number, GameTypes::players_count_t team_number);
	void Exit();
	bool ChangeOption(GameTypes::game_rules_t option_number);

	void MainMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void OptionMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void PauseMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void ShipsSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void MapPullSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void SpawnObjectsSelectMenuFunction(Vec2F* clk_pos, uint8_t clk_status);
	void ShipsControlMenuFunction(Vec2F* clk_pos, uint8_t clk_status);

	~MenuFunctions();

	//game pointers

	bool* game_p__start_game;
	bool* game_p__pause_game;
	bool* game_p__map_pull_array;
	bool* game_p__object_pull_array;

	GameTypes::entities_count_t* game_p__teams;
	GameTypes::game_rules_t* game_p__game_rules;
	 
	Menu** game_p__current_active_menu;
	Menu* game_p__option_menu;
	Menu* game_p__main_menu;
	Menu* game_p__pause_menu;
	Menu* game_p__ships_select_menu;
	Menu* game_p__map_pull_select_menu;
	Menu* game_p__spawning_objects_select_menu;
	Menu* game_p__ships_control_menu;
};