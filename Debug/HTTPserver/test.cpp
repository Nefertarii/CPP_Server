#include <iostream>
#include <fstream>
#include <map>
#include <string>
std::map<std::string, std::string> global_string_settings;
std::map<std::string, size_t> global_value_settings;

bool ReadConfig() {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool stringdone = false;
    file.open("/home/cs18/vscode/Webserver/Http.conf", std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            std::cout << fileline << "\n";
            for (auto i : global_string_settings) {
                std::cout << i.first << ":" << i.second << "\n";
            }
            for (auto i : global_value_settings) {
                std::cout << i.first << ":" << i.second << "\n";
            }
            file.close();
            return true;
        }
    } else {
        std::cout << "Can't open file \"Http.conf\"\n";
    }
    return false;
}

int main() {
    ReadConfig();
    return 0;
}