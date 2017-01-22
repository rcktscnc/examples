#include <iostream>
#include <string>

#include <subject.h>

class Observer2
{
public:
    Observer2()
    {
        auto f = [](std::string arg) { return arg; };
        subscription_id = Subject::on_request.subscribe(f);
    }

    ~Observer2()
    {
        Subject::on_request.unsubscribe(subscription_id);
    }
private:
    std::size_t subscription_id;
};