#ifndef GSTRING_H_
#define GSTRING_H_

#include <string.h>
#include <string>

//string to char*
char *string_to_char(std::string str);
// ... ///

//substring from index 'begin' to char 'end' 
//maximum length of 'maxlength'
std::string Substr(std::string str, int begin, int maxlength, char end);

#endif