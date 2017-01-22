#include <iostream>
#include <string>

#include <listener_interface.h>
#include <event_manager.h>

class Observer
{
public:
    int x = 0;
    Observer()
    {
        subscription_id = event_manager::on_lambda.subscribe([this](int a) { x = a; });
    }

    ~Observer()
    {
        event_manager::on_lambda.unsubscribe(subscription_id);
    }

private:
    std::size_t subscription_id;
};