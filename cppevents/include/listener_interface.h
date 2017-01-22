#ifndef ___LISTENER_INTERFACE_H___
#define ___LISTENER_INTERFACE_H___

template<typename ReturnType = void, typename... Args>
class listener_interface
{
public:
    virtual ReturnType on_invoke(Args... args) = 0;
};

#endif
