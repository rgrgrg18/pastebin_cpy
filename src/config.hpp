#pragma once

#include <string>

#include "sql_interface.hpp"

namespace Config {

    std::string __attribute__((weak)) ListenPort = "0.0.0.0:9999";

    std::string __attribute__((weak)) Files_directory = "/usr/src/app/build/";

    std::string __attribute__((weak)) Bucket_name = "";

    std::string __attribute__((weak)) Conn = "postgresql://root:root@postgres:5432/pastebin";

    std::string __attribute__((weak)) Endpoint = "";

    std::string __attribute__((weak)) Redis_conn = "tcp://redis:6379";

    using DefaultStorage = postgres;
};