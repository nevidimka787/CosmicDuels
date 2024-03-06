#if defined(_WIN32) || defined(_WIN64)
#define memcpy(dst, src, len) memcpy_s(dst, len, src, len)
#endif // _WIN32 || _WIN64

#include "Commands.h"

#include <cstring>
#include <stdint.h>
#include <iostream>

#define COMMAND_ID_LENGTH		1
#define COMMAND_8_BIT_LENGTH	1
#define COMMAND_16_BIT_LENGTH	2
#define	COMMAND_32_BIT_LENGTH	4

Command::Command()
{
#if CHECK_COMMAND_SYNTAX == true
	std::cout << "ERROR::Command::Empty command." << std::endl;
#endif // CHECK_COMMAND_SYNTAX == true

	command_full_data = nullptr;
	command_length = 0;
}

Command::Command(command_id_t command_id, size_t command_data_length, void* command_data)
{
#if CHECK_COMMAND_SYNTAX == true
	if (
		command_id != COMMAND__END_ROUND		&&
		command_id != COMMAND__SET_HISTORY_LIST	&&
		command_id != COMMAND__START_MATCH		&&
		command_id != COMMAND__NEED_UPDATE)
	{
		std::cout << "ERROR::Command::Command " << GetName(command_id) << "isn't available is this function overload." << std::endl;
	}
#endif // CHECK_COMMAND_SYNTAX == true

	command_full_data = malloc(COMMAND_ID_LENGTH + command_data_length);

	*(uint8_t*)command_full_data = command_id;

	std::memcpy(
		(uint8_t*)command_full_data + COMMAND_ID_LENGTH,
		command_data,
		command_data_length);
}

Command::Command(command_id_t command_id, uint8_t first_argument, size_t command_data_length, void* command_data)
{
#if CHECK_COMMAND_SYNTAX == true
	if (
		command_id != COMMAND__END_MATCH						&&
		command_id != COMMAND__UPDATE_PLAYER_FLAGS_ON_SERVER	&&
		command_id != COMMAND__SET_TEAM)
	{
		std::cout << "ERROR::Command::Command " << GetName(command_id) << "isn't available is this function overload." << std::endl;
	}
#endif // CHECK_COMMAND_SYNTAX == true

	command_full_data = malloc(COMMAND_ID_LENGTH + COMMAND_8_BIT_LENGTH + command_data_length);

	*(uint8_t*)command_full_data = command_id;
	((uint8_t*)command_full_data)[COMMAND_ID_LENGTH] = first_argument;

	std::memcpy(
		(uint8_t*)command_full_data + COMMAND_ID_LENGTH + COMMAND_8_BIT_LENGTH,
		command_data,
		command_data_length);
}

Command::Command(command_id_t command_id, uint8_t first_argument, uint32_t second_argument, size_t command_data_length, void* command_data)
{
#if CHECK_COMMAND_SYNTAX == true
	if (
		command_id != COMMAND__DELETE &&
		command_id != COMMAND__UPDATE_PLAYER_FLAGS &&
		command_id != COMMAND__UPDATE)
	{
		std::cout << "ERROR::Command::Command " << GetName(command_id) << "isn't available is this function overload." << std::endl;
	}
#endif // CHECK_COMMAND_SYNTAX == true

	command_full_data = malloc(COMMAND_ID_LENGTH + COMMAND_8_BIT_LENGTH + COMMAND_32_BIT_LENGTH + command_data_length);

	*(uint8_t*)command_full_data = command_id;
	((uint8_t*)command_full_data)[COMMAND_ID_LENGTH] = first_argument;

/*	Actions numbers:
	5     4          1             0         2                   3                       5                */
	*(uint32_t*)((uint8_t*)command_full_data + COMMAND_ID_LENGTH + COMMAND_8_BIT_LENGTH) = second_argument;

	std::memcpy(
		(uint8_t*)command_full_data + COMMAND_ID_LENGTH + COMMAND_8_BIT_LENGTH + COMMAND_32_BIT_LENGTH,
		command_data,
		command_data_length);
}

Command::Command(command_id_t command_id, uint32_t first_argument)
{
#if CHECK_COMMAND_SYNTAX == true
	if (
		command_id != COMMAND__END_MATCH &&
		command_id != COMMAND__UPDATE_PLAYER_FLAGS_ON_SERVER &&
		command_id != COMMAND__SET_TEAM)
	{
		std::cout << "ERROR::Command::Command " << GetName(command_id) << "isn't available is this function overload." << std::endl;
	}
#endif // CHECK_COMMAND_SYNTAX == true

	command_full_data = malloc(COMMAND_ID_LENGTH + COMMAND_32_BIT_LENGTH);

	*(uint8_t*)command_full_data = command_id;
	*(uint32_t*)((uint8_t*)command_full_data + COMMAND_ID_LENGTH) = first_argument;
}


Command::Command(size_t command_length, void* command_full_data) : command_length(command_length)
{
	this->command_full_data = malloc(command_length);

	std::memcpy(
		this->command_full_data,
		command_full_data,
		command_length
	);
}

void Command::Execute()
{

}

std::string Command::GetStr() const
{
	switch (*(uint8_t*)command_full_data)
	{
	case COMMAND__SPAWN:
		return "spawn";
	case COMMAND__DELETE:
		return "delete";
	case COMMAND__SET_HISTORY_LIST:		// 0 args
		return "set_history_list";
	case COMMAND__END_MATCH:
		return "end_match";
	case COMMAND__END_ROUND:			// 0 args
		return "end_round";
	case COMMAND__START_MATCH:			// 0 args
		return "start_match";
	case COMMAND__START_ROUND:
		return "start_round";
	case COMMAND__UPDATE:
		return "update";
	case COMMAND__UPDATE_PLAYER_FLAGS:
		return "update_player_flags";
	case COMMNAD__UPDATE_GLOBAL_TIMER:
		return "update_global_timer";
	case COMMAND__NEED_UPDATE:			// 0 args
		return "need_update";
	case COMMAND__UPDATE_PLAYER_FLAGS_ON_SERVER:
		return "update_player_flags_on_server";
	case COMMAND__SET_TEAM:
		return "set_team";
	default:
		return "<unknow_command>";
	}
}

Command::~Command()
{
	if (command_full_data)
	{
		free(command_full_data);
	}
}

std::string Command::GetName(command_id_t command_id)
{
	switch (command_id)
	{
	case COMMAND__SPAWN:				return "spawn";
	case COMMAND__DELETE:				return "delete";
	case COMMAND__SET_HISTORY_LIST:		return "set_history_list";
	case COMMAND__END_MATCH:			return "end_match";
	case COMMAND__END_ROUND:			return "end_round";
	case COMMAND__START_MATCH:			return "start_match";
	case COMMAND__START_ROUND:			return "start_round";
	case COMMAND__UPDATE:				return "update";
	case COMMAND__UPDATE_PLAYER_FLAGS:	return "update_player_flags";
	case COMMNAD__UPDATE_GLOBAL_TIMER:	return "update_global_timer";
	case COMMAND__NEED_UPDATE:			return "need_update";
	case COMMAND__UPDATE_PLAYER_FLAGS_ON_SERVER: return "update_player_flags_on_server";
	case COMMAND__SET_TEAM:				return "set_team";
	default:							return "<unknow_command>";
	}
}

std::string Command::GetName(entity_types_t entity_type)
{
	switch (entity_type)
	{
	case ENTITY_TYPE__ASTEROID:	return "asteroid";
	case ENTITY_TYPE__BONUS:	return "bonus";
	case ENTITY_TYPE__BOMB:		return "bomb";
	case ENTITY_TYPE__BULLET:	return "bullet";
	case ENTITY_TYPE__KNIFE:	return "knife";
	case ENTITY_TYPE__LASER:	return "laser";
	case ENTITY_TYPE__PILOT:	return "pilot";
	case ENTITY_TYPE__SHIP:		return "ship";
	case ENTITY_TYPE__TURRET:	return "turret";
	default:					return "<unknow_type>";
	}
}
