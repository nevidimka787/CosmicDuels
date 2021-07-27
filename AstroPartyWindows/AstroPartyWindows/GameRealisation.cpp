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
	
	Update::Bullets();
	SheepsShoot();

	//shoots
	
	//collisions

	DynamicEntitiesCollisions((DynamicEntity*)sheeps, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asterooids_count);

	DynamicEntitiesCollisions((DynamicEntity*)sheeps, players_count, (DynamicEntity*)pilots, players_count);
	DynamicEntitiesCollisions((DynamicEntity*)sheeps, players_count, (DynamicEntity*)asteroids, asterooids_count);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asterooids_count, (DynamicEntity*)pilots, players_count);

	DynamicEntitiesCollisions((DynamicEntity*)sheeps, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)pilots, players_count, map);
	DynamicEntitiesCollisions((DynamicEntity*)asteroids, asterooids_count, map);

	DynamicEntitiesAddForce((DynamicEntity*)sheeps, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)pilots, players_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)asteroids, asterooids_count, grav_gens, grav_gens_count);
	DynamicEntitiesAddForce((DynamicEntity*)bullets, asterooids_count, grav_gens, grav_gens_count);

	//collisions

	//recalculation


	Update::Mines();

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

void Game::MinesCollisions()
{
	Mine* temp1;
	Mine* temp2;
	for (entities_count_t mine1 = 0, found_mines1 = 0; found_mines1 < mines_count; mine1++)
	{
		temp1 = &mines[mine1];
		if (temp1->exist)
		{
			for (entities_count_t mine2 = mine1 + 1, found_mines2 = found_mines1; found_mines2 < mines_count; mine2++)
			{
				temp2 = &mines[mine2];
				if (temp2->exist)
				{
					if (temp1->GetDistance(temp2) == 0.0f)
					{
						if (temp1->IsBoom() == true && temp2->IsBoom() == false)
						{
							temp2->Boom();
						}
						else if (temp1->IsBoom() == false && temp2->IsBoom() == true)
						{
							temp1->Boom();
						}
					}
					found_mines2++;
				}
			}

			found_mines1++;
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
		Temp::sheep_p = &sheeps[sheep];
		if (Temp::sheep_p->exist == true)
		{
			if (Temp::sheep_p->GetShootInputValue() && Temp::sheep_p->can_shoot)
			{
				bool bonus_activate = false;
				//Magicka and Magicka 2 are the best games I've seen.
				if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_MINE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LOOP | BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_LOOP))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_MINE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LOOP | BONUS_MINE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LOOP | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_MINE | BONUS_KNIFE))
				{

				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LASER))
				{
					Temp::lazer = Temp::sheep_p->CreateLazer();
					AddEntity(&Temp::lazer);
				}
				else if (Temp::sheep_p->HaveBonus(BONUS_LOOP))
				{
					bonus_activate = true;
					for (entities_count_t bullet = 0; bullet < BULLETS_IN_LOOP; bullet++)
					{
						Temp::bullet = Temp::sheep_p->CreateLoop(bullet);
						AddEntity(&Temp::bullet);
					}
				}
				else if (Temp::sheep_p->HaveBonus(BONUS_MINE))
				{
					Temp::mine = Temp::sheep_p->CreateMine();
					AddEntity(&Temp::mine);
				}
				else if (Temp::sheep_p->HaveBonus(BONUS_KNIFE))
				{
					Temp::knife = Temp::sheep_p->CreateKnife(0);
					AddEntity(&Temp::knife);
					Temp::knife = Temp::sheep_p->CreateKnife(1);
					AddEntity(&Temp::knife);
				}
				else
				{
					Temp::bullet = Temp::sheep_p->CreateBullet();
					AddEntity(&Temp::bullet);
				}
			}
		}
	}
}

void Game::AddEntity(Asteroid* new_asteroid)
{
	if (asterooids_count < GAME_ASTEROIDS_MAX_COUNT)
	{
		for (entities_count_t asteroid = 0; asteroid < GAME_ASTEROIDS_MAX_COUNT; asteroid++)
		{
			if (asteroids[asteroid].exist == false)
			{
				asteroids[asteroid].Set(new_asteroid);
				asterooids_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Bonus* new_bonus)
{
	if (bonuses_count < GAME_BONUSES_MAX_COUNT)
	{
		for (entities_count_t bonus = 0; bonus < GAME_BONUSES_MAX_COUNT; bonus++)
		{
			if (bonuses[bonus].exist == false)
			{
				bonuses[bonus].Set(new_bonus);
				bonuses_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Bullet* new_bullet)
{
	if (bullets_count < GAME_BULLETS_MAX_COUNT)
	{
		for (entities_count_t bullet = 0; bullet < GAME_BULLETS_MAX_COUNT; bullet++)
		{
			if (bullets[bullet].exist == false)
			{
				bullets[bullet].Set(new_bullet);
				bullets_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Knife* new_knife)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_KNIFES_MAX_COUNT; particle++)
		{
			if (knifes[particle].exist == false)
			{
				knifes[particle].Set(new_knife);
				knifes_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Mine* new_mine)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_MINES_MAX_COUNT; particle++)
		{
			if (mines[particle].exist == false)
			{
				mines[particle].Set(new_mine);
				mines_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(Lazer* new_lazer)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_LAZERS_MAX_COUNT; particle++)
		{
			if (lazers[particle].exist == false)
			{
				lazers[particle].Set(new_lazer);
				lazers_count++;
				return;
			}
		}
	}
}

void Game::AddEntity(DynamicEntity* new_particle)
{
	if (bullets_count < GAME_PARTICLES_MAX_COUNT)
	{
		for (entities_count_t particle = 0; particle < GAME_PARTICLES_MAX_COUNT; particle++)
		{
			if (particles[particle].exist == false)
			{
				particles[particle].Set(new_particle);
				bullets_count++;
				return;
			}
		}
	}
}

void Game::DeleteEntity(Asteroid* deleting_asteroid)
{
	deleting_asteroid->exist = false;
	asterooids_count--;
}

void Game::DeleteEntity(Bonus* deleting_bonus)
{
	deleting_bonus->exist = false;
	bonuses_count--;
}

void Game::DeleteEntity(Bullet* deleting_bullet)
{
	deleting_bullet->exist = false;
	bullets_count--;
}

void Game::DeleteEntity(Knife* deleting_knife)
{
	deleting_knife->exist = false;
	knifes_count--;
}

void Game::DeleteEntity(Mine* deleting_mine)
{
	deleting_mine->exist = false;
	mines_count--;
}

void Game::DeleteEntity(Lazer* deleting_lazer)
{
	deleting_lazer->exist = false;
	lazers_count--;
}

void Game::DeleteParticle(DynamicEntity* deleting_particle)
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