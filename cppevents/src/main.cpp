#include <iostream>
#include <string>

#include <observer.h>
#include <observer2.h>
#include <event_manager.h>
#include <lambda_event.h>
#include <subject.h>
#include <clock.h>

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

int main(int argc, char** argv)
{
    /*
    microclock mc;
    Subject sub;

    Observer2 ob2;
    Observer2 ob22;

    auto f = [](std::string arg)
    {
        std::cout << arg + " ";
    };
    
    sub.on_request.invoke("HAR", f);

    std::cout << "\ntime: " << mc.elapsed();
    */
    Observer ob;

    std::cout << ob.x;
    return 0;
}