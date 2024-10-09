#include "file_commands.hpp"
#include "base64.hpp"

// converting a string to a bin file with the specified directory
bool file_commands::StringToBin(const std::string& file_name, const std::string& str, const std::string& directory){

    std::string open_dir = directory +
                        file_name +
                        ".bin";

    std::ofstream file(open_dir,
                        std::ios_base::out);

    if (file.is_open()) {
        file << base64_code::Base64Encode(str);
        file.close();
        return true;
    } else {
        return false;
    }
}

// converting a bin file to string with file deleting
std::pair<std::string, bool> file_commands::BinToString(const std::string& file_name, const std::string& directory) {

    std::string open_dir = directory +
                        file_name +
                        ".bin";
    std::ifstream file(open_dir);

    if (file.is_open()) {
        std::string file_content = "";
        unsigned char symbol;
        while (file >> symbol) {
            file_content.push_back(symbol);
        }

        remove(open_dir.c_str());

        return {base64_code::Base64Decode(file_content), true};

    } else {
        return {"", false};
    }
}