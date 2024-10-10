#pragma once

#include <cstdint>

// Define a macro to hide the weak attribute
// Use this if more than one file depends on it to avoid linker errors
#ifdef USE_WEAK
    #define WEAK_ATTR __attribute__((weak))
#else
    #define WEAK_ATTR
#endif

namespace redis_settings {

uint32_t WEAK_ATTR life_time_in_seconds = 600;

};
