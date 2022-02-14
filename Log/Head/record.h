#ifndef RECORD_H_
#define RECORD_H_

#include <string>
#include <fstream>
#include <queue>

class Record {
private:
    std::string filename;
    bool concurrency_flag;
public:
    Record();
    Record(std::string save_file, bool concurrency_flag_);
    void Set_flag(bool concurrency_flag_);
    void Set_filename(std::string save_file);
    bool Save_to_file(std::string info);
    bool Save_to_file(std::queue<std::string>* info);
};


#endif