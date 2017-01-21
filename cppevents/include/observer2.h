#include <iostream>
#include <string>

#include <listener_interface.h>
#include <event_manager.h>

class Observer2 : public listener_interface<const std::string&>
{
public:
    Observer2() : listener_interface(event_manager::on_start)
    {
        //event_manager::on_start.subscribe(this);
    }
    void on_invoke(const std::string& arg) override
    {
        std::cout << arg;
    }
};