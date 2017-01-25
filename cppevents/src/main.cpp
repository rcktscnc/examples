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
    template <typename T>
    std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

std::string ff(const std::string& arg) { return arg; }

int main(int argc, char** argv)
{
    typename lambda_event<std::string, const std::string&>::id_type id = Subject::on_request.subscribe(ff);
    id = Subject::on_request.subscribe(ff);
    id = Subject::on_request.subscribe(ff);
    id = Subject::on_request.subscribe(ff);
    id = Subject::on_request.subscribe(ff);
    std::cout << (unsigned int)id << "\n";
    return 0;
}