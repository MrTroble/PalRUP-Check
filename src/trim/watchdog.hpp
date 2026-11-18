
#pragma once

#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <assert.h>

void* run(void* arg);

// Aborts the process if it has not been poked for some time
class Watchdog {
    std::string _label;
    pthread_t _thread = 0;
    time_t _last_poke;

public:
    std::time_t allowed_span = 60;      // defaults to 1 minute
    time_t sleep_interval = 1;          // defaults to 1 second

    Watchdog() : Watchdog("default label") {};
    Watchdog(std::string label) : _label(label), _last_poke(time(NULL)) {
        pthread_create(&_thread, NULL, run, this);
        assert(_thread);
        if (_thread) pthread_detach(_thread);
    };
    ~Watchdog() = default;

    void poke() { _last_poke = time(NULL); };
    time_t get_time_since_last_poke() { return (time(NULL) - _last_poke); };
    std::string get_label() { return _label; };
};


