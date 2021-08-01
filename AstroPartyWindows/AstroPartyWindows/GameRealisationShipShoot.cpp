#include "GameRealisation.h"

void Game::ShipShoot::Shoot(Ship* ship)
{
	//Magicka and Magicka 2 are the best games I've seen.
	if (ship->SpendBonus(BONUS_LASER))
	{
		if (ship->SpendBonus(BONUS_LOOP))
		{
			if (ship->SpendBonus(BONUS_BOMB))
			{
				if (ship->SpendBonus(BONUS_KNIFE))
				{
					LaserLoopBombKnife(ship);
					return;
				}
				LaserLoopBomb(ship);
				return;
			}
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				LaserLoopKnife(ship);
				return;
			}
			LaserLoop(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				LaserBombKnife(ship);
				return;
			}
			LaserBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			LaserKnife(ship);
			return;
		}
		Laser(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_LOOP))
	{
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				LoopBombKnife(ship);
				return;
			}
			LoopBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			LoopKnife(ship);
			return;
		}
		Loop(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_BOMB))
	{
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			BombKnife(ship);
			return;
		}
		Bomb(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_KNIFE))
	{
		Knife(ship);
		return;
	}
	NoBonus(ship);
	return;
}

inline void Game::ShipShoot::LaserLoopBombKnife(Ship* ship)
{

}

inline void Game::ShipShoot::LaserLoopBomb(Ship* ship)
{

}

inline void Game::ShipShoot::LaserLoopKnife(Ship* ship)
{

}

inline void Game::ShipShoot::LaserBombKnife(Ship* ship)
{

}

inline void Game::ShipShoot::LoopBombKnife(Ship* ship)
{

}

inline void Game::ShipShoot::LaserLoop(Ship* ship)
{

}

inline void Game::ShipShoot::LaserBomb(Ship* ship)
{

}

inline void Game::ShipShoot::LoopBomb(Ship* ship)
{

}

inline void Game::ShipShoot::LaserKnife(Ship* ship)
{

}

inline void Game::ShipShoot::LoopKnife(Ship* ship)
{

}

inline void Game::ShipShoot::BombKnife(Ship* ship)
{

}

inline void Game::ShipShoot::Knife(Ship* ship)
{
	Add::Entity(ship->CreateKnife(0));
	Add::Entity(ship->CreateKnife(1));
}

inline void Game::ShipShoot::Bomb(Ship* ship)
{
	Add::Entity(ship->CreateBomb());
}

inline void Game::ShipShoot::Loop(Ship* ship)
{
	for (entities_count_t bullet = 0; bullet < BULLETS_IN_LOOP; bullet++)
	{
		Add::Entity(ship->CreateLoop(bullet));
	}
}

inline void Game::ShipShoot::Laser(Ship* ship)
{
	Add::Entity(ship->CreateLazer());
}

inline void Game::ShipShoot::NoBonus(Ship* ship)
{
	Add::Entity(ship->CreateBullet());
}
