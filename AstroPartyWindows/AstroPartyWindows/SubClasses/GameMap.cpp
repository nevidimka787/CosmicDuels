#include "Game.h"

#define M_PI	3.14159265358979323846	// pi
#define M_PI_2	1.5707796326794897		// pi/2
#define M_PI_4  0.785398163397448309616	// pi/4

#define ALL_MAPS__SPAWN_DELTA_ANGLE	0.05f

#pragma warning(disable : 6385)
#pragma warning(disable : 26451)

void Game::Event0()
{
#define EVENT0__CENTER_POSITION	0.0f
#define EVENT0__BONUS_POSITION	1.0f
#define EVENT0__BAD_AREA_RADIUS	1.5f
#define EVENT0__RM_ARAE_RADIUS	2.0f
	if (!(global_timer % 100))
	{
		Ship* temp__ship_p = ships;
		bool can_spawn = true;
		ships_array_mtx.lock();
		for (GameTypes::players_count_t found_ships = 0; found_ships < ships_count; temp__ship_p++)
		{
			if (temp__ship_p->exist)
			{
				if (can_spawn && temp__ship_p->GetPosition().Length() < EVENT0__BAD_AREA_RADIUS)
				{
					can_spawn = false;
					break;
				}
				found_ships++;
			}
		}
		ships_array_mtx.unlock();

		if (can_spawn)
		{
			bonuses_array_mtx.lock();
			if (!bonuses[0].exist)
			{
				bonuses_count++;
				bonuses[0] = Bonus(Vec2F(EVENT0__CENTER_POSITION, EVENT0__CENTER_POSITION + EVENT0__BONUS_POSITION), Vec2F(), BONUS_LOOP);//up
			}
			if (!bonuses[1].exist)
			{
				bonuses_count++;
				bonuses[1] = Bonus(Vec2F(EVENT0__CENTER_POSITION + EVENT0__BONUS_POSITION, EVENT0__CENTER_POSITION), Vec2F(), BONUS_LASER);//right
			}
			if (!bonuses[2].exist)
			{
				bonuses_count++;
				bonuses[2] = Bonus(Vec2F(EVENT0__CENTER_POSITION, EVENT0__CENTER_POSITION - EVENT0__BONUS_POSITION), Vec2F(), BONUS_BOMB);//down
			}
			if (!bonuses[3].exist)
			{
				bonuses_count++;
				bonuses[3] = Bonus(Vec2F(EVENT0__CENTER_POSITION - EVENT0__BONUS_POSITION, EVENT0__CENTER_POSITION), Vec2F(), BONUS_KNIFE);//left
			}
			bonuses_array_mtx.unlock();
		}
	}

	if (!((global_timer + 50) % 100))
	{
		if (asteroids_count == 0)
		{
			AddEntity(Asteroid(Vec2F(EVENT0__CENTER_POSITION), Vec2F(), GenerateRandomInventory(BONUS_BUFF_STREAM, 3, 3, 1, 1) | GenerateRandomInventory(BONUS_LOOP | BONUS_LASER | BONUS_BOMB | BONUS_KNIFE, 1, 1, 1, 4), ASTEROID_MAX_SIZE));
		}
	}
}

void Game::Event1()
{
	turels_array_mtx.lock();
	if (turels[0].exist)
	{
		turels[0].Rotate(0.01f);
	}
	turels_array_mtx.unlock();
	asteroids_array_mtx.lock();
#define EVENT1_SQUARE_SIZE 0.7f
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !((global_timer + 1) % 1000) && asteroids_count < 2)
	{
		AddEntity(Asteroid(
			Vec2F(-EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
			ASTEROID_SIZE_BIG));
	}
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !((global_timer + 2) % 1000) && asteroids_count < 3)
	{
		AddEntity(Asteroid(
			Vec2F(-EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
			ASTEROID_SIZE_BIG));
	}
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !((global_timer + 3) % 1000) && asteroids_count < 4)
	{
		AddEntity(Asteroid(
			Vec2F(EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
			ASTEROID_SIZE_BIG));
	}
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !((global_timer + 4) % 1000) && asteroids_count < 5)
	{
		AddEntity(Asteroid(
			Vec2F(EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
			ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event2()
{
#define EVENT2_SQUARE_SIZE 1.5f
	if (object_pull_array[GAME_OBJECT_ASTEROID])
	{
		asteroids_array_mtx.lock();
		if (!((global_timer + 1) % 1000) && asteroids_count < 5)
		{
			AddEntity(Asteroid(
				Vec2F(-EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
				ASTEROID_SIZE_BIG));
		}
		if (!((global_timer + 2) % 1000) && asteroids_count < 6)
		{
			AddEntity(Asteroid(
				Vec2F(-EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
				ASTEROID_SIZE_BIG));
		}
		if (!((global_timer + 3) % 1000) && asteroids_count < 7)
		{
			AddEntity(Asteroid(
				Vec2F(EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
				ASTEROID_SIZE_BIG));
		}
		if (!((global_timer + 4) % 1000) && asteroids_count < 8)
		{
			AddEntity(Asteroid(
				Vec2F(EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
				ASTEROID_SIZE_BIG));
		}
		asteroids_array_mtx.unlock();
	}
}

void Game::Event3()
{
	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 500) && asteroids_count < 4)
	{
		asteroids_array_mtx.unlock();
		Vec2F asteroid_velocityes[4];
#define EVENT3_ASTEROID_VELOCIRY		0.015f
#define EVENT3_ASTEROID_SPAWN_RADIUS	0.2f
		asteroid_velocityes[0].Set(EVENT3_ASTEROID_VELOCIRY, EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[1].Set(EVENT3_ASTEROID_VELOCIRY, -EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[2].Set(-EVENT3_ASTEROID_VELOCIRY, -EVENT3_ASTEROID_VELOCIRY);
		asteroid_velocityes[3].Set(-EVENT3_ASTEROID_VELOCIRY, EVENT3_ASTEROID_VELOCIRY);
		uint8_t dir = rand() % 4;
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(
			-asteroid_velocityes[dir].Normalize() * EVENT3_ASTEROID_SPAWN_RADIUS,
			asteroid_velocityes[dir],
			GenerateRandomInventory(BONUS_BONUS | BONUS_BUFF_STREAM, 1, 1, 1, 2),
			ASTEROID_SIZE_MEDIUM));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event4()
{
#define EVENT4__CENTER_POSITION	6.0f

	asteroids_array_mtx.lock();
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count < 2)
	{
		AddEntity(Asteroid(
				Vec2F(EVENT4__CENTER_POSITION, EVENT4__CENTER_POSITION),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS | BONUS_BUFF_STREAM, 2, 3, 3, 4) | GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 1, 1, 1, 1),
				ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event5()
{
#define EVENT5__CENTER_POSITION	6.0f
#define EVENT5__SQUARE_SIZE		1.0f
#define EVENT5__UP_RECTANGLE	1u
#define EVENT5__RIGHT_RECTANGLE	2u
#define EVENT5__DONW_RECTANGLE	3u
#define EVENT5__LEFT_RECTANGLE	4u

#define EVENT5__RECTANGLE_SPEED	(0.005f * EVENT5__SQUARE_SIZE)

#define EVENT5__PERIOD	1000lu
#define EVENT5_PHASE	200lu

	map_data_mtx.lock();

	if (rotation_inverse)
	{
		Map::Rectangle* rectangle_p = map.RectanglePointer(EVENT5__UP_RECTANGLE);
		if (rectangle_p->Position().x < EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE)
		{
			//up rectangle
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__RIGHT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__DONW_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__LEFT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
		}

		rectangle_p->Move(Vec2F(-EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.RectanglePointer(EVENT5__RIGHT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, EVENT5__RECTANGLE_SPEED));
		rectangle_p = map.RectanglePointer(EVENT5__DONW_RECTANGLE);
		rectangle_p->Move(Vec2F(EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.RectanglePointer(EVENT5__LEFT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, -EVENT5__RECTANGLE_SPEED));
	}
	else
	{
		Map::Rectangle* rectangle_p = map.RectanglePointer(EVENT5__UP_RECTANGLE);

		if (rectangle_p->Position().x > EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE)
		{
			//up rctangle
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__RIGHT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__DONW_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION + EVENT5__SQUARE_SIZE, EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
			rectangle_p = map.RectanglePointer(EVENT5__LEFT_RECTANGLE);
			rectangle_p->SetPosition(Vec2F(EVENT5__CENTER_POSITION - EVENT5__SQUARE_SIZE));
		}

		rectangle_p->Move(Vec2F(EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.RectanglePointer(EVENT5__RIGHT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, -EVENT5__RECTANGLE_SPEED));
		rectangle_p = map.RectanglePointer(EVENT5__DONW_RECTANGLE);
		rectangle_p->Move(Vec2F(-EVENT5__RECTANGLE_SPEED, 0.0f));
		rectangle_p = map.RectanglePointer(EVENT5__LEFT_RECTANGLE);
		rectangle_p->Move(Vec2F(0.0f, EVENT5__RECTANGLE_SPEED));
	}

	map_data_mtx.unlock();

	asteroids_array_mtx.lock();
	if (!(global_timer % EVENT5__PERIOD) && asteroids_count == 0)
	{
		AddEntity(Asteroid(
			Vec2F(EVENT4__CENTER_POSITION, EVENT4__CENTER_POSITION),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS | BONUS_BUFF_STREAM, 2, 3, 3, 4) |
			GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 1, 1, 1, 1),
			ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event6()
{
	asteroids_array_mtx.lock();
	if (!(global_timer % EVENT5__PERIOD) && asteroids_count == 0)
	{
		AddEntity(Asteroid(
			Vec2F(EVENT5__CENTER_POSITION, EVENT5__CENTER_POSITION),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS | BONUS_RULE_REVERSE, 2, 3, 4, 5) |
			GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 0, 1, 0, 1) |
			GenerateRandomInventory(BONUS_BUFF_STREAM, 1, 3, 1, 1),
			ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event7()
{
	asteroids_array_mtx.lock();
	if (!(global_timer % EVENT5__PERIOD) && asteroids_count == 0)
	{
		AddEntity(Asteroid(
			Vec2F(EVENT5__CENTER_POSITION, EVENT5__CENTER_POSITION),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS | BONUS_RULE_REVERSE, 2, 3, 4, 5) |
			GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 0, 1, 0, 1) |
			GenerateRandomInventory(BONUS_BUFF_STREAM, 1, 3, 1, 1),
			ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

void Game::Event8()
{
#define EVENT8__POLYGONS_COUNT	4u
#define EVENT8__PERIOD			1000u
#define EVENT8__MAX_SIZE		1.5f
#define EVENT8__MIN_SIZE		0.5f

	Map::Polygon* temp__polygon_p;
	float temp__angle = (float)(global_timer % EVENT8__PERIOD) / (float)EVENT8__PERIOD * (float)M_PI * 2.0f;
	float temp__size = (sinf(temp__angle * 4.0f) / 2.0f + 0.5f) * (EVENT8__MAX_SIZE - EVENT8__MIN_SIZE);

	for (EngineTypes::Map::array_length_t polygon = 0; polygon < EVENT8__POLYGONS_COUNT; polygon++)
	{
		temp__polygon_p = map.PolygonPointer(polygon);
		if (temp__polygon_p->exist)
		{
			temp__polygon_p->angle = temp__angle;
			temp__polygon_p->size.Set(EVENT8__MIN_SIZE + temp__size, EVENT8__MAX_SIZE - temp__size);
			temp__polygon_p->ShouldUpdate();
			temp__angle -= (float)M_PI / (float)EVENT8__POLYGONS_COUNT;
		}
	}

	if (object_pull_array[GAME_OBJECT_ASTEROID])
	{
		asteroids_array_mtx.lock();
		if (!(global_timer % EVENT5__PERIOD) && asteroids_count == 0)
		{
			AddEntity(Asteroid(
				Vec2F(EVENT5__CENTER_POSITION, EVENT5__CENTER_POSITION),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS | BONUS_RULE_REVERSE, 2, 3, 4, 5) |
				GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 0, 1, 0, 1) |
				GenerateRandomInventory(BONUS_BUFF_STREAM, 1, 3, 1, 1),
				ASTEROID_SIZE_BIG));
		}
		asteroids_array_mtx.unlock();
	}
}

void Game::Event9()
{
#define EVENT9__CENTER_POSITION 6.0f
#define EVENT9__ANGLE_PORTAL_TP_POSITION	0.30f
	if (object_pull_array[GAME_OBJECT_ASTEROID])
	{
		asteroids_array_mtx.lock();
		if (!(global_timer % 500) && asteroids_count < 10)
		{
			uint8_t spawn = 0x0F;//0000 1111
			Asteroid* temp__asteroid_p = asteroids;
			for (GameTypes::entities_count_t found_asteroids = 0; found_asteroids < asteroids_count; temp__asteroid_p++)
			{
				if (temp__asteroid_p->exist)
				{
					Vec2F position = temp__asteroid_p->GetPosition() - EVENT9__CENTER_POSITION;
					if (spawn & 0x03 && position.x > 0.0f && position.x < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
					{
						if (position.y > 0.0f && position.y < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
						{
							spawn &= 0xFF - 0x01;//up right
						}
						else if (position.y < 0.0f && position.y > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
						{
							spawn &= 0xFF - 0x02;//down right
						}
					}
					else if (spawn & 0x0C && position.x < 0.0f && position.x > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
					{
						if (position.y > 0.0f && position.y < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
						{
							spawn &= 0xFF - 0x04;//up left
						}
						else if (position.y < 0.0f && position.y > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
						{
							spawn &= 0xFF - 0x08;//down left
						}
					}

					if (!(spawn & 0x0F))
					{
						break;
					}

					found_asteroids++;
				}
			}

			Ship* temp__ship_p = ships;
			for (GameTypes::entities_count_t found_ships = 0; found_ships < ships_count; temp__ship_p++)
			{
				if (temp__ship_p->exist)
				{
					Vec2F position = temp__ship_p->GetPosition() - EVENT9__CENTER_POSITION;
					if (spawn & 0x03 && position.x > 0.0f && position.x < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
					{
						if (position.y > 0.0f && position.y < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
						{
							spawn &= 0xFF - 0x01;//up right
						}
						else if (position.y < 0.0f && position.y > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
						{
							spawn &= 0xFF - 0x02;//down right
						}
					}
					else if (spawn & 0x0C && position.x < 0.0f && position.x > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
					{
						if (position.y > 0.0f && position.y < EVENT9__ANGLE_PORTAL_TP_POSITION * 2.0f)
						{
							spawn &= 0xFF - 0x04;//up left
						}
						else if (position.y < 0.0f && position.y > EVENT9__ANGLE_PORTAL_TP_POSITION * -2.0f)
						{
							spawn &= 0xFF - 0x08;//down left
						}
					}

					if (!(spawn & 0x0F))
					{
						break;
					}

					found_ships++;
				}
			}

			if (spawn & 0x01)//up right
			{
				AddEntity(Asteroid(
					Vec2F(EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION),
					Vec2F(),
					GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) | GenerateRandomInventory(BONUS_BUFF | BONUS_RULE_REVERSE, 1, 1, 1, 1),
					ASTEROID_SIZE_MEDIUM));
			}
			if (spawn & 0x02)//down right
			{
				AddEntity(Asteroid(
					Vec2F(EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION, -EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION),
					Vec2F(),
					GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) | GenerateRandomInventory(BONUS_BUFF | BONUS_RULE_REVERSE, 1, 1, 1, 1),
					ASTEROID_SIZE_MEDIUM));
			}
			if (spawn & 0x04)//up left
			{
				AddEntity(Asteroid(
					Vec2F(-EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION, EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION),
					Vec2F(),
					GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) | GenerateRandomInventory(BONUS_BUFF | BONUS_RULE_REVERSE, 1, 1, 1, 1),
					ASTEROID_SIZE_MEDIUM));
			}
			if (spawn & 0x08)//donw left
			{
				AddEntity(Asteroid(
					Vec2F(-EVENT9__ANGLE_PORTAL_TP_POSITION + EVENT9__CENTER_POSITION),
					Vec2F(),
					GenerateRandomInventory(BONUS_BONUS, 1, 1, 1, 1) | GenerateRandomInventory(BONUS_BUFF | BONUS_RULE_REVERSE, 1, 1, 1, 1),
					ASTEROID_SIZE_MEDIUM));
			}
		}
		asteroids_array_mtx.unlock();
	}
}

void Game::CreateMap0(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_TEST_MAP__CENTER_POSITION	EVENT0__CENTER_POSITION
#define MAP_TEST_MAP__FRAME_SIZE		2.0f
#define MAP_TEST_MAP__RECTANGLES_COUNT	1
#define MAP_TEST_MAP__CAMERA_SIZE		10.0f

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_TEST_MAP__RECTANGLES_COUNT];

	new_segment.Set(
		Vec2F(MAP_TEST_MAP__CENTER_POSITION - MAP_TEST_MAP__FRAME_SIZE, MAP_TEST_MAP__CENTER_POSITION - MAP_TEST_MAP__FRAME_SIZE),
		Vec2F(MAP_TEST_MAP__CENTER_POSITION + MAP_TEST_MAP__FRAME_SIZE, MAP_TEST_MAP__CENTER_POSITION + MAP_TEST_MAP__FRAME_SIZE),
		true);
	rectangles[0].Set(&new_segment);

	map.Set(rectangles, MAP_TEST_MAP__RECTANGLES_COUNT);


	/* Spawn entities */

	ships_positions[0].Set(MAP_TEST_MAP__CENTER_POSITION - 1.85f, MAP_TEST_MAP__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_TEST_MAP__CENTER_POSITION + 1.9f, MAP_TEST_MAP__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_TEST_MAP__CENTER_POSITION + 1.85f, MAP_TEST_MAP__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_TEST_MAP__CENTER_POSITION - 1.9f, MAP_TEST_MAP__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_TEST_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_TEST_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_TEST_MAP__CENTER_POSITION, MAP_TEST_MAP__CENTER_POSITION));
	camera.SetSize(MAP_TEST_MAP__CAMERA_SIZE);

	delete[] rectangles;
}

void Game::CreateMap1(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_TUREL_ON_CENTER__CENTER_POSITION		0.0f
#define MAP_TUREL_ON_CENTER__FRAME_SIZE				2.0f
#define MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE		0.1f
#define MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION	1.5f
#define MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION	1.0f
#define MAP_TUREL_ON_CENTER__CENTER					0.0f
#define MAP_TUREL_ON_CENTER__RECTANGLE_COUNT		6u

#define MAP_TUREL_ON_CENTER__FRAME_RECTANGLE		0u
#define MAP_TUREL_ON_CENTER__CENTER_RECTANGLE		1u
#define MAP_TUREL_ON_CENTER__DOWN_LEFT_RECTANGLE	2u
#define MAP_TUREL_ON_CENTER__UP_RIGHT_RECTANGLE		3u
#define MAP_TUREL_ON_CENTER__UP_LEFT_RECTANGLE		4u
#define MAP_TUREL_ON_CENTER__DOWN_RIGHT_RECTANGLE	5u

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_TUREL_ON_CENTER__RECTANGLE_COUNT];

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__FRAME_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__FRAME_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__FRAME_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__CENTER_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__DOWN_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__UP_RIGHT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__UP_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION, -MAP_TUREL_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TUREL_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TUREL_ON_CENTER__DOWN_RIGHT_RECTANGLE].Set(&new_segment);

	map.Set(rectangles, MAP_TUREL_ON_CENTER__RECTANGLE_COUNT);

	new_position.Set(MAP_TUREL_ON_CENTER__CENTER, MAP_TUREL_ON_CENTER__CENTER);
	AddEntity(Turel(&new_position));


	/* Spawn entities */

	ships_positions[0].Set(MAP_TUREL_ON_CENTER__CENTER_POSITION - 1.85f, MAP_TUREL_ON_CENTER__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_TUREL_ON_CENTER__CENTER_POSITION + 1.9f, MAP_TUREL_ON_CENTER__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_TUREL_ON_CENTER__CENTER_POSITION + 1.85f, MAP_TUREL_ON_CENTER__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_TUREL_ON_CENTER__CENTER_POSITION - 1.9f, MAP_TUREL_ON_CENTER__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	delete[] rectangles;
}

void Game::CreateMap2(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment diagonal;

#define MAP_GRAV_GEN__CENTER_POSITION	0.0f
#define MAP_GRAV_GEN__RECTANGLES_COUNT	5
#define MAP_GRAV_GEN__CYRCLES_COUNT		1

#define MAP_GRAV_GEN__FRAME_SIZE		2.0f
#define MAP_GRAV_GEN__SQUARE_SIZE		0.5f
#define MAP_GRAV_GEN__SQUARE_POSITION	1.5f
#define MAP_GRAV_GEN__CYRCLE_SIZE		(0.1f / 0.8f)
#define MAP_GRAV_GEN__CENTER			MAP_GRAV_GEN__CENTER_POSITION

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_GRAV_GEN__RECTANGLES_COUNT];
	Map::Cyrcle* cyrcles = new Map::Cyrcle[MAP_GRAV_GEN__CYRCLES_COUNT];

	diagonal.Set(
		Vec2F(-MAP_GRAV_GEN__FRAME_SIZE + MAP_GRAV_GEN__CENTER_POSITION, -MAP_GRAV_GEN__FRAME_SIZE + MAP_GRAV_GEN__CENTER_POSITION),
		Vec2F(MAP_GRAV_GEN__FRAME_SIZE + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__FRAME_SIZE + MAP_GRAV_GEN__CENTER_POSITION),
		true);
	rectangles[0].Set(&diagonal);

	diagonal.Set(
		Vec2F(-MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION, -MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION),
		Vec2F(MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION));
	rectangles[1].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION),
		Vec2F(-MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION, -MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION));
	rectangles[2].Set(&diagonal);

	diagonal.Set(
		Vec2F(-MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION),
		Vec2F(MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION, -MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION));
	rectangles[3].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION, -MAP_GRAV_GEN__SQUARE_POSITION + MAP_GRAV_GEN__CENTER_POSITION),
		Vec2F(-MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__SQUARE_SIZE + MAP_GRAV_GEN__CENTER_POSITION));
	rectangles[4].Set(&diagonal);


	cyrcles[0].Set(
		Vec2F(MAP_GRAV_GEN__CENTER + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__CENTER + MAP_GRAV_GEN__CENTER_POSITION),
		MAP_GRAV_GEN__CYRCLE_SIZE,
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER | MAP_PROPERTY_AGRESSIVE);

	map.Set(rectangles, MAP_GRAV_GEN__RECTANGLES_COUNT, cyrcles, MAP_GRAV_GEN__CYRCLES_COUNT);

	delete[] rectangles;
	delete[] cyrcles;

	AddEntity(GravGen(Vec2F(MAP_GRAV_GEN__CENTER + MAP_GRAV_GEN__CENTER_POSITION, MAP_GRAV_GEN__CENTER + MAP_GRAV_GEN__CENTER_POSITION),
		-GRAVITY_GENERATOR_DEFAULT_GRAVITY));

	/* Spawn entities */

	ships_positions[0].Set(MAP_GRAV_GEN__CENTER_POSITION - 1.85f, MAP_GRAV_GEN__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_GRAV_GEN__CENTER_POSITION + 1.9f, MAP_GRAV_GEN__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_GRAV_GEN__CENTER_POSITION + 1.85f, MAP_GRAV_GEN__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_GRAV_GEN__CENTER_POSITION - 1.9f, MAP_GRAV_GEN__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
}

void Game::CreateMap3(Vec2F* ships_positions, float* ships_angles)
{
	Vec2F new_position;
	Segment new_segment;

#define MAP_DECELERATION_AREA__FRAME_SIZE				2.0f
#define MAP_DECELERATION_AREA__IN_SEGMENT_HIGH			0.75f
#define MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH			1.25f
#define MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH		((MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH - MAP_DECELERATION_AREA__IN_SEGMENT_HIGH) / 2.0f)
#define MAP_DECELERATION_AREA__DECELERATION_AREA_POS	0.5f
#define MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS	0.25f

#define MAP_DECELERATION_AREA__FRAME_RECTANGEL_ID	0
#define MAP_DECELERATION_AREA__DOWN_RECTANGEL_ID	1
#define MAP_DECELERATION_AREA__RIGHT_RECTANGEL_ID	2
#define MAP_DECELERATION_AREA__UP_RECTANGEL_ID		3
#define MAP_DECELERATION_AREA__LEFT_RECTANGEL_ID	4
#define MAP_DECELERATION_AREA__RECTANGLES_COUNT		5

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_DECELERATION_AREA__RECTANGLES_COUNT];

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__FRAME_SIZE, -MAP_DECELERATION_AREA__FRAME_SIZE),
		Vec2F(MAP_DECELERATION_AREA__FRAME_SIZE, MAP_DECELERATION_AREA__FRAME_SIZE),
		true);
	rectangles[MAP_DECELERATION_AREA__FRAME_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, -MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH),
		Vec2F(MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, -MAP_DECELERATION_AREA__IN_SEGMENT_HIGH),
		true);
	rectangles[MAP_DECELERATION_AREA__DOWN_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH, -MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		Vec2F(MAP_DECELERATION_AREA__IN_SEGMENT_HIGH, MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		true);
	rectangles[MAP_DECELERATION_AREA__RIGHT_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH),
		Vec2F(-MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH, MAP_DECELERATION_AREA__IN_SEGMENT_HIGH),
		true);
	rectangles[MAP_DECELERATION_AREA__UP_RECTANGEL_ID].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_DECELERATION_AREA__OUT_SEGMENT_HIGH, MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		Vec2F(-MAP_DECELERATION_AREA__IN_SEGMENT_HIGH, -MAP_DECELERATION_AREA__SIDE_SEGMENT_WIDTH),
		true);
	rectangles[MAP_DECELERATION_AREA__LEFT_RECTANGEL_ID].Set(&new_segment);

	map.Set(rectangles, MAP_DECELERATION_AREA__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawning entities */

	new_position.Set(MAP_DECELERATION_AREA__DECELERATION_AREA_POS, MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_CEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	ships_positions[0].Set(-1.85f, 1.9f);
	ships_positions[1].Set(1.9f, 1.85f);
	ships_positions[2].Set(1.85f, -1.9f);
	ships_positions[3].Set(-1.9f, -1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
}

void Game::CreateMap4(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_MEGA_LASERS__RECTANGLES_COUNT	5u
#define MAP_MEGA_LASERS__FRAME_RECTANGLE	0u
#define MAP_MEGA_LASERS__UP_RECTANGLE		1u
#define MAP_MEGA_LASERS__RIGHT_RECTANGLE	2u
#define MAP_MEGA_LASERS__DOWN_RECTANGLE		3u
#define MAP_MEGA_LASERS__LEFT_RECTANGLE		4u

#define MAP_MEGA_LASERS__FRAME_SIZE						2.0f
#define MAP_MEGA_LASERS__CENTER_POSITION				EVENT4__CENTER_POSITION
#define MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X	1.5f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y	1.5f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X	0.0f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y	1.3f

#define MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X	0.0f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y	1.4f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X	-1.3f
#define MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y	1.4f

#define MAP_MEGA_LASERS__CAMERA_SIZE	10.0f

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_MEGA_LASERS__RECTANGLES_COUNT];

	Segment diagonal = Segment(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE),
		true);
	rectangles[MAP_MEGA_LASERS__FRAME_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y),
		true);
	rectangles[MAP_MEGA_LASERS__UP_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X),
		true);
	rectangles[MAP_MEGA_LASERS__RIGHT_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y),
		true);
	rectangles[MAP_MEGA_LASERS__DOWN_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION_X),
		true);
	rectangles[MAP_MEGA_LASERS__LEFT_RECTANGLE].Set(&diagonal);

	map.Set(rectangles, MAP_MEGA_LASERS__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y),
		true);
	AddEntity(MegaLaser(&diagonal, MEGA_LASER_DEFAULT_WIDTH, 0, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X),
		true);
	AddEntity(MegaLaser(&diagonal, MEGA_LASER_DEFAULT_WIDTH, 500, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y),
		true);
	AddEntity(MegaLaser(&diagonal, MEGA_LASER_DEFAULT_WIDTH, 0, 1000, 500));

	diagonal.Set(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_1_POSITION_X),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_Y, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__MEGA_LASER_POINT_2_POSITION_X),
		true);
	AddEntity(MegaLaser(&diagonal, MEGA_LASER_DEFAULT_WIDTH, 500, 1000, 500));

	ships_positions[0].Set(MAP_MEGA_LASERS__CENTER_POSITION - 1.85f, MAP_MEGA_LASERS__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_MEGA_LASERS__CENTER_POSITION + 1.9f, MAP_MEGA_LASERS__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_MEGA_LASERS__CENTER_POSITION + 1.85f, MAP_MEGA_LASERS__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_MEGA_LASERS__CENTER_POSITION - 1.9f, MAP_MEGA_LASERS__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_MEGA_LASERS__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_MEGA_LASERS__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_MEGA_LASERS__CENTER_POSITION, MAP_MEGA_LASERS__CENTER_POSITION));
	camera.SetSize(MAP_MEGA_LASERS__CAMERA_SIZE);
}

void Game::CreateMap5(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_DYNAMICAL__RECTANGLES_COUNT	5u
#define MAP_DYNAMICAL__FRAME_RECTANGLE	0u
#define MAP_DYNAMICAL__UP_RECTANGLE		EVENT5__UP_RECTANGLE
#define MAP_DYNAMICAL__RIGHT_RECTANGLE	EVENT5__RIGHT_RECTANGLE
#define MAP_DYNAMICAL__DOWN_RECTANGLE	EVENT5__DONW_RECTANGLE
#define MAP_DYNAMICAL__LEFT_RECTANGLE	EVENT5__LEFT_RECTANGLE

#define MAP_DYNAMICAL__FRAME_SIZE		2.0f
#define MAP_DYNAMICAL__CENTER_POSITION	EVENT5__CENTER_POSITION

#define MAP_DYNAMICAL_RECTANGLE_POSITION	EVENT5__SQUARE_SIZE
#define MAP_DYNAMICAL_RECTANGLE_SIZE		0.25f

#define MAP_DYNAMICAL__CAMERA_SIZE	10.0f

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_DYNAMICAL__RECTANGLES_COUNT];

	Segment diagonal = Segment(
		Vec2F(
			MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE,
			MAP_DYNAMICAL__CENTER_POSITION + MAP_DYNAMICAL__FRAME_SIZE),
		Vec2F(
			MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE,
			MAP_DYNAMICAL__CENTER_POSITION - MAP_DYNAMICAL__FRAME_SIZE),
		true);
	rectangles[MAP_DYNAMICAL__FRAME_RECTANGLE].Set(&diagonal);

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__UP_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__RIGHT_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__DOWN_RECTANGLE].NormaliseThis();

	diagonal.Set(
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_POSITION - MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			-MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		Vec2F(
			-MAP_DYNAMICAL_RECTANGLE_POSITION + MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION,
			MAP_DYNAMICAL_RECTANGLE_SIZE + MAP_DYNAMICAL__CENTER_POSITION),
		true);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].Set(&diagonal);
	rectangles[MAP_DYNAMICAL__LEFT_RECTANGLE].NormaliseThis();

	map.Set(rectangles, MAP_DYNAMICAL__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.85f, MAP_DYNAMICAL__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMap6(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_DESTRUCTIBLE__CENTER_POSTION	6.0f
#define MAP_DESTRUCTIBLE__FRAME_SIZE		2.0f

#define MAP_DESTRUCTIBLE__RECTANGLES_SIZE			(MAP_DESTRUCTIBLE__FRAME_SIZE / 5.0f)
#define MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X	(1.0f / 2.5f * MAP_DESTRUCTIBLE__FRAME_SIZE)
#define MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y	(MAP_DESTRUCTIBLE__FRAME_SIZE - MAP_DESTRUCTIBLE__RECTANGLES_SIZE)
#define MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION		MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X

#define MAP_DESTRUCTIBLE__RECTANGLES_COUNT			17u


#define MAP_DESTRUCTABLE__OUT_UP_RIGHT_RECTANGLE	0u
#define MAP_DESTRUCTABLE__OUT_RIGHT_UP_RECTANGLE	1u
#define MAP_DESTRUCTABLE__OUT_RIGHT_DOWN_RECTANGLE	2u
#define MAP_DESTRUCTABLE__OUT_DOWN_RIGHT_RECTANGLE	3u
#define MAP_DESTRUCTABLE__OUT_DOWN_LEFT_RECTANGLE	4u
#define MAP_DESTRUCTABLE__OUT_LEFT_DOWN_RECTANGLE	5u
#define MAP_DESTRUCTABLE__OUT_LEFT_UP_RECTANGLE		6u
#define MAP_DESTRUCTABLE__OUT_UP_LEFT_RECTANGLE		7u
#define MAP_DESTRUCTABLE__UP_RECTANGLE				8u
#define MAP_DESTRUCTABLE__RIGHT_RECTANGLE			9u
#define MAP_DESTRUCTABLE__DOWN_RECTANGLE			10u
#define MAP_DESTRUCTABLE__LEFT_RECTANGLE			11u

#define MAP_DESTRUCTIBLE__FRAME_RECTANGLE			12u
#define MAP_DESTRUCTABLE__UP_RIGHT_RECTANGLE		13u
#define MAP_DESTRUCTABLE__DOWN_RIGHT_RECTANGLE		14u
#define MAP_DESTRUCTABLE__DOWN_LEFT_RECTANGLE		15u
#define MAP_DESTRUCTABLE__UP_LEFT_RECTANGLE			16u

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_DESTRUCTIBLE__RECTANGLES_COUNT];
	Segment diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__FRAME_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTIBLE__FRAME_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_UP_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_RIGHT_UP_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_RIGHT_DOWN_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_DOWN_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_DOWN_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_LEFT_DOWN_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_LEFT_UP_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_Y + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__OUT_RECTANGLE_POSITION_X + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__OUT_UP_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__UP_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__DOWN_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	diagonal = Segment(
		Vec2F(
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			-MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		Vec2F(
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE - MAP_DESTRUCTUBLE__IN_RECTANGLE_POSITION + MAP_DESTRUCTIBLE__CENTER_POSTION,
			MAP_DESTRUCTIBLE__RECTANGLES_SIZE + MAP_DESTRUCTIBLE__CENTER_POSTION),
		true);
	rectangles[MAP_DESTRUCTABLE__LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_NO_PROPERTY);

	map.Set(rectangles, MAP_DESTRUCTIBLE__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.85f, MAP_DYNAMICAL__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMap7(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_AGGRESSIVE__CENTER_POSITION		6.0f
#define MAP_AGGRESSIVE__FRAME_SIZE			2.0f
#define MAP_AGGRESSIVE__ELEMENT_POSITION_X	0.4f
#define MAP_AGGRESSIVE__ELEMENT_POSITION_Y	1.0f
#define MAP_AGGRESSIVE__ELEMENT_DIAGONAL	0.15f

#define MAP_AGGRESSIVE__RECTANGLES_COUNT		9u
#define MAP_AGGRESSIVE__FRAME_RECTANGLE			0u
#define MAP_AGGRESSIVE__UP_RIGHT_RECTANGLE		1u
#define MAP_AGGRESSIVE__RIGHT_UP_RECTANGLE		2u
#define MAP_AGGRESSIVE__RIGHT_DOWN_RECTANGLE	3u
#define MAP_AGGRESSIVE__DOWN_RIGHT_RECTANGLE	4u
#define MAP_AGGRESSIVE__DOWN_LEFT_RECTANGLE		5u
#define MAP_AGGRESSIVE__LEFT_DOWN_RECTANGLE		6u
#define MAP_AGGRESSIVE__LEFT_UP_RECTANGLE		7u
#define MAP_AGGRESSIVE__UP_LEFT_RECTANGLE		8u

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_AGGRESSIVE__RECTANGLES_COUNT];
	Segment diagonal = Segment(
		Vec2F(MAP_AGGRESSIVE__CENTER_POSITION - MAP_AGGRESSIVE__FRAME_SIZE),
		Vec2F(MAP_AGGRESSIVE__CENTER_POSITION + MAP_AGGRESSIVE__FRAME_SIZE),
		true);
	rectangles[MAP_AGGRESSIVE__FRAME_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__UP_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__RIGHT_UP_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL -  MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__RIGHT_DOWN_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__DOWN_RIGHT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__DOWN_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__LEFT_DOWN_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__LEFT_UP_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	diagonal = Segment(
		Vec2F(
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			-MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		Vec2F(
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL - MAP_AGGRESSIVE__ELEMENT_POSITION_Y + MAP_AGGRESSIVE__CENTER_POSITION,
			MAP_AGGRESSIVE__ELEMENT_DIAGONAL + MAP_AGGRESSIVE__ELEMENT_POSITION_X + MAP_AGGRESSIVE__CENTER_POSITION),
		true);
	rectangles[MAP_AGGRESSIVE__UP_LEFT_RECTANGLE].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_KILLER);

	map.Set(rectangles, MAP_AGGRESSIVE__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.85f, MAP_DYNAMICAL__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMap8(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_BROKEN__CENTER_POSITION		6.0f
#define MAP_BROKEN__FRAME_SIZE			2.0f
#define MAP_BROKEN__STAR_RAY_SIZE		0.4f
#define MAP_BROKEN__STAR_CENTER_SIZE	0.1f
#define MAP_BROKEN__STAR_POSITION		0.7f

#define MAP_BROKEN__RECTANGLES_COUNT	1u
#define MAP_BROKEN__FRAME_RECTANGLE		0u

#define MAP_BROKEN__POLYGONS_COUNT			EVENT8__POLYGONS_COUNT
#define MAP_BROKEN__UP_RIGHT_STAR_POLYGON	0u
#define MAP_BROKEN__DOWN_RIGHT_STAR_POLYGON	1u
#define MAP_BROKEN__DOWN_LEFT_STAR_POLYGON	2u
#define MAP_BROKEN__UP_LEFT_STAR_POLYGON	3u

#define MAP_BROKEN__STAR_POLYGON__POINTS_COUNT		8u
#define MAP_BROKEN__STAR_POLYGON__UP_POINT			0u
#define MAP_BROKEN__STAR_POLYGON__RIGHT_POINT		2u
#define MAP_BROKEN__STAR_POLYGON__DOWN_POINT		4u
#define MAP_BROKEN__STAR_POLYGON__LEFT_POINT		6u
#define MAP_BROKEN__STAR_POLYGON__UP_RIGHT_POINT	1u
#define MAP_BROKEN__STAR_POLYGON__DOWN_RIGHT_POINT	3u
#define MAP_BROKEN__STAR_POLYGON__DOWN_LEFT_POINT	5u
#define MAP_BROKEN__STAR_POLYGON__UP_LEFT_POINT		7u

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_BROKEN__RECTANGLES_COUNT];
	Map::Polygon* polygons = new Map::Polygon[MAP_BROKEN__POLYGONS_COUNT];
	Vec2F* points = new Vec2F[MAP_BROKEN__STAR_POLYGON__POINTS_COUNT];

	Segment diagonal = Segment(
		Vec2F(MAP_BROKEN__CENTER_POSITION + MAP_BROKEN__FRAME_SIZE),
		Vec2F(MAP_BROKEN__CENTER_POSITION - MAP_BROKEN__FRAME_SIZE),
		true);
	rectangles[MAP_BROKEN__FRAME_RECTANGLE].Set(&diagonal);

	points[MAP_BROKEN__STAR_POLYGON__UP_POINT].Set(0.0f, MAP_BROKEN__STAR_RAY_SIZE);
	points[MAP_BROKEN__STAR_POLYGON__RIGHT_POINT].Set(MAP_BROKEN__STAR_RAY_SIZE, 0.0f);
	points[MAP_BROKEN__STAR_POLYGON__DOWN_POINT].Set(0.0f, -MAP_BROKEN__STAR_RAY_SIZE);
	points[MAP_BROKEN__STAR_POLYGON__LEFT_POINT].Set(-MAP_BROKEN__STAR_RAY_SIZE, 0.0f);
	points[MAP_BROKEN__STAR_POLYGON__UP_RIGHT_POINT].Set(MAP_BROKEN__STAR_CENTER_SIZE, MAP_BROKEN__STAR_CENTER_SIZE);
	points[MAP_BROKEN__STAR_POLYGON__DOWN_RIGHT_POINT].Set(MAP_BROKEN__STAR_CENTER_SIZE, -MAP_BROKEN__STAR_CENTER_SIZE);
	points[MAP_BROKEN__STAR_POLYGON__DOWN_LEFT_POINT].Set(-MAP_BROKEN__STAR_CENTER_SIZE, -MAP_BROKEN__STAR_CENTER_SIZE);
	points[MAP_BROKEN__STAR_POLYGON__UP_LEFT_POINT].Set(-MAP_BROKEN__STAR_CENTER_SIZE, MAP_BROKEN__STAR_CENTER_SIZE);

	polygons[MAP_BROKEN__UP_RIGHT_STAR_POLYGON].Set(
		Vec2F(MAP_BROKEN__CENTER_POSITION + MAP_BROKEN__STAR_POSITION, MAP_BROKEN__CENTER_POSITION + MAP_BROKEN__STAR_POSITION),
		0.0f,
		Vec2F(1.0f),
		points,
		MAP_BROKEN__STAR_POLYGON__POINTS_COUNT,
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);
	
	polygons[MAP_BROKEN__DOWN_RIGHT_STAR_POLYGON].Set(
		Vec2F(MAP_BROKEN__CENTER_POSITION + MAP_BROKEN__STAR_POSITION, MAP_BROKEN__CENTER_POSITION - MAP_BROKEN__STAR_POSITION),
		0.0f,
		Vec2F(1.0f),
		points,
		MAP_BROKEN__STAR_POLYGON__POINTS_COUNT,
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);

	polygons[MAP_BROKEN__DOWN_LEFT_STAR_POLYGON].Set(
		Vec2F(MAP_BROKEN__CENTER_POSITION - MAP_BROKEN__STAR_POSITION, MAP_BROKEN__CENTER_POSITION - MAP_BROKEN__STAR_POSITION),
		0.0f,
		Vec2F(1.0f),
		points,
		MAP_BROKEN__STAR_POLYGON__POINTS_COUNT,
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);

	polygons[MAP_BROKEN__UP_LEFT_STAR_POLYGON].Set(
		Vec2F(MAP_BROKEN__CENTER_POSITION - MAP_BROKEN__STAR_POSITION, MAP_BROKEN__CENTER_POSITION + MAP_BROKEN__STAR_POSITION),
		0.0f,
		Vec2F(1.0f),
		points,
		MAP_BROKEN__STAR_POLYGON__POINTS_COUNT,
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);
	
	delete[] points;

	map.Set(rectangles, MAP_BROKEN__RECTANGLES_COUNT, nullptr, 0, polygons, MAP_BROKEN__POLYGONS_COUNT);

	delete[] rectangles;
	delete[] polygons;

	/* Spawn entities */

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.9f, MAP_DYNAMICAL__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.85f, MAP_DYNAMICAL__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.9f, MAP_DYNAMICAL__CENTER_POSITION - 1.85f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - MAP_BROKEN__FRAME_SIZE,
		MAP_DYNAMICAL__CENTER_POSITION - MAP_BROKEN__FRAME_SIZE,
		MAP_DYNAMICAL__CENTER_POSITION + MAP_BROKEN__FRAME_SIZE,
		MAP_DYNAMICAL__CENTER_POSITION + MAP_BROKEN__FRAME_SIZE);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMap9(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_PORTAL__CENTER_POSITION				EVENT9__CENTER_POSITION
#define MAP_PORTAL__FRAME_SIZE					2.0f
#define MAP_PORTAL__LOW_FRAME_SIZE				1.9f
#define MAP_PORTAL__SHORT_BARIER_POSITION_X		0.6f
#define MAP_PORTAL__SHORT_BARIER_LENGTH			0.9f
#define MAP_PORTAL__SHORT_BARIER_WIDTH_2		0.1f

#define MAP_PORTAL__SIDE_PORTAL_POSITION_X		0.3f
#define MAP_PORTAL__SIDE_PORTAL_POSITION_Y		1.85f
#define MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X	-0.80f
#define MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y	1.87f
#define MAP_PORTAL__ANGLE_PORTAL_POSITION		1.87f
#define MAP_PORTAL__ANGLE_PORTAL_TP_POSITION	(-EVENT9__ANGLE_PORTAL_TP_POSITION)

#define MAP_PORTAL__RECTANGLES_COUNT			12

#define MAP_PORTAL__UP_RIGTH_BIG_RECTANGLE		0
#define MAP_PORTAL__DOWN_RIGTH_BIG_RECTANGLE	1
#define MAP_PORTAL__DOWN_LEFT_BIG_RECTANGLE		2
#define MAP_PORTAL__UP_LEFT_BIG_RECTANGLE		3

#define MAP_PORTAL__UP_RIGHT_SHORT_RECTANGLE	4
#define MAP_PORTAL__RIGHT_UP_SHORT_RECTANGLE	5
#define MAP_PORTAL__RIGHT_DOWN_SHORT_RECTANGLE	6
#define MAP_PORTAL__DOWN_RIGHT_SHORT_RECTANGLE	7
#define MAP_PORTAL__DOWN_LEFT_SHORT_RECTANGLE	8
#define MAP_PORTAL__LEFT_DOWN_SHORT_RECTANGLE	9
#define MAP_PORTAL__LEFT_UP_SHORT_RECTANGLE		10
#define MAP_PORTAL__UP_LEFT_SHORT_RECTANGLE		11

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_PORTAL__RECTANGLES_COUNT];

	Segment segment = Segment(
		Vec2F(MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE),
		Vec2F(MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__LOW_FRAME_SIZE),
		true);
	rectangles[MAP_PORTAL__UP_RIGTH_BIG_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE, MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE),
		Vec2F(MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__LOW_FRAME_SIZE, MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__LOW_FRAME_SIZE),
		true);
	rectangles[MAP_PORTAL__DOWN_RIGTH_BIG_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE),
		Vec2F(MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__LOW_FRAME_SIZE),
		true);
	rectangles[MAP_PORTAL__DOWN_LEFT_BIG_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE, MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE),
		Vec2F(MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__LOW_FRAME_SIZE, MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__LOW_FRAME_SIZE),
		true);
	rectangles[MAP_PORTAL__UP_LEFT_BIG_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__SHORT_BARIER_LENGTH),
		true);
	rectangles[MAP_PORTAL__UP_RIGHT_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__SHORT_BARIER_LENGTH,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		true);
	rectangles[MAP_PORTAL__RIGHT_UP_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__SHORT_BARIER_LENGTH,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		true);
	rectangles[MAP_PORTAL__RIGHT_DOWN_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__SHORT_BARIER_LENGTH),
		true);
	rectangles[MAP_PORTAL__DOWN_RIGHT_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__SHORT_BARIER_LENGTH),
		true);
	rectangles[MAP_PORTAL__DOWN_LEFT_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__SHORT_BARIER_LENGTH,
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		true);
	rectangles[MAP_PORTAL__LEFT_DOWN_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__FRAME_SIZE + MAP_PORTAL__SHORT_BARIER_LENGTH,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2),
		true);
	rectangles[MAP_PORTAL__LEFT_UP_SHORT_RECTANGLE].Set(&segment);

	segment.Set(
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X + MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE),
		Vec2F(
			MAP_PORTAL__CENTER_POSITION - MAP_PORTAL__SHORT_BARIER_POSITION_X - MAP_PORTAL__SHORT_BARIER_WIDTH_2,
			MAP_PORTAL__CENTER_POSITION + MAP_PORTAL__FRAME_SIZE - MAP_PORTAL__SHORT_BARIER_LENGTH),
		true);
	rectangles[MAP_PORTAL__UP_LEFT_SHORT_RECTANGLE].Set(&segment);

	map.Set(rectangles, MAP_PORTAL__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_POSITION_Y + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__SIDE_PORTAL_TP_POSITION_Y + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__SIDE_PORTAL_TP_POSITION_X + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION),
		Vec2F(MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION, -MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	AddEntity(Portal(
		Vec2F(-MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__ANGLE_PORTAL_POSITION + MAP_PORTAL__CENTER_POSITION),
		Vec2F(-MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION, MAP_PORTAL__ANGLE_PORTAL_TP_POSITION + MAP_PORTAL__CENTER_POSITION),
		PORTAL_DEFAULT_RADIUS,
		(float)M_PI));

	ships_positions[0].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.05f, MAP_DYNAMICAL__CENTER_POSITION + 1.1f);
	ships_positions[1].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.1f, MAP_DYNAMICAL__CENTER_POSITION + 1.05f);
	ships_positions[2].Set(MAP_DYNAMICAL__CENTER_POSITION + 1.05f, MAP_DYNAMICAL__CENTER_POSITION - 1.1f);
	ships_positions[3].Set(MAP_DYNAMICAL__CENTER_POSITION - 1.1f, MAP_DYNAMICAL__CENTER_POSITION - 1.05f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_DYNAMICAL__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_DYNAMICAL__CENTER_POSITION, MAP_DYNAMICAL__CENTER_POSITION));
	camera.SetSize(MAP_DYNAMICAL__CAMERA_SIZE);
}

void Game::CreateMapRoundResults(GameTypes::players_count_t players_count, GameTypes::score_t max_score, float cell_size)
{
	GameTypes::score_t score_amplitude = max_score * 2 + 1;
	GameTypes::map_elements_count_t rectangles_count = players_count + score_amplitude;

	Map::Rectangle* rectangles = new Map::Rectangle[rectangles_count];

	//Create rows

	float row_down_position = -(float)players_count / 2.0f * cell_size;
	float column_left_position = -(float)score_amplitude / 2.0f * cell_size;

	for (GameTypes::map_elements_count_t row = 0; row < players_count; row++)
	{
		float row_position = row_down_position + (float)row * cell_size;
		Segment diagonal = Segment(
			Vec2F(column_left_position, row_position),
			Vec2F(cell_size * (float)score_amplitude, cell_size));
		rectangles[row].Set(&diagonal);
	}

	for (GameTypes::map_elements_count_t column = 0; column < score_amplitude; column++)
	{
		float column_position = column_left_position + (float)column * cell_size;
		Segment diagonal = Segment(
			Vec2F(column_position, row_down_position),
			Vec2F(cell_size, cell_size * (float)players_count));
		rectangles[players_count + column].Set(&diagonal);
	}

	map.Set(rectangles, rectangles_count);

	delete[] rectangles;

	camera.SetPosition(Vec2F());
	camera.SetSize(5.0f);
}