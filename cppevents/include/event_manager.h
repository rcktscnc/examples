#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H

#include <event.h>
#include <string>

class event_manager
{
public:
    static event<std::string*> on_start;
};
event<std::string*> event_manager::on_start;

#endif