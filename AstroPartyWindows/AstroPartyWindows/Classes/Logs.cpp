#include "Logs.h"

#pragma warning(disable : 6385)
#pragma warning(disable : 26451)

Logs::Logs() :
	data(std::queue<data_t>())
{
}

Logs::Logs(const Logs& log) : data(log.data)
{
}

Logs::Logs(const std::queue<data_t>& data) : data(data)
{
}

Logs::Logs(const std::vector<data_t>& data)
{
	this->data = std::queue<data_t>();
	for (const auto& cell : data)
	{
		this->data.push(cell);
	}
}

void Logs::Clear()
{
	data = std::queue<data_t>();
}

data_t Logs::GenerateLog(const data_t& head, const data_t& ship, const data_t& team, const data_t score_action)
{
	return
		((head & LOG_MASK_BITS) << LOG_HEAD		) |
		((ship & LOG_MASK_BITS) << LOG_DATA_SHIP) |
		((team & LOG_MASK_BITS) << LOG_DATA_TEAM) |
		((score_action & 1)		<< LOG_DATA_SCORE);
}

length_t Logs::GetCellsCount() const
{
	return data.size();
}

bool Logs::HaveData() const
{
	return !data.empty();
}

void Logs::PushToEnd(const data_t& data)
{
	this->data.push(data);
}

data_t Logs::PopFromStart()
{
	if (data.empty()) return LOG_NULL;
	const auto front = data.front();
	data.pop();
	return front;
}

void Logs::ParsLog(const data_t& log, data_t& head, data_t& ship, data_t& team, data_t& score_actions)
{
	head = (log >> LOG_HEAD) & LOG_MASK_BITS;
	ship = (log >> LOG_DATA_SHIP) & LOG_MASK_BITS;
	team = (log >> LOG_DATA_TEAM) & LOG_MASK_BITS;
	score_actions = (log >> LOG_DATA_SCORE) & 1;
}

void Logs::operator=(const Logs& log)
{
	data = this->data;
}

Logs::~Logs()
{
}