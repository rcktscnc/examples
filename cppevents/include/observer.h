#include <iostream>
#include <string>

#include <listener_interface.h>
#include <event_manager.h>

class Observer : public listener_interface<std::string*>
{
public:
    Observer()
    {
        event_manager::on_start.subscribe(this);
    }
    void on_invoke(std::string* arg) override
    {
        std::cout << *arg;
    }
};