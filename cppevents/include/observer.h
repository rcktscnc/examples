#include <iostream>
#include <string>

#include <listener_interface.h>
#include <event_manager.h>

class Observer : public listener_interface<>
{
public:
    int x = 0;
    Observer()
    {
        event_manager::on_test.subscribe(this);
        event_manager::on_lambda.subscribe([this](int a) { x = a; });
    }

    ~Observer()
    {
        event_manager::on_test.unsubscribe(this);
    }
    
    void on_invoke() override
    {
        std::cout << "INVOKE CALLED!\n";
    }
};