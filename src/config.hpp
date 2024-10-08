#pragma once

#include <string>

// Define a macro to hide the weak attribute
// Use this if more than one file depends on it to avoid linker errors
#ifdef USE_WEAK
    #define WEAK_ATTR __attribute__((weak))
#else
    #define WEAK_ATTR
#endif

namespace config {

std::string WEAK_ATTR listen_port = "0.0.0.0:9999";

std::string WEAK_ATTR files_directory = "/usr/src/app/build/";

std::string WEAK_ATTR bucket_name = "";

std::string WEAK_ATTR conn = "postgresql://root:root@postgres:5432/pastebin";

std::string WEAK_ATTR endpoint = "";

std::string WEAK_ATTR redis_conn = "tcp://redis:6379";

};