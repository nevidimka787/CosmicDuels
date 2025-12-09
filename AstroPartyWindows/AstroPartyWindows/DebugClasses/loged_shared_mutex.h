#pragma once

#include <iostream>
#include <shared_mutex>
#include <vector>

class logged_shared_mutex : public std::shared_mutex
{
private:
	static std::shared_mutex logged_shared_mutex_add_log_mtx;
	static std::vector<std::string> lock_log;
	static size_t log_id;
	static int log_len;
public:
	std::string name;
	logged_shared_mutex(const std::string& name) : name(name), shared_mutex() {}
	logged_shared_mutex(const logged_shared_mutex& mtx) : name(name) {}
	void lock()
	{
		logged_shared_mutex_add_log_mtx.lock();
		log_id %= log_len;
		lock_log[log_id] = name + " try lock";
		++log_id;
		logged_shared_mutex_add_log_mtx.unlock();
		shared_mutex::lock();
		logged_shared_mutex_add_log_mtx.lock();
		lock_log[log_id - 1].append(" success");
		logged_shared_mutex_add_log_mtx.unlock();
	}
	void unlock()
	{
		logged_shared_mutex_add_log_mtx.lock();
		log_id %= log_len;
		lock_log[log_id] = name + " unlock";
		++log_id;
		logged_shared_mutex_add_log_mtx.unlock();
		shared_mutex::unlock();
	}
	const logged_shared_mutex& operator=(const logged_shared_mutex& mtx)
	{
		name = mtx.name;
		return *this;
	}
	void printLogs()
	{
		logged_shared_mutex_add_log_mtx.lock();
		for (size_t i = 0; i < log_len; ++i)
		{
			std::cout << lock_log[(i + log_id) % log_len] << std::endl;
		}
		logged_shared_mutex_add_log_mtx.unlock();
	}
};

