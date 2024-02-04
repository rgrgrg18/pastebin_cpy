#include "HashGenerator.h"
#include <cstring>
#include <iostream>

using namespace paste_hash;

const std::string Base64::code_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

std::string Base64::generate_4 (std::string str) {
	std::string hash_4(4, '\0');

	str += '\0';
	str += '\0';

	char first = str[0];
	char second = str[1];
	char third = str[2];

	char encoded_1 = first >> 2;
	char encoded_2 = ((first << 4) & 48) | (second >> 4);
	char encoded_3 = ((second << 2) & 60) | (third >> 6);
	char encoded_4 = third & 63;

	if (str[2] == '\0') {
		encoded_3 = 64;
		encoded_4 = 64; 
	} else if (str[3] == '\0') {
		encoded_4 = 64;
	}

	hash_4[0] = code_string[encoded_1]; 
	hash_4[1] = code_string[encoded_2];
	hash_4[2] = code_string[encoded_3];  
	hash_4[3] = code_string[encoded_4];

	return hash_4;
}

std::string Base64::generate_8 (std::string str) {
	std::string hash_8;
	hash_8 += generate_4(str.substr(0, 4));
	hash_8 += generate_4(str.substr(4, 4));

	return hash_8;
}