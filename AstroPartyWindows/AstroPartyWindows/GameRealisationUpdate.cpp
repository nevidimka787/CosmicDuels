#include "GameRealisation.h"

inline void Game::Update::All()
{
	Bullets();
	Ships();
	Turels();
	Bombs();
	Lazers();
	MegaLazers();
	Knifes();
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

void Game::Update::TransportAsteroids()
{
	for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
	{
		Temp::asteroid_p = &asteroids[asteroid];
		if (Temp::asteroid_p->exist == true)
		{
			Temp::asteroid_p->Recalculate();
			found_asteroids++;
		}
	}
}

void Game::Update::TransportBullets()
{
	for (entities_count_t bullet = 0, found_bullets = 0; found_bullets < asteroids_count; bullet++)
	{
		Temp::bullet_p = &bullets[bullet];
		if (Temp::bullet_p->exist == true)
		{
			Temp::bullet_p->Recalculate();
			found_bullets++;
		}
	}
}

void Game::Update::TransportBombs()
{
	for (entities_count_t bomb = 0, found_bombs = 0; found_bombs < asteroids_count; bomb++)
	{
		Temp::bomb_p = &bombs[bomb];
		if (Temp::bomb_p->exist == true)
		{
			Temp::bomb_p->Recalculate();
			found_bombs++;
		}
	}
}

void Game::Update::Asteroids()
{}

void Game::Update::Bombs()
{
	BombsChainReaction();
	
	for (entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count && bomb < bombs_count; bomb++)
	{
		Temp::bomb_p = &bombs[bomb];
		if (Temp::bomb_p->exist == true)
		{
			if (Temp::bomb_p->IsBoom())
			{
				for (players_count_t ship = 0; ship < players_count; ship++)
				{
					Temp::ship_p = &ships[ship];
					if (Temp::ship_p->exist == true && Temp::ship_p->IsCollision(Temp::bomb_p))
					{
						Destroy::Entity(Temp::bomb_p, Temp::ship_p);
					}
				}
				for (players_count_t pilot = 0; pilot < players_count; pilot++)
				{
					Temp::pilot_p = &pilots[pilot];
					if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(Temp::bomb_p))
					{
						Destroy::Entity(Temp::bomb_p, Temp::pilot_p);
					}
				}
				for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					Temp::asteroid_p = &asteroids[asteroid];
					if (Temp::bonus_p->exist == true)
					{
						if (Temp::asteroid_p->IsCollision(Temp::bomb_p))
						{
							Destroy::Entity(Temp::bomb_p, Temp::asteroid_p);
						}
						else
						{
							found_asteroids++;
						}
					}
				}
				for (entities_count_t bonus = 0, found_bonus = 0; found_bonus < bonuses_count; bonus++)
				{
					Temp::bonus_p = &bonuses[bonus];
					if (Temp::bonus_p->exist == true)
					{
						if (Temp::bonus_p->IsCollision(Temp::bomb_p))
						{
							Destroy::Entity(Temp::bomb_p, Temp::bonus_p);
						}
						else
						{
							found_bonus++;
						}
					}
				}
				for (entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
				{
					Temp::bullet_p = &bullets[bullet];
					if (Temp::bullet_p->exist == true)
					{
						if (Temp::bullet_p->Entity::IsCollision(Temp::bomb_p))
						{
							Destroy::Entity(Temp::bomb_p, Temp::bullet_p);
						}
						else
						{
							found_bullets++;
						}
					}
				}
				for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					Temp::knife_p = &knifes[knife];
					if (Temp::bullet_p->exist == true)
					{
						if (Temp::knife_p->IsCollision(Temp::bomb_p))
						{
							Destroy::Entity(Temp::bomb_p, Temp::knife_p);
						}
						else
						{
							found_knifes++;
						}
					}
				}
				for (entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
				{
					Temp::turel_p = &turels[turel];
					if (Temp::turel_p->exist == true)
					{
						if (Temp::turel_p->IsCollision(Temp::bomb_p))
						{
							Destroy::Entity(Temp::bomb_p, Temp::turel_p);
						}
						found_turels++;
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
		found_bombs++;
	}
}

void Game::Update::Bonuses()
{}

void Game::Update::Bullets()
{
	for (entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		Temp::bullet_p = &bullets[bullet];
		if (Temp::bullet_p->exist == true)
		{
			if (Temp::bullet_p->IsCollision(map))
			{
				Delete::Entity(Temp::bullet_p);
				goto end_of_cycle;
			}
			for (players_count_t ship = 0; ship < players_count; ship++)
			{
				Temp::ship_p = &ships[ship];
				if (Temp::ship_p->exist == true && Temp::ship_p->IsCollision(Temp::bullet_p))
				{
					if (Temp::ship_p->GetActiveBaffs() & BUFF_SHIELD)
					{
						Temp::ship_p->BreakShield();
					}
					else
					{
						Destroy::Entity(Temp::bullet_p, Temp::ship_p);
					}
					Delete::Entity(Temp::bullet_p);
					goto end_of_cycle;
				}
			}
			for (players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				Temp::pilot_p = &pilots[pilot];
				if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(Temp::bullet_p))
				{
					Destroy::Entity(Temp::bullet_p, Temp::pilot_p);
					if (game_rules & GAME_RULE_NEED_KILL_PILOT)
					{
						Delete::Entity(Temp::bullet_p);
						goto end_of_cycle;
					}
				}
			}
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				Temp::asteroid_p = &asteroids[asteroid];
				if (Temp::asteroid_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(Temp::bullet_p))
					{
						Destroy::Entity(Temp::bullet_p, Temp::asteroid_p);
						Delete::Entity(Temp::bullet_p);
						goto end_of_cycle;
					}
					found_asteroids++;
				}
			}
			if (game_rules & GAME_RULE_KNIFES_CAN_BREAKE_BULLETS)
			{
				for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					Temp::knife_p = &knifes[knife];
					if (Temp::bullet_p->exist == true)
					{
						if (Temp::knife_p->IsCollision(Temp::bullet_p))
						{
							Destroy::Entity(Temp::bullet_p, Temp::knife_p);
							Delete::Entity(Temp::bullet_p);
							goto end_of_cycle;
						}
						found_knifes++;
					}
				}
			}
			for (entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				Temp::bomb_p = &bombs[bomb];
				if (Temp::bullet_p->exist == true)
				{
					if (Temp::bomb_p->IsActive() == false && Temp::bomb_p->IsBoom() == false && Temp::bomb_p->IsCollision(Temp::bullet_p))
					{
						Temp::bomb_p->Activate();
						Delete::Entity(Temp::bullet_p);
						goto end_of_cycle;
					}
					found_bombs++;
				}
			}
		}

		found_bullets++;
	end_of_cycle:;
	}
}

void Game::Update::Knifes()
{
	for (entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		Temp::knife_p = &knifes[knife];
		if (Temp::knife_p->exist == true)
		{
			for (players_count_t ship = 0; ship < players_count; ship++)
			{
				Temp::ship_p = &ships[ship];
				if (Temp::ship_p->exist == true && Temp::ship_p->GetPlayerNumber() != Temp::knife_p->GetPlayerMasterNumber() && Temp::ship_p->IsCollision(Temp::knife_p))
				{
					Destroy::Entity(Temp::knife_p, Temp::ship_p);
					Delete::Entity(Temp::knife_p);
					goto end_of_cycle;
				}
			}
			for (players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				Temp::pilot_p = &pilots[pilot];
				if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(Temp::knife_p))
				{
					Destroy::Entity(Temp::knife_p, Temp::pilot_p);
				}
			}
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				Temp::asteroid_p = &asteroids[asteroid];
				if (Temp::asteroid_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(Temp::knife_p))
					{
						Destroy::Entity(Temp::knife_p, Temp::asteroid_p);
						if (Temp::knife_p->LoseHealth() == false)
						{
							Delete::Entity(Temp::knife_p);
							goto end_of_cycle;
						}
					}
					found_asteroids++;
				}
			}
			found_knifes++;
		end_of_cycle:;
		}
	}
}

void Game::Update::Lazers()
{
	for (entities_count_t laser = 0, found_laser = 0; found_laser < lasers_count; laser++)
	{
		Temp::lazer_p = &lasers[laser];
		if (Temp::lazer_p->exist == true)
		{
			Temp::beam = Temp::lazer_p->GetBeam();
			for (players_count_t ship = 0; ship < players_count; ship++)
			{
				Temp::ship_p = &ships[ship];
				if (Temp::ship_p->exist == true && Temp::ship_p->IsCollision(&Temp::beam))
				{
					Destroy::Entity(Temp::lazer_p, Temp::ship_p);
				}
			}
			for (players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				Temp::pilot_p = &pilots[pilot];
				if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(&Temp::beam))
				{
					Destroy::Entity(Temp::lazer_p, Temp::pilot_p);
				}
			}
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				Temp::asteroid_p = &asteroids[asteroid];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(&Temp::beam))
					{
						Destroy::Entity(Temp::lazer_p, Temp::asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				Temp::asteroid_p = &asteroids[bonus];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(&Temp::beam))
					{
						Destroy::Entity(Temp::lazer_p, Temp::asteroid_p);
					}
					else
					{
						found_bonuses++;
					}
				}
			}
			for (entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				Temp::bullet_p = &bullets[bullet];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::bullet_p->Entity::IsCollision(&Temp::beam))
					{
						Destroy::Entity(Temp::lazer_p, Temp::bullet_p);
					}
					else
					{
						found_bullets++;
					}
				}
			}
			for (entities_count_t turel = 0, found_turels = 0; found_turels < bullets_count; turel++)
			{
				Temp::turel_p = &turels[turel];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::bullet_p->Entity::IsCollision(&Temp::beam))
					{
						Destroy::Entity(Temp::lazer_p, Temp::bullet_p);
					}
					else
					{
						found_turels++;
					}
				}
			}
			found_laser++;
		}
	}
}

void Game::Update::MegaLazers()
{
	for (entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lazers_count; mega_laser++)
	{
		Temp::mega_lazer_p = &mega_lasers[mega_laser];
		if (Temp::mega_lazer_p->exist == true)
		{
			Temp::segment = Temp::mega_lazer_p->GetSegment();
			for (players_count_t ship = 0; ship < players_count; ship++)
			{
				Temp::ship_p = &ships[ship];
				if (Temp::ship_p->exist == true && Temp::ship_p->IsCollision(&Temp::segment))
				{
					Destroy::Entity(Temp::mega_lazer_p, Temp::ship_p);
				}
			}
			for (players_count_t pilot = 0; pilot < players_count; pilot++)
			{
				Temp::pilot_p = &pilots[pilot];
				if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(&Temp::segment))
				{
					Destroy::Entity(Temp::mega_lazer_p, Temp::pilot_p);
				}
			}
			for (entities_count_t asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				Temp::asteroid_p = &asteroids[asteroid];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(&Temp::segment))
					{
						Destroy::Entity(Temp::mega_lazer_p, Temp::asteroid_p);
					}
					else
					{
						found_asteroids++;
					}
				}
			}
			for (entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				Temp::asteroid_p = &asteroids[bonus];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::asteroid_p->IsCollision(&Temp::segment))
					{
						Destroy::Entity(Temp::mega_lazer_p, Temp::asteroid_p);
					}
					else
					{
						found_bonuses++;
					}
				}
			}
			for (entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				Temp::bullet_p = &bullets[bullet];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::bullet_p->Entity::IsCollision(&Temp::segment))
					{
						Destroy::Entity(Temp::mega_lazer_p, Temp::bullet_p);
					}
					else
					{
						found_bullets++;
					}
				}
			}
			found_mega_lasers++;
		}
	}
}

void Game::Update::Pilots()
{
	for (players_count_t pilot = 0; pilot < players_count; pilot++)
	{
		Temp::pilot_p = &pilots[pilot];
		if (Temp::pilot_p->exist == true)
		{
			if (shoot_flags[pilot] == true)
			{
				Temp::pilot_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_PILOT);
			}
			if (rotate_flags[pilot] == true)
			{
				if (rotation_inverse == true)
				{
					Temp::pilot_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					Temp::pilot_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				Temp::pilot_p->SetAngularVelocity(0.0f);
			}
		}
	}
}

void Game::Update::Ships()
{
	for (players_count_t ship = 0; ship < players_count; ship++)
	{
		Temp::ship_p = &ships[ship];
		if (Temp::ship_p->exist == true)
		{
			Temp::ship_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
			for (entities_count_t pilot = 0; pilot < players_count; pilot++)
			{
				Temp::pilot_p = &pilots[pilot];
				if (Temp::pilot_p->exist == true && Temp::pilot_p->IsCollision(Temp::ship_p) == true)
				{
					if (game_rules & GAME_RULE_PILOT_CAN_RESPAWN && game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE)
					{
						Spawn::Entity(Temp::ship_p, Temp::pilot_p);
					}
					else
					{
						Destroy::Entity(Temp::ship_p, Temp::pilot_p);
					}
				}
			}
			for (entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				Temp::bonus_p = &bonuses[bonus];
				if (Temp::bonus_p->exist == true)
				{
					if (Temp::bonus_p->GetDistance(Temp::ship_p) < GAME_SHIP_INFLUENCE_RADIUS)
					{
						Temp::bonus_p->AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, Temp::ship_p);
					}
					found_bonuses++;
				}
			}
			if (shoot_flags[ship] == true && sheeps_can_shoot_flags[ship] == true)
			{
				ShipShoot::Shoot(Temp::ship_p);
			}
			if (rotate_flags[ship] == true)
			{
				if (rotation_inverse == true)
				{
					Temp::ship_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					Temp::ship_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				Temp::ship_p->SetAngularVelocity(0.0f);
			}
		}
	}
}

void Game::Update::Turels()
{
	for (entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
	{
		Temp::turel_p = &turels[turel];
		if (Temp::turel_p->exist == true)
		{
			if (Temp::turel_p->CanShoot(current_tic))
			{
				Add::Entity(Temp::turel_p->Shoot());
			}
			found_turels++;
		}
	}
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