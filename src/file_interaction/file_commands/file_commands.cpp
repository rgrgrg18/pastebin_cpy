#include "file_commands.hpp"
#include "base64.hpp"

// converting a string to a bin file with the specified directory
bool FileCommands::string_to_bin(const std::string& fileName, const std::string& str, const std::string& directory){

    std::string openDir = directory +
                        fileName +
                        ".bin";

    std::ofstream file(openDir,
                        std::ios_base::out);

    if (file.is_open()) {
        file << Base64_code::base64_encode(str);
        file.close();
        return true;
    } else {
        return false;
    }
}

// converting a bin file to string with file deleting
std::pair<std::string, bool> FileCommands::bin_to_string(const std::string& fileName, const std::string& directory) {

    std::string openDir = directory +
                        fileName +
                        ".bin";
    std::ifstream file(openDir);

    if (file.is_open()) {
        std::string fileContent = "";
        unsigned char symbol;
        while (file >> symbol) {
            fileContent.push_back(symbol);
        }

        remove(openDir.c_str());

        return {Base64_code::base64_decode(fileContent), true};

    } else {
        return {"", false};
    }
}