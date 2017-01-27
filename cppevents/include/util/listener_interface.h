#ifndef ___LISTENER_INTERFACE_H___
#define ___LISTENER_INTERFACE_H___

namespace util
{

template<typename ReturnType = void, typename... Args>
class listener_interface
{
public:
    virtual ReturnType on_invoke(Args... args) = 0;
    virtual ~listener_interface() {}
};

} // namepace

#endif // ___LISTENER_INTERFACE_H___
