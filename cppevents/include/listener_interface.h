#ifndef __LISTENER_INTERFACE_H
#define __LISTENER_INTERFACE_H

template<typename... T>
class listener_interface
{
public:
    virtual void on_invoke(T... args) = 0;
};

#endif