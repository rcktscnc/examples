#include <iostream>
#include <string>

#include <event_manager.h>

class Observer
{
public:
    int x = 0;
    Observer()
    {
        event_manager::on_lambda.subscribe([this](int a) { x = a; });
    }

    ~Observer()
    {
        //event_manager::on_lambda.unsubscribe(subscription_id);
    }

private:

};