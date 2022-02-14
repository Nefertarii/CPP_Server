#include "Head/record.h"

Record::Record() {
    concurrency_flag = false;
    filename = "Default_log.txt";
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
    std::fstream file(filename, std::ios_base::out | std::ios::app);
    if (file) {
        file.write(info.c_str(), info.size());
        file.close();
        return true;
    }
    file.close();
    return false;
}

bool Record::Save_to_file(std::queue<std::string>* info) {
    std::fstream file(filename, std::ios_base::out | std::ios::app);
    std::string tmp_str;
    if (file) {
        size_t queue_size = info->size();
        for (size_t i = 0;i < queue_size;i++) {
            tmp_str = info->front();
            file.write(tmp_str.c_str(), tmp_str.size());
            info->pop();
        }
        file.close();
        return true;
    }
    file.close();
    return false;
}