#include "validate_uniq_code.h"

// "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=" - symbols, that we can use 


bool validate::validate_code(const string& code) {
    if (code.length() != 8) return false;
    for (char symbol : code) {
        if ((int)symbol >= 65 && (int)symbol <= 90) continue;
        if ((int)symbol >= 97 && (int)symbol <= 122) continue;
        if ((int)symbol >= 48 && (int)symbol <= 57) continue;
        if ((int)symbol == 43 || (int)symbol == 47 || (int)symbol == 61) continue;
        return false;
    }
    return true;
}

