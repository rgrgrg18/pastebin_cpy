#ifndef __HASHGENERATOR_H__
#define __HASHGENERATOR_H__

#include <string>

namespace paste_hash {
	class Base64 {
	private:
		static const std::string code_string;
	public: 
		const std::string hash;

		Base64() = delete;	
		Base64(const Base64& other) = delete;
		Base64& operator=(const Base64& other) = delete;

		explicit Base64(unsigned long long value): Base64(std::to_string(value)) {}
		explicit Base64(unsigned long long value, int length_hash): Base64(std::to_string(value), length_hash) {}
		explicit Base64(std::string str): hash(generate(str, (str.size() / 3) * 4 + (str.size() % 3))) {}
		explicit Base64(std::string str, int length_hash): hash(generate(str, length_hash)) {}
	private:
		static std::string generate_4 (std::string str);
		static std::string generate (std::string str, int length_hash);
	}; 		
}

#endif