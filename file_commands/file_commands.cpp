#include "file_commands.h"

// returns the file type
std::string FileCommands::file_type(const std::string& path) {
    std::string ans;
    int i = path.size() - 1;
    while (i >= 0 && path[i] != '.') --i;
    ++i;
    for (int j = i; j < path.size(); ++j) {
        ans.push_back(path[j]);
    }
    return ans;
}

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
        std::cout << "can't open file in function string_to_bin\n"; 
        return false;
    }
}

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

        return {Base64_code::base64_decode(fileContent), true};

    } else {
        std::cout << "can't open files in function bin_to_string";
        return {"", false};
    }
}

bool FileCommands::string_to_txt(const std::string& fileName, const std::string& str, const std::string& directory) {
    std::string openDir = directory + 
                        fileName + 
                        ".txt";
    std::ofstream file(openDir, 
                        std::ios_base::out);

    if (file.is_open()) {
        file << Base64_code::base64_encode(str);
        file.close();
        return true;
    } else {
        std::cout << "can't open file in function string_to_txt\n"; 
        return false;
    }
}

// converting a txt file to a bin file
bool FileCommands::txt_to_bin(const std::string& txt_path, const std::string& bin_name){
    std::string bin_path;
    if (file_type(txt_path) != "txt") {
        std::cout << "incorrect path name in func txt_to_bin";
        return false;
    }

    for (int i = 0; i < txt_path.size() - 3; ++i) {
        bin_path.push_back(txt_path[i]);
    }
    
    while (bin_path.back() != '/') bin_path.pop_back();
    bin_path += bin_name + ".bin";

    std::ifstream txt_file(txt_path);
    std::ofstream bin_file(bin_path,
                            std::ios_base::out);

    if (bin_file.is_open() && txt_file.is_open()) {
        std::string fileContent = "";
        unsigned char symbol;
        while (txt_file >> symbol) {
            fileContent.push_back(symbol);
        }

        const char *path = txt_path.data();
        remove(path);

        bin_file << Base64_code::base64_decode(fileContent);
        bin_file.close();
        return true;
    } else {
        std::cout << "can't open files in function txt_to_bin";
        return false;
    }                        

};

// converting a bin file to a txt file
bool FileCommands::bin_to_txt(const std::string& bin_path, const std::string& txt_name){

    std::string txt_path;
    if (file_type(bin_path) != "bin") {
        std::cout << "incorrect path name in func bin_to_txt";
        return false;
    }

    for (int i = 0; i < bin_path.size() - 3; ++i) {
        txt_path.push_back(bin_path[i]);
    }

    while (txt_path.back() != '/') txt_path.pop_back();
    txt_path += txt_name + ".txt";

    std::ofstream txt_file(txt_path,
                            std::ios_base::out);

    std::ifstream bin_file(bin_path, std::ios::binary);

    if (bin_file.is_open() && txt_file.is_open()) {
        std::string fileContent = "";
        unsigned char symbol;
        while (bin_file >> symbol) {
            fileContent.push_back(symbol);
        }

        const char *path = bin_path.data();
        remove(path);

        
        txt_file << Base64_code::base64_decode(fileContent);
        txt_file.close();
        
        return true;
    } else {
        std::cout << "can't open files in function txt_to_bin";
        return false;
    }


};