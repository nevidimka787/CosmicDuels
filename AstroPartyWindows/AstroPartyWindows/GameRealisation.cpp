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

void Game::ShipShoot(Ship* ship)
{
	bool bonus_activate = false;
	//Magicka and Magicka 2 are the best games I've seen.
	if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_BOMB | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_BOMB))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_BOMB | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_BOMB | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_LOOP))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_BOMB))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_BOMB))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LOOP | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_BOMB | BONUS_KNIFE))
	{

	}
	else if (Temp::ship_p->HaveBonus(BONUS_LASER))
	{
		Add::Entity(Temp::ship_p->CreateLazer());
	}
	else if (Temp::ship_p->HaveBonus(BONUS_LOOP))
	{
		bonus_activate = true;
		for (entities_count_t bullet = 0; bullet < BULLETS_IN_LOOP; bullet++)
		{
			Add::Entity(Temp::ship_p->CreateLoop(bullet));
		}
	}
	else if (Temp::ship_p->HaveBonus(BONUS_BOMB))
	{
		Add::Entity(Temp::ship_p->CreateBomb());
	}
	else if (Temp::ship_p->HaveBonus(BONUS_KNIFE))
	{
		Add::Entity(Temp::ship_p->CreateKnife(0));
		Add::Entity(Temp::ship_p->CreateKnife(1));
	}
	else
	{
		Add::Entity(Temp::ship_p->CreateBullet());
	}
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