#ifndef __HASHGENERATOR_H__
#define __HASHGENERATOR_H__

#include <string>

namespace paste_hash {
	class Base64 {
	private:
		static const std::string code_string;
	public: 
		std::string hash_4;
		std::string hash_8;

		Base64() = delete;	
		Base64(const Base64& other) = delete;
		Base64& operator=(const Base64& other) = delete;

		explicit Base64(std::string str): hash_8(generate_8(str)) {}
	private:
		std::string generate_4 (std::string str);
		std::string generate_8 (std::string str);
	}; 		
}

#endif