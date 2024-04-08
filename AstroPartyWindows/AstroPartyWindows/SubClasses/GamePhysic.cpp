#include "Game.h"

#define M_PI	3.14159265358979323846

#pragma warning(disable : 26451)


template<typename EntityType>
void Game::DynamicEntitiesCollisions(std::vector<EntityType>& entities, GameTypes::entities_count_t entities_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities_count; first++)
	{
		if (entities[first].exist)
		{
			for (GameTypes::entities_count_t second = first + 1, found_entites_count2 = found_entites_count1 + 1; found_entites_count2 < entities_count; second++)
			{
				if (entities[second].exist)
				{
					entities[first].DynamicEntity::Collision(entities[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid>(std::vector<Asteroid>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bomb>(std::vector<Bomb>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bullet>(std::vector<Bullet>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bonus>(std::vector<Bonus>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Ship>(std::vector<Ship>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Pilot>(std::vector<Pilot>& entities, GameTypes::entities_count_t entities_count);

template<typename Entity1Type, typename Entity2Type>
void Game::DynamicEntitiesCollisions(std::vector<Entity1Type>& entities1, std::vector<Entity2Type>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count)
{
	for (GameTypes::entities_count_t first = 0, found_entites_count1 = 0; found_entites_count1 < entities1_count; first++)
	{
		if (entities1[first].exist)
		{
			for (GameTypes::entities_count_t second = 0, found_entites_count2 = 0; found_entites_count2 < entities2_count; second++)
			{
				if (entities2[second].exist)
				{
					entities1[first].DynamicEntity::Collision(entities2[second]);
					found_entites_count2++;
				}
			}
			found_entites_count1++;
		}
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid,	Bomb>(std::vector<Asteroid>& entities1, std::vector<Bomb>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Bullet>(std::vector<Asteroid>& entities1, std::vector<Bullet>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Bonus>(std::vector<Asteroid>& entities1, std::vector<Bonus>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Ship>(std::vector<Asteroid>& entities1, std::vector<Ship>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Asteroid, Pilot>(std::vector<Asteroid>& entities1, std::vector<Pilot>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Asteroid>(std::vector<Bomb>&  entities1, std::vector<Asteroid>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Bullet>(std::vector<Bomb>&  entities1, std::vector<Bullet>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Bonus>(std::vector<Bomb>&  entities1, std::vector<Bonus>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Ship>(std::vector<Bomb>&  entities1, std::vector<Ship>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bomb, Pilot>(std::vector<Bomb>&  entities1, std::vector<Pilot>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Asteroid>(std::vector<Bullet>&  entities1, std::vector<Asteroid>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Bomb>(std::vector<Bullet>&  entities1, std::vector<Bomb>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Bonus>(std::vector<Bullet>&  entities1, std::vector<Bonus>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Ship>(std::vector<Bullet>&  entities1, std::vector<Ship>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bullet, Pilot>(std::vector<Bullet>&  entities1, std::vector<Pilot>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Asteroid>(std::vector<Bonus>&  entities1, std::vector<Asteroid>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Bomb>(std::vector<Bonus>&  entities1, std::vector<Bomb>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Bullet>(std::vector<Bonus>&  entities1, std::vector<Bullet>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Ship>(std::vector<Bonus>&  entities1, std::vector<Ship>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Bonus, Pilot>(std::vector<Bonus>&  entities1, std::vector<Pilot>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Asteroid>(std::vector<Ship>&  entities1, std::vector<Asteroid>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bomb>(std::vector<Ship>&  entities1, std::vector<Bomb>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bullet>(std::vector<Ship>&  entities1, std::vector<Bullet>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Bonus>(std::vector<Ship>&  entities1, std::vector<Bonus>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Ship, Pilot>(std::vector<Ship>&  entities1, std::vector<Pilot>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Asteroid>(std::vector<Pilot>& entities1, std::vector<Asteroid>& entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bomb>(std::vector<Pilot>& entities1, std::vector<Bomb>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bullet>(std::vector<Pilot>& entities1, std::vector<Bullet>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Bonus>(std::vector<Pilot>& entities1, std::vector<Bonus>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);
template void Game::DynamicEntitiesCollisions<Pilot, Ship>(std::vector<Pilot>& entities1, std::vector<Ship>&  entities2, GameTypes::entities_count_t entities1_count, GameTypes::entities_count_t entities2_count);

template<typename EntityType>
void Game::DynamicEntitiesCollisions(Map::MapData& map, std::vector<EntityType>& entities, GameTypes::entities_count_t entities_count)
{
	GameTypes::entities_count_t found = 0;
	for (auto& entity : entities)
	{
		if (found >= entities_count) break;
		if (!entity.exist) continue;
		entity.Collision(map);
		if (entity.exist)
		{
			++found;
			continue;
		}
		log_data_mtx.lock();
		entity.exist = true;
		DestroyEntity(entity);
		log_data_mtx.unlock();
	}
}
template void Game::DynamicEntitiesCollisions<Asteroid>(Map::MapData& map, std::vector<Asteroid>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bonus>(Map::MapData& map, std::vector<Bonus>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Bullet>(Map::MapData& map, std::vector<Bullet>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Ship>(Map::MapData& map, std::vector<Ship>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesCollisions<Pilot>(Map::MapData& map, std::vector<Pilot>& entities, GameTypes::entities_count_t entities_count);

void Game::DynamicEntitiesCollisions(Map::MapData& map, std::vector<Asteroid>& asteroids, GameTypes::entities_count_t asteroids_count)
{
	GameTypes::entities_count_t found = 0;
	for (auto& entity : asteroids)
	{
		if (found >= asteroids_count)
		{
			break;
		}
		if (entity.exist)
		{
			++found;
		}
	}
	
	found = 0;
	for (auto& entity : asteroids)
	{
		if (entity.exist)
		{
			entity.Collision(map);
			if (!entity.exist)//if entity was destroed after collision
			{
				entity.exist = true;
				DestroyEntity(entity);
				continue;
			}
			found++;
		}
	}
}

void Game::DynamicEntitiesCollisions(Map::MapData& map, std::vector<Bomb>& bombs, GameTypes::entities_count_t entities_count)
{
	GameTypes::entities_count_t found = 0;
	for (auto& bomb : bombs)
	{
		if (found >= bombs_count)
		{
			break;
		}
		if (!bomb.exist)
		{
			continue;
		}
		bomb.Collision(map);
		found++;
	}
}

void Game::DynamicEntitiesCollisions(Map::MapData& map, std::vector<DynamicParticle>& dynamic_particles_array, GameTypes::entities_count_t entities_count)
{
	GameTypes::entities_count_t found = 0;
	for (auto& dynamic_particle : dynamic_particles_array)
	{
		if (found >= dynamic_particles_count)
		{
			break;
		}
		if (!dynamic_particle.exist)
		{
			continue;
		}
		if (dynamic_particle.properties & DYNAMIC_PARTICLE_PROPERTY_FORCED_BY_MAP)
		{
			dynamic_particle.Collision(map);
		}
		found++;

	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(const Vec2F& force, std::vector<EntityType>& entities, GameTypes::entities_count_t entities_count)
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
template void Game::DynamicEntitiesAddForce<Asteroid>(const Vec2F& force,  std::vector<Asteroid>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bomb>(const Vec2F& force, std::vector<Bomb>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(const Vec2F& force, std::vector<Bonus>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(const Vec2F& force, std::vector<Bullet>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(const Vec2F& force, std::vector<Ship>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(const Vec2F& force, std::vector<Pilot>& entities, GameTypes::entities_count_t entities_count);

template<typename EntityType>
void Game::DynamicEntitiesAddForce(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<EntityType>& entities, GameTypes::entities_count_t entities_count)
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
					entities[i].AddForce(force);

					found++;
				}
			}
		}
	}
}
template void Game::DynamicEntitiesAddForce<Asteroid>(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count,  std::vector<Asteroid>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bullet>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bonus>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<Ship>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<Pilot>& entities, GameTypes::entities_count_t entities_count);

void Game::DynamicEntitiesAddForce(std::vector<GravGen>& grav_gens, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bomb>&  entities, GameTypes::entities_count_t entities_count)
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
						entities[i].AddForce(force);
					}
					found++;
				}
			}
			found_grav_gens++;
		}
	}
}

template<typename EntityType>
void Game::DynamicEntitiesAddForce(std::vector<DecelerationArea>& deceler_areas, GameTypes::map_elements_count_t deceler_areas_count, std::vector<EntityType>& entities, GameTypes::entities_count_t entities_count)
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
					if (deceler_areas[deceler_area].IsCollision(entities[i]))
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
template void Game::DynamicEntitiesAddForce<Asteroid>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count,  std::vector<Asteroid>& entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bomb>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bomb>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bonus>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bonus>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Bullet>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count, std::vector<Bullet>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Ship>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count, std::vector<Ship>&  entities, GameTypes::entities_count_t entities_count);
template void Game::DynamicEntitiesAddForce<Pilot>(std::vector<DecelerationArea>&  deceler_areas, GameTypes::map_elements_count_t grav_gens_count, std::vector<Pilot>& entities, GameTypes::entities_count_t entities_count);

template <typename EntityType>
void Game::TeleportEntity(const Portal& portal, EntityType& entity)
{
	portal.Teleport(entity);
	AddEntity(portal.CreateParticles(global_timer));
	AddEntity(portal.CreateParticlesTP(global_timer, 5.0f * entity.radius));

	camera.move_velocity_coefficient = CAMERA_LOW_MOVE_VELOCITY;
	camera.resize_velocity_coefficient = CAMERA_LOW_RESIZE_VELOCITY;
}
template void Game::TeleportEntity<Asteroid>(const Portal& portal, Asteroid& asteroid);
template void Game::TeleportEntity<Bonus>(const Portal& portal, Bonus& bonus);
template void Game::TeleportEntity<Bomb>(const Portal& portal, Bomb& bomb);
template void Game::TeleportEntity<Bullet>(const Portal& portal, Bullet& bullet);
template void Game::TeleportEntity<DynamicParticle>(const Portal& portal, DynamicParticle& dynamic_entity);
template void Game::TeleportEntity<Ship>(const Portal& portal, Ship& ship);
template void Game::TeleportEntity<Pilot>(const Portal& portal, Pilot& pilot);

void Game::AnnihAreaGenShoot(const AnnihAreaGen& annih_area_gen)
{
	const ControledEntity* host_p = annih_area_gen.GetHostP();
	if (host_p->ShouldShoot() && ships_shooting_flags[host_p->GetPlayerNumber()])
	{
		AddEntity(annih_area_gen.Shoot());
	}
}

void Game::ShipShoot(Ship& ship)
{
	if (ship.CanCreatingObject())
	{
		GameTypes::objects_types_count_t type = ship.GetTypeOfElemntInLoop();
		GameTypes::entities_count_t entity_number;
		float angle;
		Vec2F r_vec;
		Segment local_segment;
		switch (type)
		{
		case GAME_OBJECT_TYPE_LASER_BOMB:
			bombs_array_mtx.lock();
			entity_number = ship.GetElemntFromList();
			AddEntity(
				Bomb(
					ship.GetPosition(),
					ship.GetVelocity() + ship.GetDirection() * SHIP_SUPER_BONUS__BOMBS_LASER_VELOCITY * (float)(SHIP_SUPER_BONUS__BOMBS_IN_LASER / 2 + SHIP_SUPER_BONUS__BOMBS_IN_LASER - entity_number) / 1.5f / (float)SHIP_SUPER_BONUS__BOMBS_IN_LASER,
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					BOMB_DEFAULT_ACTIVATION_PERIOD + entity_number,
					ship.GetAngle(),
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
			entity_number = (SHIP_SUPER_BONUS__BOMBS_IN_LOOP - ship.GetElemntFromList()) / 2;
			angle = (float)M_PI / 2.0f * (1.0f + (float)(entity_number * 2) / (float)SHIP_SUPER_BONUS__BOMBS_IN_LOOP);
			AddEntity(
				Bomb(ship.GetPosition(),
					ship.GetVelocity() + Vec2F(SHIP_SUPER_BONUS__BOMBS_LOOP_VELOCITY, 0.0f).Rotate(ship.GetAngle() + angle),
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					0,
					ship.GetAngle() + angle));

			if (ship.GetElemntFromList() == 0)
			{
				bombs_array_mtx.unlock();
				return;
			}

			AddEntity(
				Bomb(ship.GetPosition(),
					ship.GetVelocity() + Vec2F(SHIP_SUPER_BONUS__BOMBS_LOOP_VELOCITY, 0.0f).Rotate(ship.GetAngle() - angle),
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					0,
					ship.GetAngle() - angle));

			bombs_array_mtx.unlock();
			return;

		case GAME_OBJECT_TYPE_LOOP_BULLET:
			bullets_array_mtx.lock();
			entity_number = ship.GetElemntFromList();
			angle = (float)(((entity_number - 1u) / 2u) + 0.5f) / (float)SHIP_BONUS__LOOP_BULLETS_IN_LOOP * (float)M_PI * 2.0f;

			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship.GetAngle() + angle);
			AddEntity(
				Bullet(
					ship.GetPosition() + r_vec * ship.radius,
					ship.GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
					ship.GetPlayerNumber(),
					ship.GetTeamNumber(),
					true,
					ship.GetAngle() + angle,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
					SHIP_BONUS__LOOP_BULLETS_RADIUS));

			entity_number = ship.GetElemntFromList();
			if (entity_number == 0)
			{
				bullets_array_mtx.unlock();
				return;
			}
			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship.GetAngle() - angle);
			AddEntity(
				Bullet(
					ship.GetPosition() + r_vec * ship.radius,
					ship.GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
					ship.GetPlayerNumber(),
					ship.GetTeamNumber(),
					true,
					ship.GetAngle() - angle,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
					SHIP_BONUS__LOOP_BULLETS_RADIUS));
			bullets_array_mtx.unlock();
			return;

		case GAME_OBJECT_TYPE_LOOP_KNIFE:
			knifes_array_mtx.lock();
			entity_number = ship.GetElemntFromList() - 1;
			local_segment.Set(
				Vec2F(0.5f, 0.0f).Rotate(((float)entity_number + 0.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				Vec2F(0.5f, 0.0f).Rotate(((float)entity_number + 1.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				true);
			AddEntity(Knife(&ship, local_segment, 1u));

			local_segment.Set(
				Vec2F(0.5f, 0.0f).Rotate(-((float)entity_number + 0.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				Vec2F(0.5f, 0.0f).Rotate(-((float)entity_number + 1.5f) / (float)SHIP_SUPER_BONUS__KNIFES_IN_LOOP * (float)M_PI * 2.0f).Scale(Vec2F(1.0f, sqrtf(2.0f))),
				true);
			AddEntity(Knife(&ship, local_segment, 1u));
			knifes_array_mtx.unlock();
			return;

		case GAME_OBJECT_TYPE_LOOP_BOMB_KNIFE:
			bombs_array_mtx.lock();
			entity_number = ship.GetElemntFromList();
			angle = (float)(((entity_number - 1u) / 2u) + 0.5f) / (float)SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP * (float)M_PI * 2.0f;

			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship.GetAngle() + angle);
			AddEntity(Bomb(
				ship.GetPosition() + r_vec * (ship.radius * 3.0f + SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP_DEFAULT_VELOCITY),
				ship.GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
				ship.GetTeamNumber(),
				ship.GetTeamNumber(),
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
				0.0f,
				0.0f,
				DEFAULT_FORCE_COLLISION_COEFFICIENT,
				0.0f,
				BOMB_DEFAULT_RADIUS,
				BOMB_STATUS_BOOM,
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));

			entity_number = ship.GetElemntFromList();
			if (entity_number == 0)
			{
				bombs_array_mtx.unlock();
				return;
			}
			r_vec = Vec2F(1.0f, 0.0f).RotateClockwise(ship.GetAngle() - angle);
			AddEntity(Bomb(
				ship.GetPosition() + r_vec * (ship.radius * 3.0f + SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP_DEFAULT_VELOCITY),
				ship.GetVelocity() + r_vec * SHIP_BONUS__LOOP_BULLET_VELOCITY,
				ship.GetTeamNumber(),
				ship.GetTeamNumber(),
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
				0.0f,
				0.0f,
				DEFAULT_FORCE_COLLISION_COEFFICIENT,
				0.0f,
				BOMB_DEFAULT_RADIUS,
				BOMB_STATUS_BOOM,
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
				SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));
			bombs_array_mtx.unlock();
			return;

		case GAME_OBJECT_TYPE_LASER_BOMB_KNIFE:
			bombs_array_mtx.lock();
			entity_number = ship.GetElemntFromList();
			if (entity_number == 0)
			{
				bombs_array_mtx.unlock();
				return;
			}

			{

				Vec2F annihilation_area_position;
				float basic_length = ship.radius * 3.0f + BOMB_DEFAULT_RADIUS;
#if SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER % 2 == 1

				entity_number = (SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER - entity_number + 1) / 2;//0 -> (SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER - 1)
				r_vec = Vec2F(basic_length, 0.0f) - Vec2F(BOMB_DEFAULT_RADIUS * (float)entity_number);
				annihilation_area_position = ship.GetVelocity() + r_vec.Rotate(ship.GetAngle());
				AddEntity(Bomb(
					ship.GetPosition() + ship.GetVelocity() + annihilation_area_position,
					ship.GetVelocity() + annihilation_area_position / basic_length * SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER_DEFAULT_VELOCITY,
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					0.0f,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					0.0f,
					BOMB_DEFAULT_RADIUS,
					BOMB_STATUS_BOOM,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));

				if (entity_number != 0)
				{
					r_vec.y = -r_vec.y;
					annihilation_area_position = r_vec.Rotate(ship.GetAngle());

					AddEntity(Bomb(
						ship.GetPosition() + ship.GetVelocity() + annihilation_area_position,
						ship.GetVelocity() + annihilation_area_position / basic_length * SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER_DEFAULT_VELOCITY,
						ship.GetTeamNumber(),
						ship.GetTeamNumber(),
						SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
						0.0f,
						0.0f,
						DEFAULT_FORCE_COLLISION_COEFFICIENT,
						0.0f,
						BOMB_DEFAULT_RADIUS,
						BOMB_STATUS_BOOM,
						SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
						SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));
				}
#else
				entity_number = (SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER - entity_number) / 2 + 1;//0 -> (SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER - 1)

				r_vec = Vec2F(0.0f, ship.radius * 3.0f + BOMB_DEFAULT_RADIUS) - Vec2F(BOMB_DEFAULT_RADIUS * (float)entity_number);
				annihilation_area_position = ship.GetVelocity() + r_vec.Rotate(ship.GetAngle());
				AddEntity(Bomb(
					ship.GetPosition() + ship.GetVelocity() + annihilation_area_position,
					ship.GetVelocity() + annihilation_area_position / basic_length * SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER_DEFAULT_VELOCITY,
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					0.0f,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					0.0f,
					BOMB_DEFAULT_RADIUS,
					BOMB_STATUS_BOOM,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));

				r_vec.y = -r_vec.y;
				annihilation_area_position = ship.GetVelocity() + r_vec.Rotate(ship.GetAngle());
				AddEntity(Bomb(
					ship.GetPosition() + ship.GetVelocity() + annihilation_area_position,
					ship.GetVelocity() + annihilation_area_position / basic_length * SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER_DEFAULT_VELOCITY,
					ship.GetTeamNumber(),
					ship.GetTeamNumber(),
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					0.0f,
					0.0f,
					DEFAULT_FORCE_COLLISION_COEFFICIENT,
					0.0f,
					BOMB_DEFAULT_RADIUS,
					BOMB_STATUS_BOOM,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
					SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));
#endif
				ship.GetElemntFromList();
			}
			bombs_array_mtx.unlock();
			return;

		case GAME_OBJECT_TYPE_LOOP_LASER_KNIFE:

			bombs_array_mtx.lock();

			entity_number = ship.GetElemntFromList();

#if SHIP_SUPER_BONUS__LONG_KNIFES_IN_LOOP % 2 == 1

			entity_number = (SHIP_SUPER_BONUS__LONG_KNIFES_IN_LOOP - entity_number + 1) / 2;
			angle = (float)entity_number / (float)(SHIP_SUPER_BONUS__LONG_KNIFES_IN_LOOP / 2) * (float)M_PI;

			local_segment.Set(Vec2F(0.0f), Vec2F(-1.0f, 0.0f).Rotate(angle).Scale(Vec2F(1.0f, sqrtf(2.0f))));
			AddEntity(Knife(ship, &local_segment, 1));

			if (ship.GetElemntFromList() != 0)
			{

				local_segment.Set(Vec2F(0.0f), Vec2F(-1.0f, 0.0f).Rotate(-angle).Scale(Vec2F(1.0f, sqrtf(2.0f))));
				AddEntity(Knife(ship, &local_segment, 1));

				ship.GetElemntFromList();
			}
		
#else
			angle = (float)(((entity_number - 1u) / 2u) + 0.5f) / (float)SHIP_SUPER_BONUS__LONG_KNIFES_IN_LOOP * (float)M_PI * 2.0f;

			local_segment.Set(Vec2F(0.0f), Vec2F(-1.0f, 0.0f).Rotate(angle).Scale(Vec2F(1.0f, sqrtf(2.0f))));
			AddEntity(Knife(&ship, local_segment, 1));

			local_segment.Set(Vec2F(0.0f), Vec2F(-1.0f, 0.0f).Rotate(-angle).Scale(Vec2F(1.0f, sqrtf(2.0f))));
			AddEntity(Knife(&ship, local_segment, 1));

			ship.GetElemntFromList();

#endif
			bombs_array_mtx.unlock();
			return;

		default:
			std::cout << "WARNING::ShipShoot::Unknow type " << (int)type << std::endl;
			return;
		}
	}

	if (!control_flags.shoot_flags[ship.GetPlayerNumber()] || ships_can_shoot_flags[ship.GetPlayerNumber()] > 0)
	{
		return;
	}
	ships_can_shoot_flags[ship.GetPlayerNumber()] = GAME_DELLAY_BETWEEN_SHOOTS;

	if (ship.ActivateAvailableBuffs())
	{
		return;
	}

	//Magicka and Magicka 2 are the best games I've seen.

	if (ship.SpendBonus(BONUS_LASER))
	{
		if (ship.SpendBonus(BONUS_LOOP))
		{
			if (ship.SpendBonus(BONUS_BOMB))
			{
				if (ship.SpendBonus(BONUS_KNIFE))
				{
					ShipShoot_LaserLoopBombKnife(ship);
					return;
				}
				ShipShoot_LaserLoopBomb(ship);
				return;
			}
			if (ship.SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserLoopKnife(ship);
				return;
			}
			ShipShoot_LaserLoop(ship);
			return;
		}
		if (ship.SpendBonus(BONUS_BOMB))
		{
			if (ship.SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LaserBombKnife(ship);
				return;
			}
			ShipShoot_LaserBomb(ship);
			return;
		}
		if (ship.SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LaserKnife(ship);
			return;
		}
		ShipShoot_Laser(ship);
		return;
	}
	if (ship.SpendBonus(BONUS_LOOP))
	{
		if (ship.SpendBonus(BONUS_BOMB))
		{
			if (ship.SpendBonus(BONUS_KNIFE))
			{
				ShipShoot_LoopBombKnife(ship);
				return;
			}
			ShipShoot_LoopBomb(ship);
			return;
		}
		if (ship.SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_LoopKnife(ship);
			return;
		}
		ShipShoot_Loop(ship);
		return;
	}
	if (ship.SpendBonus(BONUS_BOMB))
	{
		if (ship.SpendBonus(BONUS_KNIFE))
		{
			ShipShoot_BombKnife(ship);
			return;
		}
		ShipShoot_Bomb(ship);
		return;
	}
	if (ship.SpendBonus(BONUS_KNIFE))
	{
		ShipShoot_Knife(ship);
		return;
	}
	ShipShoot_NoBonus(ship);
	//AddEntity(ship.CreateShootingExaust(global_timer));
	return;
}

void Game::ShipShoot_LaserLoopBombKnife(const Ship& ship)
{
	annih_area_gens_array_mtx.lock();
	AddEntity(AnnihAreaGen(&ship, Vec2F(0.0f, 1.0f), SHIP_BUFF_SHIELD));
	AddEntity(AnnihAreaGen(&ship, Vec2F(0.0f, -1.0f), SHIP_BUFF_SHIELD));
	annih_area_gens_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 3;
}

void Game::ShipShoot_LaserLoopBomb(Ship& ship)
{
	ship.AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	lasers_array_mtx.lock();
	Game::AddEntity(Laser(&ship, LASER_DEFAULT_LOCAL_BEAM, LASER_DEFAULT_WIDTH, LASER_DEFAULT_SHOOT_TIME, LASER_PROPERTY_CAN_MULTIPLICATE));
	lasers_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserLoopKnife(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_LASER_KNIFE);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserBombKnife(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__ANNIHILATORS_IN_LASER, GAME_OBJECT_TYPE_LASER_BOMB_KNIFE);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LoopBombKnife(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__ANNIHILATORS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_BOMB_KNIFE);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE * 2;
}

void Game::ShipShoot_LaserLoop(Ship& ship)
{
	ship.AddForceAlongDirection(-SHIP_SHOOT_FORCE * 4.0f);
	lasers_array_mtx.lock();
	Game::AddEntity(Laser(&ship, LASER_DEFAULT_LOCAL_BEAM, LASER_DEFAULT_WIDTH, LASER_DEFAULT_SHOOT_TIME, LASER_PROPERTY_CAN_REFLECT));
	lasers_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LaserBomb(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__BOMBS_IN_LASER, GAME_OBJECT_TYPE_LASER_BOMB);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LoopBomb(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__BOMBS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_BOMB);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LaserKnife(Ship& ship)
{
	const Segment& local_segment = Segment(Vec2F(0.4f, 0.0f), Vec2F(3.0f, 0.0f));
	knifes_array_mtx.lock();
	Game::AddEntity(Knife(&ship, local_segment, SHIP_SUPER_BONUS__LONG_KNIFE_HEALTH));
	knifes_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_LoopKnife(Ship& ship)
{
	ship.CreatingEntities(SHIP_SUPER_BONUS__KNIFES_IN_LOOP / 2, GAME_OBJECT_TYPE_LOOP_KNIFE);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_BombKnife(Ship& ship)
{
	bombs_array_mtx.lock();
	Game::AddEntity(
		Bomb(ship.GetPosition() - ship.GetVelocity() - ship.GetDirection() * (ship.radius * 2.0f + BOMB_DEFAULT_RADIUS),
			ship.GetVelocity() / 1.5f,
			ship.GetTeamNumber(),
			ship.GetTeamNumber(),
			SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
			0.0f,
			0.0f,
			DEFAULT_FORCE_COLLISION_COEFFICIENT,
			0.0f,
			BOMB_DEFAULT_RADIUS,
			BOMB_STATUS_BOOM,
			SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
			SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD));
	bombs_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE + GAME_ADD_DELLAY_COMBO_USE;
}

void Game::ShipShoot_Knife(Ship& ship)
{
	knifes_array_mtx.lock();
	Game::AddEntity(ship.CreateKnife(0));
	Game::AddEntity(ship.CreateKnife(1));
	knifes_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Bomb(Ship& ship)
{
	bombs_array_mtx.lock();
	Game::AddEntity(ship.CreateBomb());
	bombs_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_Loop(Ship& ship)
{
	ship.CreatingEntities(SHIP_BULLETS_IN_LOOP, GAME_OBJECT_TYPE_LOOP_BULLET);
	ships_can_shoot_flags[ship.GetPlayerNumber()] += 2;
}

void Game::ShipShoot_Laser(Ship& ship)
{
	lasers_array_mtx.lock();
	Game::AddEntity(ship.CreateLaser());
	lasers_array_mtx.unlock();
	ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BONUS_USE;
}

void Game::ShipShoot_NoBonus(Ship& ship)
{
	bullets_array_mtx.lock();
	if (ship.IsHaveBuff(SHIP_BUFF_TRIPLE))
	{
		AddEntity(ship.CreateTriple(0));
		AddEntity(ship.CreateTriple(1));
		AddEntity(ship.CreateTriple(2));
		ships_can_shoot_flags[ship.GetPlayerNumber()] += GAME_ADD_DELLAY_BETWEEN_TRIPLES;
	}
	else
	{
		AddEntity(ship.CreateBullet());
	}
	bullets_array_mtx.unlock();

	ships_shooting_flags[ship.GetPlayerNumber()] = true;
}

void Game::CreateBomb(const Laser& creator, const Asteroid& producer)
{
	AddEntity(Bomb(
		producer.GetPosition() + creator.GetSegment().vector.Normalize() * producer.radius * 3.0f,
		producer.GetVelocity() + creator.GetSegment().vector.Normalize() * 0.003f,
		creator.GetPlayerMasterTeamNumber(),
		creator.GetPlayerMasterTeamNumber(),
		SHIP_SUPER_BONUS__ANNIHILATION_AREA_EXIST_PERIOD,
		0.0f,
		0.0f,
		DEFAULT_FORCE_COLLISION_COEFFICIENT,
		BOMB_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
		producer.radius * 3.0f,
		BOMB_STATUS_BOOM));
}

void Game::CreateLoop(const Laser& creator, const Asteroid& producer)
{
	Vec2F loop_center_position = producer.GetPosition();
	GameTypes::entities_count_t bullets_in_loop =
		(producer.GetSize() == ASTEROID_SIZE_BIG) ?
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_BIG :
		(producer.GetSize() == ASTEROID_SIZE_MEDIUM) ?
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_MEDIUM :
		SHIP_SUPER_BONUS__BULLETS_IN_LOOP_BY_ASTEROID_SMALL;
	float bullets_radius =
		(producer.GetSize() == ASTEROID_SIZE_BIG) ?
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_BIG :
		(producer.GetSize() == ASTEROID_SIZE_MEDIUM) ?
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_MEDIUM :
		SHIP_SUPER_BONUS__BULLETS_RADIUS_BY_ASTEROID_SMALL;
	Vec2F laser_vector = creator.GetSegment().vector.Normalize();
	Vec2F radius_vector;

	for (GameTypes::entities_count_t bullet = 0; bullet < bullets_in_loop; bullet++)
	{
		radius_vector = Vec2F(producer.radius, 0.0f).Rotate(
			(float)bullet / (float)bullets_in_loop * (float)M_PI * 2.0f);
		Game::AddEntity(
			Bullet(loop_center_position + radius_vector,
				(radius_vector.Normalize() + laser_vector * 0.75f) * BULLET_DEFAULT_VELOCITY * 2.0f,
				creator.GetPlayerMasterNumber(),
				creator.GetPlayerMasterTeamNumber(),
				false,
				0.0f,
				0.0f,
				DEFAULT_FORCE_COLLISION_COEFFICIENT,
				BULLET_DEFAULT_RESISTANCE_AIR_COEFFICIENT,
				bullets_radius));
	}
}

void Game::UpdateAnnihAreaGensPhase2()
{
	ships_array_mtx.lock();
	annih_area_gens_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : annih_area_gens)
	{
		if (found >= annih_area_gens_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	annih_area_gens_array_mtx.unlock();
	ships_array_mtx.unlock();
}

void Game::UpdateAsteroidsPhase2()
{
	asteroids_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : asteroids)
	{
		if (found >= asteroids_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	asteroids_array_mtx.unlock();
}

void Game::UpdateBombsPhase2()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : bombs)
	{
		if (found >= bombs_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	bombs_array_mtx.unlock();
}

void Game::UpdateBonusesPhase2()
{
	bonuses_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : bonuses)
	{
		if (found >= bonuses_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	bonuses_array_mtx.unlock();
}

void Game::UpdateBulletsPhase2()
{
	bullets_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : bullets)
	{
		if (found >= bullets_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	bullets_array_mtx.unlock();
}

void Game::UpdateDecelerAreasPhase2()
{
	deceler_areas_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : deceler_areas)
	{
		if (found >= deceler_areas_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	deceler_areas_array_mtx.unlock();
}

void Game::UpdateDynamicParticlesPhase2()
{
	dynamic_particles_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : dynamic_particles)
	{
		if (found >= dynamic_particles_count) break;
		if (!entity.exist) continue;
		if (entity.CanRemove(global_timer))
		{
			RemoveEntity(entity);
			continue;
		}

		entity.Activate(global_timer);
		entity.Update(global_timer);
		++found;
	}
	dynamic_particles_array_mtx.unlock();
}

void Game::UpdateGravGensPhase2()
{
	grav_gens_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : grav_gens)
	{
		if (found >= grav_gens_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	grav_gens_array_mtx.unlock();
}

void Game::UpdateKnifesPhase2()
{
	knifes_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : knifes)
	{
		if (found >= knifes_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	knifes_array_mtx.unlock();
}

void Game::UpdateLasersPhase2()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : lasers)
	{
		if (found >= lasers_count) break;
		if (!entity.exist) continue;

		entity.Update();
		if (!entity.IsActive())
		{
			RemoveEntity(entity);
			continue;
		}
		++found;
	}
	lasers_array_mtx.unlock();
}

void Game::UpdateParticlesPhase2()
{
	particles_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : particles)
	{
		if (found >= particles_count) break;
		if (!entity.exist) continue;
		if (entity.CanRemove(global_timer))
		{
			RemoveEntity(entity);
			continue;
		}
		entity.Activate(global_timer);
		entity.Update(global_timer);
		++found;
	}
	particles_array_mtx.unlock();
}

void Game::UpdatePortalsPhase2()
{
	portals_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : portals)
	{
		if (found >= portals_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	portals_array_mtx.unlock();
}

void Game::UpdatePilotsPhase2()
{
	pilots_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : pilots)
	{
		if (found >= pilots_count) break;
		if (!entity.exist) continue;

		entity.Update();
		entity.UpdateMatrix();
		++found;
	}
	pilots_array_mtx.unlock();
}

void Game::UpdateShipsPhase2()
{
	ships_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : ships)
	{
		if (found >= ships_count) break;
		if (!entity.exist) continue;

		entity.AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_SHIP);
		entity.Update();
		entity.UpdateMatrix();
		++found;
	}
	ships_array_mtx.unlock();
}

void Game::UpdateTurretsPhase2()
{
	turrets_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : turrets)
	{
		if (found >= turrets_count) break;
		if (!entity.exist) continue;

		entity.Update();
		++found;
	}
	turrets_array_mtx.unlock();
}

void Game::UpdateMapPhase2()
{
	void* element_pointer;
	map_data_mtx.lock();
	for (auto& element : map.cyrcles_array)
	{
		if (element.exist) element.Update();
	}
	for (auto& element : map.polygons_array)
	{
		if (element.exist) element.Update();
	}
	for (auto& element : map.rectangles_array)
	{
		if (element.exist) element.Update();
	}
	map_data_mtx.unlock();
}

void Game::AnnihAreaGensShoot()
{
	annih_area_gens_array_mtx.lock();
	GameTypes::entities_count_t found = 0;
	for (auto& entity : annih_area_gens)
	{
		if (found >= annih_area_gens_count) break;
		if (!entity.exist) continue;

		AnnihAreaGenShoot(entity);
		++found;
	}
	annih_area_gens_array_mtx.unlock();
}

void Game::BombsChainReaction()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t found1 = 0;
	for (auto& bomb1 : bombs)
	{
		if (found1 >= bombs_count) break;
		if (!bomb1.exist) continue;
		if (bomb1.CanRemove())
		{
			RemoveEntity(bomb1);
			continue;
		}
		++found1;
		if (!bomb1.IsBoom()) continue;

		GameTypes::entities_count_t found2 = 0;
		for (auto& bomb2 : bombs)
		{
			if (found2 >= bombs_count) break;
			if (!bomb1.exist) continue;
			++found2;
			if (
				&bomb1 == &bomb2 ||
				bomb2.IsBoom() ||
				!bomb2.Entity::IsCollision(bomb1))
			{
				continue;
			}

			bomb2.Boom();
			stuning_timer = GAME_BOMB_STUNING_TIME;
		}
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyAsteroids()
{
	bombs_array_mtx.lock();

	bool rescan_needed = false;
	do {
		GameTypes::entities_count_t bombs_found = 0;
		for (auto& bomb : bombs)
		{
			if (bombs_found >= bombs_count) break;
			if (!bomb.exist) continue;
			++bombs_found;
			if (!bomb.IsBoom()) continue;

			asteroids_array_mtx.lock();
			rescan_needed = false;
			GameTypes::entities_count_t asteroids_found = 0;
			for (auto& asteroid : asteroids)
			{
				if (asteroids_found >= asteroids_count) break;
				if (!asteroid.exist) continue;

				if (!asteroid.IsCollision(static_cast<DynamicEntity>(bomb)))
				{
					++asteroids_found;
					continue;
				}

				bonuses_array_mtx.lock();
				dynamic_particles_array_mtx.lock();
				DestroyEntity(bomb, asteroid);
				dynamic_particles_array_mtx.unlock();
				bonuses_array_mtx.unlock();
				rescan_needed = true;
			}
			asteroids_array_mtx.unlock();
		}
	} while(rescan_needed);
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyBonuses()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t bombs_found = 0;
	for (auto& bomb : bombs)
	{
		if (bombs_found >= bombs_count) break;
		if (!bomb.exist) continue;
		++bombs_found;

		if (!bomb.IsBoom()) continue;

		bonuses_array_mtx.lock();
		GameTypes::entities_count_t bonuses_found = 0;
		for (auto& bonus : bonuses)
		{
			if (bonuses_found >= bonuses_count) break;
			if (!bonus.exist) continue;
			if (bonus.IsCollision(static_cast<DynamicEntity>(bomb)))
			{
				DestroyEntity(bonus);
				continue;
			}
			++bonuses_found;
		}
		bonuses_array_mtx.unlock();
	}
	bombs_array_mtx.unlock();
}

void Game::BombsCollisionsWithBullets()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t bombs_found = 0;
	for (auto& bomb : bombs)
	{
		if (bombs_found >= bombs_count) break;
		if (!bomb.exist) continue;
		++bombs_found;

		if (bomb.IsBoom()) continue;

		bullets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& bullet : bullets)
		{
			if (bullets_found >= bullets_count) break;
			if (!bullet.exist) continue;
			if (!bullet.IsCollision(static_cast<DynamicEntity>(bomb)))
			{
				++bullets_found;
				continue;
			}

			if (bomb.IsBoom())
			{
				RemoveEntity(bullet);
				continue;
			}
			if (!bomb.IsActive())
			{
				bomb.Activate();
				RemoveEntity(bullet);
				continue;
			}
			++bullets_found;
		}
		bullets_array_mtx.unlock();

	}
	bombs_array_mtx.unlock();
}

void Game::BombsSpawnedByBulletsAnnihilation()
{
	bombs_array_mtx.lock();
	bullets_array_mtx.lock();
	for (
		GameTypes::entities_count_t bullet_id1 = 0, bullets_found1 = 0; 
		bullets_found1 < bullets_count && bullet_id1 < GAME_BULLETS_MAX_COUNT;
		++bullet_id1)
	{
		auto& bullet1 = bullets[bullet_id1];
		if (!bullet1.exist) continue;

		++bullets_found1;
		if (bullet1.is_ignore != BULLET_IGNORE_NOTHING)
		{
			continue;
		}

		for (
			GameTypes::entities_count_t bullet_id2 = bullet_id1 + 1, bullets_found2 = bullets_found1;
			bullets_found2 < bullets_count && bullet_id2 < GAME_BULLETS_MAX_COUNT;
			++bullet_id2)
		{
			auto& bullet2 = bullets[bullet_id2];
			if (!bullet2.exist) continue;
			
			if (!bullet1.IsCollision(static_cast<DynamicEntity>(bullet2)))
			{
				++bullets_found2;
				continue;
			}
			AddEntity(Bomb(bullet1, bullet2));
			RemoveEntity(bullet1);
			RemoveEntity(bullet2);
			--bullets_found1;
			break;
		}
		if (bullet1.exist)	// bullet 1 sleel exist after all checks
		{
			++bullets_found1;
		}
	}
	bullets_array_mtx.unlock();
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyKnifes()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t bombs_found = 0;
	for (auto& bomb : bombs)
	{
		if (bombs_found >= bombs_count) break;
		if (!bomb.exist) continue;
		++bombs_found;
		if (!bomb.IsBoom()) continue;
		
		GameTypes::entities_count_t knifes_found = 0;
		knifes_array_mtx.lock();
		for (auto& knife : knifes)
		{
			if (knifes_found >= knifes_count) break;
			if (!bomb.exist) continue;
			if (bomb.IsCollision(knife.GetSegment()))
			{
				DestroyEntity(bomb, knife);
				continue;
			}
			++knifes_found;
		}
		knifes_array_mtx.unlock();
	}
	bombs_array_mtx.unlock();
}

void Game::BombsDestroyTurrets()
{
	bombs_array_mtx.lock();
	GameTypes::entities_count_t bombs_found = 0;
	for (auto& bomb : bombs)
	{
		if (bombs_found >= bombs_count) break;
		if (!bomb.exist) continue;
		++bombs_found;
		if (!bomb.IsBoom()) continue;
		GameTypes::entities_count_t turrets_found = 0;
		turrets_array_mtx.lock();
		for (auto& turret : turrets)
		{
			if (turrets_found >= turrets_count) break;
			if (!bomb.exist) continue;
			if (turret.IsCollision(bomb))
			{
				DestroyEntity(bomb, turret);
				continue;
			}
			++turrets_found;
		}
		turrets_array_mtx.unlock();
	}
	bombs_array_mtx.unlock();
}

void Game::BulletsDestroyAsteroids()
{
	bullets_array_mtx.lock();
	GameTypes::entities_count_t bullets_found = 0;
	for (auto& bullet : bullets)
	{
		if (bullets_found >= bullets_count) break;
		if (!bullet.exist) continue;

		asteroids_array_mtx.lock();
		GameTypes::entities_count_t asteroids_found = 0;
		for (auto& asteroid : asteroids)
		{
			if (asteroids_found >= asteroids_count) break;
			if (!asteroid.exist) continue;
			if (!asteroid.IsCollision(static_cast<DynamicEntity>(bullet)))
			{
				asteroids_found;
				continue;
			}

			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			DestroyEntity(bullet, asteroid);
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();
			RemoveEntity(bullet);
		}
		asteroids_array_mtx.unlock();
		if (bullet.exist)
		{
			++bullets_found;
		}
	}
	bullets_array_mtx.unlock();
}

void Game::BulletsDestroedByMap()
{
	bullets_array_mtx.lock();
	GameTypes::entities_count_t bullets_found = 0;
	for (auto& bullet : bullets)
	{
		if (bullets_found >= bullets_count) break;
		if (!bullet.exist) continue;
		map_data_mtx.lock();
		if (!bullet.Collision(map))
		{
			++bullets_found;
			map_data_mtx.unlock();
			continue;
		}
		map_data_mtx.unlock();
		RemoveEntity(bullet);

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
	knifes_array_mtx.lock();
	GameTypes::entities_count_t knifes_found = 0;
	for (auto& knife : knifes)
	{
		if (knifes_found >= knifes_count) break;
		if (!knife.exist) continue;

		asteroids_array_mtx.lock();
		GameTypes::entities_count_t asteroids_found = 0;
		for (auto& asteroid : asteroids)
		{
			if (asteroids_found >= asteroids_count) break;
			if (!asteroid.exist) continue;

			const Segment& temp__segment = knife.GetSegment();
			if (!asteroid.IsCollision(temp__segment))
			{
				++asteroids_found;
				continue;
			}

			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			DestroyEntity(knife, asteroid);
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();

			if (knife.LoseHealth()) continue;

			RemoveEntity(knife);
			break;
		}
		asteroids_array_mtx.unlock();

		if (knife.exist)
		{
			++knifes_found;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyBullets()
{
	knifes_array_mtx.lock();
	GameTypes::entities_count_t knifes_found = 0;
	for (auto& knife : knifes)
	{
		if (knifes_found >= knifes_count) break;
		if (!knife.exist) continue;

		bullets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& bullet : bullets)
		{
			if (bullets_found >= bullets_count) break;
			if (!bullet.exist) continue;

			if (!bullet.IsCollision(knife.GetSegment()))
			{
				++bullets_found;
				continue;
			}

			DestroyEntity(bullet, knife);
			RemoveEntity(bullet);
			break;
		}
		if (knife.exist)
		{
			++knifes_found;
		}
		bullets_array_mtx.unlock();
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyMap()
{
	knifes_array_mtx.lock();
	GameTypes::entities_count_t knifes_found = 0;
	for (auto& knife : knifes)
	{
		if (knifes_found >= knifes_count) break;
		if (!knife.exist) continue;

		map_data_mtx.lock();
		knife.Collision(map);
		map_data_mtx.unlock();
		if (knife.exist)
		{
			++knifes_found;
		}
	}
	knifes_array_mtx.unlock();
}

void Game::KnifesDestroyTurrets()
{
	knifes_array_mtx.lock();
	GameTypes::entities_count_t knifes_found = 0;
	for (auto& knife : knifes)
	{
		if (knifes_found >= knifes_count) break;
		if (!knife.exist) continue;

		bullets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& turret : turrets)
		{
			if (bullets_found >= bullets_count) break;
			if (!turret.exist) continue;

			if (!turret.IsCollision(knife.GetSegment()))
			{
				++bullets_found;
				continue;
			}

			DestroyEntity(knife, turret);
			RemoveEntity(knife);
			break;
		}
		if (knife.exist)
		{
			++knifes_found;
		}
		bullets_array_mtx.unlock();
	}
	knifes_array_mtx.unlock();
}

void Game::LasersDestroyAsteroids()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;

		asteroids_array_mtx.lock();
		GameTypes::entities_count_t asteroids_found = 0;
		for (auto& asteroid : asteroids)
		{
			if (asteroids_found >= asteroids_count) break;
			if (!asteroid.exist) continue;

			const Segment& temp__segment = laser.GetSegment();
			if (!asteroid.IsCollision(temp__segment))
			{
				++asteroids_found;
				continue;
			}
			
			if (laser.GetProperty(LASER_PROPERTY_CAN_MULTIPLICATE))
			{
				for (int angle = -3; angle <= 3; ++angle)
				{
					if (!angle) continue;

					const Vec2F& direction = temp__segment.vector.Rotate((static_cast<float>(angle) / 4.0f * M_PI));
					AddEntity(Laser(
						laser.GetHostP(),
						Beam(asteroid.GetPosition(), direction),
						LASER_DEFAULT_WIDTH,
						laser.GetShootTime(),
						LASER_PROPERTY_FREE_FROM_HOST));
				}
			}

			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			DestroyEntity(laser, asteroid);
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();
		}
		asteroids_array_mtx.unlock();

		if (laser.exist)
		{
			++lasers_found;
		}
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyBonuses()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		bonuses_array_mtx.lock();
		GameTypes::entities_count_t bonuses_found = 0;
		for (auto& bonus : bonuses)
		{
			if (bonuses_found >= bullets_count) break;
			if (!bonus.exist) continue;

			if (!bonus.IsCollision(laser.GetSegment()))
			{
				++bonuses_found;
				continue;
			}

			DestroyEntity(laser, bonus);
		}
		bonuses_array_mtx.unlock();
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDetonateBombs()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		bombs_array_mtx.lock();
		GameTypes::entities_count_t bombs_found = 0;
		for (auto& bomb : bombs)
		{
			if (bombs_found >= bombs_count) break;
			if (!bomb.exist) continue;
			++bombs_found;
			if (bomb.IsBoom() || !bomb.IsCollision(laser.GetSegment())) continue;
			bomb.Boom();
			stuning_timer = GAME_BOMB_STUNING_TIME;
		}
		bombs_array_mtx.unlock();
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyBullets()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		bullets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& bullet : bullets)
		{
			if (bullets_found >= bullets_count) break;
			if (!bullet.exist) continue;

			if (!bullet.IsCollision(laser.GetSegment()))
			{
				++bullets_found;
				continue;
			}

			DestroyEntity(laser, bullet);
		}
		bullets_array_mtx.unlock();
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyKnifes()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		knifes_array_mtx.lock();
		GameTypes::entities_count_t knifes_found = 0;
		for (auto& knife : knifes)
		{
			if (knifes_found >= knifes_count) break;
			if (!knife.exist) continue;

			if (!knife.IsCollision(laser.GetSegment()))
			{
				++knifes_found;
				continue;
			}

			DestroyEntity(laser, knife);
		}
		knifes_array_mtx.unlock();
	}
	lasers_array_mtx.unlock();
}

void Game::LasersCollisionWthMap()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		map_data_mtx.lock();
		Vec2F pos, vec;
		if (!laser.Collision(map, pos, vec))
		{
			map_data_mtx.unlock();
			continue;
		}
		map_data_mtx.unlock();

		if (laser.reflections_count > 3 || !(laser.properties & LASER_PROPERTY_CAN_REFLECT)) continue;

		const Vec2F& laser_vector = laser.GetSegment().vector;
		AddEntity(Laser(
			laser.GetHostP(),
			Beam(pos - laser_vector.Length(laser.width * 2.0f), laser_vector - vec.Project(laser_vector) * 2.0f),
			laser.width,
			4,
			LASER_PROPERTY_CAN_REFLECT | LASER_PROPERTY_FREE_FROM_HOST,
			laser.reflections_count + 1));
		// If The laser was added befor pointer then the cycle will be doing 256 iterations.
	}
	lasers_array_mtx.unlock();
}

void Game::LasersDestroyTurrets()
{
	lasers_array_mtx.lock();
	GameTypes::entities_count_t lasers_found = 0;
	for (auto& laser : lasers)
	{
		if (lasers_found >= lasers_count) break;
		if (!laser.exist) continue;
		++lasers_found;

		turrets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& turret : turrets)
		{
			if (bullets_found >= bullets_count) break;
			if (!turret.exist) continue;

			if (!turret.IsCollision(laser.GetSegment()))
			{
				++bullets_found;
				continue;
			}

			DestroyEntity(laser, turret);
		}
		turrets_array_mtx.unlock();
	}
	lasers_array_mtx.unlock();
}

void Game::PortalsCreateParticles()
{
	portals_array_mtx.lock();
	GameTypes::map_elements_count_t portals_found = 0;
	for (auto& portal : portals)
	{
		if (portals_found >= portals_count) break;
		if (!portal.exist) continue;
		++portals_found;

		particles_array_mtx.lock();
		AddEntity(portal.CreateParticlesTP(global_timer, PORTAL_DEFAULT_RADIUS * 2.0f));
		particles_array_mtx.unlock();
	}
	portals_array_mtx.unlock();
}

template<typename Entity_T>
void Game::PortalsTPEntityes(std::vector<Entity_T>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx)
{
	portals_array_mtx.lock();
	for (auto& portal : portals)
	{
		if (!portal.exist) continue;

		entity_array_mtx.lock();
		GameTypes::entities_count_t entityes_found = 0;
		for (auto& entity : entityes)
		{
			if (entityes_found >= exist_entityes_count) break;
			if (!entity.exist) continue;
			++entityes_found;
			if (!portal.IsCollision(entity)) continue;

			TeleportEntity(portal, entity);
		}
		entity_array_mtx.unlock();
	}
	portals_array_mtx.unlock();
}
template void Game::PortalsTPEntityes(std::vector<Asteroid>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);
template void Game::PortalsTPEntityes(std::vector<Bonus>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);
template void Game::PortalsTPEntityes(std::vector<Bullet>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);
template void Game::PortalsTPEntityes(std::vector<DynamicParticle>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);
template void Game::PortalsTPEntityes(std::vector<Pilot>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);
template void Game::PortalsTPEntityes(std::vector<Ship>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);

template<>
void Game::PortalsTPEntityes<Bomb>(std::vector<Bomb>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx)
{
	for (auto& portal : portals)
	{
		if (!portal.exist) continue;

		GameTypes::entities_count_t entityes_found = 0;
		for (auto& entity : entityes)
		{
			if (entityes_found >= exist_entityes_count) break;
			if (!entity.exist) continue;
			++entityes_found;
			if (entity.IsBoom() || !portal.IsCollision(entity)) continue;

			TeleportEntity(portal, entity);
		}
	}
}
template void Game::PortalsTPEntityes(std::vector<Bomb>& entityes, const GameTypes::entities_count_t exist_entityes_count, std::shared_mutex& entity_array_mtx);

void Game::PilotsCheckInput()
{
	GameTypes::players_count_t pilot_player_number;

	pilots_array_mtx.lock();
	input_values_mtx.lock();
	for (auto& pilot : pilots)
	{
		if (!pilot.exist) continue;
		pilot_player_number = pilot.GetPlayerNumber();
		if (control_flags.shoot_flags[pilot_player_number])
		{
			pilot.AddForceAlongDirection(GAME_KNOCKBACK_FORCE_OF_THE_PILOT);
		}
		if (!control_flags.rotate_flags[pilot_player_number])
		{
			pilot.SetAngularVelocity(0.0f);
			continue;
		}
		pilot.SetAngularVelocity(rotation_inverse ? -GAME_SHIP_ANGULAR_VELOCITY : GAME_SHIP_ANGULAR_VELOCITY);

	}
	input_values_mtx.unlock();
	pilots_array_mtx.unlock();
}

template <>
void Game::PilotsKilledBy<Bomb>(std::vector<Bomb>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx)
{
	pilots_array_mtx.lock();
	for (auto& pilot : pilots)
	{
		if (!pilot.exist) continue;

		GameTypes::entities_count_t bombs_found = 0;
		killers_array_mtx.lock();
		for (auto& bomb : bombs)
		{
			if (bombs_found >= killers_count) break;
			if (!bomb.exist) continue;
			++bombs_found;

			if (!bomb.IsBoom() || !pilot.IsCollision(static_cast<DynamicEntity>(bomb))) continue;

			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(bomb, pilot);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			break;
		}
		killers_array_mtx.unlock();
	}
	pilots_array_mtx.unlock();
}
template void Game::PilotsKilledBy(std::vector<Bomb>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx);

template <>
void Game::PilotsKilledBy<Bullet>(std::vector<Bullet>& bullets, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx)
{
	pilots_array_mtx.lock();
	for (auto& pilot : pilots)
	{
		if (!pilot.exist) continue;
		killers_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& bullet : bullets)
		{
			if (bullets_found >= killers_count) break;
			if (!bullet.exist) continue;

			if (bullet.is_ignore != BULLET_IGNORE_NOTHING || !pilot.IsCollision(static_cast<DynamicEntity>(bullet)))
			{
				++bullets_found;
				continue;
			}
			if (!(game_rules & GAME_RULE_FRIENDLY_FIRE) && bullet.CreatedByTeam(pilot))
			{
				RemoveEntity(bullet);
				continue;
			}

			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(bullet, pilot);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			break;
		}
		killers_array_mtx.unlock();
	}
	pilots_array_mtx.unlock();
}

template <typename Entity_T>
void Game::PilotsKilledBy(std::vector<Entity_T>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx)
{
	pilots_array_mtx.lock();
	for (auto& pilot : pilots)
	{
		if (!pilot.exist) continue;
		killers_array_mtx.lock();
		GameTypes::entities_count_t knifes_found = 0;
		for (auto& killer : killers)
		{
			if (knifes_found >= killers_count) break;
			if (!killer.exist) continue;
			++knifes_found;

			if (
				!(game_rules & GAME_RULE_FRIENDLY_FIRE) && killer.IsCreatedByTeam(pilot) ||
				!pilot.IsCollision(killer.GetSegment())) continue;

			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(killer, pilot);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			break;
		}
		killers_array_mtx.unlock();
	}
	pilots_array_mtx.unlock();
}
template void Game::PilotsKilledBy(std::vector<Laser>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx);
template void Game::PilotsKilledBy(std::vector<Knife>& killers, GameTypes::entities_count_t killers_count, std::shared_mutex& killers_array_mtx);

void Game::PilotsRespawnAuto()
{
	ships_array_mtx.lock();
	pilots_array_mtx.lock();
	for (auto& pilot : pilots)
	{
		if (!pilot.exist || !pilot.CanRespawn()) continue;
		ships_can_shoot_flags[pilot.GetPlayerNumber()] = SHIP_DEFAULT_UNBRAKABLE_PERIOD;
		AddEntity(pilot.Respawn());
		RemoveEntity(pilot);
	}
	pilots_array_mtx.unlock();
	ships_array_mtx.unlock();
}

void Game::ShipsCreateExaust()
{
	if ((global_timer % (PARTICLE_PERIOD_EXAUST_ENGINE / 10))) return;

	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;

		dynamic_particles_array_mtx.lock();
		AddEntity(ship.CreateEnginExaust(global_timer));
		dynamic_particles_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsInfluenceToBonuses()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;
		bonuses_array_mtx.lock();
		GameTypes::entities_count_t bonuses_found = 0;
		for (auto& bonus : bonuses)
		{
			if (bonuses_found >= bonuses_count) break;
			if (!bonus.exist) continue;
			
			if (bonus.GetDistance(ship) > ship.radius * GAME_SHIP_INFLUENCE_RADIUS_COEFFISIENT)
			{
				++bonuses_found;
				continue;
			}
			bonus.AddGravityForce(GAME_SHIP_GRAVITATION_FORCE, ship.GetPosition());

			if (!bonus.IsCollision(static_cast<DynamicEntity>(ship)))
			{
				++bonuses_found;
				continue;
			}
			if (bonus.bonus_inventory & (BONUS_RULE_REVERSE * BONUS_CELL))
			{
				rotation_inverse ^= 1;
				rotate_inverse_chage_tic = global_timer;
			}
			if (bonus.bonus_inventory & (BONUS_BUFF_STREAM * BONUS_CELL))
			{
				ship.IncrementSizeOfMagazine();
			}
			bonus.bonus_inventory &= BONUS_ALL - ((BONUS_RULE_REVERSE * BONUS_CELL) | (BONUS_BUFF_STREAM * BONUS_CELL));
			ship.TakeBonus(&bonus, game_rules & GAME_RULE_TRIPLE_BONUSES);
			RemoveEntity(bonus);

		}
		bonuses_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsCheckInput()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;
		input_values_mtx.lock();
		
		const GameTypes::players_count_t& ship_number = ship.GetPlayerNumber();
		
		ship.SetAngularVelocity(!control_flags.rotate_flags[ship_number] ? 0.0f : rotation_inverse ? -GAME_SHIP_ANGULAR_VELOCITY : GAME_SHIP_ANGULAR_VELOCITY);
		
		if (ships_can_shoot_flags[ship_number] > 0) --ships_can_shoot_flags[ship_number];
		if (control_flags.burnout_flags[ship_number])
		{
			control_flags.burnout_flags[ship_number] = false;
			ship.Burnout(BULLET_DEFAULT_VELOCITY, !rotation_inverse);
		}
		input_values_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsShoot()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;
		input_values_mtx.lock();
		ShipShoot(ship);
		input_values_mtx.unlock();
	}
	ships_array_mtx.unlock();

	AnnihAreaGensShoot();
	ClearShipsShootingFlags();
}

void Game::ShipsRespawnOrDestroyPilots()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;
		pilots_array_mtx.lock();
		for (auto& pilot : pilots)
		{
			if (!pilot.exist) continue;
			
			const bool& same_team = pilot.IsSameTeams(ship);
			const bool& collision = pilot.IsCollision(static_cast<DynamicEntity>(ship));
			if (game_rules & GAME_RULE_FRIEDNLY_SHEEP_CAN_RESTORE && same_team && collision)
			{
				SpawnEntity(ship, pilot);
				continue;
			}
			
			if (!(game_rules & GAME_RULE_FRIENDLY_FIRE) && same_team || !collision) continue;
			
			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(ship, pilot);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
		}
		pilots_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBombsOrActivateBombs()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;

		pilots_array_mtx.lock();
		annih_area_gens_array_mtx.lock();
		bombs_array_mtx.lock();
		GameTypes::players_count_t bombs_found = 0;
		for (auto& bomb : bombs)
		{
			if (bombs_found >= bombs_count) break;
			if (!bomb.exist) continue;
			++bombs_found;

			if (bomb.IsBoom() && ship.IsCollision(static_cast<DynamicEntity>(bomb)))
			{
				bonuses_array_mtx.lock();
				dynamic_particles_array_mtx.lock();
				log_data_mtx.lock();
				DestroyEntity(bomb, ship);
				log_data_mtx.unlock();
				dynamic_particles_array_mtx.unlock();
				bonuses_array_mtx.unlock();
				break;
			}
			if (
				!bomb.IsActive() &&
				!bomb.IsCreatedByTeam(&ship) &&
				ship.GetDistance(bomb) < bomb.radius * BOMB_ACTIVATION_RADIUS_COEF)
			{
				bomb.Activate();
			}
		}
		bombs_array_mtx.unlock();
		annih_area_gens_array_mtx.unlock();
		pilots_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByBullets()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;
		
		pilots_array_mtx.lock();
		annih_area_gens_array_mtx.lock();
		bullets_array_mtx.lock();
		GameTypes::entities_count_t bullets_found = 0;
		for (auto& bullet : bullets)
		{
			if (bullets_found >= bullets_count) break;
			if (!bullet.exist) continue;
			
			if ((bullet.is_ignore & BULLET_IGNORE_MUSTER) &&
				bullet.CreatedBy(ship) &&
				!ship.IsColectEntity(bullet))
			{
				bool collect = ship.IsColectEntity(bullet);
				collect = false;

				bullet.is_ignore = BULLET_IGNORE_NOTHING;
			}
			if (ship.IsUnbrakable() ||
				bullet.is_ignore != BULLET_IGNORE_NOTHING ||
				!ship.IsCollision(static_cast<DynamicEntity>(bullet)))
			{
				++bullets_found;
				continue;
			}

			if (
				!(game_rules & GAME_RULE_FRIENDLY_FIRE) &&
				bullet.CreatedByTeam(ship) &&
				!bullet.CreatedBy(ship))
			{
				RemoveEntity(bullet);
				continue;
			}
			if (ship.IsHaveBuff(SHIP_BUFF_SHIELD))
			{
				ship.SetUnbrakablePeriod(SHIP_DEFAULT_UNBRAKABLE_PERIOD);
				ship.SpendBuffNoCheck(SHIP_BUFF_SHIELD);
				RemoveEntity(bullet); continue;
			}
			ship.AddVelocity(ship.GetVelocity() / 4.0f);
			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(bullet, ship);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();
			RemoveEntity(bullet);
			break;
		}
		bullets_array_mtx.unlock();
		annih_area_gens_array_mtx.unlock();
		pilots_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByKnifes()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;

		pilots_array_mtx.lock();
		annih_area_gens_array_mtx.lock();
		knifes_array_mtx.lock();
		GameTypes::entities_count_t knifes_found = 0;
		for (auto& knife : knifes)
		{
			if (knifes_found >= knifes_count) break;
			if (!knife.exist) continue;

			if (
				!(game_rules & GAME_RULE_FRIENDLY_FIRE) && knife.IsCreatedByTeam(ship) ||
				ship.IsUnbrakable() ||
				knife.IsCreatedBy(ship) ||
				!ship.IsCollision(knife.GetSegment()))
			{
				++knifes_found;
				continue;
			}
			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(knife, ship);
			RemoveEntity(knife);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();
			break;
		}
		knifes_array_mtx.unlock();
		annih_area_gens_array_mtx.unlock();
		pilots_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::ShipsDestroedByLasers()
{
	ships_array_mtx.lock();
	for (auto& ship : ships)
	{
		if (!ship.exist) continue;

		pilots_array_mtx.lock();
		annih_area_gens_array_mtx.lock();
		lasers_array_mtx.lock();
		GameTypes::entities_count_t lasers_found = 0;
		for (auto& laser : lasers)
		{
			if (lasers_found >= lasers_count) break;
			if (!laser.exist) continue;

			if (
				!(game_rules & GAME_RULE_FRIENDLY_FIRE) && laser.IsCreatedByTeam(ship) && !laser.IsCreatedBy(ship) ||
				ship.IsUnbrakable() ||
				!laser.GetProperty(LASER_PROPERTY_FREE_FROM_HOST) && laser.IsCreatedBy(ship) ||
				!ship.IsCollision(laser.GetSegment()))
			{
				++lasers_found;
				continue;
			}
			bonuses_array_mtx.lock();
			dynamic_particles_array_mtx.lock();
			log_data_mtx.lock();
			DestroyEntity(laser, ship);
			log_data_mtx.unlock();
			dynamic_particles_array_mtx.unlock();
			bonuses_array_mtx.unlock();
			break;
		}
		lasers_array_mtx.unlock();
		annih_area_gens_array_mtx.unlock();
		pilots_array_mtx.unlock();
	}
	ships_array_mtx.unlock();
}

void Game::TurretsShoot()
{
	turrets_array_mtx.lock();
	GameTypes::entities_count_t turrets_found = 0;
	for (auto& turret : turrets)
	{
		if (turrets_found >= turrets_count) break;
		if (!turret.exist) continue;
		++turrets_found;
		if (!turret.CanShoot(global_timer)) continue;

		bullets_array_mtx.lock();
		AddEntity(turret.Shoot());
		bullets_array_mtx.unlock();

	}
	turrets_array_mtx.unlock();
}
