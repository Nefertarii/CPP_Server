#ifndef GFILEFUNC_H_
#define GFILEFUNC_H_

#include <string>
#include <map>
#include <fstream>

namespace Gfile {
    bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map,
                    std::map<std::string, size_t>* value_map);
    bool ReadConfig(std::string config_file_dir, std::map<std::string, std::string>* string_map);
    bool SaveData(std::string config_file_dir, std::map<std::string, std::string> string_map);
}

#endif