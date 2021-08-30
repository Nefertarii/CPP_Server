#include "Gstring.h"

char *string_to_char(std::string str)
{
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    return cstr;
}

std::string Substr(std::string str,int begin,int maxlength,char end) {
    int beg_i = begin, end_i = 0;
    while (end_i <= maxlength) {
        if(str[end_i + beg_i] == end)
            break;
        end_i++;
    }
    if(end_i == 0) { //not found
        return "0";
    }
    else if(end_i <= maxlength) { 
        return str.substr(beg_i, end_i);
    }
    else { //out of size
        return "-1";
    }
}

std::string Substr_Revers(std::string str, int maxlength,char end) {
    if(maxlength > str.length()) {
        return "-1";
    }
    int beg_i = str.length(), end_i = 0;
    while (end_i <= maxlength) {
        if (str[beg_i - end_i] == end) {
            break;
        }
        end_i++;
    }
    if (end_i == 0) {
        return "0";
    }
    else if (end_i <= maxlength) {
        return str.substr(beg_i - end_i + 1, end_i);
    }
    else {
        return "-1";
    }
}