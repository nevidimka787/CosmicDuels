#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

namespace MenuFunctions
{
	void* StartGame(void* data);
	void* OpenOptions(void* data);
	void* OpenMainMenu(void* data);
	void* Pause(void* data);
	void* ResumeGame(void* data);
	void* SelectSheep(void* sheep);
	void* SelectSheepTeam1(void* sheep);
	void* SelectSheepTeam2(void* sheep);
	void* Exit(void* data);
	void* ChangeOption(void* option);
};

#endif //MENU_FUNCTIONS_H