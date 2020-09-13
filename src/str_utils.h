//
// Created by delf on 9/10/20.
//

#ifndef LPTESTTASK_STR_UTILS_H
#define LPTESTTASK_STR_UTILS_H

#include <string.h>
#include <ctype.h>

/*
 * HASH_CODE_MULTIPLIER must be simple number near the number of characters in the input alphabet
 * used for str_hash function
 */
#define HASH_CODE_MULTIPLIER 31

/*
 * WHITESPACE_SYMBOLS white space symbols for trim
 */
#define WHITESPACE_SYMBOLS "\t\n\v\f\r "

unsigned long long str_hash(const char* string);
void *to_lower_case(char *string);
void trim(char *string);

#endif //LPTESTTASK_STR_UTILS_H
