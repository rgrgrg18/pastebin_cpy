#include "methods.h"

std::string PastebinMethods::addPaste(int64_t user_id, pasteData data) {

    auto [author, password, title, created_at, text] = data;
    
    keys PasteKeys = SqlRelation::PasteCache::makeNewPaste(user_id);

    std::string public_key = PasteKeys.first;
    std::string private_key = PasteKeys.second;

    
    FileCommands::string_to_bin(private_key, text, Config::Files_directory);

    if (!AWS_connect::PutObject(Config::Bucket_name, private_key + ".bin")) {

        SqlRelation::PasteCache::delNewPaste(private_key, user_id);
        public_key = "";

    } 

    
    remove((Config::Files_directory +  private_key + ".bin").c_str());

    updatePasteInfo(public_key, std::tuple(password, title));

    return public_key;
}

bool PastebinMethods::deletePaste(const std::string& public_key) {

    auto pasteInfo = SqlRelation::PasteCache::getInfoPaste(public_key);

    std::string private_key = std::get<0>(pasteInfo);
    std::string author = std::get<1>(pasteInfo);

    if (private_key == "") {
        std::cout << "deleting a non-existent post\n";
        return false;
    }

    if (!AWS_connect::DeleteObject(Config::Bucket_name, private_key + ".bin")) {
        return false;
    }

    SqlRelation::PasteCache::delNewPaste(public_key, std::atoll(author.c_str()));

    return true;

}

bool PastebinMethods::updatePasteInfo(const std::string& public_key, newPasteInfo data) {

    auto [private_key, author, old_password, old_title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    if (private_key == "") {
        std::cout << "paste to update does't exist\n";
        return false;
    }

    auto [new_password, new_title] = data;


    if (new_password != "") SqlRelation::PasteCache::changePastePassword(new_password, public_key);

    if (new_title != "") SqlRelation::PasteCache::changePasteTitle(new_title, public_key);


    return true;
}

pasteInfo PastebinMethods::getPasteInfo(const std::string& public_key) {

    auto [private_key, author, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    return std::tuple(author, password, title, created_at);

}

std::string PastebinMethods::getPasteText(const std::string& public_key) {

    auto [private_key, author, password, title, created_at] = SqlRelation::PasteCache::getInfoPaste(public_key);

    return AWS_connect::GetObjectData(Config::Bucket_name, private_key, Config::Files_directory);
}

std::vector<std::vector<std::string> > PastebinMethods::getLastPastes(int64_t user_id, int limit) {
    return SqlRelation::getLastPastes(user_id, limit);
}
