#include "GameRealisation.h"
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.
#pragma warning(disable : 4244)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
#pragma warning(disable : 6200)
#pragma warning(disable : 6386)

using namespace Game;


void Game::Recalculate()
{
	//shoots

	Update::All();

	Update::BulletsDestroy();
	SheepsShoot();

	//shoots
	
	//collisions

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count);

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, (DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, (DynamicEntity*)asteroids, asteroids_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count, (DynamicEntity*)pilots, players_count);

	DynamicEntitiesCollisions((DynamicEntity*)ships, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asteroids_count, map);

	DynamicEntitiesAddForce((DynamicEntity*)ships, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)pilots, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)asteroids, asteroids_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)bullets, asteroids_count, grav_gens, grav_gens_count);

	//collisions

	//recalculation

	Update::TransportAll();

	//recalculation
}

void Game::DynamicEntitiesCollisions(DynamicEntity* entities, entities_count_t entities_count)
{
	for (entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities_count; first++)
	{
		if (entities[first].exist)
		{
			for (entities_count_t second = first + 1, found_entites_count2 = found_entites_count1; found_entites_count2 < entities_count; second++)
			{
				if (entities[second].exist)
				{
					entities[first].Collision(&entities[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}

void Game::DynamicEntitiesCollisions(DynamicEntity* entities1, entities_count_t entities1_count, DynamicEntity* entities2, entities_count_t entities2_count)
{
	for (entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities1_count; first++)
	{
		if (entities1[first].exist)
		{
			for (entities_count_t second = 0, found_entites_count2 = 0; found_entites_count2 < entities2_count; second++)
			{
				if (entities2[second].exist)
				{
					entities1[first].Collision(&entities2[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}

void Game::DynamicEntitiesCollisions(DynamicEntity* entities, entities_count_t entities_count, Map* map)
{
	for (entities_count_t first = 0, found_entites_count = 0; found_entites_count < entities_count; first++)
	{
		if (entities[first].exist)
		{
			entities[first].Collision(map);
			found_entites_count++;
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, Vec2F* force)
{
	for (entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			entities[i].AddForce(force);
			found++;
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, GravGen* grav_gen)
{
	Vec2F force;
	float len2;
	for (entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			force = entities[i].GetPosition() - grav_gen->GetPosition();
			len2 = force.x * force.x + force.y * force.y;
			force /= len2 * sqrt(len2) / grav_gen->gravity;
			entities[i].AddForce(&force);

			found++;
		}
	}
}

void Game::DynamicEntitiesAddForce(DynamicEntity* entities, entities_count_t entities_count, GravGen* grav_gens, map_elements_count_t grav_gens_count)
{
	Vec2F force;
	float len2;
	for (map_elements_count_t grav_gen = 0; grav_gen < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen].exist)
		{
			for (entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					force = entities[i].GetPosition() - grav_gens[grav_gen].GetPosition();
					len2 = force.x * force.x + force.y * force.y;
					force /= len2 * sqrt(len2) / grav_gens[grav_gen].gravity;
					entities[i].AddForce(&force);

					found++;
				}
			}
		}
	}
}

void Game::SheepsShoot()
{
	for (players_count_t sheep = 0; sheep < players_count; sheep++)
	{
		Temp::ship_p = &ships[sheep];
		if (Temp::ship_p->exist == true)
		{
			if (Temp::ship_p->GetShootInputValue() && Temp::ship_p->can_shoot)
			{
				bool bonus_activate = false;
				//Magicka and Magicka 2 are the best games I've seen.
				if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_MINE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_MINE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_MINE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::ship_p->HaveBonus(BONUS_LASER))
				{
					AddEntity(Temp::ship_p->CreateLazer());
				}
				else if (Temp::ship_p->HaveBonus(BONUS_LOOP))
				{
					bonus_activate = true;
					for (entities_count_t bullet = 0; bullet < BULLETS_IN_LOOP; bullet++)
					{
						AddEntity(Temp::ship_p->CreateLoop(bullet));
					}
				}
				else if (Temp::ship_p->HaveBonus(BONUS_MINE))
				{
					AddEntity(Temp::ship_p->CreateBomb());
				}
				else if (Temp::ship_p->HaveBonus(BONUS_KNIFE))
				{
					AddEntity(Temp::ship_p->CreateKnife(0));
					AddEntity(Temp::ship_p->CreateKnife(1));
				}
				else
				{
					AddEntity(Temp::ship_p->CreateBullet());
				}
			}
		}
	}
}

inline void Game::AddEntity(Asteroid new_asteroid)
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

inline void Game::AddEntity(Bonus new_bonus)
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

inline void Game::AddEntity(Bullet new_bullet)
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

void Game::AddEntity(Knife new_knife)
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

inline void Game::AddEntity(Bomb new_mine)
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

inline void Game::AddEntity(Laser new_lazer)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_LAZERS_MAX_COUNT; particle++)
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

inline void Game::AddEntity(DynamicEntity new_particle)
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

inline void Game::DeleteEntity(Asteroid* deleting_asteroid)
{
	deleting_asteroid->exist = false;
	asteroids_count--;
}

inline void Game::DeleteEntity(Bonus* deleting_bonus)
{
	deleting_bonus->exist = false;
	bonuses_count--;
}

inline void Game::DeleteEntity(Bullet* deleting_bullet)
{
	deleting_bullet->exist = false;
	bullets_count--;
}

inline void Game::DeleteEntity(Knife* deleting_knife)
{
	deleting_knife->exist = false;
	knifes_count--;
}

inline void Game::DeleteEntity(Bomb* deleting_mine)
{
	deleting_mine->exist = false;
	bombs_count--;
}

inline void Game::DeleteEntity(Laser* deleting_lazer)
{
	deleting_lazer->exist = false;
	lasers_count--;
}

inline void Game::DeleteParticle(DynamicEntity* deleting_particle)
{
	deleting_particle->exist = false;
	particles_count--;
}

Bonus::bonus_t Game::GenerateRandomBonus()
{
	return 1 << (((rand() % BONUS_BONUSES_COUNT)) * 2);
}

maps_count_t Game::GenerateRandomMapId()
{
	return selected_maps_id_array[rand() % selected_maps_id_array_length];
}

score_t Game::GetMaxScore()
{
	score_t max_score = scores[0];
	for (score_t i = 1; i < players_count; i++)
	{
		if (max_score < scores[i])
		{
			max_score = scores[i];
		}
	}
	return max_score;
}