#include "GameRealisation.h"

inline void Game::Update::All()
{
	Bullets();
	Bombs();
	Ships();
	Pilots();
	Knifes();
	Lazers();
	Asteroids();
	Bonuses();
}

inline void Game::Update::TransportAll()
{
	TransportBullets();
	TransportBombs();
	TransportSheeps();
	TransportPilots();
	TransportKnifes();
	TransportLazers();
	TransportAsteroids();
	TransportBonuses();
}

void Game::Update::Bombs()
{
	BombsChainReaction();
	
	for (entities_count_t i = 0, found_mines = 0; found_mines < bombs_count && i < GAME_BOMBS_MAX_COUNT; i++)
	{
		Temp::bomb_p = &bombs[i];
		if (Temp::bomb_p->exist == true)
		{
			if (Temp::bomb_p->IsBoom())
			{
				
				for (players_count_t pilot = 0; pilot < players_count; pilot++)
				{
					Temp::pilot_p = &pilots[pilot];
					if (Temp::pilot_p->exist == true && Temp::pilot_p->GetDistance(Temp::bomb_p) == 0.0f)
					{
						if (game_rules & GAME_RULE_NEED_KILL_PILOT)
						{
							if (Temp::bomb_p->GetPlayerMasterNumber() == Temp::pilot_p->GetPlayerNumber())
							{
								scores[Temp::pilot_p->GetPlayerNumber()]--;
							}
							else
							{
								scores[Temp::bomb_p->GetPlayerMasterNumber()]++;
							}
						}
						Temp::ship_p->exist = false;
						ships_count--;
					}
				}
			}
			else if (Temp::bomb_p->IsActive() == false)
			{
				for (players_count_t sheep = 0; sheep < players_count; sheep++)
				{
					Temp::ship_p = &ships[sheep];
					if (Temp::ship_p->exist == true && Temp::ship_p->GetDistance(Temp::bomb_p) == 0.0f)
					{
						Temp::bomb_p->Activate();
					}
				}
			}
		}
		found_mines++;
	}
}

void Game::Update::BulletsDestroy()
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
				if (ships[sheep].exist && Temp::bullet_p->Entity::IsCollision(&ships[sheep]))
				{
					if (ships[sheep].GetActiveBaffs() & BUFF_SHIELD)
					{
						ships[sheep].BreakShield();
					}
					else
					{
						Destroy::Entity(Temp::bullet_p, &ships[sheep]);
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
						Destroy::Entity(Temp::bullet_p, &pilots[pilot]);
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
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
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
									AddEntity(asteroids[asteroid].Division());
								}
							}
							else
							{
								Temp::bonus = asteroids[asteroid].Destroy();
								if (Temp::bonus.bonus_type)
								{
									AddEntity(Temp::bonus);
								}
							}
						}
						else if (asteroids[asteroid].GetSize() > ASTEROID_SIZE_SMALL)
						{
							for (uint8_t div = 0; div < 4; div++)
							{
								AddEntity(asteroids[asteroid].Division());
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

void Game::Update::BombsChainReaction()
{
	bool recursion = true;
	while (recursion == true)
	{
		recursion = false;
		for (entities_count_t bomb = 0, found_bomb = 0; found_bomb < bombs_count && bomb < GAME_BOMBS_MAX_COUNT; bomb++)
		{
			Temp::bomb_p = &bombs[bomb];
			if (Temp::bomb_p->exist == true)
			{
				if (Temp::bomb_p->IsBoom() == true)
				{
					Bomb* temp_bomb;
					for (entities_count_t new_bomb = 0, found_new_bomb = 0; found_new_bomb < bombs_count && new_bomb < GAME_BOMBS_MAX_COUNT; new_bomb++)
					{
						temp_bomb = &bombs[new_bomb];
						if (temp_bomb->exist == true)
						{
							if (new_bomb != bomb && temp_bomb->IsBoom() == false && Temp::bomb_p->GetDistance(temp_bomb) == 0.0f)
							{
								recursion = true;
								temp_bomb->Boom();
							}
							found_new_bomb++;
						}
					}
				}
				found_bomb++;
			}
		}
	}
}