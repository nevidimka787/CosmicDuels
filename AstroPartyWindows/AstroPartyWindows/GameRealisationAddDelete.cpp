#include"GameRealisation.h"

inline void Game::Add::Entity(Asteroid new_asteroid)
{
	if (asteroids_count < GAME_ASTEROIDS_MAX_COUNT)
	{
		for (entities_count_t asteroid = 0; asteroid < GAME_ASTEROIDS_MAX_COUNT; asteroid++)
		{
			if (asteroids[asteroid].exist == false)
			{
				asteroids[asteroid] = new_asteroid;
				asteroids_count++;
				return;
			}
		}
	}
}

inline void Game::Add::Entity(Bonus new_bonus)
{
	if (bonuses_count < GAME_BONUSES_MAX_COUNT)
	{
		for (entities_count_t bonus = 0; bonus < GAME_BONUSES_MAX_COUNT; bonus++)
		{
			if (bonuses[bonus].exist == false)
			{
				bonuses[bonus] = new_bonus;
				bonuses_count++;
				return;
			}
		}
	}
}

inline void Game::Add::Entity(Bullet new_bullet)
{
	if (bullets_count < GAME_BULLETS_MAX_COUNT)
	{
		for (entities_count_t bullet = 0; bullet < GAME_BULLETS_MAX_COUNT; bullet++)
		{
			if (bullets[bullet].exist == false)
			{
				bullets[bullet] = new_bullet;
				bullets_count++;
				return;
			}
		}
	}
}

void Game::Add::Entity(Knife new_knife)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_KNIFES_MAX_COUNT; particle++)
		{
			if (knifes[particle].exist == false)
			{
				knifes[particle] = new_knife;
				knifes_count++;
				return;
			}
		}
	}
}

inline void Game::Add::Entity(Bomb new_mine)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_BOMBS_MAX_COUNT; particle++)
		{
			if (bombs[particle].exist == false)
			{
				bombs[particle] = new_mine;
				bombs_count++;
				return;
			}
		}
	}
}

inline void Game::Add::Entity(Laser new_lazer)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_LASERS_MAX_COUNT; particle++)
		{
			if (lasers[particle].exist == false)
			{
				lasers[particle] = new_lazer;
				lasers_count++;
				return;
			}
		}
	}
}

inline void Game::Add::Entity(DynamicEntity new_particle)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_PARTICLES_MAX_COUNT; particle++)
		{
			if (particles[particle].exist == false)
			{
				particles[particle] = new_particle;
				bullets_count++;
				return;
			}
		}
	}
}

inline void Game::Delete::Entity(Asteroid* deleting_asteroid)
{
	deleting_asteroid->exist = false;
	asteroids_count--;
}

inline void Game::Delete::Entity(Bonus* deleting_bonus)
{
	deleting_bonus->exist = false;
	bonuses_count--;
}

inline void Game::Delete::Entity(Bullet* deleting_bullet)
{
	deleting_bullet->exist = false;
	bullets_count--;
}

inline void Game::Delete::Entity(Knife* deleting_knife)
{
	deleting_knife->exist = false;
	knifes_count--;
}

inline void Game::Delete::Entity(Bomb* deleting_mine)
{
	deleting_mine->exist = false;
	bombs_count--;
}

inline void Game::Delete::Entity(Laser* deleting_lazer)
{
	deleting_lazer->exist = false;
	lasers_count--;
}

inline void Game::Delete::Particle(DynamicEntity* deleting_particle)
{
	deleting_particle->exist = false;
	particles_count--;
}