#ifndef ___LAMBDA_EVENT___
#define ___LAMBDA_EVENT___

#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

namespace util
{

enum class subscription : std::size_t {};

template<typename ReturnType = void, typename... Args>
class lambda_event
{
public:
    template<typename Func>
    subscription subscribe(Func&& func)
    {
        auto sub = _acquire_sub();
        _listeners.push_back(std::make_pair(sub, func));
        return sub;
    }

    void unsubscribe(subscription subscription)
    {
        auto predicate = [subscription](const _pair_t& e) { return e.first == subscription; };
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
    using _pair_t = std::pair<subscription, std::function<ReturnType(Args...)>>;
    std::vector<_pair_t> _listeners;
    subscription _next_sub;

    subscription _acquire_sub()
    {
        auto id = _next_sub;
        _next_sub = static_cast<subscription>(static_cast<std::size_t>(_next_sub) + 1);
        return id;
    }
};

} // namespace

#endif // ___LAMBDA_EVENT___