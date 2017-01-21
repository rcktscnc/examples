#include <string>

#include <event_manager.h>

class Subject
{
public:
    void invoke(std::string message)
    {
        event_manager::on_start.invoke(message);
    }
private:
};