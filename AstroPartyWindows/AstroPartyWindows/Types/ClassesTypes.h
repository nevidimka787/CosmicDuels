#pragma once
#include <iostream>

namespace ClassTypes
{
	namespace AgressiveEntity
	{
		typedef uint8_t shoots_count_t;
	}

	namespace Bonus
	{
		typedef uint16_t bonus_t;
	}

	namespace Knife
	{
		typedef uint8_t knife_health_t;
	}

	namespace Rectangle
	{
		typedef uint8_t sides_t;
	}

	namespace Menu
	{
		typedef uint8_t buttons_count_t;
	}

	namespace Button
	{
		typedef uint32_t button_id_t;
		typedef uint8_t button_status_t;
		typedef int32_t button_text_t;
		typedef uint16_t text_length_t;
		typedef uint8_t text_size_t;
	}

	namespace Polygon
	{
		typedef uint32_t points_array_length_t;
	}

	namespace Map
	{
		typedef uint8_t elements_array_length_t;
	}
}