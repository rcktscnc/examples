#include <event_manager.h>

event<std::string, const std::string&> event_manager::on_start;
event<> event_manager::on_test;
lambda_event<void, int> event_manager::on_lambda;