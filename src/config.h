#ifndef _config_h_
#define _config_h_

#include <string>

namespace Config {

    std::string __attribute__((weak)) ListenPort = "0.0.0.0:9999";

    std::string __attribute__((weak)) Files_directory = "";

    std::string __attribute__((weak)) Bucket_name = "";

    std::string __attribute__((weak)) Conn = "postgresql://{username}:{password}@{ip}:{port}/{db}";

    std::string __attribute__((weak)) Endpoint = "";

    std::string __attribute__((weak)) Redis_conn = "tcp://{ip}:{port}";

};

#endif