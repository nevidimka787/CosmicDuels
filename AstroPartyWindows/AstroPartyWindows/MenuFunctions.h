#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include <iostream>

namespace MenuFunctions
{
	void StartGame();
	void OpenOptions();
	void OpenMainMenu();
	void Pause();
	void ResumeGame();
	void SelectSheep(uint8_t sheep_number);
	void SelectSheep(uint8_t sheep_number, uint8_t team_number);
	void Exit();
	void ChangeOption(uint32_t option_number);

	void MainMenuFunction(Vec2F* clk_pos);
	void OptionMenuFunction(Vec2F* clk_pos);
	void PauseMenuFunction(Vec2F* clk_pos);
	void SheepsSelectMenuFunction(Vec2F* clk_pos);
	void TeamsSelectMenuFunction(Vec2F* clk_pos);
	void MapPullSelectMenuFunction(Vec2F* clk_pos);
	void SpawnObjectsSelectMenuFunction(Vec2F* clk_pos);
};

#endif //MENU_FUNCTIONS_H