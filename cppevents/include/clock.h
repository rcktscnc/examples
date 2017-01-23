#ifndef ___CLOCK_H___
#define ___CLOCK_H___

#include <chrono>

template<typename TimeUnit>
class low_latency_clock
{
public:
    low_latency_clock()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    void start()
    {
        _start = std::chrono::high_resolution_clock::now();
    }

    long long elapsed()
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - _start;
        return std::chrono::duration_cast<TimeUnit>(elapsed).count();
    }
    
private:
    std::chrono::high_resolution_clock::time_point _start;
};

using nanoclock = low_latency_clock<std::chrono::nanoseconds>;
using microclock = low_latency_clock<std::chrono::microseconds>;
using milliclock = low_latency_clock<std::chrono::milliseconds>;

#endif