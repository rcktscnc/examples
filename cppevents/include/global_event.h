#ifndef __GLOBAL_EVENT_H
#define __GLOBAL_EVENT_H

#include <functional>

template<typename... T>
class global_event
{
public:
    void subscribe(std::function<void(T...)> listener);
    void unsubscribe(std::function<void(T...)> listener);
    void unsubscribe_all();
    void invoke(T...);
private:
    std::vector<std::function<void(T...)>> _listeners;
};

template<typename... T>
void global_event<T...>::subscribe(std::function<void(T...)> listener)
{
    _listeners.push_back(listener);
}

template<typename... T>
void global_event<T...>::unsubscribe(std::function<void(T...)> listener)
{
    _listeners.push_back(listener);
}

#endif