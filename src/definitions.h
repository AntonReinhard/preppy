#pragma once

#include <chrono>
#include <string>

namespace preppy::util {
    typedef std::chrono::steady_clock clock;
}

namespace preppy::log {
 
    // The logging level, specifies what will be logged and what won't
    enum LOG_LEVEL {
        NOTHING = 0,
        ERROR   = 1,
        WARNING = 2,
        INFO    = 3,
        DEBUG   = 4
    };

}
