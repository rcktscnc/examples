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
    template<typename Func>
    std::size_t subscribe(Func&& func)
    {
        _listeners.push_back(std::make_pair(_subscription_id, func));
        return _subscription_id++;
    }

    void unsubscribe(std::size_t subscription_id)
    {
        auto predicate = [subscription_id](const _pair_t& e) -> bool
        {
            return e.first == subscription_id;
        };

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
    using _pair_t = std::pair<std::size_t, std::function<ReturnType(Args...)>>;
    std::vector<_pair_t> _listeners;
    std::size_t _subscription_id = 0;
};

#endif