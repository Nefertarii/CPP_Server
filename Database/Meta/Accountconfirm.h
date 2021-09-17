#ifndef ACCOUNTCONFIRM_H_
#define ACCOUNTCONFIRM_H_

#include "../../Important/Gfilefunc.h"
#include "../../Important/Glog.h"
#include <fstream>
#include <iostream>
#include <algorithm>

class Account_Parse {
private:
    Log* this_log;
    bool have_upper;
    bool read_success;
    std::string accountfile;
    std::map<std::string, std::string> accounts;
    std::map<std::string, size_t> fail_counts;
    bool ReadAccount();
    bool SaveAccount();

public:
    Account_Parse() { read_success = false; }
    Account_Parse(std::string accountfile_);
    bool ReadAccountFile(std::string accountfile_);
    void SetLog(Log* log_p, size_t buffer_size);
    //Login
    bool MetaLogin(std::string account, std::string password);
    bool MetaRegsiter(std::string account, std::string password);
    bool MetaChangePassword(std::string account, std::string oldpassword, std::string password);
    ~Account_Parse();
};

#endif