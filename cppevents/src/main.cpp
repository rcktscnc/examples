#include <iostream>
#include <string>

#include <observer.h>
#include <observer2.h>
#include <event_manager.h>
#include <util/lambda_event.h>
#include <subject.h>
#include <util/clock.h>

#include <sstream>

namespace patch
{
    template <typename T>
    std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

int fib(int x)
{
    if (x <= 1)
        return x;

    return fib(x - 1) + fib(x - 2);
}

std::string ff(const std::string& arg) { return arg; }

int main(int argc, char** argv)
{
    /*
    Subject sub;
    Observer2 ob;
    util::subscription subscription = sub.subscribe(ff);
    subscription = sub.subscribe(ff);
    subscription = sub.subscribe(ff);
    subscription = sub.subscribe(ff);
    subscription = sub.subscribe(ff);
    std::cout << (unsigned int) subscription << "\n";
    */

    /*

    int first = 0, second = 1, next = 0;

    for (int i = 0 ; i < 20 ; ++i)
    {
        if (i <= 1)
            next = i;
        else
        {
            next = first + second;
            first = second;
            second = next;
        }
        std::cout << next << std::endl;
    }
    */

    util::microclock mc;

    for (int i = 0; i < 10; ++i)
        std::cout << fib(i) << "\n";
    
    std::cout << mc.elapsed();

    return 0;
}