#pragma once

#include <cstdint>

// Define a macro to hide the weak attribute
// Use this if more than one file depends on it to avoid linker errors
#ifdef USE_WEAK
    #define WEAK_ATTR __attribute__((weak))
#else
    #define WEAK_ATTR
#endif

namespace redisSettins {

    int32_t WEAK_ATTR lifeTimeInSeconds = 600;
  
};
