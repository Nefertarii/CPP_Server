#ifndef GJSON_H_
#define GJSON_H_

#include <string>
#include <vector>
#include <initializer_list>

//create respone json type
std::string JsonSpliced(std::initializer_list<std::string> stril);
std::string JsonSpliced(std::vector<std::string> strvec);

#endif