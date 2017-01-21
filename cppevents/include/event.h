#ifndef __EVENT_H
#define __EVENT_H

#include <vector>
#include <algorithm>

#include <listener_interface.h>

template<typename... T>
class event
{
public:
    typedef listener_interface<T...> listener_type;
    void subscribe(listener_type* listener)
    {
        _listeners.push_back(listener);
    }
    
    void unsubscribe(listener_type* listener)
    {
        auto rit = std::find(_listeners.rbegin(), _listeners.rend(), listener);
        if (rit != _listeners.rend())
            _listeners.erase(std::prev(rit.base()));
    }

    void unsubscribe_all()
    {
        _listeners.clear();
    }

    void invoke(T... args)
    {
        for (auto e : _listeners)
            e->on_invoke(args...);
    }

    size_t count()
    {
        return _listeners.size();
    }

private:
    std::vector<listener_type*> _listeners;
};

#endif