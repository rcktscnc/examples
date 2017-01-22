#ifndef ___EVENT_H___
#define ___EVENT_H___

#include <vector>
#include <algorithm>

#include <listener_interface.h>

template<typename ReturnType = void, typename... Args>
class event
{
public:
    typedef listener_interface<ReturnType, Args...> listener_t;

    void subscribe(listener_t* listener)
    {
        _listeners.push_back(listener);
    }
    
    void unsubscribe(listener_t* listener)
    {
        auto rit = std::find(_listeners.rbegin(), _listeners.rend(), listener);
        if (rit != _listeners.rend())
            _listeners.erase(std::prev(rit.base()));
    }

    void unsubscribe_all()
    {
        _listeners.clear();
    }

    template<typename Func>
    void invoke(Args... args, Func func)
    {
        for (auto e : _listeners)
            func(e->on_invoke(args...));
    }

    void invoke(Args... args)
    {
        for (auto e : _listeners)
            e->on_invoke(args...);
    }

    std::size_t count()
    {
        return _listeners.size();
    }

private:
    std::vector<listener_t*> _listeners;
};

#endif