#ifndef ___EVENT_MANAGER_H___
#define ___EVENT_MANAGER_H___

#include <string>
#include <util/event.h>
#include <util/lambda_event.h>

class event_manager
{
public:
    static util::event<std::string, const std::string&> on_start;
    static util::event<> on_test;

    static util::lambda_event<void, int> on_lambda;
};

#endif