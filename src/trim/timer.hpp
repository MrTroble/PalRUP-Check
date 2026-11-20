
#pragma once

#include <ctime>
#include <chrono>
#include <limits>

#include "../utils/define.h"
#include "../utils/palrup_utils.h"

class Timer {
protected:
    const std::chrono::system_clock::time_point _start;

public:
    Timer() : _start(std::chrono::system_clock::now()) {};
};

class WaitTimer : Timer {

public:
    static struct WaitStats {
        u64 wait_count = 0;
        u64 worthwhile_waits = 0;
        std::chrono::nanoseconds wait_time_overall = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds max_wait = std::chrono::nanoseconds(0);
        std::chrono::nanoseconds min_wait = std::chrono::nanoseconds(std::numeric_limits<int64_t>::max());

        operator std::string() {
            return "wait_stats: "
                      "wait_count:" + std::to_string(wait_count) + " "
                    + "wait_time_overall:" + std::to_string(((std::chrono::duration<double>)wait_time_overall).count()) + " "
                    + "max_wait:" + std::to_string(((std::chrono::duration<double>)max_wait).count()) + " "
                    + "min_wait:" + std::to_string(((std::chrono::duration<double>)min_wait).count()) + " "
                    + "worthwhile_waits:" + to_string(worthwhile_waits);
        }
    } stats;

    ~WaitTimer() {
        auto wait_duration = std::chrono::system_clock::now() - _start;
        WaitTimer::stats.wait_count++;
        WaitTimer::stats.wait_time_overall += std::chrono::nanoseconds(wait_duration.count());
        if (wait_duration > WaitTimer::stats.max_wait) WaitTimer::stats.max_wait = wait_duration;
        if (wait_duration < WaitTimer::stats.min_wait) WaitTimer::stats.min_wait = wait_duration;
    };

};

WaitTimer::WaitStats WaitTimer::stats;

class TrimTimer : Timer {
public:
    ~TrimTimer() {
        std::chrono::duration<double> runtime = std::chrono::system_clock::now() - _start;
        LOG("Trimming took %f ns", runtime.count());
    };
};
