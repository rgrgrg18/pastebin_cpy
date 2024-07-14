#ifndef _config_h_
#define _config_h_

#include <string>

namespace Config {

    std::string __attribute__((weak)) Files_directory = <WORK_FILES_DIR>;

    std::string __attribute__((weak)) Token = <BOT_TOKEN>;

    std::string __attribute__((weak)) Webhook_url = <WEBHOOK_URL>;;

    std::string __attribute__((weak)) Redis_conn = "tcp://{ip}:{port}";

    std::string __attribute__((weak)) server_address = "{ip}:{port}";
};

#endif