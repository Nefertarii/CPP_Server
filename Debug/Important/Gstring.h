#ifndef GSTRING_H_
#define GSTRING_H_

#include <cstring>
#include <string>

//string to char*
char* string_to_char(std::string str);
// ... ///

//intersept string from index 'begin' to find char 'end' 
//maximum length is 'maxlength'
std::string Substr(std::string str, size_t begin, size_t maxlength, char end);
//intersept string from str.end() to find char 'end' 
//maximum length is 'maxlength'
std::string Substr_Revers(std::string str, size_t maxlength, char end);





#include "Gstring.h"

char* string_to_char(std::string str) {
    char* cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

std::string Substr(std::string str, size_t begin, size_t maxlength, char end) {
    size_t beg_i = begin, end_i = 0;
    while (end_i <= maxlength) {
        if (str[end_i + beg_i] == end)
            break;
        end_i++;
    }
    if (end_i == 0) { //not found
        return "0";
    } else if (end_i <= maxlength) {
        return str.substr(beg_i, end_i);
    } else { //out of size
        return "-1";
    }
}

std::string Substr_Revers(std::string str, size_t maxlength, char end) {
    if (maxlength > str.length()) {
        return "-1";
    }
    size_t beg_i = str.length(), end_i = 0;
    while (end_i <= maxlength) {
        if (str[beg_i - end_i] == end) {
            break;
        }
        end_i++;
    }
    if (end_i == 0) {
        return "0";
    } else if (end_i <= maxlength) {
        return str.substr(beg_i - end_i + 1, end_i);
    } else {
        return "-1";
    }
}


#endif