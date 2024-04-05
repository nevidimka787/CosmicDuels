#pragma once

#include "../Types/AllTypes.h"
#include "../Constants/AllConstants.h"
#include <iostream>
#include <queue>

using namespace EngineTypes::Log;

class Logs;

class Logs
{
private:
	//It is log's data array.
	std::queue<data_t> data;
public:
	Logs();
	Logs(const Logs& log);
	Logs(const std::queue<data_t>& data);
	Logs(const std::vector<data_t>& data);

	void Clear();
	static data_t GenerateLog(const data_t& head, const data_t& ship, const data_t& team, const data_t score_action);
	length_t GetCellsCount() const;
	bool HaveData() const;
	//The function pushs the data's cell in the end of log's data array.
	void PushToEnd(const data_t& data);
	//The function pops data from start of the log's data array.
	data_t PopFromStart();
	static void ParsLog(const data_t& log, data_t& head, data_t& ship, data_t& team, data_t& score_actions);

	void operator=(const Logs& log);

	~Logs();
};