#pragma once

using keys = std::pair<std::string, std::string>; // pair of public and private keys
using paste_info = std::tuple<std::string, std::string, std::string, std::string, std::string>; // Private key, login, password, title, created at
using last_pastes_info = std::vector<std::vector<std::string>>; // Last pastes with title, public key, created time

class Storage {
public: 
	virtual paste_info get_paste_info(const std::string& public_key) = 0;
	
	virtual void change_password(const std::string& public_key, const std::string& new_password) = 0;
	
	virtual void change_title(const std::string& public_key, const std::string& new_name) = 0;
	
	virtual keys create_new_paste(uint64_t login) = 0;
	
	virtual void del_paste(const std::string& public_key, uint64_t login) = 0;
	
	virtual last_pastes_info get_last_user_pastes(uint64_t login, uint16_t limit) = 0;

	virtual ~Storage() = default;
};