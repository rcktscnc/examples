#include <event_manager.h>

util::event<std::string, const std::string&> event_manager::on_start;
util::event<> event_manager::on_test;
util::lambda_event<void, int> event_manager::on_lambda;