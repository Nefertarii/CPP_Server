#ifndef ACCOUNTCONFIRM_H_
#define ACCOUNTCONFIRM_H_

#include "../Gfilefunc.h"
#include "../Gjson.h"
#include "../Glog.h"
#include <algorithm>

//暂时用作数据库的替代
class Account_Parse {
private:
    Log* this_log;
    bool have_upper;
    bool read_success;
    std::string account_file;
    std::string account_info_file;
    std::map<std::string, std::string> accounts;
    std::map<std::string, std::vector<std::string>> accounts_info;
    std::map<std::string, size_t> fail_counts;
    bool SaveAccount();
    bool SaveAccountInfo();

public:
    Account_Parse() { read_success = false; }
    void SetLog(Log* log_p, size_t buffer_size);
    bool ReadAccountFile(std::string account_file_);
    bool ReadAccountInfoFile(std::string account_info_file_);
    bool Login(std::string account, std::string password);
    bool Regsiter(std::string account, std::string password);
    bool ChangePassword(std::string account, std::string oldpassword, std::string password);
    bool ChangeAccountInfo(std::string account, std::string info_name, std::string new_info);
    std::string GetAccountInfo(std::string account);
    ~Account_Parse();
};

#endif