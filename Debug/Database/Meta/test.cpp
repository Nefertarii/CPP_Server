#include "../../Important/Gfilefunc.h"
#include <iostream>




int main() {
    std::string config_file_dir = "/home/cs18/vscode/Webserver/Graph.conf";
    std::map<std::string, std::string> string_map;
    std::map<std::string, size_t> value_map;
    ReadConfig(config_file_dir, &string_map);
    for (auto i : string_map) {
        std::cout << i.first << ":" << i.second << "\n";
    }
    for (auto i : value_map) {
        std::cout << i.first << ":" << i.second << "\n";
    }
}