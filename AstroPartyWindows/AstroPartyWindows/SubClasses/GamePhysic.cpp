#include "Game.h"

#define M_PI	3.14159265358979323846

#pragma warning(disable : 26451)


template<typename EntityType>
void Game::DynamicEntitiesCollisions(EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities_count; first++)
	{
		if (entities[first].exist)
		{
			for (GameTypes::entities_count_t second = first + 1, found_entites_count2 = found_entites_count1 + 1; found_entites_count2 < entities_count; second++)
			{
				if (entities[second].exist)
				{
					entities[first].DynamicEntity::Collision(&entities[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid>(Asteroid* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bomb>(Bomb* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bullet>(Bullet* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bonus>(Bonus* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Ship>(Ship* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Pilot>(Pilot* entities, GameTypes::entities_count_t entities_count);

template<typename Entity1Type, typename Entity2Type>
void Game::DynamicEntitiesCollisions(Entity1Type* entities1, Entity2Type* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities1_count; first++)
	{
		if (entities1[first].exist)
		{
			for (GameTypes::entities_count_t second = 0, found_entites_count2 = 0; found_entites_count2 < entities2_count; second++)
			{
				if (entities2[second].exist)
				{
					entities1[first].DynamicEntity::Collision(&entities2[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid,	Bomb>(Asteroid* entities1, Bomb* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Bullet>(Asteroid* entities1, Bullet* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Bonus>(Asteroid* entities1, Bonus* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Ship>(Asteroid* entities1, Ship* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Pilot>(Asteroid* entities1, Pilot* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Asteroid>(Bomb* entities1, Asteroid* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Bullet>(Bomb* entities1, Bullet* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Bonus>(Bomb* entities1, Bonus* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Ship>(Bomb* entities1, Ship* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Pilot>(Bomb* entities1, Pilot* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Asteroid>(Bullet* entities1, Asteroid* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Bomb>(Bullet* entities1, Bomb* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Bonus>(Bullet* entities1, Bonus* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Ship>(Bullet* entities1, Ship* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Pilot>(Bullet* entities1, Pilot* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Asteroid>(Bonus* entities1, Asteroid* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Bomb>(Bonus* entities1, Bomb* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Bullet>(Bonus* entities1, Bullet* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Ship>(Bonus* entities1, Ship* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Pilot>(Bonus* entities1, Pilot* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Asteroid>(Ship* entities1, Asteroid* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bomb>(Ship* entities1, Bomb* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bullet>(Ship* entities1, Bullet* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bonus>(Ship* entities1, Bonus* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Pilot>(Ship* entities1, Pilot* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Asteroid>(Pilot* entities1, Asteroid* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bomb>(Pilot* entities1, Bomb* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bullet>(Pilot* entities1, Bullet* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bonus>(Pilot* entities1, Bonus* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Ship>(Pilot* entities1, Ship* entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);

template<typename EntityType>
void Game::DynamicEntitiesCollisions(Map* map, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			entities[i].Collision(map);
			if (!entities[i].exist)//if entity was destroed after collision
			{
				log_data_mtx.lock();
				entities[i].exist = true;
				DestroyEntity(&entities[i]);
				log_data_mtx.unlock();
				goto end_of_map_cycle;
			}
			found++;
		}
	end_of_map_cycle:;
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid>(Map* map, Asteroid* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bonus>(Map* map, Bonus* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bullet>(Map* map, Bullet* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Ship>(Map* map, Ship* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Pilot>(Map* map, Pilot* entities, GameTypes::entities_count_t entities_count);

void Game::DynamicEntitiesCollisions(Map* map, Asteroid* asteroids, GameTypes::entities_count_t asteroids_count)
{
	for (GameTypes::entities_count_t found = 0; found < asteroids_count; asteroids++)
	{
		if (asteroids->exist)
		{
			asteroids->Collision(map);
			if (!asteroids->exist)//if entity was destroed after collision
			{
				asteroids->exist = true;
				switch (asteroids->GetSize())
				{
				case ASTEROID_SIZE_BIG:
					AddEntity(asteroids->Division());
					AddEntity(asteroids->Division());
					AddEntity(asteroids->Division());
					AddEntity(asteroids->Division());
					break;
				case ASTEROID_SIZE_MEDIUM:
					AddEntity(asteroids->Division());
					AddEntity(asteroids->Division());
					break;
				case ASTEROID_SIZE_SMALL:
					AddEntity(asteroids->Destroy());
				default:
					break;
				}

				DestroyEntity(asteroids);
				goto end_of_map_cycle;
			}
			found++;
		}
	end_of_map_cycle:;
	}
}

void Game::DynamicEntitiesCollisions(Map* map, Bomb* bombs_array_p, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t found = 0; found < entities_count; bombs_array_p++)
	{
		if (bombs_array_p->exist)
		{
			bombs_array_p->Collision(map);
			found++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(Vec2F force, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
	{
		if (entities[i].exist)
		{
			entities[i].AddForce(force);
			found++;
		}
	}
}
template void Game::DynamicEntitiesAddForce<Asteroid>(Vec2F force, Asteroid* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bomb>(Vec2F force, Bomb* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(Vec2F force, Bonus* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(Vec2F force, Bullet* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(Vec2F force, Ship* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(Vec2F force, Pilot* entities, GameTypes::entities_count_t entities_count);

template<typename EntityType>
void Game::DynamicEntitiesAddForce(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	float len2;
	for (GameTypes::map_elements_count_t grav_gen = 0; grav_gen < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
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
template void Game::DynamicEntitiesAddForce<Asteroid>(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Asteroid* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bullet* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bonus* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Ship* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Pilot* entities, GameTypes::entities_count_t entities_count);

void Game::DynamicEntitiesAddForce(GravGen* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bomb* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	float len2;
	for (GameTypes::map_elements_count_t grav_gen = 0, found_grav_gens = 0; found_grav_gens < grav_gens_count; grav_gen++)
	{
		if (grav_gens[grav_gen].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					if (!entities[i].IsBoom())
					{
						force = entities[i].GetPosition() - grav_gens[grav_gen].GetPosition();
						len2 = force.x * force.x + force.y * force.y;
						force /= len2 * sqrt(len2) / grav_gens[grav_gen].gravity;
						entities[i].AddForce(&force);
					}
					found++;
				}
			}
			found_grav_gens++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(DecelerationArea* deceler_areas, GameTypes::map_elements_count_t deceler_areas_count, EntityType* entities, GameTypes::entities_count_t entities_count)
{
	Vec2F force;
	for (GameTypes::map_elements_count_t deceler_area = 0, found_deceler_areas = 0; found_deceler_areas < deceler_areas_count; deceler_area++)
	{
		if (deceler_areas[deceler_area].exist)
		{
			for (GameTypes::entities_count_t i = 0, found = 0; found < entities_count; i++)
			{
				if (entities[i].exist)
				{
					if (deceler_areas[deceler_area].IsCollision(&entities[i]))
					{
						force = -entities[i].GetVelocity() * deceler_areas[deceler_area].deceleration_parameter;
						entities[i].AddForce(force);
					}
					found++;
				}
			}
			found_deceler_areas++;
		}
	}
}
template void Game::DynamicEntitiesAddForce<Asteroid>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Asteroid* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bomb>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bomb* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bonus* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Bullet* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Ship* entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(DecelerationArea* grav_gens, GameTypes::map_elements_count_t grav_gens_count, Pilot* entities, GameTypes::entities_count_t entities_count);

template <typename EntityType>
void Game::TeleportEntity(Portal* portal, EntityType* entity)
{
	portal->Teleport(entity);
	AddEntity(portal->CreateParticles(global_timer));
	AddEntity(portal->CreateParticlesTP(global_timer, 5.0f * entity->radius));

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
}
template void Game::TeleportEntity<Asteroid>(Portal* portal, Asteroid* asteroid);
template void Game::TeleportEntity<Bonus>(Portal* portal, Bonus* bonus);
template void Game::TeleportEntity<Bomb>(Portal* portal, Bomb* bomb);
template void Game::TeleportEntity<Bullet>(Portal* portal, Bullet* bullet);
template void Game::TeleportEntity<DynamicParticle>(Portal* portal, DynamicParticle* dynamic_entity);
template void Game::TeleportEntity<Ship>(Portal* portal, Ship* ship);
template void Game::TeleportEntity<Pilot>(Portal* portal, Pilot* pilot);

void Game::ShipShoot(Ship* ship)
{
	if (ship->CanCreatingLoop())
	{
		GameTypes::objects_types_count_t type = ship->GetTypeOfElemntInLoop();
		GameTypes::entities_count_t entity_number;
		float angle;
		Vec2F r_vec;
		Segment local_segment;
		switch (type)
		{
		case GAME_OBJECT_TYPE_LASER_BOMB:
			bombs_array_mtx.lock();
			entity_number = ship->GetElemntFromLoop();
			AddEntity(
				Bomb(
					ship->GetPosition(),
					ship->GetVelocity() + ship->GetDirection() * SHIP_SUPER_BONUS__BOMBS_LASER_VELOCITY * (float)(SHIP_SUPER_BONUS__BOMBS_IN_LASER / 2 + SHIP_SUPER_BONUS__BOMBS_IN_LASER - entity_number) / 1.5f / (float)SHIP_SUPER_BONUS__BOMBS_IN_LASER,
					ship->GetTeamNumber(),
					ship->GetTeamNumber(),
					BOMB_DEFAULT_ACTIVATION_PERIOD + entity_number,
					ship->GetAngle(),
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
					SHIP_SUPER_BONUS__BOMBS_LASER_RADIUS,
					BOMB_STATUS_ACTIVE,
					BOMB_DEFAULT_ACTIVATION_PERIOD + entity_number,
					BOMB_DEFAULT_ACTIVATION_PERIOD + entity_number));
			bombs_array_mtx.unlock();
			return;
		case GAME_OBJECT_TYPE_LOOP_BOMB:
			bombs_array_mtx.lock();
			entity_number = (SHIP_SUPER_BONUS__BOMBS_IN_LOOP - ship->GetElemntFromLoop()) / 2;
			angle = (float)M_PI / 2.0f * (1.0f + (float)(entity_number * 2) / (float)SHIP_SUPER_BONUS__BOMBS_IN_LOOP);
			AddEntity(
				Bomb(ship->GetPosition(),
					ship->GetVelocity() + Vec2F(SHIP_SUPER_BONUS__BOMBS_LOOP_VELOCITY, 0.0f).Rotate(ship->GetAngle() + angle),
					ship->GetTeamNumber(),
					ship->GetTeamNumber(),
					0,
					ship->GetAngle() + angle));

			if (ship->GetElemntFromLoop() == 0)
			{
				bombs_array_mtx.unlock();
				return;
			}

			AddEntity(
				Bomb(ship->GetPosition(),
					ship->GetVelocity() + Vec2F(SHIP_SUPER_BONUS__BOMBS_LOOP_VELOCITY, 0.0f).Rotate(ship->GetAngle() - angle),
					ship->GetTeamNumber(),
					ship->GetTeamNumber(),
					0,
					ship->GetAngle() - angle));

			bombs_array_mtx.unlock();
			return;
		case GAME_OBJECT_TYPE_LOOP_BULLET:
			bullets_array_mtx.lock();
			entity_number = ship->GetElemntFromLoop();
			angle = (float)(((entity_number - 1u) / 2u) + 0.5f) / (float)SHIP_BONUS__LOOP_BULLETS_IN_LOOP * (float)M_PI * 2.0f;
			
			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship->GetAngle() + angle);
			AddEntity(
				Bullet(
					ship->GetPosition() + r_vec * (ship->radius + SHIP_BONUS__LOOP_BULLETS_RADIUS * 2.0f),
					ship->GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
					ship->GetPlayerNumber(),
					ship->GetTeamNumber(),
					true,
					ship->GetAngle() + angle,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
					SHIP_BONUS__LOOP_BULLETS_RADIUS));
			
			entity_number = ship->GetElemntFromLoop();
			if (entity_number == 0)
			{
				bullets_array_mtx.unlock();
				return;
			}
			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship->GetAngle() - angle);
			AddEntity(
				Bullet(
					ship->GetPosition() + r_vec * (ship->radius + SHIP_BONUS__LOOP_BULLETS_RADIUS * 2.0f),
					ship->GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
					ship->GetPlayerNumber(),
					ship->GetTeamNumber(),
					true,
					ship->GetAngle() - angle,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
					SHIP_BONUS__LOOP_BULLETS_RADIUS));
			bullets_array_mtx.unlock();
			return;
		case GAME_OBJECT_TYPE_LOOP_KNIFE:
			knifes_array_mtx.lock();
			entity_number = ship->GetElemntFromLoop() - 1;
			local_segment.Set(
				Vec2F(0.5f, 0.0f).Rotate(((float)entity_number + 0.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				Vec2F(0.5f, 0.0f).Rotate(((float)entity_number + 1.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				true);
			AddEntity(Knife(ship, &local_segment, 1u));

			local_segment.Set(
				Vec2F(0.5f, 0.0f).Rotate(-((float)entity_number + 0.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				Vec2F(0.5f, 0.0f).Rotate(-((float)entity_number + 1.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				true);
			AddEntity(Knife(ship, &local_segment, 1u));
			knifes_array_mtx.unlock();
			return;
		default:
			std::cout << "WARNING::ShipShoot::Unknow type " << type << std::endl;
			return;
		}
	}

	if (!shoot_flags[ship->GetPlayerNumber()] || ships_can_shoot_flags[ship->GetPlayerNumber()] > 0)
	{
		return;
	}
	ships_can_shoot_flags[ship->GetPlayerNumber()] = GAME_DELLAY_BETWEEN_SHOOTS;

	if (ship->ActivateAvailableBuffs())
	{
		return;
	}

	//Magicka and Magicka 2 are the best games I've seen.

	if (ship->SpendBonus(BONUS_LASER))
	{
		if (ship->SpendBonus(BONUS_LOOP))
		{
			if (ship->SpendBonus(BONUS_BOMB))
			{
				if (ship->SpendBonus(BONUS_KNIFE))
				{
					ShipShoot_LaserLoopBombKnife(ship);
					return;
				}
				ShipShoot_LaserLoopBomb(ship);
				return;
			}
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserLoopKnife(ship);
				return;
			}
			ShipShoot_LaserLoop(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserBombKnife(ship);
				return;
			}
			ShipShoot_LaserBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LaserKnife(ship);
			return;
		}
		ShipShoot_Laser(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_LOOP))
	{
		if (ship->SpendBonus(BONUS_BOMB))
		{
			if (ship->SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LoopBombKnife(ship);
				return;
			}
			ShipShoot_LoopBomb(ship);
			return;
		}
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LoopKnife(ship);
			return;
		}
		ShipShoot_Loop(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_BOMB))
	{
		if (ship->SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_BombKnife(ship);
			return;
		}
		ShipShoot_Bomb(ship);
		return;
	}
	if (ship->SpendBonus(BONUS_KNIFE))
	{
		ShipShoot_Knife(ship);
		return;
	}
	ShipShoot_NoBonus(ship);
	return;
}

void Game::ShipShoot_LaserLoopBombKnife(Ship* ship)
{
	std::cout << "Laser Loop Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 3;
}

void Game::ShipShoot_LaserLoopBomb(Ship* ship)
{
	std::cout << "Laser Loop Bomb" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserLoopKnife(Ship* ship)
{
	std::cout << "Laser Loop Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserBombKnife(Ship* ship)
{
	std::cout << "Laser Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LoopBombKnife(Ship* ship)
{
	std::cout << "Loop Bomb Knife" << std::endl;
	ShipShoot_NoBonus(ship);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserLoop(Ship* ship)
{
	Beam local_beam = LASER_DEFAULT_LOCAL_BEAM;
	Game::AddEntity(Laser(ship, &local_beam, LASER_DEFAULT_WIDTH, LASER_DEFAULT_SHOOT_TIME, true));

	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LaserBomb(Ship* ship)
{
	ship->CreatingLoop(SHIP_SUPER_BONUS__BOMBS_IN_LASER, GAME_OBJECT_TYPE_LASER_BOMB);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LoopBomb(Ship* ship)
{
	ship->CreatingLoop(SHIP_SUPER_BONUS__BOMBS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_BOMB);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LaserKnife(Ship* ship)
{
	Segment local_segment = Segment(Vec2F(0.4f, 0.0f), Vec2F(3.0f, 0.0f));
	Game::AddEntity(Knife(ship, &local_segment, SHIP_SUPER_BONUS__LONG_KNIFE_HEALTH));

	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LoopKnife(Ship* ship)
{
	ship->CreatingLoop(SHIP_SUPER_BONUS__KNIFES_IN_LOOP / 2, GAME_OBJECT_TYPE_LOOP_KNIFE);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_BombKnife(Ship* ship)
{
	bombs_array_mtx.lock();
	Game::AddEntity(
		Bomb(ship->GetPosition() - ship->GetVelocity() - ship->GetDirection() * 1.1f * (ship->radius + BOMB_DEFAULT_RADIUS),
			ship->GetVelocity() / 2.0f,
			ship->GetTeamNumber(),
			ship->GetTeamNumber(),
			SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD,
			0.0f,
			0.0f,
			DEFAULT_FORCE_COLLISION_COEFFICIENT,
			0.0f,
			BOMB_DEFAULT_RADIUS,
			BOMB_STATUS_BOOM,
			SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD,
			SHIP_SUPER_BONUS__ANIGILATION_AREA_EXIST_PERIOD));
	bombs_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_Knife(Ship* ship)
{
	knifes_array_mtx.lock();
	Game::AddEntity(ship->CreateKnife(0));
	Game::AddEntity(ship->CreateKnife(1));
	knifes_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Bomb(Ship* ship)
{
	bombs_array_mtx.lock();
	Game::AddEntity(ship->CreateBomb());
	bombs_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Loop(Ship* ship)
{
	ship->CreatingLoop(SHIP_BULLETS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_BULLET);
	ships_can_shoot_flags[ship->GetPlayerNumber()] += 2;
}

void Game::ShipShoot_Laser(Ship* ship)
{
	lasers_array_mtx.lock();
	Game::AddEntity(ship->CreateLaser());
	lasers_array_mtx.unlock();
	ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_NoBonus(Ship* ship)
{
	//ShipShoot_Loop(ship);
	//return;

	bullets_array_mtx.lock();
	if (ship->HaveBuff(SHIP_BUFF_TRIPLE))
	{
		AddEntity(ship->CreateTriple(0));
		AddEntity(ship->CreateTriple(1));
		AddEntity(ship->CreateTriple(2));
		ships_can_shoot_flags[ship->GetPlayerNumber()] += GAME_ADD_DELLAY_BETWEEN_TRIPLES;
	}
	else
	{
		AddEntity(ship->CreateBullet());
	}
	bullets_array_mtx.unlock();
}

void Game::CreateLoop(Laser* laser, Asteroid* asteroid)
{
	Vec2F loop_center_position = asteroid->GetPosition();
	GameTypes::entities_count_t bullets_in_loop =
		(asteroid->GetSize() == ASTEROID_SIZE_BIG) ?
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_BIG :
		(asteroid->GetSize() == ASTEROID_SIZE_MEDIUM) ?
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_MEDIUM :
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_SMALL;
	float bullets_radius =
		(asteroid->GetSize() == ASTEROID_SIZE_BIG) ?
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_BIG :
		(asteroid->GetSize() == ASTEROID_SIZE_MEDIUM) ?
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_MEDIUM :
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_SMALL;
	Vec2F laser_vector = laser->GetBeam().vector;
	Vec2F radius_vector;

	for (GameTypes::entities_count_t bullet = 0; bullet < bullets_in_loop; bullet++)
	{
		radius_vector = Vec2F(asteroid->radius, 0.0f).Rotate(
			(float)bullet / (float)bullets_in_loop * (float)M_PI * 2.0f);
		Game::AddEntity(
			Bullet(loop_center_position + radius_vector,
				(radius_vector.Normalize() + laser_vector.Normalize() * 0.75f) * BULLET_DEFAULT_VELOCITY * 2.0f,
				laser->GetPlayerMasterNumber(),
				laser->GetPlayerMasterTeamNumber(),
				false,
				0.0f,
				0.0f,
				DEFAULT_FORCE_COLLISION_COEFFICIENT,
				BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
				bullets_radius));
	}
}

void Game::UpdateAsteroidsPhase2()
{
	Asteroid* temp__asteroid_p = asteroids;
	asteroids_array_mtx.lock();
	for (GameTypes::entities_count_t found_asteroids = 0; found_asteroids < asteroids_count; temp__asteroid_p++)
	{
		if (temp__asteroid_p->exist)
		{
			temp__asteroid_p->Update();
			found_asteroids++;
		}
	}
	asteroids_array_mtx.unlock();
}

void Game::UpdateBombsPhase2()
{
	Bomb* temp__bomb_p = bombs;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t found_bombs = 0; found_bombs < bombs_count; temp__bomb_p++)
	{
		if (temp__bomb_p->exist)
		{
			temp__bomb_p->Update();
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::UpdateBonusesPhase2()
{
	Bonus* temp__bonus_p = bonuses;
	bonuses_array_mtx.lock();
	for (GameTypes::entities_count_t bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; temp__bonus_p++)
	{
		if (temp__bonus_p->exist)
		{
			if (rotation_inverse)
			{
				temp__bonus_p->Rotate(-2.0f * temp__bonus_p->GetAngularVelocity());
			}
			temp__bonus_p->Update();
			found_bonuses++;
		}
	}
	bonuses_array_mtx.unlock();
}

void Game::UpdateBulletsPhase2()
{
	Bullet* temp__bullet_p = bullets;
	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t found_bullets = 0; found_bullets < bullets_count; temp__bullet_p++)
	{
		if (temp__bullet_p->exist)
		{
			temp__bullet_p->Update();
			found_bullets++;
		}
	}
	bullets_array_mtx.unlock();
}

void Game::UpdateDecelerAreasPhase2()
{
	DecelerationArea* temp__deceler_area_p = deceler_areas;
	deceler_areas_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_deceler_areas = 0; found_deceler_areas < deceler_areas_count; temp__deceler_area_p++)
	{
		if (temp__deceler_area_p->exist)
		{
			temp__deceler_area_p->Update();
			found_deceler_areas++;
		}
	}
	deceler_areas_array_mtx.unlock();
}

void Game::UpdateDynamicParticlesPhase2()
{
	DynamicParticle* temp__dynamic_particle_p = dynamic_particles;
	dynamic_particles_array_mtx.lock();
	for (GameTypes::entities_count_t found_dynamic_particles = 0; found_dynamic_particles < dynamic_particles_count; temp__dynamic_particle_p++)
	{
		if (temp__dynamic_particle_p->exist)
		{
			if (temp__dynamic_particle_p->CanRemove(global_timer))
			{
				RemoveEntity(temp__dynamic_particle_p);
				goto end_of_particle_cycle;
			}
			temp__dynamic_particle_p->Activate(global_timer);
			temp__dynamic_particle_p->Update(global_timer);
			found_dynamic_particles++;
		}
	end_of_particle_cycle:;
	}
	dynamic_particles_array_mtx.unlock();
}

void Game::UpdateGravGensPhase2()
{
	GravGen* temp__grav_gen_p = grav_gens;
	grav_gens_array_mtx.lock();
	for (GameTypes::map_elements_count_t grav_gen = 0, found_grav_gens = 0; found_grav_gens < grav_gens_count; temp__grav_gen_p++)
	{
		if (temp__grav_gen_p->exist)
		{
			temp__grav_gen_p->Update();
			found_grav_gens++;
		}
	}
	grav_gens_array_mtx.unlock();
}

void Game::UpdateKnifesPhase2()
{
	Knife* temp__knife_p = knifes;
	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t found_knifes = 0; found_knifes < knifes_count; temp__knife_p++)
	{
		if (temp__knife_p->exist)
		{
			temp__knife_p->Update();
			found_knifes++;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::UpdateLasersPhase2()
{
	Laser* temp__laser_p = lasers;
	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t found_lasers = 0; found_lasers < lasers_count; temp__laser_p++)
	{
		if (temp__laser_p->exist)
		{
			temp__laser_p->Update();
			if (!temp__laser_p->IsActive())
			{
				RemoveEntity(temp__laser_p);
				goto end_of_cycle;
			}
			found_lasers++;
		}
	end_of_cycle:;
	}
	lasers_array_mtx.unlock();
}

void Game::UpdateMegaLasersPhase2()
{
	MegaLaser* temp__mega_laser_p = mega_lasers;
	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; temp__mega_laser_p++)
	{
		if (temp__mega_laser_p->exist)
		{
			temp__mega_laser_p->Update();
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::UpdateParticlesPhase2()
{
	Particle* temp__particle_p = particles;
	particles_array_mtx.lock();
	for (GameTypes::entities_count_t found_particles = 0; found_particles < particles_count; temp__particle_p++)
	{
		if (temp__particle_p->exist)
		{
			if (temp__particle_p->CanRemove(global_timer))
			{
				RemoveEntity(temp__particle_p);
				goto end_of_particle_cycle;
			}
			temp__particle_p->Activate(global_timer);
			temp__particle_p->Update(global_timer);
			found_particles++;
		}
	end_of_particle_cycle:;
	}
	particles_array_mtx.unlock();
}

void Game::UpdatePortalsPhase2()
{
	Portal* temp__portal_p;
	for (GameTypes::map_elements_count_t portal = 0, found_portals = 0; found_portals < portals_count; portal++)
	{
		temp__portal_p = &portals[portal];
		if (temp__portal_p->exist)
		{
			temp__portal_p->Update();
			found_portals++;
		}
	}
}

void Game::UpdatePilotsPhase2()
{
	Pilot* temp__pilot_p = pilots;
	pilots_array_mtx.lock();
	for (GameTypes::entities_count_t found_pilots = 0; found_pilots < pilots_count; temp__pilot_p++)
	{
		if (temp__pilot_p->exist)
		{
			temp__pilot_p->Update();
			temp__pilot_p->UpdateMatrix();

			found_pilots++;
		}
	}
	pilots_array_mtx.unlock();
}

void Game::UpdateShipsPhase2()
{
	Ship* temp__ship_p = ships;
	ships_array_mtx.lock();
	for (GameTypes::entities_count_t found_ships = 0; found_ships < ships_count; temp__ship_p++)
	{
		if (temp__ship_p->exist)
		{
			temp__ship_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
			temp__ship_p->Update();
			temp__ship_p->UpdateMatrix();

			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::UpdateTurelsPhase2()
{
	Turel* temp__turel_p = turels;
	turels_array_mtx.lock();
	for (GameTypes::map_elements_count_t turel = 0, found_turels = 0; found_turels < turels_count; temp__turel_p++)
	{
		if (temp__turel_p->exist)
		{
			temp__turel_p->Update();
			found_turels++;
		}
	}
	turels_array_mtx.unlock();
}

void Game::UpdateMapPhase2()
{
	void* element_pointer;
	map_data_mtx.lock();
	for (EngineTypes::Map::array_length_t element = 0; element < map.rectangles_array_length; element++)
	{
		element_pointer = (void*)map.RectanglePointer(element);
		if (((Rectangle*)element_pointer)->exist)
		{
			((Rectangle*)element_pointer)->Update();
		}
	}
	for (EngineTypes::Map::array_length_t element = 0; element < map.cyrcles_array_length; element++)
	{
		element_pointer = (void*)map.CyrclePointer(element);
		if (((Cyrcle*)element_pointer)->exist)
		{
			((Cyrcle*)element_pointer)->Update();
		}
	}
	for (EngineTypes::Map::array_length_t element = 0; element < map.polygons_array_length; element++)
	{
		element_pointer = (void*)map.PolygonPointer(element);
		if (((Polygon*)element_pointer)->exist)
		{
			((Polygon*)element_pointer)->Update();
		}
	}
	map_data_mtx.unlock();
}

void Game::BombsChainReaction()
{
	Bomb* temp__bomb1_p = bombs;
	Bomb* temp__bomb2_p;
	GameTypes::entities_count_t found_second_bombs;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t found_bombs = 0; found_bombs < bombs_count; temp__bomb1_p++)
	{
		if (temp__bomb1_p->exist)
		{
			if (temp__bomb1_p->CanRemove())
			{
				RemoveEntity(temp__bomb1_p);
				goto end_of_bomb_cycle;
			}
			if (temp__bomb1_p->IsBoom())
			{
				for (temp__bomb2_p = bombs, found_second_bombs = 1; found_second_bombs < bombs_count; temp__bomb2_p++)
				{
					if (temp__bomb2_p->exist && temp__bomb1_p != temp__bomb2_p)
					{
						if (!temp__bomb2_p->IsBoom() && temp__bomb2_p->Entity::IsCollision(temp__bomb1_p))
						{
							temp__bomb2_p->Boom();
							//std::cout << "Chain BOOM: " << (unsigned)bomb << " " << (unsigned)second_bomb << std::endl;
							stuning_timer = GAME_BOMB_STUNING_TIME;
						}
						found_second_bombs++;
					}
				}
			}
			found_bombs++;
		}
	end_of_bomb_cycle:;
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Bomb* temp__bomb_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				asteroids_array_mtx.lock();
				for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__asteroid_p->exist)
					{
						if (temp__asteroid_p->Entity::IsCollision(temp__bomb_p))
						{
							bonuses_array_mtx.lock();
							dynamic_particles_array_mtx.lock();
							DestroyEntity(temp__bomb_p, temp__asteroid_p);
							dynamic_particles_array_mtx.unlock();
							bonuses_array_mtx.unlock();
							goto end_of_asteroid_cycle;
						}
						found_asteroids++;
					}
				end_of_asteroid_cycle:;
				}
				asteroids_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bomb* temp__bomb_p;
	Bonus* temp__bonus_p;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				bonuses_array_mtx.lock();
				for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
				{
					temp__bonus_p = &bonuses[bonus];
					if (temp__bonus_p->exist)
					{
						if (temp__bonus_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						found_bonuses++;
					}
				end_of_bonus_cycle:;
				}
				bonuses_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsCollisionsWithBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bomb* temp__bomb_p;
	Bullet* temp__bullet_p;

	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bomb_p->Entity::IsCollision(temp__bullet_p))
					{
						if (temp__bomb_p->IsBoom())
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						else if (!temp__bomb_p->IsActive())
						{
							temp__bomb_p->Activate();
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
					}
					found_bullets++;
				}
			}
		end_of_bullet_cycle:
			bullets_array_mtx.unlock();
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsSpawnedByBulletsAnigilation()
{
	GameTypes::entities_count_t found_second_bullets;
	Bullet* temp__bullet1_p = bullets;
	Bullet* temp__bullet2_p;

	bombs_array_mtx.lock();
	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t found_first_bullets = 0; found_first_bullets < bullets_count; temp__bullet1_p++)
	{
		if (temp__bullet1_p->exist)
		{
			if (temp__bullet1_p->is_ignore == BULLET_IGNORE_NOTHING)
			{
				for (temp__bullet2_p = temp__bullet1_p + 1, found_second_bullets = found_first_bullets + 1; found_second_bullets < bullets_count; temp__bullet2_p++)
				{
					if (temp__bullet2_p->exist)
					{
						if (temp__bullet2_p->is_ignore == BULLET_IGNORE_NOTHING)
						{
							if (temp__bullet2_p->Entity::IsCollision(temp__bullet1_p))
							{
#define BOMB_BY_BULLET__BOOM_PERIOD	75
								Vec2F bomb_position = temp__bullet1_p->GetPosition();
								Vec2F bomb_velocity = (temp__bullet1_p->GetVelocity() + temp__bullet2_p->GetVelocity()) / 2.0f;
								AddEntity(
									Bomb(
										&bomb_position,
										&bomb_velocity,
										temp__bullet1_p->GetHostTeamNumber(),
										temp__bullet2_p->GetHostTeamNumber(),
										BOMB_BY_BULLET__BOOM_PERIOD,
										0.0f,
										0.0f,
										DEFAULT_FORCE_COLLISION_COEFFICIENT,
										BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
										BOMB_DEFAULT_RADIUS * BOMB_BOOM_RADIUS_COEF / 3.0f,
										BOMB_STATUS_BOOM,
										BOMB_BY_BULLET__BOOM_PERIOD,
										BOMB_BY_BULLET__BOOM_PERIOD));
								RemoveEntity(temp__bullet1_p);
								RemoveEntity(temp__bullet2_p);
								goto end_of_bullet1_cycle;
							}
						}
						found_second_bullets++;
					}
				}
			}
			found_first_bullets++;
		end_of_bullet1_cycle:;
		}
	}
	bullets_array_mtx.unlock();
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Bomb* temp__bomb_p;
	Knife* temp__knife_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				knifes_array_mtx.lock();
				for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__knife_p->exist)
					{
						if (temp__bomb_p->IsCollision(temp__knife_p->GetSegment()))
						{
							DestroyEntity(temp__bomb_p, temp__knife_p);
							goto end_of_knife_cycle;
						}
						found_knifes++;
					}
				end_of_knife_cycle:;
				}
				knifes_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyTurels()
{
	GameTypes::entities_count_t turel, found_turels;
	Bomb* temp__bomb_p;
	Turel* temp__turel_p;
	bombs_array_mtx.lock();
	for (GameTypes::entities_count_t bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
	{
		temp__bomb_p = &bombs[bomb];
		if (temp__bomb_p->exist)
		{
			if (temp__bomb_p->IsBoom())
			{
				turels_array_mtx.lock();
				for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
				{
					temp__turel_p = &turels[turel];
					if (temp__turel_p->exist)
					{
						if (temp__turel_p->Entity::IsCollision(temp__bomb_p))
						{
							DestroyEntity(temp__bomb_p, temp__turel_p);
							goto end_of_turel_cycle;
						}
						found_turels++;
					}
				end_of_turel_cycle:;
				}
				turels_array_mtx.unlock();
			}
			found_bombs++;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BulletsDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Bullet* temp__bullet_p;

	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->Entity::IsCollision(temp__bullet_p))
					{
						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						DestroyEntity(temp__bullet_p, temp__asteroid_p);
						dynamic_particles_array_mtx.unlock();
						bonuses_array_mtx.unlock();
						RemoveEntity(temp__bullet_p);
						goto end_of_asteroid_cycle;
					}
					found_asteroids++;
				}
			end_of_asteroid_cycle:;
			}
			asteroids_array_mtx.unlock();
			found_bullets++;
		}
	}
	bullets_array_mtx.unlock();
}

void Game::BulletsDestroedByMap()
{
	Bullet* temp__bullet_p;

	bullets_array_mtx.lock();
	for (GameTypes::entities_count_t bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
	{
		temp__bullet_p = &bullets[bullet];
		if (temp__bullet_p->exist)
		{
			map_data_mtx.lock();
			if (temp__bullet_p->Collision(&map))
			{
				map_data_mtx.unlock();
				RemoveEntity(temp__bullet_p);
				goto end_of_map_cycle;
			}
			map_data_mtx.unlock();
			found_bullets++;
		}
	end_of_map_cycle:;
	}
	bullets_array_mtx.unlock();
}

void Game::CameraFocusesOnPlayers()
{
	camera_data_mtx.lock();
	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	camera.Focus(ships, pilots, ships_count, pilots_count);
	pilots_array_mtx.unlock();
	ships_array_mtx.unlock();
	camera_data_mtx.unlock();
}

void Game::KnifesDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Knife* temp__knife_p;
	Segment temp__segment;
	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					temp__segment = temp__knife_p->GetSegment();
					if (temp__asteroid_p->Entity::IsCollision(&temp__segment))
					{
						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						DestroyEntity(temp__knife_p, temp__asteroid_p);
						dynamic_particles_array_mtx.unlock();
						bonuses_array_mtx.unlock();
						if (temp__knife_p->LoseHealth() == false)
						{
							RemoveEntity(temp__knife_p);
							asteroids_array_mtx.unlock();
							goto end_of_knife_cycle;
						}
					}
					found_asteroids++;
				}
			}
			asteroids_array_mtx.unlock();
			found_knifes++;
		end_of_knife_cycle:;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Knife* temp__knife_p;

	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->IsCollision(temp__knife_p->GetSegment()))
					{
						DestroyEntity(temp__bullet_p, temp__knife_p);
						RemoveEntity(temp__bullet_p);
						bullets_array_mtx.unlock();
						goto end_of_knife_cycle;
					}
					found_bullets++;
				}
			}
			bullets_array_mtx.unlock();
			found_knifes++;
		}
	end_of_knife_cycle:;
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyMap()
{
	Knife* temp__knife_p;

	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			map_data_mtx.lock();
			temp__knife_p->Collision(&map);
			map_data_mtx.unlock();
			if (!temp__knife_p->exist)
			{
				knifes_count--;
				goto end_of_knife_cycle;
			}
			found_knifes++;
		}
	end_of_knife_cycle:;
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyTurels()
{
	GameTypes::map_elements_count_t turel, found_turels;
	Knife* temp__knife_p;
	Turel* temp__turel_p;
	Segment temp__segment;

	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
	{
		temp__knife_p = &knifes[knife];
		if (temp__knife_p->exist)
		{
			turels_array_mtx.lock();
			for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
			{
				temp__turel_p = &turels[turel];
				if (temp__turel_p->exist)
				{
					temp__segment = temp__knife_p->GetSegment();
					if (temp__turel_p->IsCollision(&temp__segment))
					{
						RemoveEntity(temp__knife_p);
						RemoveEntity(temp__turel_p);
						turels_array_mtx.unlock();
						goto end_of_knife_cycle;
					}
					found_turels++;
				}
			}
			turels_array_mtx.unlock();
			found_knifes++;
		}
	end_of_knife_cycle:;
	}
	knifes_array_mtx.unlock();
}

void Game::LasersDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			bullets_array_mtx.lock();
			asteroids_array_mtx.lock();
			for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
			{
				temp__asteroid_p = &asteroids[asteroid];
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->IsCollision(temp__laser_p))
					{
						if (temp__asteroid_p->bonus_inventory && temp__laser_p->can_create_loops)
						{
							CreateLoop(temp__laser_p, temp__asteroid_p);
						}

						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						DestroyEntity(temp__laser_p, temp__asteroid_p);
						dynamic_particles_array_mtx.unlock();
						bonuses_array_mtx.unlock();
						goto end_of_asteroid_cycle;
					}
					found_asteroids++;
				}
			end_of_asteroid_cycle:;
			}
			asteroids_array_mtx.unlock();
			bullets_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bonus* temp__bonus_p;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			bonuses_array_mtx.lock();
			for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->IsCollision(temp__laser_p))
					{
						DestroyEntity(temp__laser_p, temp__bonus_p);
						goto end_of_bonus_cycle;
					}
					found_bonuses++;
				}
			end_of_bonus_cycle:;
			}
			bonuses_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDetonateBombs()
{
	GameTypes::entities_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();

	for (GameTypes::map_elements_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			if (temp__laser_p->IsActive())
			{
				bombs_array_mtx.lock();
				for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
				{
					temp__bomb_p = &bombs[bomb];
					if (temp__bomb_p->exist)
					{
						if (!temp__bomb_p->IsBoom() && temp__bomb_p->IsCollision(temp__laser_p))
						{
							temp__bomb_p->Boom();
							stuning_timer = GAME_BOMB_STUNING_TIME;
						}
						found_bombs++;
					}
				}
				bombs_array_mtx.unlock();
			}
			found_lasers++;
		}
	}

	lasers_array_mtx.unlock();
}

void Game::LasersDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Laser* temp__laser_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->DynamicEntity::IsCollision(temp__laser_p))
					{
						DestroyEntity(temp__laser_p, temp__bullet_p);
						goto end_of_bullet_cycle;
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			bullets_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	Laser* temp__laser_p;
	Segment temp__segment;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			if (temp__laser_p->IsActive())
			{
				knifes_array_mtx.lock();
				for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__knife_p->exist)
					{
						temp__segment = temp__knife_p->GetSegment();
						if (temp__laser_p->IsCollision(&temp__segment))
						{
							RemoveEntity(temp__knife_p);
							goto end_of_knife_cycle;
						}
						found_knifes++;
					}
				end_of_knife_cycle:;
				}
				knifes_array_mtx.unlock();
			}
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyMap()
{
	Laser* temp__laser_p;

	knifes_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			map_data_mtx.lock();
			temp__laser_p->Collision(&map);
			map_data_mtx.unlock();
			found_lasers++;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::LasersDestroyTurels()
{
	GameTypes::entities_count_t turel, found_turels;
	Laser* temp__laser_p;
	Turel* temp__turel_p;

	lasers_array_mtx.lock();
	for (GameTypes::entities_count_t laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
	{
		temp__laser_p = &lasers[laser];
		if (temp__laser_p->exist)
		{
			turels_array_mtx.lock();
			for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
			{
				temp__turel_p = &turels[turel];
				if (temp__turel_p->exist)
				{
					if (temp__turel_p->IsCollision(temp__laser_p))
					{
						std::cout << "Remove turel." << std::endl;
						RemoveEntity(temp__turel_p);
						goto end_of_turel_cycle;
					}
					found_turels++;
				}
			end_of_turel_cycle:;
			}
			turels_array_mtx.unlock();
			found_lasers++;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyAsteroids()
{
	GameTypes::entities_count_t asteroid, found_asteroids;
	Asteroid* temp__asteroid_p;
	MegaLaser* temp__mega_laser_p;

	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				asteroids_array_mtx.lock();
				for (asteroid = 0, found_asteroids = 0; found_asteroids < asteroids_count; asteroid++)
				{
					temp__asteroid_p = &asteroids[asteroid];
					if (temp__asteroid_p->exist)
					{
						if (temp__asteroid_p->Entity::IsCollision(temp__mega_laser_p))
						{
							bonuses_array_mtx.lock();
							dynamic_particles_array_mtx.lock();
							DestroyEntity(temp__mega_laser_p, temp__asteroid_p);
							dynamic_particles_array_mtx.unlock();
							bonuses_array_mtx.unlock();
							goto end_of_asteroid_cycle;
						}
						found_asteroids++;
					}
				end_of_asteroid_cycle:;
				}
				asteroids_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Bonus* temp__bonus_p;
	MegaLaser* temp__mega_laser_p;

	mega_lasers_array_mtx.lock();
	for (GameTypes::entities_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				bonuses_array_mtx.lock();
				for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
				{
					temp__bonus_p = &bonuses[bonus];
					if (temp__bonus_p->exist)
					{
						if (temp__bonus_p->IsCollision(temp__mega_laser_p))
						{
							DestroyEntity(temp__mega_laser_p, temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						found_bonuses++;
					}
				end_of_bonus_cycle:;
				}
				bonuses_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDetonateBombs()
{
	GameTypes::entities_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	MegaLaser* temp__mega_laser_p;

	mega_lasers_array_mtx.lock();

	for (GameTypes::map_elements_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				bombs_array_mtx.lock();
				for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
				{
					temp__bomb_p = &bombs[bomb];
					if (temp__bomb_p->exist)
					{
						if (!temp__bomb_p->IsBoom() && temp__bomb_p->IsCollision(temp__mega_laser_p->GetSegment()))
						{
							temp__bomb_p->Boom();
							stuning_timer = GAME_BOMB_STUNING_TIME;
						}
						found_bombs++;
					}
				}
				bombs_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}

	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	MegaLaser* temp__mega_laser_p;

	mega_lasers_array_mtx.lock();
	for (GameTypes::map_elements_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				bullets_array_mtx.lock();
				for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
				{
					temp__bullet_p = &bullets[bullet];
					if (temp__bullet_p->exist)
					{
						if (temp__bullet_p->IsCollision(temp__mega_laser_p))
						{
							DestroyEntity(temp__mega_laser_p, temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						found_bullets++;
					}
				end_of_bullet_cycle:;
				}
				bullets_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	MegaLaser* temp__mega_laser_p;
	Segment temp__segment;

	mega_lasers_array_mtx.lock();
	for (GameTypes::map_elements_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				knifes_array_mtx.lock();
				for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
				{
					temp__knife_p = &knifes[knife];
					if (temp__knife_p->exist)
					{
						temp__segment = temp__knife_p->GetSegment();
						if (temp__mega_laser_p->IsCollision(&temp__segment))
						{
							DestroyEntity(temp__mega_laser_p, temp__knife_p);
							goto end_of_knife_cycle;
						}
						found_knifes++;
					}
				end_of_knife_cycle:;
				}
				knifes_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::MegaLasersDestroyMap()
{

}

void Game::MegaLasersDestroyTurels()
{
	GameTypes::entities_count_t turel, found_turels;
	Turel* temp__turel_p;
	MegaLaser* temp__mega_laser_p;

	mega_lasers_array_mtx.lock();
	for (GameTypes::map_elements_count_t mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
	{
		temp__mega_laser_p = &mega_lasers[mega_laser];
		if (temp__mega_laser_p->exist)
		{
			if (temp__mega_laser_p->IsShooting(global_timer))
			{
				turels_array_mtx.lock();
				for (turel = 0, found_turels = 0; found_turels < turels_count; turel++)
				{
					temp__turel_p = &turels[turel];
					if (temp__turel_p->exist)
					{
						if (temp__turel_p->IsCollision(temp__mega_laser_p))
						{
							RemoveEntity(temp__turel_p);
							goto end_of_turel_cycle;
						}
						found_turels++;
					}
				end_of_turel_cycle:;
				}
				turels_array_mtx.unlock();
			}
			found_mega_lasers++;
		}
	}
	mega_lasers_array_mtx.unlock();
}

void Game::PortalsCreateParticles()
{
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			particles_array_mtx.lock();
			AddEntity(temp__portal_p->CreateParticlesTP(global_timer, BULLET_DEFAULT_RADIUS * 2.0f));
			particles_array_mtx.unlock();
			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPAsteroids()
{
	GameTypes::entities_count_t found_asteroids;
	Asteroid* temp__asteroid_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			asteroids_array_mtx.lock();
			for (temp__asteroid_p = asteroids, found_asteroids = 0; found_asteroids < asteroids_count; temp__asteroid_p++)
			{
				if (temp__asteroid_p->exist)
				{
					if (temp__asteroid_p->IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__asteroid_p);
						particles_array_mtx.unlock();
					}

					found_asteroids++;
				}
			}
			asteroids_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPBombs()
{
	GameTypes::entities_count_t found_bombs;
	Bomb* temp__bomb_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			bombs_array_mtx.lock();
			for (temp__bomb_p = bombs, found_bombs = 0; found_bombs < bombs_count; temp__bomb_p++)
			{
				if (temp__bomb_p->exist)
				{
					if (!temp__bomb_p->IsBoom() && temp__bomb_p->IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__bomb_p);
						particles_array_mtx.unlock();
					}

					found_bombs++;
				}
			}
			bombs_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPBonuses()
{
	GameTypes::entities_count_t found_bonuses;
	Bonus* temp__bonuses_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			bonuses_array_mtx.lock();
			for (temp__bonuses_p = bonuses, found_bonuses = 0; found_bonuses < bonuses_count; temp__bonuses_p++)
			{
				if (temp__bonuses_p->exist)
				{
					if (temp__bonuses_p->IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__bonuses_p);
						particles_array_mtx.unlock();
					}

					found_bonuses++;
				}
			}
			bonuses_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPBullets()
{
	GameTypes::entities_count_t found_bullets;
	Bullet* temp__bullets_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			bullets_array_mtx.lock();
			for (temp__bullets_p = bullets, found_bullets = 0; found_bullets < bullets_count; temp__bullets_p++)
			{
				if (temp__bullets_p->exist)
				{
					if (temp__bullets_p->IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__bullets_p);
						particles_array_mtx.unlock();
					}

					found_bullets++;
				}
			}
			bullets_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPDynamicParticles()
{
	DynamicParticle* temp__dynamic_particles_p;
	GameTypes::entities_count_t found_dynamic_particles;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			particles_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			for (found_dynamic_particles = 0, temp__dynamic_particles_p = dynamic_particles; found_dynamic_particles < dynamic_particles_count; temp__dynamic_particles_p++)
			{
				if (temp__dynamic_particles_p->exist)
				{
					if (temp__dynamic_particles_p->IsCollision(temp__portal_p))
					{
						TeleportEntity(temp__portal_p, temp__dynamic_particles_p);
					}

					found_dynamic_particles++;
				}
			}
			dynamic_particles_array_mtx.unlock();
			particles_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPPilots()
{
	GameTypes::players_count_t found_pilots;
	Pilot* temp__pilots_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			pilots_array_mtx.lock();
			for (found_pilots = 0, temp__pilots_p = pilots; found_pilots < pilots_count; temp__pilots_p++)
			{
				if (temp__pilots_p->exist)
				{
					if (temp__pilots_p->DynamicEntity::IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__pilots_p);
						particles_array_mtx.unlock();
					}

					found_pilots++;
				}
			}
			pilots_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PortalsTPShips()
{
	GameTypes::players_count_t found_ships;
	Ship* temp__ships_p;
	Portal* temp__portal_p = portals;

	portals_array_mtx.lock();
	for (GameTypes::map_elements_count_t found_portals = 0; found_portals < portals_count; temp__portal_p++)
	{
		if (temp__portal_p->exist)
		{
			pilots_array_mtx.lock();
			for (temp__ships_p = ships, found_ships = 0; found_ships < ships_count; temp__ships_p++)
			{
				if (temp__ships_p->exist)
				{
					if (temp__ships_p->DynamicEntity::IsCollision(temp__portal_p))
					{
						particles_array_mtx.lock();
						TeleportEntity(temp__portal_p, temp__ships_p);
						particles_array_mtx.unlock();
					}

					found_ships++;
				}
			}
			pilots_array_mtx.unlock();

			found_portals++;
		}
	}
	portals_array_mtx.unlock();
}

void Game::PilotsKilledByBombs()
{
	GameTypes::players_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			bombs_array_mtx.lock();
			for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				temp__bomb_p = &bombs[bomb];
				if (temp__bomb_p->exist)
				{
					if (temp__bomb_p->IsBoom() && temp__pilot_p->IsCollision(temp__bomb_p))
					{
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__bomb_p, temp__pilot_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						goto end_of_bomb_cycle;

					}
					found_bombs++;
				}
			}
			found_pilots++;
		end_of_bomb_cycle:
			bombs_array_mtx.unlock();
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByBullet()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::entities_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if (temp__bullet_p->is_ignore == BULLET_IGNORE_NOTHING &&
						temp__pilot_p->IsCollision(temp__bullet_p))
					{
						if (game_rules & GAME_RULE_FRENDLY_FIRE && temp__bullet_p->CreatedByTeam(temp__pilot_p))
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__bullet_p, temp__pilot_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						if (game_rules & GAME_RULE_NEED_KILL_PILOT)
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			bullets_array_mtx.unlock();
			found_pilots++;
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsCheckInput()
{
	Pilot* temp__pilot_p;
	GameTypes::players_count_t pilot_player_number;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			pilot_player_number = temp__pilot_p->GetPlayerNumber();
			input_values_mtx.lock();
			if (shoot_flags[pilot_player_number] == true)
			{
				temp__pilot_p->AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_PILOT);
			}
			if (rotate_flags[pilot_player_number] == true)
			{
				if (rotation_inverse == true)
				{
					temp__pilot_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					temp__pilot_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				temp__pilot_p->SetAngularVelocity(0.0f);
			}
			input_values_mtx.unlock();
			found_pilots++;
		}
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	Pilot* temp__pilot_p;
	Segment temp__segment;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			knifes_array_mtx.lock();
			for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
			{
				temp__knife_p = &knifes[knife];
				if (temp__knife_p->exist)
				{
					temp__segment = temp__knife_p->GetSegment();
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__knife_p->CreatedByTeam(temp__pilot_p)) &&
						temp__pilot_p->IsCollision(&temp__segment))
					{
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__knife_p, temp__pilot_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						knifes_array_mtx.unlock();
						goto end_of_pilot_cycle;
					}
					found_knifes++;
				}
			}
			knifes_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsKilledByLasers()
{
	GameTypes::entities_count_t laser, found_lasers;
	Laser* temp__laser_p;
	Pilot* temp__pilot_p;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			lasers_array_mtx.lock();
			for (laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
			{
				temp__laser_p = &lasers[laser];
				if (temp__laser_p->exist)
				{
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__laser_p->CreatedByTeam(temp__pilot_p)) &&
						temp__pilot_p->IsCollision(temp__laser_p))
					{
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__laser_p, temp__pilot_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						lasers_array_mtx.unlock();
						goto end_of_pilots_cycle;
					}
					found_lasers++;
				}
			}
			lasers_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilots_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::PilotsRespawnAuto()
{
	GameTypes::players_count_t pilot, found_pilots;
	Pilot* temp__pilot_p;

	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	for (pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			if (temp__pilot_p->CanRespawn())
			{
				AddEntity(temp__pilot_p->Respawn());
				ships_can_shoot_flags[pilot] = SHIP_DEFAULT_UNBRAKABLE_PERIOD;
				RemoveEntity(temp__pilot_p);
				goto end_of_pilot_cycle;
			}
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	ships_array_mtx.unlock();
	pilots_array_mtx.unlock();
}

void Game::ShipsCreateExaust()
{
	if (!(global_timer % (PARTICLE_PERIOD_EXAUST_ENGINE / 10)))
	{
		Ship* temp__ship_p;
		ships_array_mtx.lock();
		for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
		{
			temp__ship_p = &ships[ship];
			if (temp__ship_p->exist)
			{
				dynamic_particles_array_mtx.lock();
				AddEntity(temp__ship_p->CreateEnginExaust(global_timer));
				dynamic_particles_array_mtx.unlock();
				found_ships++;
			}
		}
		ships_array_mtx.unlock();
	}
}

void Game::PilotsKilledByMegaLaser()
{
	GameTypes::entities_count_t mega_laser, found_mega_lasers;
	MegaLaser* temp__mega_laser_p;
	Pilot* temp__pilot_p;
	Segment temp__segment;

	pilots_array_mtx.lock();
	for (GameTypes::players_count_t pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
	{
		temp__pilot_p = &pilots[pilot];
		if (temp__pilot_p->exist)
		{
			mega_lasers_array_mtx.lock();
			for (mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
			{
				temp__mega_laser_p = &mega_lasers[mega_laser];
				if (temp__mega_laser_p->exist)
				{
					if (temp__mega_laser_p->IsShooting(global_timer) &&
						temp__pilot_p->IsCollision(temp__mega_laser_p))
					{
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__mega_laser_p, temp__pilot_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						mega_lasers_array_mtx.unlock();
						goto end_of_pilot_cycle;
					}
					found_mega_lasers++;
				}
			}
			mega_lasers_array_mtx.unlock();
			found_pilots++;
		}
	end_of_pilot_cycle:;
	}
	pilots_array_mtx.unlock();
}

void Game::ShipsInfluenceToBonuses()
{
	GameTypes::entities_count_t bonus, found_bonuses;
	Ship* temp__ship_p;
	Bonus* temp__bonus_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			bonuses_array_mtx.lock();
			for (bonus = 0, found_bonuses = 0; found_bonuses < bonuses_count; bonus++)
			{
				temp__bonus_p = &bonuses[bonus];
				if (temp__bonus_p->exist)
				{
					if (temp__bonus_p->GetDistance(temp__ship_p) < temp__ship_p->radius * GAME_SHIP_INFLUENCE_RADIUS_COEFFISIENT)
					{
						if (temp__bonus_p->IsCollision(temp__ship_p))
						{
							if (temp__bonus_p->bonus_inventory & BONUS_RULE_REVERSE)
							{
								if (rotation_inverse)
								{
									rotation_inverse = false;
								}
								else
								{
									rotation_inverse = true;
								}
								rotate_inverse_chage_tic = global_timer;
								temp__bonus_p->bonus_inventory &= BONUS_ALL - BONUS_RULE_REVERSE;
							}
							temp__ship_p->TakeBonus(temp__bonus_p, game_rules & GAME_RULE_TRIPLE_BONUSES);
							RemoveEntity(temp__bonus_p);
							goto end_of_bonus_cycle;
						}
						temp__bonus_p->AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, temp__ship_p->GetPosition());
					}
					found_bonuses++;
				}
			end_of_bonus_cycle:;
			}
			bonuses_array_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsCheckInput()
{
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			input_values_mtx.lock();
			if (ships_can_shoot_flags[ship] > 0)
			{
				ships_can_shoot_flags[ship]--;
			}
			if (rotate_flags[ship] == true)
			{
				if (rotation_inverse == true)
				{
					temp__ship_p->SetAngularVelocity(-GAME_SHIP_ANGULAR_VELOCITY);
				}
				else
				{
					temp__ship_p->SetAngularVelocity(GAME_SHIP_ANGULAR_VELOCITY);
				}
			}
			else
			{
				temp__ship_p->SetAngularVelocity(0.0f);
			}
			if (burnout_flags[ship] == true)
			{
				burnout_flags[ship] = false;
				temp__ship_p->Burnout(BULLET_DEFAULT_VELOCITY, !rotation_inverse);
			}
			input_values_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsShoot()
{
	Ship* temp__ship_p;
	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			input_values_mtx.lock();
			ShipShoot(temp__ship_p);
			input_values_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsRespawnOrDestroyPilots()
{
	GameTypes::players_count_t pilot, found_pilots;
	Ship* temp__ship_p;
	Pilot* temp__pilot_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			for (pilot = 0, found_pilots = 0; found_pilots < pilots_count; pilot++)
			{
				temp__pilot_p = &pilots[pilot];
				if (temp__pilot_p->exist)
				{
					if (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE && temp__pilot_p->SameTeams(temp__ship_p))
					{
						SpawnEntity(temp__ship_p, temp__pilot_p);
						goto end_of_pilot_cycle;
					}
					else if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__pilot_p->SameTeams(temp__ship_p)) &&
						temp__ship_p->DynamicEntity::IsCollision(temp__pilot_p))
					{
						log_data_mtx.lock();
						DestroyEntity(temp__ship_p, temp__pilot_p);
						log_data_mtx.unlock();
						goto end_of_pilot_cycle;
					}
					found_pilots++;
				}
			end_of_pilot_cycle:;
			}
			pilots_array_mtx.unlock();
			found_ships++;
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBombsOrActivateBombs()
{
	GameTypes::players_count_t bomb, found_bombs;
	Bomb* temp__bomb_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			for (bomb = 0, found_bombs = 0; found_bombs < bombs_count; bomb++)
			{
				temp__bomb_p = &bombs[bomb];
				if (temp__bomb_p->exist)
				{
					if (temp__bomb_p->IsBoom() && temp__ship_p->IsCollision(temp__bomb_p))
					{
						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__bomb_p, temp__ship_p);
						log_data_mtx.unlock();
						bonuses_array_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					else if (!temp__bomb_p->IsActive() && !temp__bomb_p->CreatedByTeam(temp__ship_p) && temp__ship_p->GetDistance(temp__bomb_p) < temp__bomb_p->radius * BOMB_ACTIVATION_RADIUS_COEF)
					{
						temp__bomb_p->Activate();
					}
					found_bombs++;
				}
			}
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBullets()
{
	GameTypes::entities_count_t bullet, found_bullets;
	Bullet* temp__bullet_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			bullets_array_mtx.lock();
			for (bullet = 0, found_bullets = 0; found_bullets < bullets_count; bullet++)
			{
				temp__bullet_p = &bullets[bullet];
				if (temp__bullet_p->exist)
				{
					if ((temp__bullet_p->is_ignore & BULLET_IGNORE_MUSTER) &&
						temp__bullet_p->CreatedBy(temp__ship_p) &&
						!temp__ship_p->IsColectEntity(temp__bullet_p))
					{
						temp__bullet_p->is_ignore = BULLET_IGNORE_NOTHING;
					}
					if (!temp__ship_p->IsUnbrakable() &&
						temp__bullet_p->is_ignore == BULLET_IGNORE_NOTHING &&
						temp__ship_p->IsCollision(temp__bullet_p))
					{
						if (game_rules & GAME_RULE_FRENDLY_FIRE && temp__bullet_p->CreatedByTeam(temp__ship_p))
						{
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						if (temp__ship_p->HaveBuff(SHIP_BUFF_SHIELD))
						{
							temp__ship_p->SetUnbrakablePeriod(SHIP_DEFAULT_UNBRAKABLE_PERIOD);
							temp__ship_p->SpendBuffNoCheck(SHIP_BUFF_SHIELD);
							RemoveEntity(temp__bullet_p);
							goto end_of_bullet_cycle;
						}
						else
						{
							temp__ship_p->AddVelocity(temp__bullet_p->GetVelocity());
							bonuses_array_mtx.lock();
							dynamic_particles_array_mtx.lock();
							log_data_mtx.lock();
							DestroyEntity(temp__bullet_p, temp__ship_p);
							log_data_mtx.unlock();
							dynamic_particles_array_mtx.unlock();
							bonuses_array_mtx.unlock();
							RemoveEntity(temp__bullet_p);
							goto end_of_ship_cycle;
						}
					}
					found_bullets++;
				}
			end_of_bullet_cycle:;
			}
			found_ships++;
		end_of_ship_cycle:
			bullets_array_mtx.unlock();
			pilots_array_mtx.unlock();
		}
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByKnifes()
{
	GameTypes::entities_count_t knife, found_knifes;
	Knife* temp__knife_p;
	Ship* temp__ship_p;
	Segment temp__segment;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			knifes_array_mtx.lock();
			for (knife = 0, found_knifes = 0; found_knifes < knifes_count; knife++)
			{
				temp__knife_p = &knifes[knife];
				temp__segment = temp__knife_p->GetSegment();
				if (temp__knife_p->exist)
				{
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__knife_p->CreatedByTeam(temp__ship_p)) &&
						!temp__ship_p->IsUnbrakable() &&
						!temp__knife_p->CreatedBy(temp__ship_p) &&
						temp__ship_p->IsCollision(&temp__segment))
					{
						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__knife_p, temp__ship_p);
						RemoveEntity(temp__knife_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						bonuses_array_mtx.unlock();
						knifes_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					found_knifes++;
				}
			}
			knifes_array_mtx.unlock();
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByLasers()
{
	GameTypes::entities_count_t laser, found_lasers;
	Laser* temp__laser_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::players_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			pilots_array_mtx.lock();
			lasers_array_mtx.lock();
			for (laser = 0, found_lasers = 0; found_lasers < lasers_count; laser++)
			{
				temp__laser_p = &lasers[laser];
				if (temp__laser_p->exist)
				{
					if (!(game_rules & GAME_RULE_FRENDLY_FIRE && temp__laser_p->CreatedByTeam(temp__ship_p)) &&
						!temp__ship_p->IsUnbrakable() &&
						!temp__laser_p->CreatedBy(temp__ship_p) &&
						temp__ship_p->IsCollision(temp__laser_p))
					{
						bonuses_array_mtx.lock();
						dynamic_particles_array_mtx.lock();
						log_data_mtx.lock();
						DestroyEntity(temp__laser_p, temp__ship_p);
						log_data_mtx.unlock();
						dynamic_particles_array_mtx.unlock();
						bonuses_array_mtx.unlock();
						lasers_array_mtx.unlock();
						pilots_array_mtx.unlock();
						goto end_of_ship_cycle;
					}
					found_lasers++;
				}
			}
			lasers_array_mtx.unlock();
			pilots_array_mtx.unlock();
			found_ships++;
		}
	end_of_ship_cycle:;
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByMegaLasers()
{
	GameTypes::entities_count_t mega_laser, found_mega_lasers;
	MegaLaser* temp__mega_laser_p;
	Ship* temp__ship_p;

	ships_array_mtx.lock();
	for (GameTypes::entities_count_t ship = 0, found_ships = 0; found_ships < ships_count; ship++)
	{
		temp__ship_p = &ships[ship];
		if (temp__ship_p->exist)
		{
			mega_lasers_array_mtx.lock();
			for (mega_laser = 0, found_mega_lasers = 0; found_mega_lasers < mega_lasers_count; mega_laser++)
			{
				temp__mega_laser_p = &mega_lasers[mega_laser];
				if (temp__mega_laser_p->exist)
				{
					if (temp__mega_laser_p->IsShooting(global_timer))
					{
						if (temp__ship_p->IsCollision(temp__mega_laser_p))
						{
							bonuses_array_mtx.lock();
							dynamic_particles_array_mtx.lock();
							log_data_mtx.lock();
							DestroyEntity(temp__mega_laser_p, temp__ship_p);
							log_data_mtx.unlock();
							dynamic_particles_array_mtx.unlock();
							bonuses_array_mtx.unlock();
							goto end_of_ship_cycle;
						}
					}
					found_mega_lasers++;
				}
			}
			found_ships++;
		end_of_ship_cycle:
			mega_lasers_array_mtx.unlock();
		}
	}
	ships_array_mtx.unlock();
}

void Game::TurelsShoot()
{
	Turel* temp__turel_p;

	turels_array_mtx.lock();
	for (GameTypes::entities_count_t turel = 0, found_turels = 0; found_turels < turels_count; turel++)
	{
		temp__turel_p = &turels[turel];
		if (temp__turel_p->exist)
		{
			if (temp__turel_p->CanShoot(global_timer))
			{
				bullets_array_mtx.lock();
				AddEntity(temp__turel_p->Shoot());
				bullets_array_mtx.unlock();
			}
			found_turels++;
		}
	}
	turels_array_mtx.unlock();
}