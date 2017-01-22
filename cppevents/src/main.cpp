#include <iostream>
#include <string>

#include <observer.h>
#include <observer2.h>
#include <event_manager.h>
#include <lambda_event.h>

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
    Observer2 ob2;
    event_manager::on_start.subscribe(&ob2);
    event_manager::on_start.subscribe(&ob2);
    event_manager::on_start.subscribe(&ob2);
    event_manager::on_start.subscribe(&ob2);

    std::string result_string;

    auto func = [&result_string](const std::string& arg) -> void
    {
        static int x = 0;
        result_string += arg + patch::to_string(x) + "\n";
        ++x;
    };

    event_manager::on_start.invoke("This method was called", func);

    std::cout << result_string;

    std::cout << "\nEnd.";
    */

    Observer ob;
    Observer ob2;
    std::cout << ob.x;
    std::cout << ob2.x;
    event_manager::on_lambda.invoke(42);

    std::cout << ob.x;
    std::cout << ob2.x;
    return 0;
}