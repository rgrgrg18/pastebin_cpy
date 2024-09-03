#include "HashGenerator.hpp"

using namespace paste_hash;

const std::string Base64::code_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

std::string Base64::generate_4 (std::string str) {
	std::string hash_4(4, '\0');

	while (str.size() % 3 != 0) {
		str.push_back('\0');
	}

	char first = str[0];
	char second = str[1];
	char third = str[2];

	char encoded_1 = first >> 2;
	char encoded_2 = ((first << 4) & 48) | (second >> 4);
	char encoded_3 = ((second << 2) & 60) | (third >> 6);
	char encoded_4 = third & 63;

	if (str[1] == '\0') {
		encoded_3 = 64;
		encoded_4 = 64; 
	} else if (str[2] == '\0') {
		encoded_4 = 64;
	}

	hash_4[0] = code_string[encoded_1]; 
	hash_4[1] = code_string[encoded_2];
	hash_4[2] = code_string[encoded_3];  
	hash_4[3] = code_string[encoded_4];

	return hash_4;
}

std::string Base64::generate (std::string str, int length_hash) {
	if (length_hash % 4 != 0) 
		length_hash += 4 - length_hash % 4;
	std::string result_hash;

	for (size_t amount_strings = 0 
			; amount_strings < (str.size() / 3) 
			&& amount_strings < (length_hash / 4)
			; ++amount_strings) {
		result_hash += generate_4(str.substr(amount_strings * 3, 3));
	}
	if (str.size() % 3 != 0 && result_hash.size() != length_hash) {
		result_hash += generate_4(str.substr(str.size() - (str.size() % 3), str.size() % 3));
	}
	while (result_hash.size() != length_hash) {
		result_hash += '=';
	}
	return result_hash;
}