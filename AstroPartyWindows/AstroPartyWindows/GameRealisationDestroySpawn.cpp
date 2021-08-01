#include "GameRealisation.h"



inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::Delete::Entity(asteroid);
}

inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bonus);
}

inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bullet);
}

inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::Delete::Entity(knife);
}

inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		score_t* current_score_p = &scores[pilot->GetTeamNumber()];
		if (*current_score_p > 0)
		{
			(*current_score_p)--;
		}
	}
	pilot->exist = false;
	pilots_count--;
}

inline void Game::Update::Destroy::Entity(AggressiveEntity* destried_entity, Ship* ship)
{
	if (destried_entity->exist == false || ship->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		score_t* current_score_p = &scores[ship->GetTeamNumber()];
		if(current_score_p > 0)
		{
			(*current_score_p)--;
		}
	}
	ship->exist = false;
	ships_count--;
}



inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bonus);
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bullet);
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::Delete::Entity(knife);
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	pilot->exist = false;
	pilots_count--;
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Ship* sheep)
{
	if (destried_entity->exist == false || sheep->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == sheep->GetTeamNumber())
		{
			scores[sheep->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		Temp::knife_p = &knifes[knife];
		if (Temp::knife_p->exist == true)
		{
			if (Temp::knife_p->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
			{
				Delete::Entity(Temp::knife_p);
			}
			found_knifes++;
		}
	}
	Temp::bonus = sheep->LoseBonus();
	Bonus new_bonus = Temp::bonus.Division();
	while (new_bonus.exist == true)
	{
		Add::Entity(new_bonus);
		new_bonus = Temp::bonus.Division();
	}
	Add::Entity(Temp::bonus);
	pilots[sheep->GetPlayerNumber()] = sheep->Destroy();
	pilots_count++;
	sheep->exist = false;
	ships_count--;
}

inline void Game::Update::Destroy::Entity(KillerEntity* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::Delete::Entity(turel);
}



inline void Game::Update::Destroy::Entity(Laser* destried_entity, Asteroid* asteroid)
{
	if (destried_entity->exist == false || asteroid->exist == false)
	{
		return;
	}
	Game::Delete::Entity(asteroid);
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Bonus* bonus)
{
	if (destried_entity->exist == false || bonus->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bonus);
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Bullet* bullet)
{
	if (destried_entity->exist == false || bullet->exist == false)
	{
		return;
	}
	Game::Delete::Entity(bullet);
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Knife* knife)
{
	if (destried_entity->exist == false || knife->exist == false)
	{
		return;
	}
	Game::Delete::Entity(knife);
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	pilot->exist = false;
	pilots_count--;
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Ship* sheep)
{
	if (destried_entity->exist == false || sheep->exist == false)
	{
		return;
	}
	if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
	{
		if (destried_entity->GetPlayerMasterTeamNumber() == sheep->GetTeamNumber())
		{
			scores[sheep->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetPlayerMasterTeamNumber()]++;
		}
	}
	for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		Temp::knife_p = &knifes[knife];
		if (Temp::knife_p->exist == true)
		{
			if (Temp::knife_p->GetPlayerMasterNumber() == sheep->GetPlayerNumber())
			{
				Delete::Entity(Temp::knife_p);
			}
			found_knifes++;
		}
	}
	Temp::bonus = sheep->LoseBonus();
	Bonus new_bonus = Temp::bonus.Division();
	while (new_bonus.exist == true)
	{
		Add::Entity(new_bonus);
		new_bonus = Temp::bonus.Division();
	}
	Add::Entity(Temp::bonus);
	pilots[sheep->GetPlayerNumber()] = sheep->Destroy();
	pilots_count++;
	sheep->exist = false;
	ships_count--;
}

inline void Game::Update::Destroy::Entity(Laser* destried_entity, Turel* turel)
{
	if (destried_entity->exist == false || turel->exist == false)
	{
		return;
	}
	Game::Delete::Entity(turel);
}



inline void Game::Update::Destroy::Entity(Ship* destried_entity, Pilot* pilot)
{
	if (destried_entity->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_NEED_KILL_PILOT)
	{
		if (destried_entity->GetTeamNumber() == pilot->GetTeamNumber())
		{
			scores[pilot->GetTeamNumber()]--;
		}
		else
		{
			scores[destried_entity->GetTeamNumber()]++;
		}
	}
	pilot->exist = false;
	pilots_count--;
}



inline void Game::Update::Spawn::Entity(Ship* spawner, Pilot* pilot)
{
	if (spawner->exist == false || pilot->exist == false)
	{
		return;
	}
	if (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)
	{
		Game::Add::Entity(pilot->Respawn());
		pilot->exist = false;
		pilots_count--;
	}
}