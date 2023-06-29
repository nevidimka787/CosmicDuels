#pragma once

#include <iostream>
#include <stdint.h>
#include <string>

#define 	CHECK_COMMAND_SYNTAX true

class Command
{
typedef enum {
	// Server commands.

	COMMAND__SPAWN = ((uint8_t)0x01),
	COMMAND__DELETE = ((uint8_t)0x02),
	COMMAND__SET_HISTORY_LIST = ((uint8_t)0x03),
	COMMAND__END_MATCH = ((uint8_t)0x04),
	COMMAND__END_ROUND = ((uint8_t)0x05),
	COMMAND__START_MATCH = ((uint8_t)0x06),
	COMMAND__START_ROUND = ((uint8_t)0x07),
	COMMAND__UPDATE = ((uint8_t)0x08),
	COMMAND__UPDATE_PLAYER_FLAGS = ((uint8_t)0x09),
	COMMNAD__UPDATE_GLOBAL_TIMER = ((uint8_t)0x0A),

	// Client commands.

	COMMAND__NEED_UPDATE = ((uint8_t)0x71),
	COMMAND__UPDATE_PLAYER_FLAGS_ON_SERVER = ((uint8_t)0x72),
	COMMAND__SET_TEAM = ((uint8_t)0x73)
} command_id_t;

typedef enum {
	ENTITY_TYPE__ASTEROID,
	ENTITY_TYPE__BONUS,
	ENTITY_TYPE__BOMB,
	ENTITY_TYPE__BULLET,
	ENTITY_TYPE__KNIFE,
	ENTITY_TYPE__LASER,
	ENTITY_TYPE__PILOT,
	ENTITY_TYPE__SHIP,
	ENTITY_TYPE__TURRET
} entity_types_t;

private:
	size_t command_length;
	void* command_full_data;
public:
	Command();
	// SetHistoryList; EndRound; StartMatch; NeedUpdate;
	Command(command_id_t command_id, size_t command_data_length = 0, void* command_data = nullptr);
	
	// Spawn; EndMatch; UpdatePlayerFlagsOnServer; SetTeam;
	Command(command_id_t command_id, uint8_t first_argument, size_t command_data_length = 0, void* command_data = nullptr);
	
	// Delete; Update; UpdatePlayerFlags;
	Command(command_id_t command_id, uint8_t first_argument, uint32_t second_argument, size_t command_data_length = 0, void* command_data = nullptr);
	
	// UpdateGlobalTimer;
	Command(command_id_t command_id, uint32_t first_argument);
	
	Command(size_t command_length, void* command_full_data);

	void Execute();

	std::string GetStr() const;

	~Command();

	std::string GetName(command_id_t command_id);
	std::string GetName(entity_types_t entity_name);

private:
	std::string ParsHistoryList();
	std::string ParsEntity(entity_types_t entity_type, size_t shift);

	std::string ParsAsteroid(size_t shift);
	std::string ParsBonus(size_t shift);
	std::string ParsBomb(size_t shift);
	std::string ParsBullet(size_t shift);
	std::string ParsKnife(size_t shift);
	std::string ParsLaser(size_t shift);
	std::string ParsPilot(size_t shift);
	std::string ParsShip(size_t shift);
	std::string ParsTurret(size_t shift);

	std::string ParsSpaw();
	std::string ParsDelete();
	std::string ParsSetHistoryList();
	std::string ParsEndMatch();
	std::string ParsEndRound();
	//std::string Pars
};

std::ostream& operator<< (std::ostream& out, Command const& command) {
	//out << command.GetStr() << ;

	return out;
}