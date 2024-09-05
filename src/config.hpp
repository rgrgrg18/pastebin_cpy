#pragma once

#include <string>

// Define a macro to hide the weak attribute
// Use this if more than one file depends on it to avoid linker errors
#ifdef USE_WEAK
    #define WEAK_ATTR __attribute__((weak))
#else
    #define WEAK_ATTR
#endif

namespace Config {

    std::string WEAK_ATTR ListenPort = "0.0.0.0:9999";

    std::string WEAK_ATTR Files_directory = "/usr/src/app/build/";

    std::string WEAK_ATTR Bucket_name = "";

    std::string WEAK_ATTR Conn = "postgresql://root:root@postgres:5432/pastebin";

    std::string WEAK_ATTR Endpoint = "";

    std::string WEAK_ATTR Redis_conn = "tcp://redis:6379";

};