#include "GameRealisation.h"
#include <random>
#pragma warning(disable : 26451)//All integer operations can be overflow. It is absolutly useless warnintg.

void Game::GameInit()
{
	sheeps = (Sheep**)malloc(sizeof(Sheep*) * GAME_PLAYERS_MAX_COUNT);
	pilots = (Pilot**)malloc(sizeof(Pilot*) * GAME_PLAYERS_MAX_COUNT);

	bullets = (DynamicEntity**)malloc(sizeof(DynamicEntity*) * GAME_BULLETS_MAX_COUNT);
	bullets_count = 0;
	particles = (DynamicEntity**)malloc(sizeof(DynamicEntity*) * GAME_PARTICLES_MAX_COUNT);
	particles_count = 0;
	asteroids = (Asteroid**)malloc(sizeof(Asteroid*) * GAME_ASTEROIDS_MAX_COUNT);
	asterooids_count = 0;
	bonuses = (Bonus**)malloc(sizeof(Bonus*) * GAME_BONUSES_MAX_COUNT);
	bonuses_count = 0;

	srand((unsigned int)sheeps * (unsigned int)pilots);
}

void Game::MachInit(uint8_t players_count, uint32_t game_rules, uint16_t start_bonus)
{
	if (players_count > GAME_PLAYERS_MAX_COUNT)
	{
		Game::players_count = GAME_PLAYERS_MAX_COUNT;
	}
	else if (players_count < 2)
	{
		Game::players_count = 2;
	}
	else
	{
		Game::players_count = players_count;
	}
	shoot_keys = new bool[Game::players_count];
	rotate_keys = new bool[Game::players_count];
	scores = new uint8_t[Game::players_count];
	for (uint8_t i = 0; i < Game::players_count; i++)
	{
		scores[i] = 0;
	}


	Game::start_bonus = game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_SHIELD ? BUFF_SHIELD : 0x0000;
	Game::start_bonus |= game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_TRIPLE_BONUS ? BUFF_TRIPLE : 0x0000;
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)
	{
		if (game_rules & GAME_RULE_START_BONUS_RANDOMIZE)
		{
			Game::start_bonus |= GenerateRandomBonus();
		}
		else
		{
			Game::start_bonus |= start_bonus;
		}
	}

	current_map_id = 0;
}

void Game::LevelInit(uint8_t map_id)
{
	Vec2F* temp_points;
	Rectangle* temp_rectangles;

	const uint8_t max_score = GetMaxScore();

	switch (map_id)
	{
	case MAP_TEST_MAP:
		/* Create map */
		temp_points = new Vec2F[2];
		temp_points[0] = Vec2F(1.0f, 1.0f);
		temp_points[1] = Vec2F(2.0f - 1.0f / (float)((uint32_t)1 << 23), 2.0f - 1.0f / (float)((uint32_t)1 << 23));
		temp_rectangles = new Rectangle[1];
		temp_rectangles[0] = Rectangle(&temp_points[0], &temp_points[1]);

		map = new Map(temp_rectangles, 1, nullptr, 0, nullptr, 0, &temp_points[0]);

		/*Spawn entities*/
		
		temp_points = new Vec2F[Game::players_count];
		temp_points[0] = Vec2F(1.1f, 1.1f);
		temp_points[1] = Vec2F(1.9f, 1.9f);
		if (Game::players_count > 2)
		{
			temp_points[2] = Vec2F(1.1f, 1.9f);
		}
		if (Game::players_count > 3)
		{
			temp_points[3] = Vec2F(1.9f, 1.1f);
		}
		if (game_rules & GAME_RULE_PLAYERS_SPAWN_POSITION_RANDOMIZE)
		{
			for (uint8_t i = 0; i < RANDOM_ITERATIONS_COUNT; i++)
			{
				uint8_t temp1 = rand() % Game::players_count;
				uint8_t temp2 = rand() % Game::players_count;
				Vec2F temp3 = temp_points[temp1];
				temp_points[temp1] = temp_points[temp2];
				temp_points[temp2] = temp3;
			}
		}

		break;
	default:
		break;
	}


	float* temp_angles = new float[Game::players_count];
	temp_angles[0] = -(float)M_PI_4;
	temp_angles[1] = (float)M_PI - (float)M_PI_4;
	if (Game::players_count > 2)
	{
		temp_angles[2] = (float)M_PI_4;
	}
	if (Game::players_count > 3)
	{
		temp_angles[3] = (float)M_PI + (float)M_PI_4;
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_DIRECTION_RANDOMIZE)
	{
		for (uint8_t i = 0; i < Game::players_count; i++)
		{
			int temp1 = rand();
			float temp2 = 1.0f;
			float* temp1_pointer = (float*)&temp1;
			int* temp2_pointer = (int*)&temp2;
			temp1 &= 0x007FFFFF;					//0000 0000 0111 1111 1111 1111 1111 1111 - mantisa
			temp1 |= *temp2_pointer & 0xFF800000;	//1111 1111 1000 0000 0000 0000 0000 0000
			temp_angles[i] += (*temp1_pointer - 1.0f) * 2.0 * (float)M_PI;
		}
	}
	if (game_rules & GAME_RULE_PLAYERS_SPAWN_THIS_BONUS)
	{
		for (uint8_t i = 0; i < Game::players_count; i++)
		{
			Vec2F temp_vel = Vec2F();
			if (game_rules & GAME_RULE_BALANCE_ACTIVE && scores[i] <= max_score - BALANCE_ACTIVATE_DIFFERENCE_SCORES)
			{
				sheeps[i] = new Sheep(i, (void*)&rotate_keys[i], (void*)&shoot_keys[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus | BUFF_SHIELD);
			}
			else
			{
				sheeps[i] = new Sheep(i, (void*)&rotate_keys[i], (void*)&shoot_keys[i], &temp_points[i], &temp_vel, temp_angles[i], 0.0f, start_bonus);
			}
		}
	}

	current_map_id = map_id;
}

void Game::Recalculate()
{

}

uint8_t Game::GenerateRandomBonus()
{
	return 1 << (((rand() % BONUSES_COUNT)) * 2);
}

uint8_t Game::GetMaxScore()
{
	uint8_t max_score = scores[0];
	for (uint8_t i = 1; i < players_count; i++)
	{
		if (max_score < scores[i])
		{
			max_score = scores[i];
		}
	}
	return max_score;
}