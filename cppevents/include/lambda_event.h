#ifndef ___LAMBDA_EVENT___
#define ___LAMBDA_EVENT___

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

template<typename ReturnType = void, typename... Args>
class lambda_event
{
public:
    enum class id_type : std::size_t {};

    template<typename Func>
    id_type subscribe(Func&& func)
    {
        auto id = _acquire_id();
        _listeners.push_back(std::make_pair(id, func));
        return id;
    }

    void unsubscribe(id_type subscription_id)
    {
        auto predicate = [subscription_id](const _pair_t& e) { return e.first == subscription_id; };
        auto it = std::find_if(_listeners.begin(), _listeners.end(), predicate);
        if (it != _listeners.end())
            _listeners.erase(it);
    }

    void unsubscribe_all()
    {
        _listeners.clear();
    }

    template<typename Func>
    void invoke(Args... args, Func func)
    {
        for (auto& e : _listeners)
            func(e.second(args...));
    }

    void invoke(Args... args)
    {
        for (auto& e : _listeners)
            e.second(args...);
    }

    std::size_t count()
    {
        return _listeners.size();
    }

private:
    using _pair_t = std::pair<id_type, std::function<ReturnType(Args...)>>;

    std::vector<_pair_t> _listeners;
    id_type _subscription_id;

    id_type _acquire_id()
    {
        auto const id = _subscription_id;
        _subscription_id = static_cast<id_type>(static_cast<std::size_t>(_subscription_id) + 1);
        return id;
    }
};

template<typename ReturnType = void, typename... Args>
using id_type = typename lambda_event<ReturnType, Args...>::id_type;

#endif