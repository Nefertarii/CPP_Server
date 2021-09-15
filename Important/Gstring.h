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

#endif