#pragma once

// Define a macro to hide the weak attribute
// Use this if more than one file depends on it to avoid linker errors
#ifdef USE_WEAK
    #define WEAK_ATTR __attribute__((weak))
#else
    #define WEAK_ATTR
#endif

namespace redisSettins {

    int WEAK_ATTR lifeTime = 600; // lifeTime in seconds

};
