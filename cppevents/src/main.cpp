#include <iostream>
#include <string>

#include <subject.h>
#include <observer.h>
#include <observer2.h>
#include <event_manager.h>


int main(int argc, char** argv)
{
    Subject s;
    Observer o;
    Observer2 o2;

    s.invoke("COOL MESSAGE!\n");
    
    std::cout << "End.";
    return 0;
}