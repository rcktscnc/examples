#ifndef ___SUBJECT_H___
#define ___SUBJECT_H___

#include <string>

#include <lambda_event.h>

class Subject
{
public:
    static lambda_event<std::string, const std::string&> on_request;

    void invoke(std::string message)
    {
        on_request.invoke(message);
    }

private:
    
};

#endif