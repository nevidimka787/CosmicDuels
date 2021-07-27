#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include <iostream>
#include "Vec.h"
#include "GameRealisation.h"

namespace MenuFunctions
{
#define SHEEPS_SELECT_BUTTONS_NO_TEAM		0
#define SHEEPS_SELECT_BUTTONS_TEAM_RED		1
#define SHEEPS_SELECT_BUTTONS_TEAM_BLUE		2
#define SHEEPS_SELECT_BUTTONS_TEAM_GREEN	3
#define SHEEPS_SELECT_BUTTONS_TEAM_PURPURE	4
	static uint8_t* sheeps_select_buttons;

	static Bonus::bonus_t start_bonus;


	void Back();

	void OpenOptionsMenu();
	void OpenMainMenu();
	void OpenPauseMenu();
	void OpenSheepsSelectMenu();
	void OpenTeamsSelectMenu();
	void OpenMapPullSelectMenu();
	void OpenSpawnObjectsSelectMenu();
	void OpenSheepsControlMenu();

	void StartGame();
	void PauseGame();
	void ResumeGame();
	void SelectSheep(Game::players_count_t sheep_number, uint8_t team_number);
	void Exit();
	void ChangeOption(Game::game_rules_t option_number);

	void MainMenuFunction(Vec2F* clk_pos);
	void OptionMenuFunction(Vec2F* clk_pos);
	void PauseMenuFunction(Vec2F* clk_pos);
	void SheepsSelectMenuFunction(Vec2F* clk_pos);
	void MapPullSelectMenuFunction(Vec2F* clk_pos);
	void SpawnObjectsSelectMenuFunction(Vec2F* clk_pos);
	void SheepsControlMenuFunction(Vec2F* clk_pos);
};

#endif //MENU_FUNCTIONS_H