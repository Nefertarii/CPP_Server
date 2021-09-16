#include "../../Important/Gfilefunc.h"
#include <algorithm>
#include <iostream>




int main() {
    std::string config_file_dir = "/home/cs18/vscode/Webserver/Accountinfo.txt";
    std::map<std::string, std::string> string_map;
    std::map<std::string, size_t> value_map;
    ReadConfig(config_file_dir, &string_map);
    SaveData(config_file_dir, string_map);
}