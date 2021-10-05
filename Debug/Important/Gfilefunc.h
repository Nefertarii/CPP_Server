#ifndef GFILEFUNC_H_
#define GFILEFUNC_H_

#include <string>
#include <map>
#include <vector>
#include <fstream>

namespace Gfile {
    bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map,
                    std::map<std::string, size_t>* value_map);
    bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map);
    bool ReadConfig(std::string config_file_dir, std::map<std::string, size_t>* string_map);
    bool ReadConfig(std::string config_file_dir, std::map<std::string, std::vector<std::string>>* strvec_map);
    bool SaveData(std::string config_file_dir, std::map<std::string, std::string>* string_map);
    bool SaveData(std::string config_file_dir, std::map<std::string, std::vector<std::string>>* strvec_map);
}





bool Gfile::ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map,
                        std::map<std::string, size_t>* value_map) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool linedone = false;
    file.open(config_file_dir, std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            if (fileline == "----") {
                linedone = true;
            }
            if (!linedone) {
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

bool Gfile::ReadConfig(std::string config_file_dir, std::map<std::string, size_t>* value_map) {
    std::fstream file;
    std::string fileline;
    std::string key;
    size_t value_ul;
    file.open(config_file_dir, std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        key = fileline.substr(0, i);
                        value_ul = std::stoul(fileline.substr(i + 1, fileline.size()));
                        (*value_map)[key] = value_ul;
                    }
                }
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool Gfile::ReadConfig(std::string config_file_dir, std::map<std::string, std::vector<std::string>>* strvec_map) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    bool linedone = false;
    file.open(config_file_dir, std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            if (fileline == "----") {
                linedone = false;
                continue;
            }
            if (!linedone) {
                key = fileline;
                (*strvec_map)[key];
                linedone = true;
            } else {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        value_str = fileline.substr(0, i);
                        (*strvec_map)[key].push_back(value_str);
                        value_str = fileline.substr(i + 1, fileline.size());
                        (*strvec_map)[key].push_back(value_str);
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

bool Gfile::SaveData(std::string config_file_dir, std::map<std::string, std::string>* string_map) {
    std::fstream file;
    std::string fileline;
    file.open(config_file_dir, std::ios::out);
    if (file) {
        for (auto it : *string_map) {
            fileline = it.first + " " + it.second + "\n";
            file.write(fileline.c_str(), fileline.size());
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool Gfile::SaveData(std::string config_file_dir, std::map<std::string, std::vector<std::string>>* strvec_map) {
    std::fstream file;
    std::string fileline;
    file.open(config_file_dir, std::ios::out);
    if (file) {
        for (auto it : *strvec_map) {
            fileline = it.first + "\n";
            file.write(fileline.c_str(), fileline.size());
            for (size_t i = 0; i != it.second.size(); i+=2) {
                fileline = it.second.at(i) + " " + it.second.at(i + 1) + "\n";
                file.write(fileline.c_str(), fileline.size());
            }
            fileline = "----\n";
            file.write(fileline.c_str(), fileline.size());
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}

#endif