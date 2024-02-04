#include "HashGenerator.h"
#include <cstring>

using namespace paste_hash;

const std::string code_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

std::string generate_4 (int value) {
	std::string hash_4(4, '\0');

	char default_1_byte = value;
	char default_2_byte = value >> 8;
	char default_3_byte = value >> 16;

	char encoded_1 = default_1_byte & 63;
	char encoded_2 = ((default_1_byte >> 6) | ((default_2_byte & 15) << 2));
	char encoded_3 = ((default_2_byte >> 4) | ((default_3_byte & 3) << 4));
	char encoded_4 = (default_3_byte & 257) >> 2;

	hash_4[0] = code_string[encoded_1]; 
	hash_4[1] = code_string[encoded_2];
	hash_4[2] = code_string[encoded_3];  
	hash_4[3] = code_string[encoded_4];

	return hash_4;
}
