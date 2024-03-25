#pragma once

#include <iostream>
#include <shared_mutex>
#include <vector>

class loged_shared_mutex : public std::shared_mutex
{
private:
	static std::shared_mutex loged_shared_mutex_add_log_mtx;
	static std::vector<std::string> lock_log;
	static size_t log_id;
	static int log_len;
public:
	std::string name;
	loged_shared_mutex(const std::string& name) : name(name), shared_mutex() {}
	loged_shared_mutex(const loged_shared_mutex& mtx) : name(name) {}
	void lock()
	{
		loged_shared_mutex_add_log_mtx.lock();
		log_id %= log_len;
		lock_log[log_id] = name + " try lock";
		++log_id;
		loged_shared_mutex_add_log_mtx.unlock();
		shared_mutex::lock();
		loged_shared_mutex_add_log_mtx.lock();
		lock_log[log_id - 1].append(" success");
		loged_shared_mutex_add_log_mtx.unlock();
	}
	void unlock()
	{
		loged_shared_mutex_add_log_mtx.lock();
		log_id %= log_len;
		lock_log[log_id] = name + " unlock";
		++log_id;
		loged_shared_mutex_add_log_mtx.unlock();
		shared_mutex::unlock();
	}
	const loged_shared_mutex& operator=(const loged_shared_mutex& mtx)
	{
		name = mtx.name;
	}
	void printLogs()
	{
		loged_shared_mutex_add_log_mtx.lock();
		for (size_t i = 0; i < log_len; ++i)
		{
			std::cout << lock_log[(i + log_id) % log_len] << std::endl;
		}
		loged_shared_mutex_add_log_mtx.unlock();
	}
};

