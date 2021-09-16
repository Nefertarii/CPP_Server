#ifndef GFILEFUNC_H_
#define GFILEFUNC_H_

#include <string>
#include <map>
#include <fstream>

bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map,
                std::map<std::string, size_t>* value_map) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool stringdone = false;
    file.open(config_file_dir, std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            if (fileline == "----") {
                stringdone = true;
            }
            if (!stringdone) {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        key = fileline.substr(0, i);
                        value_str = fileline.substr(i + 1, fileline.size());
                        (*string_map)[key] = value_str;
                    }
                }
            } else {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        key = fileline.substr(0, i);
                        value_ul = std::stoul(fileline.substr(i + 1, fileline.size()));
                        (*value_map)[key] = value_ul;
                    }
                }
            }
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    file.open(config_file_dir, std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            for (size_t i = 0; i != fileline.size(); i++) {
                if (fileline[i] == ' ') {
                    key = fileline.substr(0, i);
                    value_str = fileline.substr(i + 1, fileline.size());
                    (*string_map)[key] = value_str;
                }
            }
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}



#endif