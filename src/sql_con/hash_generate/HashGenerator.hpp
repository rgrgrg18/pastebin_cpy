#pragma once

#include <cstdint>
#include <string>

namespace paste_hash {
class Base64 {
public: 
	Base64() = delete;	

	Base64(const Base64& other) = delete;
	Base64& operator=(const Base64& other) = delete;

	explicit Base64(uint64_t value): Base64(std::to_string(value)) {}
	explicit Base64(uint64_t value, uint64_t length_hash): Base64(std::to_string(value), length_hash) {}
	explicit Base64(std::string str): hash_(Generate(str, (str.size() / 3) * 4 + (str.size() % 3))) {}
	explicit Base64(std::string str, uint64_t length_hash): hash_(Generate(str, length_hash)) {}

	const std::string hash_;
private:
	static const std::string kCodeString;

	static std::string Generate4(std::string str);
	static std::string Generate(std::string str, uint64_t length_hash);
}; 		
}