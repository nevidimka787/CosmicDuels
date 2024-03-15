#include "Game.h"

#include <vector>

#define M_PI	3.14159265358979323846	// pi
#define M_PI_2	1.5707796326794897		// pi/2
#define M_PI_4  0.785398163397448309616	// pi/4

#define ALL_MAPS__SPAWN_DELTA_ANGLE	0.05f

#pragma warning(disable : 6385)
#pragma warning(disable : 26451)

#define MAP_ORBIT_MAP__AREA_RADIUS		2.0f
#define MAP_ORBIT_MAP__CENTER_POSITION	(MAP_ORBIT_MAP__AREA_RADIUS * 3.0f)
#define MAP_ORBIT_MAP__CYRCLES_COUNT	10
#define MAP_ORBIT_MAP__CAMERA_SIZE		10.0f

#define MAP_ORBIT_MAP__CENTER_GRAV_GEN_POS		MAP_ORBIT_MAP__CENTER_POSITION
#define MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS	(MAP_ORBIT_MAP__AREA_RADIUS / 3.0f)
#define MAP_ORBIT_MAP__EXTERNAL_ORBIT_RADIUS	(MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS * 2.0f)

#define MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS	(MAP_ORBIT_MAP__AREA_RADIUS / 20.0f)
#define MAP_ORBIT_MAP__GRAVGEN_KILL_BARIER_RADIUS	(MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS * 0.8f)
#define MAP_ORBIT_MAP__GRAVGEN_RADIUS				(MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS * 0.5f)

#define MAP_ORBIT_MAP__RELATIVE_SHIP_POSITION	Vec2F(MAP_ORBIT_MAP__AREA_RADIUS * 0.6f, MAP_ORBIT_MAP__AREA_RADIUS * 1.025f * 0.6f)

#define MAP_ORBIT_MAP__GRAVGEN_FORCE			0.00001f

void Game::Event0()
{
#define EVENT0__INTERNAL_PERIOD	2500 // 25 seconds
#define EVENT0__EXTERNAL_PERIOD	5000 // 50 seconds

	Vec2F center_position(MAP_ORBIT_MAP__CENTER_POSITION);
	Vec2F in_direction(0.0f, -MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS);
	Vec2F ex_direction(MAP_ORBIT_MAP__EXTERNAL_ORBIT_RADIUS, 0.0f);

	Vec2F pos_2_3(center_position + in_direction.Rotate((float)(global_timer % EVENT0__INTERNAL_PERIOD) / (float)EVENT0__INTERNAL_PERIOD * M_PI * 2.0f));
	Vec2F pos_4_5(center_position + in_direction.Rotate((float)(global_timer % EVENT0__INTERNAL_PERIOD) / (float)EVENT0__INTERNAL_PERIOD * M_PI * 2.0f - M_PI));
	Vec2F pos_6_7(center_position + ex_direction.RotateClockwise((float)(global_timer % EVENT0__EXTERNAL_PERIOD) / (float)EVENT0__EXTERNAL_PERIOD * M_PI * 2.0f));
	Vec2F pos_8_9(center_position + ex_direction.RotateClockwise((float)(global_timer % EVENT0__EXTERNAL_PERIOD) / (float)EVENT0__EXTERNAL_PERIOD * M_PI * 2.0f - M_PI));

	float in_radius = 
		MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS + 
		sinf(
			(float)(global_timer % EVENT0__EXTERNAL_PERIOD) / (float)EVENT0__EXTERNAL_PERIOD * M_PI * 2.0f
		) * (MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS - MAP_ORBIT_MAP__GRAVGEN_KILL_BARIER_RADIUS);

	float ex_radius =
		MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS +
		sinf(
			(float)(global_timer % EVENT0__INTERNAL_PERIOD) / (float)EVENT0__INTERNAL_PERIOD * M_PI * 2.0f
		) * (MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS - MAP_ORBIT_MAP__GRAVGEN_KILL_BARIER_RADIUS);

	map_data_mtx.lock();

	map.CyrclePointer(2)->SetPosition(pos_2_3);
	map.CyrclePointer(3)->SetPosition(pos_2_3);
	map.CyrclePointer(4)->SetPosition(pos_4_5);
	map.CyrclePointer(5)->SetPosition(pos_4_5);
	map.CyrclePointer(6)->SetPosition(pos_6_7);
	map.CyrclePointer(7)->SetPosition(pos_6_7);
	map.CyrclePointer(8)->SetPosition(pos_8_9);
	map.CyrclePointer(9)->SetPosition(pos_8_9);

	map.CyrclePointer(2)->SetRadius(in_radius);
	map.CyrclePointer(4)->SetRadius(in_radius);
	map.CyrclePointer(6)->SetRadius(ex_radius);
	map.CyrclePointer(8)->SetRadius(ex_radius);

	map_data_mtx.unlock();

	grav_gens_array_mtx.lock();

	grav_gens[0].SetPosition(pos_2_3);
	grav_gens[1].SetPosition(pos_4_5);
	grav_gens[2].SetPosition(pos_6_7);
	grav_gens[3].SetPosition(pos_8_9);

	grav_gens_array_mtx.unlock();

	if (!(global_timer % 100) && global_timer > 1000)
	{
		if (asteroids_count <= 2)
		{

			asteroids_array_mtx.lock();
			for (size_t i = 0; i < 4; ++i)
			{
				Vec2F asteroid_position = center_position + Vec2F(MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS, MAP_ORBIT_MAP__EXTERNAL_ORBIT_RADIUS).Rotate((float)i / 4.0f * M_PI * 2.0f);
				AddEntity(Asteroid(
					asteroid_position,
					Vec2F(),
					GenerateRandomInventory(BONUS_ALL, 1, 3, 1, 3),
					ASTEROID_MAX_SIZE));
			}
			asteroids_array_mtx.unlock();
		}
	}
}

void Game::Event1()
{
	turrets_array_mtx.lock();
	if (turrets[0].exist)
	{
		turrets[0].Rotate(0.01f);
	}
	turrets_array_mtx.unlock();

	static int event1_spawn_asteroids;

#define EVENT1_SQUARE_SIZE 0.7f

	Vec2F asteroid_position(-EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE);

	switch (event1_spawn_asteroids)
	{
	case 4:
		break;
	case 3:
		asteroid_position.Set(-EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE);
		break;
	case 2:
		asteroid_position.Set(EVENT1_SQUARE_SIZE, -EVENT1_SQUARE_SIZE);
		break;
	case 1:
		asteroid_position.Set(EVENT1_SQUARE_SIZE, EVENT1_SQUARE_SIZE);
		break;
	default:
		event1_spawn_asteroids = 0;
		break;
	}

	if (event1_spawn_asteroids)
	{
		asteroids_array_mtx.lock();
		AddEntity(Asteroid(
			asteroid_position,
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS, 1, 2, 2, 4) | GenerateRandomInventory(BONUS_BUFF_STREAM, 0, 3, 1, 1) | GenerateRandomInventory(BONUS_RULE_REVERSE, 0, 1, 0, 1),
			ASTEROID_SIZE_BIG));
		asteroids_array_mtx.unlock();

		--event1_spawn_asteroids;
	}

	if (object_pull_array[GAME_OBJECT_ASTEROID] && event1_spawn_asteroids <= 0 && !((global_timer + 5) % 1000) && asteroids_count < 2)
	{
		event1_spawn_asteroids = 4;
	}
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

#define MAP_MEGA_LASERS__FRAME_RECTANGLE		0u

#define MAP_MEGA_LASERS__SIDE_RECTANGLE			1u
#define MAP_MEGA_LASERS__SIDE_RECTANGLES_COUNT	4u

#define MAP_MEGA_LASERS__LASER_RECTANGLE		5u
#define MAP_MEGA_LASERS__LASER_RECTANGLES_COUNT	4u

#define MAP_MEGA_LASERS__RECTANGLES_COUNT	(1u + MAP_MEGA_LASERS__SIDE_RECTANGLES_COUNT + MAP_MEGA_LASERS__LASER_RECTANGLES_COUNT)

#define MAP_MEGA_LASERS__FRAME_SIZE						2.0f
#define MAP_MEGA_LASERS__CENTER_POSITION				6.0f
#define MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION		Vec2F(1.5f, 1.5f)
#define MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION		Vec2F(0.0f, 1.3f)

#define MAP_MEGA_LASERS__LASER_POINT_1_POSITION			Vec2F(1.4f, 1.45f)
#define MAP_MEGA_LASERS__LASER_POINT_2_POSITION			Vec2F(0.1f, 1.35f)
#define MAP_MEGA_LASERS__LASER_POINTS_X_DELTA			(MAP_MEGA_LASERS__LASER_POINT_1_POSITION.x - MAP_MEGA_LASERS__LASER_POINT_2_POSITION.x)
#define MAP_MEGA_LASERS__LASER_POINTS_Y_DELTA			(MAP_MEGA_LASERS__LASER_POINT_1_POSITION.y - MAP_MEGA_LASERS__LASER_POINT_2_POSITION.y)

#define MAP_MEGA_LASERS__CAMERA_SIZE	10.0f

#define EVENT_6__LASER_PERIOD	1500

void Game::Event4()
{
	if (object_pull_array[GAME_OBJECT_ASTEROID] && !(global_timer % 1000) && asteroids_count < 2)
	{
		for (size_t id = 0, count = 0; count < asteroids_count; ++id)
		{
			if (asteroids[id].exist)
			{
				if (asteroids[id].GetDistance(Vec2F(MAP_MEGA_LASERS__CENTER_POSITION)) < ASTEROID_RADIUS_BIG)
				{
					goto SKIP_SPAWN_ASTEROID;
				}
				++count;
			}
		}

		asteroids_array_mtx.lock();
		AddEntity(Asteroid(
				Vec2F(MAP_MEGA_LASERS__CENTER_POSITION, MAP_MEGA_LASERS__CENTER_POSITION),
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS | BONUS_BUFF_STREAM, 2, 3, 3, 4) | GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 1, 1, 1, 1),
				ASTEROID_SIZE_BIG));
		asteroids_array_mtx.unlock();

	SKIP_SPAWN_ASTEROID:;
	}

	float animation_x = 0.0f;
	float animation_y = 0.0f;
	long animation = (global_timer + 1000) % EVENT_6__LASER_PERIOD;
	if (0 <= animation && animation < 100)
	{
		animation_x = -static_cast<float>(animation - 0) / 1000.0f;
	}
	if (100 <= animation && animation < 120)
	{
		animation_x = -0.1f + static_cast<float>(animation - 100) / 20.0f * 1.1f;
	}
	else if (120 <= animation && animation < 520)
	{
		animation_x = 1.0f;
		animation_y = static_cast<float>(animation - 120) / 400.0f;
	}
	else if (520 <= animation && animation < 540)
	{
		animation_x = 1.0f - static_cast<float>(animation - 520) / 20.0f;
		animation_y = 1.0f - static_cast<float>(animation - 520) / 20.0f;
	}

	Vec2F position1(
		MAP_MEGA_LASERS__LASER_POINT_1_POSITION.x,
		MAP_MEGA_LASERS__LASER_POINT_1_POSITION.y + animation_y * MAP_MEGA_LASERS__LASER_POINTS_Y_DELTA / 3.0f);
	Vec2F position2(
		MAP_MEGA_LASERS__LASER_POINT_2_POSITION.x - animation_x * 1.4f,
		MAP_MEGA_LASERS__LASER_POINT_2_POSITION.y - animation_y * MAP_MEGA_LASERS__LASER_POINTS_Y_DELTA / 3.0f);
	for (size_t i = MAP_MEGA_LASERS__LASER_RECTANGLE; i < MAP_MEGA_LASERS__LASER_RECTANGLE + MAP_MEGA_LASERS__LASER_RECTANGLES_COUNT; ++i)
	{
		Map::Rectangle* rectangle_p = map.RectanglePointer(i);
		rectangle_p->Set(
			Segment(position1 + Vec2F(MAP_MEGA_LASERS__CENTER_POSITION), position2 + Vec2F(MAP_MEGA_LASERS__CENTER_POSITION), true),
			rectangle_p->Prorerties());

		position1.PerpendicularThis();
		position2.PerpendicularThis();
	}
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
			Vec2F(MAP_MEGA_LASERS__CENTER_POSITION, MAP_MEGA_LASERS__CENTER_POSITION),
			Vec2F(),
			GenerateRandomInventory(BONUS_BONUS | BONUS_BUFF_STREAM, 2, 3, 3, 4) |
			GenerateRandomInventory(BONUS_BUFF_TRIPLE | BONUS_BUFF_SHIELD, 1, 1, 1, 1),
			ASTEROID_SIZE_BIG));
	}
	asteroids_array_mtx.unlock();
}

#define MAP_DESTRUCTIBLE__CENTER_POSTION	6.0f
#define MAP_DESTRUCTIBLE__FRAME_SIZE		4.0f

#define MAP_DESTRUCTIBLE__RECTANGLES_SIZE						(MAP_DESTRUCTIBLE__FRAME_SIZE / 5.0f)
#define MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLE_POSITION_STEP	(MAP_DESTRUCTIBLE__RECTANGLES_SIZE / 3.0f)
#define MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLES_SIZE			(MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLE_POSITION_STEP * 0.8f)

#define MAP_DESTRUCTIBLE__UNBREACABLE_RECTANGLES_COUNT	4u
#define MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLES_COUNT	(12u * 9u)
#define MAP_DESTRUCTIBLE__RECTANGLES_COUNT				(MAP_DESTRUCTIBLE__UNBREACABLE_RECTANGLES_COUNT + MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLES_COUNT + 1u)

#define MAP_DESTRUCTABLE__FRAME_RECTANGLE_ID			0
#define MAP_DESTRUCTABLE__UNBREACABLE_RECTANGLE_ID		(MAP_DESTRUCTABLE__FRAME_RECTANGLE_ID + 1u)
#define MAP_DESTRUCTABLE__DESTROYABLE_RECTANGLE_ID		(MAP_DESTRUCTABLE__UNBREACABLE_RECTANGLE_ID + MAP_DESTRUCTIBLE__UNBREACABLE_RECTANGLES_COUNT)

void Game::Event6()
{
#define EVENT_6__PERIOD	1000

	if (!(global_timer % EVENT_6__PERIOD))
	{
		Vec2F asteroid_position(MAP_DESTRUCTIBLE__FRAME_SIZE / 10.0f * 4.0f, 0.0f);

		for (size_t id = 0, count = 0; count < asteroids_count; ++id)
		{
			if (asteroids[id].exist)
			{
				if (asteroids[id].GetDistance(Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION)) < MAP_DESTRUCTIBLE__RECTANGLES_SIZE)
				{
					goto SKIP_CENTER_SPAWN;
				}
				++count;
			}
		}

		asteroids_array_mtx.lock();

		AddEntity(Asteroid(
			Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION),
			Vec2F(),
			GenerateRandomInventory(BONUS_BUFF_SHIELD | BONUS_BUFF_TRIPLE, 1, 1, 1, 1),
			ASTEROID_SIZE_MEDIUM));

		for (size_t asteroid = 0; asteroid < 4; ++asteroid)
		{
			AddEntity(Asteroid(
				Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION) + asteroid_position / 8.0f,
				Vec2F(),
				GenerateRandomInventory(BONUS_BUFF_STREAM, 1, 3, 1, 1),
				ASTEROID_SIZE_MEDIUM));

			asteroid_position.PerpendicularThis();
		}

		asteroids_array_mtx.unlock();
	SKIP_CENTER_SPAWN:
		asteroids_array_mtx.lock();
			
		for (size_t asteroid = 0; asteroid < 4; ++asteroid)
		{
			for (size_t id = 0, count = 0; count < asteroids_count; ++id)
			{
				if (asteroids[id].exist)
				{
					if (asteroids[id].GetDistance(Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION) + asteroid_position) < ASTEROID_RADIUS_BIG * 2.0f)
					{
						goto SKIP_EDGE_SPAWN;
					}
					++count;
				}
			}

			AddEntity(Asteroid(
				Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION) + asteroid_position,
				Vec2F(),
				GenerateRandomInventory(BONUS_BONUS | BONUS_RULE_REVERSE, 1, 2, 1, 2),
				ASTEROID_SIZE_BIG));
			
		SKIP_EDGE_SPAWN:
			asteroid_position.PerpendicularThis();
		}

		asteroids_array_mtx.unlock();
	}
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

void Game::Event10()
{
#define EVENT_10__FRAME_SIZE	3.0f
#define EVENT_10__FRAME_POS		6.0f

#define EVENT_10__MAX_SIZE_COEF 0.47f
#define EVENT_10__MIN_SIZE_COEF	0.16f
// from -1.0f (none) to 1.0f (full)
#define EVENT_10__WORK_PART		-0.8f

#define EVENT_10__PERIOD		1000lu

#define EVENT_10__FIRST_POLYGON_ID	1
#define EVENT_10__LAST_POYGON_ID	4

#define EVENT_10__MIN_BONUSES_COUNT 1
#define EVENT_10__MAX_BONUSES_COUNT 3
#define EVENT_10__MIN_BONUSES_TYPES_COUNT	4
#define EVENT_10__MAX_BONUSES_TYPES_COUNT	8

#define EVENT_10__ASTEROID_SPAWN_RADIUS	(EVENT_10__FRAME_SIZE * 0.4f)

	float polygon_size = fmaxf(sinf((float)global_timer / (float)EVENT_10__PERIOD * 2.0f * M_PI) + EVENT_10__WORK_PART, 0.0f) / (1.0f + EVENT_10__WORK_PART);
	polygon_size = (EVENT_10__MIN_SIZE_COEF + polygon_size *	(EVENT_10__MAX_SIZE_COEF - EVENT_10__MIN_SIZE_COEF)) * EVENT_10__FRAME_SIZE;

	Map::Polygon* temp_polygon_p;
	for (size_t polygon = EVENT_10__FIRST_POLYGON_ID; polygon <= EVENT_10__LAST_POYGON_ID; polygon++)
	{
		temp_polygon_p = map.PolygonPointer(polygon);
		temp_polygon_p->size.Set(polygon_size, polygon_size);
		temp_polygon_p->ShouldUpdate();
	}

	if (global_timer % EVENT_10__PERIOD == 0 && object_pull_array[GAME_OBJECT_ASTEROID])
	{
		if (asteroids_count < 4 && bonuses_count < 20)
		{
			AddEntity(
				Asteroid(
					Vec2F(EVENT_10__FRAME_POS),
					Vec2F(0.0f),
					GenerateRandomInventory(
						BONUS_ALL,
						EVENT_10__MIN_BONUSES_COUNT,
						EVENT_10__MAX_BONUSES_COUNT,
						EVENT_10__MIN_BONUSES_TYPES_COUNT,
						EVENT_10__MAX_BONUSES_TYPES_COUNT),
					ASTEROID_SIZE_BIG));
		}
		else
		{
			AddEntity(
				Asteroid(
					Vec2F(EVENT_10__FRAME_POS),
					Vec2F(0.0f),
					BONUS_RULE_REVERSE,
					ASTEROID_SIZE_BIG));
		}
	}

	if (global_timer % (EVENT_10__PERIOD * 5) == 0 && asteroids_count < 4)
	{
		EngineTypes::Bonus::inventory_t inventory = GenerateRandomInventory(
			BONUS_BONUS | BONUS_RULE_REVERSE,
			1, 
			1, 
			1, 
			1);

		AddEntity(Asteroid(
			Vec2F(EVENT_10__FRAME_POS + EVENT_10__ASTEROID_SPAWN_RADIUS),
			Vec2F(0.0f),
			inventory,
			ASTEROID_SIZE_MEDIUM));
		AddEntity(Asteroid(
			Vec2F(EVENT_10__FRAME_POS + EVENT_10__ASTEROID_SPAWN_RADIUS, EVENT_10__FRAME_POS - EVENT_10__ASTEROID_SPAWN_RADIUS),
			Vec2F(0.0f),
			inventory,
			ASTEROID_SIZE_MEDIUM));
		AddEntity(Asteroid(
			Vec2F(EVENT_10__FRAME_POS - EVENT_10__ASTEROID_SPAWN_RADIUS),
			Vec2F(0.0f),
			inventory,
			ASTEROID_SIZE_MEDIUM));
		AddEntity(Asteroid(
			Vec2F(EVENT_10__FRAME_POS - EVENT_10__ASTEROID_SPAWN_RADIUS, EVENT_10__FRAME_POS + EVENT_10__ASTEROID_SPAWN_RADIUS),
			Vec2F(0.0f),
			inventory,
			ASTEROID_SIZE_MEDIUM));
	}
}

#define MAP_COLLAIDER__CENTER			6.0f
#define MAP_COLLAIDER__FRAME_SIZE		4.0f

#define MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS	(MAP_COLLAIDER__FRAME_SIZE / 12.0f)
#define MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS	(MAP_COLLAIDER__FRAME_SIZE / 5.0f)
#define MAP_COLLAIDER__BARIER_LENGTH	(MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS * 2.0f)
#define MAP_COLLAIDER__BARIER_SHIFT		(MAP_COLLAIDER__FRAME_SIZE / 5.0f * 4.0f / 2.0f)

#define MAP_COLLAIDER__RECTANGLES_COUNT	5u
#define MAP_COLLAIDER__CYRCLES_COUNT	2u

#define MAP_COLLAIDER__SPAWN_SHIFT		(MAP_COLLAIDER__FRAME_SIZE * 0.9f / 2.0f)

#define EVENT_11__ASTEROID_SPAWN_PERIOD		200u
#define EVENT_11__DESTROY_PERIOD			500u

void Game::Event11()
{
	if (!((global_timer + 50) % EVENT_11__ASTEROID_SPAWN_PERIOD))
	{
		Vec2F position((MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS + MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS) / 2.0f, 0.0f);
		uint8_t times = rand() % 4;

		for (size_t i = 0; i < times; ++i)
		{
			position.PerpendicularThis();
		}

		AddEntity(Asteroid(
			Vec2F(MAP_COLLAIDER__CENTER) + position,
			position.Perpendicular() / ((MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS + MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS) / 2.0f) * 0.002f,
			GenerateRandomInventory(BONUS_ALL, 0, 1, 0, 1),
			ASTEROID_SIZE_SMALL));
	}

	asteroids_array_mtx.lock();
	for (size_t id = 0, count = 0; count < asteroids_count; ++id)
	{
		if (asteroids[id].exist)
		{
			if (asteroids[id].GetVelocity().LengthPow2() > 0.01f)
			{
				DestroyEntity(&asteroids[id]);
			}
			else
			{
				++count;
			}
		}
	}
	asteroids_array_mtx.unlock();

	if (!((global_timer + 1) % EVENT_11__DESTROY_PERIOD))
	{
		bonuses_array_mtx.lock();
		for (size_t id = 0, count = 0; count < bonuses_count; ++id)
		{
			if (bonuses[id].exist)
			{
				if (bonuses[id].GetDistance(Vec2F(MAP_COLLAIDER__CENTER)) < MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS)
				{
					std::cout << "Event11::Destroy lost bonus" << std::endl;
						DestroyEntity(&bonuses[id]);
				}
				else
				{
					++count;
				}
			}
		}
		bonuses_array_mtx.unlock();
	}
}

#define MAP_KALEIDOSCOPE__CENTER	6.0f
#define MAP_KALEIDOSCOPE__SIZE		4.0f

#define MAP_KALEIDOSCOPE__RECTANGLES_COUNT	1u
#define MAP_KALEIDOSCOPE__POLYGONS_COUNT	2u

#define MAP_KALEIDOSCOPE__SPAWN_SHIFT	(MAP_KALEIDOSCOPE__SIZE * 0.9f / 2.0f)

#define MAP_KALEIDOSCOPE__SHIP_GAP_WIDTH	(MAP_KALEIDOSCOPE__SIZE / 2.0f / 5.0f)

#define MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT	(sqrtf(2.0f) / 2.0f * 0.8f)
#define MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT	(1.0f / 2.0f * 0.8f * 0.8f / sqrt(2.0f))

#define MAP_KALEIDOSCOPE__PORTAL_SHIFT	0.15f

#define MAP_KALEIDOSCOPE__EXTERNAL_FROM_IN_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT + MAP_KALEIDOSCOPE__PORTAL_SHIFT) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__EXTERNAL_TO_IN_PORTAL_COEFFICIENT		((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT - MAP_KALEIDOSCOPE__PORTAL_SHIFT * 3.0f) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__INTERNAL_FROM_IN_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT + MAP_KALEIDOSCOPE__PORTAL_SHIFT) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__INTERNAL_TO_IN_PORTAL_COEFFICIENT		((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT - MAP_KALEIDOSCOPE__PORTAL_SHIFT * 3.0f) / MAP_KALEIDOSCOPE__SIZE)

#define MAP_KALEIDOSCOPE__EXTERNAL_FROM_OUT_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT - MAP_KALEIDOSCOPE__PORTAL_SHIFT) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__EXTERNAL_TO_OUT_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT + MAP_KALEIDOSCOPE__PORTAL_SHIFT * 3.0f) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__INTERNAL_FROM_OUT_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT - MAP_KALEIDOSCOPE__PORTAL_SHIFT) / MAP_KALEIDOSCOPE__SIZE)
#define MAP_KALEIDOSCOPE__INTERNAL_TO_OUT_PORTAL_COEFFICIENT	((MAP_KALEIDOSCOPE__SIZE * MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT + MAP_KALEIDOSCOPE__PORTAL_SHIFT * 3.0f) / MAP_KALEIDOSCOPE__SIZE)

#define EVENT_12__ROTATION_PERIOD	6000
#define EVENT_12__SPAWN_PERIOD		1000

void Game::Event12()
{
	Vec2F points[4];
	points[0].Set(MAP_KALEIDOSCOPE__SIZE / 2.0f);
	for (size_t i = 1; i < 4; ++i)
	{
		points[i] = points[0];
		points[0].PerpendicularThis();
	}

	float angle = (global_timer % EVENT_12__ROTATION_PERIOD) / (float)EVENT_12__ROTATION_PERIOD * M_PI * 2.0f;

	map_data_mtx.lock();
	map.PolygonPointer(0)->SetAngle(M_PI_4 + angle);
	map.PolygonPointer(1)->SetAngle(-angle);
	map_data_mtx.unlock();

	portals_array_mtx.lock();
	for (size_t i = 0; i < 4; ++i)
	{
		portals[i * 4].SetPosition(points[i].Rotate(M_PI_4 + angle) * MAP_KALEIDOSCOPE__EXTERNAL_FROM_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 0 4 8 12
		portals[i * 4].SetTPPosition(points[i].Rotate(M_PI_4 + angle) * MAP_KALEIDOSCOPE__EXTERNAL_TO_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 0 4 8 12

		portals[i * 4 + 1].SetPosition(points[i].Rotate(-angle) * MAP_KALEIDOSCOPE__INTERNAL_FROM_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 1 5 9 13
		portals[i * 4 + 1].SetTPPosition(points[i].Rotate(-angle) * MAP_KALEIDOSCOPE__INTERNAL_TO_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 1 5 9 13

		portals[i * 4 + 2].SetPosition(points[i].Rotate(M_PI_4 + angle) * MAP_KALEIDOSCOPE__EXTERNAL_FROM_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 2 6 10 14
		portals[i * 4 + 2].SetTPPosition(points[i].Rotate(M_PI_4 + angle) * MAP_KALEIDOSCOPE__EXTERNAL_TO_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 2 6 10 14

		portals[i * 4 + 3].SetPosition(points[i].Rotate(-angle) * MAP_KALEIDOSCOPE__INTERNAL_FROM_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 3 7 11 15
		portals[i * 4 + 3].SetTPPosition(points[i].Rotate(-angle) * MAP_KALEIDOSCOPE__INTERNAL_TO_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER)); // 3 7 11 15
	}
	portals_array_mtx.unlock();

	if (!(global_timer % EVENT_12__SPAWN_PERIOD))
	{
		asteroids_array_mtx.lock();

		if (asteroids_count < GAME_ASTEROIDS_MAX_COUNT / 4)
		{
			AddEntity(Asteroid(
				Vec2F(MAP_KALEIDOSCOPE__CENTER),
				Vec2F(1.0f, 0.0f).Rotate(rand() / static_cast<float>(RAND_MAX) * M_PI * 2.0f) * 0.01f,
				GenerateRandomInventory((*(int*)&angle) & 0x30000 ? BONUS_BONUS | BONUS_RULE : BONUS_BUFF, 1, 1, 1, 2),
				ASTEROID_SIZE_MEDIUM,
				0.0f,
				angle / 10.0f));
		}

		asteroids_array_mtx.unlock();
	}
}

void CreateMap0_PlaceTwoCyrclesAndGravGen(Game* game_object, Map::Cyrcle* cyrcles, size_t id, const Vec2F& position, float gravity) {
	cyrcles[id].Set(
		position,
		MAP_ORBIT_MAP__GRAVGEN_KILL_BARIER_RADIUS,
		MAP_PROPERTY_AGRESSIVE | MAP_PROPERTY_KILLER | MAP_PROPERTY_UNBREACABLE);
	cyrcles[id + 1].Set(
		position,
		MAP_ORBIT_MAP__GRAVGEN_SAVE_BARIER_RADIUS,
		MAP_PROPERTY_UNBREACABLE);

	/* Spawn entities */

	game_object->AddEntity(GravGen(position, -gravity, MAP_ORBIT_MAP__GRAVGEN_RADIUS));
}

void CreateMap0_PlaceShips(Vec2F* ships_positions, float* ships_angles) {
	Vec2F center_position(MAP_ORBIT_MAP__CENTER_POSITION);
	Vec2F ship_position = MAP_ORBIT_MAP__RELATIVE_SHIP_POSITION;
	ships_positions[0] = center_position + ship_position.RotateClockwise(-M_PI_2);
	ships_positions[1] = center_position + ship_position;
	ships_positions[2] = center_position + ship_position.RotateClockwise(M_PI_2);
	ships_positions[3] = center_position + ship_position.RotateClockwise(M_PI);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
}

void Game::CreateMap0(Vec2F* ships_positions, float* ships_angles)
{

	Map::Cyrcle* cyrcles = new Map::Cyrcle[MAP_ORBIT_MAP__CYRCLES_COUNT];
	Vec2F position(MAP_ORBIT_MAP__CENTER_POSITION);

	cyrcles[0].Set(
		position,
		MAP_ORBIT_MAP__AREA_RADIUS,
		MAP_PROPERTY_UNBREACABLE);

	cyrcles[1].Set(
		position,
		MAP_ORBIT_MAP__GRAVGEN_KILL_BARIER_RADIUS,
		MAP_PROPERTY_AGRESSIVE | MAP_PROPERTY_KILLER | MAP_PROPERTY_UNBREACABLE);
	
	position += Vec2F(0.0f, MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS);
	CreateMap0_PlaceTwoCyrclesAndGravGen(this, cyrcles, 2, position, -MAP_ORBIT_MAP__GRAVGEN_FORCE);

	position -= Vec2F(0.0f, 2.0f * MAP_ORBIT_MAP__INTERNAL_ORBIT_RADIUS);
	CreateMap0_PlaceTwoCyrclesAndGravGen(this, cyrcles, 4, position, -MAP_ORBIT_MAP__GRAVGEN_FORCE);

	position = Vec2F(MAP_ORBIT_MAP__CENTER_POSITION) + Vec2F(MAP_ORBIT_MAP__EXTERNAL_ORBIT_RADIUS, 0.0f);
	CreateMap0_PlaceTwoCyrclesAndGravGen(this, cyrcles, 6, position, MAP_ORBIT_MAP__GRAVGEN_FORCE);

	position -= Vec2F(2.0f * MAP_ORBIT_MAP__EXTERNAL_ORBIT_RADIUS, 0.0f);
	CreateMap0_PlaceTwoCyrclesAndGravGen(this, cyrcles, 8, position, MAP_ORBIT_MAP__GRAVGEN_FORCE);

	map.Set(nullptr, 0, cyrcles, MAP_ORBIT_MAP__CYRCLES_COUNT, nullptr, 0);

	/* Spawn entities */
	
	AddEntity(GravGen(Vec2F(MAP_ORBIT_MAP__CENTER_POSITION), -MAP_ORBIT_MAP__GRAVGEN_FORCE, MAP_ORBIT_MAP__GRAVGEN_RADIUS));

	CreateMap0_PlaceShips(ships_positions, ships_angles);

	camera.SetHightLimits(
		MAP_ORBIT_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_ORBIT_MAP__CENTER_POSITION - CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_ORBIT_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS,
		MAP_ORBIT_MAP__CENTER_POSITION + CAMERA_DEFAULT_HIGH_LIMITS);
	camera.SetPosition(Vec2F(MAP_ORBIT_MAP__CENTER_POSITION));
	camera.SetSize(MAP_ORBIT_MAP__CAMERA_SIZE);

	delete[] cyrcles;
}

void Game::CreateMap1(Vec2F* ships_positions, float* ships_angles)
{
	/* Init static variables */

	static int event1_spawn_asteroids = 0;

	Vec2F new_position;
	Segment new_segment;

#define MAP_TURRET_ON_CENTER__CENTER_POSITION			0.0f
#define MAP_TURRET_ON_CENTER__FRAME_SIZE				2.0f
#define MAP_TURRET_ON_CENTER__CENTER_SQUARE_SIZE		0.1f
#define MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION	1.5f
#define MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION		1.0f
#define MAP_TURRET_ON_CENTER__CENTER					0.0f
#define MAP_TURRET_ON_CENTER__RECTANGLE_COUNT			6u

#define MAP_TURRET_ON_CENTER__FRAME_RECTANGLE		0u
#define MAP_TURRET_ON_CENTER__CENTER_RECTANGLE		1u
#define MAP_TURRET_ON_CENTER__DOWN_LEFT_RECTANGLE	2u
#define MAP_TURRET_ON_CENTER__UP_RIGHT_RECTANGLE	3u
#define MAP_TURRET_ON_CENTER__UP_LEFT_RECTANGLE		4u
#define MAP_TURRET_ON_CENTER__DOWN_RIGHT_RECTANGLE	5u

	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_TURRET_ON_CENTER__RECTANGLE_COUNT];

	new_segment.Set(
		Vec2F(-MAP_TURRET_ON_CENTER__FRAME_SIZE + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TURRET_ON_CENTER__FRAME_SIZE + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__FRAME_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TURRET_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TURRET_ON_CENTER__CENTER_SQUARE_SIZE + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__CENTER_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__DOWN_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__UP_RIGHT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(-MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION, MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(-MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION, MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__UP_LEFT_RECTANGLE].Set(&new_segment);

	new_segment.Set(
		Vec2F(MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION, -MAP_TURRET_ON_CENTER__RECTANGLE_IN_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		Vec2F(MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION, -MAP_TURRET_ON_CENTER__RECTANGLE_OUT_POSITION + MAP_TURRET_ON_CENTER__CENTER_POSITION),
		true);
	rectangles[MAP_TURRET_ON_CENTER__DOWN_RIGHT_RECTANGLE].Set(&new_segment);

	map.Set(rectangles, MAP_TURRET_ON_CENTER__RECTANGLE_COUNT);

	new_position.Set(MAP_TURRET_ON_CENTER__CENTER, MAP_TURRET_ON_CENTER__CENTER);
	AddEntity(Turret(&new_position, 0.0f, 200u));


	/* Spawn entities */

	ships_positions[0].Set(MAP_TURRET_ON_CENTER__CENTER_POSITION - 1.85f, MAP_TURRET_ON_CENTER__CENTER_POSITION + 1.9f);
	ships_positions[1].Set(MAP_TURRET_ON_CENTER__CENTER_POSITION + 1.9f, MAP_TURRET_ON_CENTER__CENTER_POSITION + 1.85f);
	ships_positions[2].Set(MAP_TURRET_ON_CENTER__CENTER_POSITION + 1.85f, MAP_TURRET_ON_CENTER__CENTER_POSITION - 1.9f);
	ships_positions[3].Set(MAP_TURRET_ON_CENTER__CENTER_POSITION - 1.9f, MAP_TURRET_ON_CENTER__CENTER_POSITION - 1.85f);

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
		DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
		MAP_DECELERATION_AREA__DECELERATION_AREA_RADIUS));

	new_position.Set(-MAP_DECELERATION_AREA__DECELERATION_AREA_POS, -MAP_DECELERATION_AREA__DECELERATION_AREA_POS);
	AddEntity(DecelerationArea(
		&new_position,
		DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT,
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
	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_MEGA_LASERS__RECTANGLES_COUNT];

	Segment diagonal = Segment(
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION + MAP_MEGA_LASERS__FRAME_SIZE),
		Vec2F(MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE, MAP_MEGA_LASERS__CENTER_POSITION - MAP_MEGA_LASERS__FRAME_SIZE),
		true);
	rectangles[MAP_MEGA_LASERS__FRAME_RECTANGLE].Set(&diagonal);

	Vec2F position1(MAP_MEGA_LASERS__RECTANGLE_POINT_1_POSITION);
	Vec2F position2(MAP_MEGA_LASERS__RECTANGLE_POINT_2_POSITION);
	for (size_t i = MAP_MEGA_LASERS__SIDE_RECTANGLE; i < MAP_MEGA_LASERS__SIDE_RECTANGLE + MAP_MEGA_LASERS__LASER_RECTANGLES_COUNT; ++i)
	{
		diagonal.Set(
			Vec2F(MAP_MEGA_LASERS__CENTER_POSITION) + position1,
			Vec2F(MAP_MEGA_LASERS__CENTER_POSITION) + position2,
			true);
		rectangles[i].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);
		position1.PerpendicularThis();
		position2.PerpendicularThis();
	}

	position1 = MAP_MEGA_LASERS__LASER_POINT_1_POSITION;
	position2 = MAP_MEGA_LASERS__LASER_POINT_2_POSITION;
	for (size_t i = MAP_MEGA_LASERS__LASER_RECTANGLE; i < MAP_MEGA_LASERS__LASER_RECTANGLE + MAP_MEGA_LASERS__LASER_RECTANGLES_COUNT; ++i)
	{
		diagonal.Set(
			Vec2F(MAP_MEGA_LASERS__CENTER_POSITION) + position1,
			Vec2F(MAP_MEGA_LASERS__CENTER_POSITION) + position2,
			true);
		rectangles[i].Set(&diagonal, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_AGRESSIVE | MAP_PROPERTY_KILLER);
		position1.PerpendicularThis();
		position2.PerpendicularThis();
	}

	map.Set(rectangles, MAP_MEGA_LASERS__RECTANGLES_COUNT);

	delete[] rectangles;

	/* Spawn entities */

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
	/* Create map */

	Map::Rectangle* rectangles = new Map::Rectangle[MAP_DESTRUCTIBLE__RECTANGLES_COUNT];

	Segment diagonal(Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION - MAP_DESTRUCTIBLE__FRAME_SIZE / 2.0f), Vec2F(MAP_DESTRUCTIBLE__FRAME_SIZE));
	rectangles[MAP_DESTRUCTABLE__FRAME_RECTANGLE_ID].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);

	Vec2F rectangle_position(MAP_DESTRUCTIBLE__FRAME_SIZE / 10.0f * 2.0f);
	for (size_t i = MAP_DESTRUCTABLE__UNBREACABLE_RECTANGLE_ID; i < MAP_DESTRUCTABLE__DESTROYABLE_RECTANGLE_ID; ++i)
	{
		diagonal.Set(rectangle_position - Vec2F(MAP_DESTRUCTIBLE__RECTANGLES_SIZE / 2.0f) + Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION), Vec2F(MAP_DESTRUCTIBLE__RECTANGLES_SIZE));
		rectangles[i].Set(&diagonal, MAP_PROPERTY_UNBREACABLE);
		rectangle_position.PerpendicularThis();
	}

	rectangle_position.Set(MAP_DESTRUCTIBLE__FRAME_SIZE / 10.0f * 2.0f, MAP_DESTRUCTIBLE__FRAME_SIZE / 10.0f * 4.0f);

	std::vector<Vec2F> rectangles_positions = {
		rectangle_position,
		Vec2F(rectangle_position.y, rectangle_position.x),
		Vec2F(MAP_DESTRUCTIBLE__FRAME_SIZE / 10.0f * 2.0f, 0.0f)
	};
	for (size_t group = 0; group < 4; ++group)
	{
		for (size_t position = 0; position < 3; ++position)
		{
			for (size_t rec = 0; rec < 9; ++rec)
			{
				rectangle_position = rectangles_positions[position] - Vec2F(MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLE_POSITION_STEP);
				rectangle_position.x += (rec % 3) * MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLE_POSITION_STEP;
				rectangle_position.y += (rec / 3) * MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLE_POSITION_STEP;

				diagonal.Set(
					rectangle_position - Vec2F(MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLES_SIZE / 2.0f) + Vec2F(MAP_DESTRUCTIBLE__CENTER_POSTION),
					Vec2F(MAP_DESTRUCTIBLE__DESTROYABLE_RECTANGLES_SIZE));

				rectangles[MAP_DESTRUCTABLE__DESTROYABLE_RECTANGLE_ID + group * 9 * 3 + position * 9 + rec].Set(
					&diagonal,
					rec == 4 ? MAP_PROPERTY_KILLER :
					rec % 2 ? MAP_PROPERTY_AGRESSIVE :
					MAP_PROPERTY_NO_PROPERTY);
			}
			rectangles_positions[position].PerpendicularThis();
		}
	}

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

	map.Set(
		rectangles,
		MAP_BROKEN__RECTANGLES_COUNT,
		nullptr, 0,
		polygons,
		MAP_BROKEN__POLYGONS_COUNT);

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

void Game::CreateMap10(Vec2F* ships_positions, float* ships_angles)
{
#define MAP_NO_CENTER__POLYGONS_COUNT	5

#define MAP_NO_CENTER__FRAME 0
#define MAP_NO_CENTER__FRAME_POINTS_COUNT	20
#define MAP_NO_CENTER__FRAME_SIZE					EVENT_10__FRAME_SIZE
#define MAP_NO_CENTER__FRAME_POS					EVENT_10__FRAME_POS
#define MAP_NO_CENTER__FRAME_OUT_SIDE_LENGHT		(MAP_NO_CENTER__FRAME_SIZE * 0.4f)
#define MAP_NO_CENTER__FRAME_DISTANCE_TO_IN_SIDE	(MAP_NO_CENTER__FRAME_SIZE * 0.65f)

#define MAP_NO_CENTER__DESINTEGRATOR_UP		1
#define MAP_NO_CENTER__DESINTEGRATOR_RIGHT	2
#define MAP_NO_CENTER__DESINTEGRATOR_DOWN	3
#define MAP_NO_CENTER__DESINTEGRATOR_LEFT	4
#define MAP_NO_CENTER__DESINTEGRATORS_COUNT	(MAP_NO_CENTER__DESINTEGRATOR_LEFT - MAP_NO_CENTER__DESINTEGRATOR_UP + 1)	
#define MAP_NO_CENTER__DESINTEGRATOR_POINTS_COUNT	3
#define MAP_NO_CENTER__DESINTEGRATOR_SHIFT	(MAP_NO_CENTER__FRAME_OUT_SIDE_LENGHT * 0.95f)
#define MAP_NO_CENTER_DESINTEGRATOR_MIN_SIZE	(EVENT_10__MIN_SIZE_COEF * MAP_NO_CENTER__FRAME_SIZE)
#define MAP_NO_CENTER_DESINTEGRATOR_MAX_SIZE	(EVENT_10__MAX_SIZE_COEF * MAP_NO_CENTER__FRAME_SIZE)

	Map::Polygon* polygons = new Map::Polygon[MAP_NO_CENTER__POLYGONS_COUNT];

	Vec2F* points = new Vec2F[MAP_NO_CENTER__FRAME_POINTS_COUNT];
	points[0].Set(0.0f, 0.0f);
	points[1].Set(MAP_NO_CENTER__FRAME_OUT_SIDE_LENGHT, 0.0f);
	points[2].Set(MAP_NO_CENTER__FRAME_DISTANCE_TO_IN_SIDE, MAP_NO_CENTER__FRAME_DISTANCE_TO_IN_SIDE - MAP_NO_CENTER__FRAME_OUT_SIDE_LENGHT);

	points[3].Set(MAP_NO_CENTER__FRAME_SIZE - points[2].x, points[2].y);
	points[4].Set(MAP_NO_CENTER__FRAME_SIZE - points[1].x, points[1].y);

	for (size_t point = 5; point < MAP_NO_CENTER__FRAME_POINTS_COUNT / 2; point++)
	{
		points[point].Set(MAP_NO_CENTER__FRAME_SIZE - points[point - 5].y, points[point - 5].x);
	}
	for (size_t point = 10; point < MAP_NO_CENTER__FRAME_POINTS_COUNT; point++)
	{
		points[point] = Vec2F(MAP_NO_CENTER__FRAME_SIZE) - points[point - 10];
	}

	polygons[MAP_NO_CENTER__FRAME].Set(
		Vec2F(MAP_NO_CENTER__FRAME_POS - MAP_NO_CENTER__FRAME_SIZE / 2.0f, MAP_NO_CENTER__FRAME_POS + MAP_NO_CENTER__FRAME_SIZE / 2.0f),
		0.0f,
		Vec2F(1.0f),
		points, 
		MAP_NO_CENTER__FRAME_POINTS_COUNT, 
		MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);

	delete[] points;

	points = new Vec2F[MAP_NO_CENTER__DESINTEGRATOR_POINTS_COUNT];

	points[0].Set(0.0f, 0.0f);
	points[1].Set(1.0f, 0.0f);
	points[2].Set(0.0f, 1.0f);

	Vec2F* desintegrators_positions = new Vec2F[MAP_NO_CENTER__DESINTEGRATORS_COUNT];
	desintegrators_positions[MAP_NO_CENTER__DESINTEGRATOR_UP - MAP_NO_CENTER__DESINTEGRATOR_UP].Set(0.0f, MAP_NO_CENTER__DESINTEGRATOR_SHIFT);
	desintegrators_positions[MAP_NO_CENTER__DESINTEGRATOR_RIGHT - MAP_NO_CENTER__DESINTEGRATOR_UP].Set(MAP_NO_CENTER__DESINTEGRATOR_SHIFT, 0.0f);
	desintegrators_positions[MAP_NO_CENTER__DESINTEGRATOR_DOWN - MAP_NO_CENTER__DESINTEGRATOR_UP].Set(0.0f, -MAP_NO_CENTER__DESINTEGRATOR_SHIFT);
	desintegrators_positions[MAP_NO_CENTER__DESINTEGRATOR_LEFT - MAP_NO_CENTER__DESINTEGRATOR_UP].Set(-MAP_NO_CENTER__DESINTEGRATOR_SHIFT, 0.0f);

	for (size_t desintegrator = MAP_NO_CENTER__DESINTEGRATOR_UP; desintegrator <= MAP_NO_CENTER__DESINTEGRATOR_LEFT; desintegrator++)
	{
		polygons[desintegrator].Set(
			desintegrators_positions[desintegrator - MAP_NO_CENTER__DESINTEGRATOR_UP] + Vec2F(MAP_NO_CENTER__FRAME_POS),
			M_PI_4 - M_PI_2 * (desintegrator),
			Vec2F(MAP_NO_CENTER_DESINTEGRATOR_MAX_SIZE),
			points,
			MAP_NO_CENTER__DESINTEGRATOR_POINTS_COUNT,
			MAP_PROPERTY_CLOSED | MAP_PROPERTY_AGRESSIVE | MAP_PROPERTY_KILLER | MAP_PROPERTY_UNBREACABLE);
	}

	delete[] points;
	delete[] desintegrators_positions;
	
	map.Set(
		nullptr, 
		0, 
		nullptr,
		0, 
		polygons, 
		MAP_NO_CENTER__POLYGONS_COUNT);

	delete[] polygons;

	ships_positions[0].Set(MAP_NO_CENTER__FRAME_POS - 1.25f, MAP_NO_CENTER__FRAME_POS + 1.3f);
	ships_positions[1].Set(MAP_NO_CENTER__FRAME_POS + 1.3f, MAP_NO_CENTER__FRAME_POS + 1.25f);
	ships_positions[2].Set(MAP_NO_CENTER__FRAME_POS + 1.25f, MAP_NO_CENTER__FRAME_POS - 1.3f);
	ships_positions[3].Set(MAP_NO_CENTER__FRAME_POS - 1.3f, MAP_NO_CENTER__FRAME_POS - 1.25f);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetPosition(Vec2F(MAP_NO_CENTER__FRAME_POS));
}

void Game::CreateMap11(Vec2F* ships_positions, float* ships_angles)
{
	Map::Rectangle* rectangles = new Map::Rectangle[MAP_COLLAIDER__RECTANGLES_COUNT];
	rectangles[0].Set(Segment(Vec2F(MAP_COLLAIDER__CENTER - MAP_COLLAIDER__FRAME_SIZE / 2.0f), Vec2F(MAP_COLLAIDER__FRAME_SIZE)), MAP_PROPERTY_UNBREACABLE);

	Vec2F point1(MAP_COLLAIDER__BARIER_LENGTH / 2.0f, MAP_COLLAIDER__BARIER_SHIFT + 0.01f);
	Vec2F point2(-MAP_COLLAIDER__BARIER_LENGTH / 2.0f, MAP_COLLAIDER__BARIER_SHIFT - 0.01f);

	for (size_t i = 1; i < 5; ++i)
	{
		rectangles[i].Set(
			Segment(
				Vec2F(MAP_COLLAIDER__CENTER) + point1,
				Vec2F(MAP_COLLAIDER__CENTER) + point2,
				true),
			MAP_PROPERTY_UNBREACABLE);

		point1.PerpendicularThis();
		point2.PerpendicularThis();
	}

	Map::Cyrcle* cyrcles = new Map::Cyrcle[MAP_COLLAIDER__CYRCLES_COUNT];
	cyrcles[0].Set(Vec2F(MAP_COLLAIDER__CENTER), MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_AGRESSIVE);
	cyrcles[1].Set(Vec2F(MAP_COLLAIDER__CENTER), MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS, MAP_PROPERTY_UNBREACABLE, false);

	map.Set(
		rectangles, MAP_COLLAIDER__RECTANGLES_COUNT,
		cyrcles, MAP_COLLAIDER__CYRCLES_COUNT);

	delete[] rectangles;
	delete[] cyrcles;

	AddEntity(DecelerationArea(Vec2F(MAP_COLLAIDER__CENTER), -DEFAULT_FORCE_RESISTANSE_AIR_COEFFICIENT * 1.05f, MAP_COLLAIDER__EXTERNAL_CYRCLE_RADIUS * 0.95f));
	AddEntity(GravGen(Vec2F(MAP_COLLAIDER__CENTER), -GRAVITY_GENERATOR_DEFAULT_GRAVITY, MAP_COLLAIDER__INTERNAL_CYRCLE_RADIUS * 0.9f));

	ships_positions[0].Set(MAP_COLLAIDER__CENTER - MAP_COLLAIDER__SPAWN_SHIFT, MAP_COLLAIDER__CENTER + MAP_COLLAIDER__SPAWN_SHIFT);
	ships_positions[1].Set(MAP_COLLAIDER__CENTER + MAP_COLLAIDER__SPAWN_SHIFT, MAP_COLLAIDER__CENTER + MAP_COLLAIDER__SPAWN_SHIFT);
	ships_positions[2].Set(MAP_COLLAIDER__CENTER + MAP_COLLAIDER__SPAWN_SHIFT, MAP_COLLAIDER__CENTER - MAP_COLLAIDER__SPAWN_SHIFT);
	ships_positions[3].Set(MAP_COLLAIDER__CENTER - MAP_COLLAIDER__SPAWN_SHIFT, MAP_COLLAIDER__CENTER - MAP_COLLAIDER__SPAWN_SHIFT);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_COLLAIDER__CENTER - MAP_COLLAIDER__FRAME_SIZE,
		MAP_COLLAIDER__CENTER - MAP_COLLAIDER__FRAME_SIZE,
		MAP_COLLAIDER__CENTER + MAP_COLLAIDER__FRAME_SIZE,
		MAP_COLLAIDER__CENTER + MAP_COLLAIDER__FRAME_SIZE);

	camera.SetPosition(Vec2F(MAP_COLLAIDER__CENTER));
	camera.SetSize();
}

void Game::CreateMap12(Vec2F* ships_positions, float* ships_angles)
{
	/* Generate map */
	
	Map::Rectangle* rectangles = new Map::Rectangle[MAP_KALEIDOSCOPE__RECTANGLES_COUNT];
	rectangles[0].Set(Segment(Vec2F(MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SIZE / 2.0f), Vec2F(MAP_KALEIDOSCOPE__SIZE)), MAP_PROPERTY_UNBREACABLE);

	Map::Polygon* polygons = new Map::Polygon[MAP_KALEIDOSCOPE__POLYGONS_COUNT];
	Vec2F* points = new Vec2F[4];
	points[0].Set(MAP_KALEIDOSCOPE__SIZE / 2.0f);
	for (size_t i = 1; i < 4; ++i)
	{
		points[i] = points[0];
		points[0].PerpendicularThis();
	}
	polygons[0].Set(Vec2F(MAP_KALEIDOSCOPE__CENTER), M_PI_4, Vec2F(MAP_KALEIDOSCOPE__EXTERNAL_CORNER_COEFFICIENT), points, 4, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);
	polygons[1].Set(Vec2F(MAP_KALEIDOSCOPE__CENTER), 0.0f, Vec2F(MAP_KALEIDOSCOPE__INTERNAL_CORNER_COEFFICIENT), points, 4, MAP_PROPERTY_UNBREACABLE | MAP_PROPERTY_CLOSED);

	/* Create entities */

	Vec2F* decel_positions = new Vec2F[3];
	decel_positions[0].Set(MAP_KALEIDOSCOPE__SIZE / 2.0f - 0.2f, -0.8f);
	decel_positions[1].Set(MAP_KALEIDOSCOPE__SIZE / 2.0f - 0.2f, 0.0f);
	decel_positions[2].Set(MAP_KALEIDOSCOPE__SIZE / 2.0f - 0.2f, 0.8f);

	for (size_t i = 0; i < 4; ++i)
	{
		AddEntity(Portal(
			points[i].Rotate(M_PI_4) * MAP_KALEIDOSCOPE__EXTERNAL_FROM_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			points[i].Rotate(M_PI_4) * MAP_KALEIDOSCOPE__EXTERNAL_TO_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			0.05f)); // 0 4 8 12
		AddEntity(Portal(
			points[i] * MAP_KALEIDOSCOPE__INTERNAL_FROM_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			points[i] * MAP_KALEIDOSCOPE__INTERNAL_TO_IN_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			0.05f)); // 1 5 9 13
		AddEntity(Portal(
			points[i].Rotate(M_PI_4) * MAP_KALEIDOSCOPE__EXTERNAL_FROM_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			points[i].Rotate(M_PI_4) * MAP_KALEIDOSCOPE__EXTERNAL_TO_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			0.05f)); // 2 6 10 14
		AddEntity(Portal(
			points[i] * MAP_KALEIDOSCOPE__INTERNAL_FROM_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			points[i] * MAP_KALEIDOSCOPE__INTERNAL_TO_OUT_PORTAL_COEFFICIENT + Vec2F(MAP_KALEIDOSCOPE__CENTER),
			0.05f)); // 3 7 11 15

		for (size_t des = 0; des < 3; ++des)
		{
			AddEntity(DecelerationArea(decel_positions[des] + Vec2F(MAP_KALEIDOSCOPE__CENTER), -DECELERATION_AREA_DEFAULT_DECELERATION_COEFFICIENT, 0.2f));
			decel_positions[des].PerpendicularThis();
		}
	}

	delete[] points;
	delete[] decel_positions;

	map.Set(rectangles, MAP_KALEIDOSCOPE__RECTANGLES_COUNT, nullptr, 0, polygons, MAP_KALEIDOSCOPE__POLYGONS_COUNT);

	/* Configure players */

	delete[] rectangles;
	delete[] polygons;

	ships_positions[0].Set(MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SPAWN_SHIFT, MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SPAWN_SHIFT);
	ships_positions[1].Set(MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SPAWN_SHIFT, MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SPAWN_SHIFT);
	ships_positions[2].Set(MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SPAWN_SHIFT, MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SPAWN_SHIFT);
	ships_positions[3].Set(MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SPAWN_SHIFT, MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SPAWN_SHIFT);

	ships_angles[0] = -(float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[1] = -(float)M_PI_2 - (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[2] = (float)M_PI_2 + (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;
	ships_angles[3] = (float)M_PI_4 + ALL_MAPS__SPAWN_DELTA_ANGLE;

	camera.SetHightLimits(
		MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SIZE,
		MAP_KALEIDOSCOPE__CENTER - MAP_KALEIDOSCOPE__SIZE,
		MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SIZE,
		MAP_KALEIDOSCOPE__CENTER + MAP_KALEIDOSCOPE__SIZE);

	camera.SetPosition(Vec2F(MAP_KALEIDOSCOPE__CENTER));
	camera.SetSize();
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