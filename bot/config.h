#ifndef _config_h_
#define _config_h_

#include <string>

std::string __attribute__((weak)) Token = <Token_from_BotFather>;

std::string __attribute__((weak)) files_directory = <WORKING_FILES_DIR>;  

std::string __attribute__((weak)) bucket_name = <BUCKET_NAME>;

std::string __attribute__((weak)) conn_config = "postgresql://{user_name}:{password}@{ip}:{port}/{database}";

std::string __attribute__((weak)) endpoint = <ENDPOINT_AWS>;

#endif