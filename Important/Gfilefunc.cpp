#include "Gfilefunc.h"

bool Gfile::ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map,
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

bool Gfile::ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
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

bool Gfile::SaveData(std::string config_file_dir, std::map<std::string, std::string> string_map) {
    //std::sort(string_map.begin(), string_map.end());
    std::fstream file;
    std::string fileline;
    file.open(config_file_dir, std::ios::out);
    if (file) {
        for (auto it : string_map) {
            fileline = it.first + " " + it.second + "\n";
            file.write(fileline.c_str(), fileline.size());
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}