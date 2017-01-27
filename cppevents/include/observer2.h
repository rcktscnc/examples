#include <iostream>
#include <string>

#include <subject.h>

class Observer2
{
public:
    Observer2()
    {
        auto f = [](const std::string& arg) { return arg; };
        subscription = Subject::subscribe(f);
    }
    
    ~Observer2()
    {
        Subject::unsubscribe(subscription);
    }
    
private:
    util::subscription subscription;
};