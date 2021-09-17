#pragma once
#include <iostream>

namespace GameTypes
{
	//typedefs

#define PLAYERS_COUNT_T_MAX			UINT8_MAX
#define ENTITIES_COUNT_T_MAX		UINT16_MAX
#define MAP_ELEMENTS_COUNT_T_MAX	UINT8_MAX
#define MAPS_COUNT_T_MAX			UINT16_MAX
#define GAME_RULES_T_MAX			UINT32_MAX
#define SCORE_T_MAX					UINT8_MAX
#define OBJECTS_TYPE_COUNT_T_MAX	UINT8_MAX

#define PLAYERS_COUNT_T_MIN			0ui8
#define ENTITIES_COUNT_T_MIN		0ui16
#define MAP_ELEMENTS_COUNT_T_MIN	0ui8
#define MAPS_COUNT_T_MIN			0ui16
#define GAME_RULES_T_MIN			0ui32
#define SCORE_T_MIN					0ui8
#define OBJECTS_TYPE_COUNT_T_MIN	0ui8

	//typedefs

	typedef uint8_t players_count_t;
	typedef uint16_t entities_count_t;
	typedef uint8_t entities_types_count_t;
	typedef uint8_t map_elements_count_t;
	typedef uint16_t maps_count_t;
	typedef uint32_t game_rules_t;
	typedef int8_t score_t;
	typedef uint32_t tic_t;
	typedef uint8_t objects_types_count_t;
	//Size of element in bytes.
	typedef uint16_t element_size_t;
}