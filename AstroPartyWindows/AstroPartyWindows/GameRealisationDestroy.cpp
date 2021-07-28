#include "GameRealisation.h"

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Ship* sheep)
{
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
		{
			scores[sheep->GetPlayerNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterNumber()]++;
		}
	}
	for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		Temp::knife_p = &knifes[knife];
		if (Temp::knife_p->exist == true)
		{
			if (Temp::knife_p->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
			{
				DeleteEntity(Temp::knife_p);
			}
			found_knifes++;
		}
	}
	Temp::bonus = sheep->LoseBonus();
	Bonus new_bonus = Temp::bonus.Division();
	while (new_bonus.exist == true)
	{
		AddEntity(new_bonus);
		new_bonus = Temp::bonus.Division();
	}
	AddEntity(Temp::bonus);
	pilots[sheep->GetPlayerNumber()] = sheep->Destroy();
	pilots_count++;
	sheep->exist = false;
	ships_count--;
}