#ifndef __LISTENER_INTERFACE_H
#define __LISTENER_INTERFACE_H

template<typename... T>
class event;

template<typename... T>
class listener_interface
{
public:
    listener_interface(event<T...>& event) : event(&event)
    {
        event.subscribe(this);
    }

    ~listener_interface()
    {
        event->unsubscribe(this);
    }

    virtual void on_invoke(T... args) = 0;

private:
    event<T...>* event;
};
#endif