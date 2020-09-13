//
// Created by delf on 9/10/20.
//
#include "str_utils.h"

unsigned long long str_hash(const char *string) {
    unsigned long long result = 0;
    unsigned long long multiplier = HASH_CODE_MULTIPLIER;
    const char *s;
    for (s = string; *s; ++s) {
        result += (*s) * multiplier;
        multiplier *= multiplier;
    }
    return result;
}

void *to_lower_case(char *string) {
    for (char *s = string; *s; ++s) {
        *s = (char) tolower(*s);
    }
    return string;
}

void ltrim(char *string) {
    size_t fist_valuable_char_position = strspn(string, WHITESPACE_SYMBOLS);
    if (fist_valuable_char_position > 0) {
        size_t str_len = strlen(string);
        if (str_len == fist_valuable_char_position) {
            string[0] = '\0';
        } else {
            memmove(string, string + fist_valuable_char_position, str_len - fist_valuable_char_position + 1);
        }
    }
}

void rtrim(char *string) {
    size_t current_char_position;
    size_t str_len = strlen(string);
    for(current_char_position = str_len-1;current_char_position>=0 && strchr(WHITESPACE_SYMBOLS, string[current_char_position]) != NULL; --current_char_position){
        ;
    }
    if(current_char_position+1<str_len){
        string[current_char_position+1] = '\0';
    }
}

void trim(char *string){
    ltrim(string);
    rtrim(string);
}

