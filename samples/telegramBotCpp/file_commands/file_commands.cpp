#include "file_commands.h"

// returns the file type
std::string FileCommands::file_type(const std::string& path) {
    std::string ans;

    size_t i = path.size() - 1;
    while (i >= 0 && path[i] != '.') --i;
    ++i;

    for (size_t j = i; j < path.size(); ++j) {
        ans.push_back(path[j]);
    }

    return ans;
}

bool FileCommands::string_to_txt(const std::string& fileName, const std::string& str, const std::string& directory) {
    std::string openDir = directory + 
                        fileName + 
                        ".txt";
    std::ofstream file(openDir, 
                        std::ios_base::out);

    if (file.is_open()) {
        file << str;
        file.close();
        return true;
    } else {
        std::cout << "can't open file in function string_to_txt\n"; 
        return false;
    }
}

std::string FileCommands::txt_to_string(const std::string& fileName, const std::string& directory) {
    std::string openDir = directory + 
                        fileName + 
                        ".txt";
    std::ifstream file(openDir);

    std::string str;

    if (file.is_open()) {

        unsigned char symbol;
        while (file >> symbol) {
            str.push_back(symbol);
        }

        file.close();
    } else {
        std::cout << "can't open file in function string_to_txt\n"; 
    }

    return str;
}
