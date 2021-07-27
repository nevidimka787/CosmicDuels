#include "GameRealisation.h"


void Game::Update::Bullets()
{
	//deleting bullets

	for (entities_count_t i = 0, found_bullets = 0; found_bullets < bullets_count && i < GAME_BULLETS_MAX_COUNT; i++)
	{
		if (bullets[i].exist)
		{
			Temp::bullet_p = &bullets[i];
			if (Temp::bullet_p->IsCollision(map))
			{
				DeleteEntity(Temp::bullet_p);
				goto end_cycle;
			}
			for (players_count_t sheep = 0; sheep < players_count; sheep++)
			{
				if (sheeps[sheep].exist && Temp::bullet_p->Entity::IsCollision(&sheeps[sheep]))
				{
					if (sheeps[sheep].GetActiveBaffs() & BUFF_SHIELD)
					{
						sheeps[sheep].BreakShield();
					}
					else
					{
						if (!(game_rules & GAME_RULE_NEED_KILL_PILOT))
						{
							if (Temp::bullet_p->GetPlayerMasterNumber() == sheep && !(game_rules & GAME_RULE_FRENDLY_FIRE))
							{
								scores[Temp::bullet_p->GetPlayerMasterNumber()]--;
							}
							else
							{
								scores[Temp::bullet_p->GetPlayerMasterNumber()]++;
							}
						}

						Bonus new_bonus = sheeps[sheep].LoseBonus();
						for (uint8_t b = 0, count = new_bonus.GetTypesCount() - 1; b < count; b++)
						{
							Temp::bonus = new_bonus.Division();
							AddEntity(&Temp::bonus);
						}
						AddEntity(&new_bonus);
						if (!(game_rules & GAME_RULE_FRENDLY_FIRE))
						{
							pilots[sheep] = sheeps[sheep].Destroy();
							pilots_count++;
							sheeps[sheep].exist = false;
							sheeps_count--;
						}
					}

					DeleteEntity(Temp::bullet_p);
					goto end_cycle;
				}
			}
			for (players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				if (pilots[pilot].exist && Temp::bullet_p->Entity::IsCollision(&pilots[pilot]))
				{
					if (game_rules & BUTTON_ID_SET_NEED_KILL_PILOT)
					{
						if (Temp::bullet_p->GetPlayerMasterNumber() == pilot && !(game_rules & GAME_RULE_FRENDLY_FIRE))
						{
							scores[Temp::bullet_p->GetPlayerMasterNumber()]--;
						}
						else
						{
							scores[Temp::bullet_p->GetPlayerMasterNumber()]++;
						}

						if (!(game_rules & GAME_RULE_FRENDLY_FIRE))
						{
							pilots[pilot].exist = false;
							pilots_count--;
						}
						DeleteEntity(Temp::bullet_p);
						goto end_cycle;
					}
					else
					{
						pilots[pilot].exist = false;
						pilots_count--;
					}
				}
			}
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asterooids_count; asteroid++)
			{
				if (asteroids[asteroid].exist)
				{
					if (Temp::bullet_p->Entity::IsCollision(&asteroids[asteroid]))
					{
						if (asteroids[asteroid].GetBuffBonus())
						{
							if (asteroids[asteroid].GetSize() > ASTEROID_SIZE_SMALL)
							{
								for (uint8_t div = 0; div < 2; div++)
								{
									Temp::asteroid = asteroids[asteroid].Division();
									AddEntity(&Temp::asteroid);
								}
							}
							else
							{
								Temp::bonus = asteroids[asteroid].Destroy();
								if (Temp::bonus.bonus_type)
								{
									AddEntity(&Temp::bonus);
								}
							}
						}
						else if (asteroids[asteroid].GetSize() > ASTEROID_SIZE_SMALL)
						{
							for (uint8_t div = 0; div < 4; div++)
							{
								Temp::asteroid = asteroids[asteroid].Division();
								AddEntity(&Temp::asteroid);
							}
						}
						DeleteEntity(&asteroids[asteroid]);

						DeleteEntity(Temp::bullet_p);
						goto end_cycle;
					}
					found_asteroids++;
				}
			}
			found_bullets++;
		end_cycle:;
		}
	}

	//deleting bullets
}

void Game::Update::Mines()
{
	MinesCollisions();
	Mine* mine;
	for (entities_count_t i = 0, found_mines = 0; found_mines < mines_count && i < GAME_MINES_MAX_COUNT; i++)
	{
		mine = &mines[i];
		if (mine->exist == true)
		{
			if (mine->IsActive())
			{
				mine->Recalculate();
			}
			else
			{
				for (players_count_t sheep = 0; sheep < players_count; sheep++)
				{
					if (sheeps[sheep].GetDistance(mine) == 0.0f)
					{
						if (mine->IsBoom())
						{
							pilots[sheep] = sheeps[sheep].Destroy();
							pilots_count++;
							sheeps[sheep].exist = false;
							sheeps_count--;
						}
						else
						{
							mine->Activate();
						}
					}
				}
			}
		}
		found_mines++;
	}
}

