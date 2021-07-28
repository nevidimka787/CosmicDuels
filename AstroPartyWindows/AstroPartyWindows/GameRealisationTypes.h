#ifndef GAME_REALISATION_TYPES_H
#define GAME_REALISATION_TYPES_H

#include <iostream>

namespace Game
{
	//typedefs

#define PLAYERS_COUNT_T_MAX			UINT8_MAX
#define ENTITIES_COUNT_T_MAX		UINT16_MAX
#define MAP_ELEMENTS_COUNT_T_MAX	UINT8_MAX
#define MAPS_COUNT_T_MAX			UINT16_MAX
#define GAME_RULES_T_MAX			UINT32_MAX
#define SCORE_T_MAX					UINT8_MAX

#define PLAYERS_COUNT_T_MIN			0u
#define ENTITIES_COUNT_T_MIN		0u
#define MAP_ELEMENTS_COUNT_T_MIN	0u
#define MAPS_COUNT_T_MIN			0u
#define GAME_RULES_T_MIN			0u
#define SCORE_T_MIN					0u

	//typedefs

	typedef typename uint8_t players_count_t;
	typedef typename uint16_t entities_count_t;
	typedef typename uint8_t map_elements_count_t;
	typedef typename uint16_t maps_count_t;
	typedef typename uint32_t game_rules_t;
	typedef typename uint8_t score_t;
	typedef typename uint32_t tic_t;
};

#endif //GAME_REALISATION_TYPES_H