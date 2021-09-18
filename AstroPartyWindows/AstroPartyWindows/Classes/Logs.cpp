#include "Logs.h"

#pragma warning(disable : 6385)
#pragma warning(disable : 26451)

Logs::Logs() :
	data(nullptr),
	cells_count(0)
{
}

Logs::Logs(const Logs& log) :
	cells_count(log.cells_count)
{
	if (log.cells_count > 0)
	{
		data = new data_t[cells_count];
		return;
	}
	data = nullptr;
}

Logs::Logs(data_t* data, length_t data_length) :
	cells_count(data_length)
{
	if (cells_count > 0)
	{
		this->data = new data_t[cells_count];

		for (length_t i = 0; i < cells_count; i++)
		{
			this->data[i] = data[i];
		}
		return;
	}
	this->data = nullptr;
}

void Logs::Clear()
{
	if (cells_count > 0)
	{
		delete[] data;
		data = nullptr;
		cells_count = 0;
		return;
	}
	data = nullptr;
}

length_t Logs::GetCellsCount()
{
	return cells_count;
}

bool Logs::HaveData()
{
	return cells_count > 0;
}

void Logs::PushToEnd(data_t data)
{
	if (cells_count > 0)
	{
		data_t* new_data = new data_t[cells_count + 1];
		for (length_t i = 0; i < cells_count; i++)
		{
			new_data[i] = this->data[i];
		}
		new_data[cells_count] = data;
		cells_count++;
		delete[] this->data;
		this->data = new_data;
		return;
	}
	this->data = new data_t[1];
	cells_count = 1;
	this->data[0] = data;
}

data_t Logs::PopFromStart()
{
	if (cells_count > 0)
	{
		data_t return_data = data[0];

		data_t* new_data = new data_t[--cells_count];
		for (length_t i = 0; i < cells_count; i++)
		{
			new_data[i] = data[i + 1];
		}
		delete[] data;
		data = new_data;
		return return_data;
	}
	return LOG_NULL;
}

void Logs::Set(Logs* log)
{
	if (cells_count > 0)
	{
		delete[] data;
	}
	cells_count = log->cells_count;
	if (cells_count > 0)
	{
		data = new data_t[cells_count];
		for (length_t i = 0; i < cells_count; i++)
		{
			data[i] = log->data[i];
		}
		return;
	}
	data = nullptr;
}

void Logs::Set(data_t* data, length_t data_length)
{
	if (this->cells_count > 0)
	{
		delete[] this->data;
	}
	this->cells_count = data_length;
	if (data_length > 0)
	{
		this->data = new data_t[data_length];
		for (length_t i = 0; i < data_length; i++)
		{
			this->data[i] = data[i];
		}
		return;
	}
	this->data = nullptr;
}

void Logs::operator=(Logs log)
{
	if (cells_count > 0)
	{
		delete[] data;
	}
	cells_count = log.cells_count;
	if (cells_count > 0)
	{
		data = new data_t[cells_count];
		for (length_t i = 0; i < cells_count; i++)
		{
			data[i] = log.data[i];
		}
		return;
	}
	data = nullptr;
}

Logs::~Logs()
{
	if (cells_count > 0)
	{
		delete[] data;
		return;
	}
}