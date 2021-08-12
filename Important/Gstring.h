#ifndef GSTRING_H_
#define GSTRING_H_

#include <string.h>
#include <string>

//string to char*
inline char *sToc(std::string str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

// ... ///


#endif