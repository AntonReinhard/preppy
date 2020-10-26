#pragma once

#include <chrono>
#include <string>

namespace preppy::util {
    typedef std::chrono::steady_clock clock;
}

namespace preppy::log {
 
    // The logging level, specifies what will be logged and what won't
    enum LOG_LEVEL {
        DEBUG   = 0,
        INFO    = 1,
        WARNING = 2,
        ERROR   = 3
    };

}
