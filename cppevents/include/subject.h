#ifndef ___SUBJECT_H___
#define ___SUBJECT_H___

#include <string>
#include <util/lambda_event.h>

class Subject
{
public:
    template<typename Func>
    static util::subscription subscribe(Func&& func)
    {
        return on_request.subscribe(func);
    }

    template<typename... Args, typename Func>
    static void invoke(Args... args, Func&& func)
    {
        on_request.invoke(args..., func);
    }

    static void unsubscribe(util::subscription subscription)
    {
        on_request.unsubscribe(subscription);
    }

private:
    static util::lambda_event<std::string, const std::string&> on_request;
};

#endif