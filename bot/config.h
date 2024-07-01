#ifndef _config_h_
#define _config_h_

#include <string>

namespace Config {

    std::string __attribute__((weak)) Token = <BOT_TOKEN>;

    std::string __attribute__((weak)) Files_directory = <WORK_FILES_DIR>;

    std::string __attribute__((weak)) Bucket_name = <BUCKET_NAME>;

    std::string __attribute__((weak)) Conn = "postgresql://{username}:{password}@{ip}:{port}/{db}";

    std::string __attribute__((weak)) Endpoint = <AWS_ENDPOINT>;

    std::string __attribute__((weak)) Webhook_url = <WEBHOOK_URL>;

};

#endif