#include "file_commands.h"

// converting a string to a txt file with the specified directory
void FileCommands::string_to_txt(int64_t user_id, std::string message, std::string& directory){

    std::ofstream file(directory + 
                        std::to_string(user_id) + 
                        ".txt", 
                        std::ios_base::out);

    if (file.is_open()) {
        file << message;
        file.close();
    } else {
        std::cout << "can't open file in function string_to_txt";
        return;
    }
};

// converting a string to a bin file with the specified directory
void FileCommands::string_to_bin(int64_t user_id, std::string message, std::string& directory){

    std::ofstream file(directory + 
                        std::to_string(user_id) + 
                        ".bin", 
                        std::ios_base::out);

    if (file.is_open()) {
        file << message;
        file.close();
    } else {
        std::cout << "can't open file in function string_to_bin"; 
        return;
    }
};

// returns the file type
std::string FileCommands::file_type(const std::string& path) {
    std::string ans;
    int i = path.size() - 1;
    while (i >= 0 && path[i] != '.') --i;
    ++i;
    for (i; i < path.size(); ++i) {
        ans.push_back(path[i]);
    }
    return ans;
}

// converting a txt file to a bin file
void FileCommands::txt_to_bin(std::string txt_path){
    std::string bin_path;
    if (file_type(txt_path) != "txt") {
        std::cout << "incorrect path name in func txt_to_bin";
        return;
    }

    for (int i = 0; i < txt_path.size() - 3; ++i) {
        bin_path.push_back(txt_path[i]);
    }
    bin_path += "bin";

    std::ifstream txt_file(txt_path);
    std::ofstream bin_file(bin_path,
                            std::ios_base::out);

    if (bin_file.is_open() && txt_file.is_open()) {
        char symbol;
        while (txt_file >> symbol) {
            bin_file << symbol;
        }
        const char *path = txt_path.data();
        remove(path);
    } else {
        std::cout << "can't open files in function txt_to_bin";
        return;
    }                        

};

// converting a bin file to a txt file
void FileCommands::bin_to_txt(std::string bin_path){

    std::string txt_path;
    if (file_type(bin_path) != "bin") {
        std::cout << "incorrect path name in func bin_to_txt";
        return;
    }

    for (int i = 0; i < bin_path.size() - 3; ++i) {
        txt_path.push_back(bin_path[i]);
    }
    txt_path += "txt";

    std::ofstream txt_file(txt_path,
                            std::ios_base::out);

    std::ifstream bin_file(bin_path);

    if (bin_file.is_open() && txt_file.is_open()) {
        char symbol;
        while (bin_file >> symbol) {
            txt_file << symbol;
        }
        const char *path = bin_path.data();
        remove(path);

    } else {
        std::cout << "can't open files in function bin_to_txt";
        return;
    }

};