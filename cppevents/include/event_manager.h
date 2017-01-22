#ifndef ___EVENT_MANAGER_H___
#define ___EVENT_MANAGER_H___

#include <event.h>
#include <string>
#include <lambda_event.h>
class event_manager
{
public:
    static event<std::string, const std::string&> on_start;
    static event<> on_test;

    static lambda_event<void, int> on_lambda;
};

#endif