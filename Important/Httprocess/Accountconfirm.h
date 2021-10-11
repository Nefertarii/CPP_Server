#ifndef ACCOUNTCONFIRM_H_
#define ACCOUNTCONFIRM_H_

#include "../Gfilefunc.h"
#include "../Gjson.h"
#include "../Glog.h"
#include <algorithm>

//文字行 数据行在其文字下一行
struct Info_Line {
    size_t password = 0;
    size_t image = 0;
    size_t alias = 0;
    size_t id = 0;
};


//暂时用作数据库的替代
class Account_Parse {
private:
    Log* this_log;
    bool have_upper;
    bool read_success;
    std::string account_info_file;
    std::map<std::string, std::vector<std::string>> accounts_info;
    std::map<std::string, size_t> fail_counts;
    std::vector<std::string> account_template;
    Info_Line info_line;
    bool SaveAccountInfo();
    void AccountTemplate();

public:
    Account_Parse() { read_success = false; }
    void SetLog(Log* log_p, size_t buffer_size);
    bool ReadAccountInfoFile(std::string account_info_file_);
    std::string FindInfo(std::vector<std::string> account_info, std::string info_name);
    std::string FindInfo(std::string account, std::string info_name = "");
    bool Login(std::string account, std::string password);
    bool Regsiter(std::string account, std::string password, std::string alias);
    bool ChangePassword(std::string account, std::string oldpassword, std::string password);
    bool ChangeAccountInfo(std::string account, std::string password, std::string info_name, std::string new_info);
    std::string GetAccountInfo(std::string account);
    ~Account_Parse();
};

#endif