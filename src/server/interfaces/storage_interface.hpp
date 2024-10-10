#pragma once

#include <string>
#include <tuple>
#include <vector>

using Keys = std::pair<std::string, std::string>; // pair of public and private Keys
using PasteInfo = std::tuple<std::string, std::string, std::string, std::string, std::string>; // Private key, login, password, title, created at
using LastPastesInfo = std::vector<std::vector<std::string>>; // Last pastes with title, public key, created time

class Storage {
public: 
	virtual ~Storage() = default;

	virtual PasteInfo GetPasteInfo(const std::string& public_key) = 0;
	
	virtual void ChangePassword(const std::string& public_key, const std::string& new_password) = 0;
	
	virtual void ChangeTitle(const std::string& public_key, const std::string& new_name) = 0;
	
	virtual Keys CreateNewPaste(uint64_t login) = 0;
	
	virtual void DelPaste(const std::string& public_key, uint64_t login) = 0;
	
	virtual LastPastesInfo GetLastUserPastes(uint64_t login, uint16_t limit) = 0;
};