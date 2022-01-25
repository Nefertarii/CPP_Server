#include "Head/record.h"

Record::Record() {
    concurrency_flag = false;
    filename = "Default_Save.txt";
}

Record::Record(std::string save_file, bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
    filename = save_file;
}

void Record::Set_filename(std::string save_file) {
    filename = save_file;
}

void Record::Set_flag(bool concurrency_flag_) {
    concurrency_flag = concurrency_flag_;
}

bool Record::Save_to_file(std::string info) {
    std::fstream file(filename, std::ios::out);
    if (file) {
        file.write(info, info.size());
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool Record::Save_to_file(std::vector<std::string> info) {
    std::fstream file(filename);
    if (file) {
        for (auto line : info) {
            file.write(line, line.size());
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}