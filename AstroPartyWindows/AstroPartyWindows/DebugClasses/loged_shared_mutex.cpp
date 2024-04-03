#include "loged_shared_mutex.h"

int loged_shared_mutex::log_len = 20;
std::vector<std::string> loged_shared_mutex::lock_log = std::vector<std::string>(log_len, "");
size_t loged_shared_mutex::log_id = 0;
std::shared_mutex loged_shared_mutex::loged_shared_mutex_add_log_mtx;
