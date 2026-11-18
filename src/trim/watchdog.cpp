
#include <iostream>
#include <unistd.h>

#include "watchdog.hpp"

extern "C" {
    #include "../utils/palrup_utils.h"
}

void* run(void* arg) {
    Watchdog* dog = static_cast<Watchdog*>(arg);

    while(true) {
        if (dog->get_time_since_last_poke() > dog->allowed_span) {
            LOG_ERR("TIMEOUT detected by watchdog \"%s\"", dog->get_label().c_str());
            abort();
        }
        sleep(dog->sleep_interval);
    }

    return 0;
}

