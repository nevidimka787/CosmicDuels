#pragma once

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"
#include <iostream>

using namespace EngineTypes::Log;

class Logs;

class Logs
{
private:
	//It is log's data array.
	data_t* data;
	//It is count of data's cells in the current log.
	length_t cells_count;
public:
	Logs();
	Logs(const Logs& log);
	Logs(
		data_t* data,
		length_t data_length);

	void Clear();
	length_t GetCellsCount();
	bool HaveData();
	//The function pushs the data's cell in the end of log's data array.
	void PushToEnd(data_t data);
	//The function pops data from start of the log's data array.
	data_t PopFromStart();
	void Set(Logs* log);
	void Set(
		data_t* data,
		length_t data_length);

	void operator=(Logs log);

	~Logs();
};