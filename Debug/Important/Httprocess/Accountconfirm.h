#ifndef ACCOUNTCONFIRM_H_
#define ACCOUNTCONFIRM_H_

#include "../Gfilefunc.h"
#include "../Gjson.h"
#include "../Glog.h"
#include <algorithm>

//文字行 数据行在其下一行
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

bool Account_Parse::SaveAccountInfo() {
    return Gfile::SaveData(account_info_file, &accounts_info);
}

void Account_Parse::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p != nullptr) {
        this_log = log_p;
        have_upper = true;
    } else {
        this_log = new Log("Meta_Confirm_txt", buffer_size);
        have_upper = false;
    }
}

bool Account_Parse::ReadAccountInfoFile(std::string account_info_file_) {
    account_info_file = account_info_file_;
    if (Gfile::ReadConfig(account_info_file, &accounts_info)) {
        read_success = true;
        AccountTemplate();
        return true;
    }
    std::cout << "Can't read account info file:" << account_info_file << "\n";
    read_success = false;
    return false;
}

std::string Account_Parse::FindInfo(std::vector<std::string> account_info, std::string info_name) {
    auto it = std::find(account_info.begin(), account_info.end(), info_name);
    it++;
    return *it;
}

std::string Account_Parse::FindInfo(std::string account, std::string info_name) {
    std::string log = "User:" + account + " findinfo " + info_name;
    auto it = accounts_info.find(account);
    if (it != accounts_info.end()) {
        if (info_name == "") {
            log += "Account found.";
            this_log->Infolog(log);
            return "Account found.";
        }
        for (size_t i = 0; i < it->second.size(); i += 2) {
            if (it->second.at(i) == info_name) {
                log += " found.";
                this_log->Infolog(log);
                return it->second.at(i + 1);
            }
        }
    }
    log += " Not found.";
    this_log->Infolog(log);
    return "";
}

void Account_Parse::AccountTemplate() {
    auto it1 = accounts_info.find("hncsgwc@gmail.com");
    for (size_t i = 0; i < it1->second.size(); i += 2) {
        if (it1->second.at(i) == "AccountPassword") {
            info_line.password = i;
        } else if (it1->second.at(i) == "AccountImage") {
            info_line.image = i;
        } else if (it1->second.at(i) == "AccountAlias") {
            info_line.alias = i;
        } else if (it1->second.at(i) == "AccountID") {
            info_line.id = i;
        } else {
            this_log->Warninglog("Not define Account info line.");
        }
    }
    std::string ID_now = "10000000";
    std::string ID_max = "10000000";
    for (auto it2 : accounts_info) {
        fail_counts[it2.first] = 0;
        ID_now = it2.second.at(info_line.id + 1);
        if (ID_max < ID_now) {
            ID_max = ID_now;
        }
    }
    for (size_t i = 0; i < it1->second.size(); i += 2) {
        account_template.push_back(it1->second.at(i));
        account_template.push_back("");
    }
    account_template[info_line.id + 1] = ID_max;
}

bool Account_Parse::Login(std::string account, std::string input_password) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " Login ";
    auto account_it = accounts_info.find(account);
    if (account_it == accounts_info.end()) {
        log += "Not this account.";
        this_log->Infolog(log);
        return false; //Not this account.; 
    } else {
        auto failcount_it = fail_counts.find(account);
        std::string password = account_it->second.at(info_line.password + 1);
        if (password == input_password && failcount_it->second <= 5) {
            log += "success.";
            this_log->Infolog(log);
            return true; //Login success;   
        } else if (failcount_it->second <= 5) {
            log += "fail. try times:" + std::to_string(failcount_it->second);
            this_log->Infolog(log);
            failcount_it->second += 1;
            return false; //Error password.;
        } else {
            log += "Maximum try";
            this_log->Infolog(log);
            failcount_it->second += 1;
            return false; //Error time maximum.;                    
        }
    }
    log += "wtf.";
    this_log->Infolog(log);
    return false;
}

bool Account_Parse::Regsiter(std::string account, std::string password, std::string alias) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " regsiter ";
    if (password.size() < 8 && 65 < password.size()) {
        log += "fail, password length to long/short.";
        this_log->Infolog(log);
        return false;
    }
    auto account_it = accounts_info.find(account);
    if (account_it == accounts_info.end()) {
        std::vector<std::string> newaccount;
        for (size_t i = 0; i < account_template.size(); i += 2) {
            if (i == info_line.password) {
                newaccount.push_back("AccountPassword");
                newaccount.push_back(password);
            } else if (i == info_line.image) {
                newaccount.push_back("AccountImage");
                newaccount.push_back("Image/Account/Default.png");
            } else if (i == info_line.alias) {
                newaccount.push_back("AccountAlias");
                newaccount.push_back(alias);
            } else if (i == info_line.id) {
                size_t tmpID = std::stoul(account_template[i + 1]) + 1;
                account_template[i + 1] = std::to_string(tmpID);
                newaccount.push_back("AccountID");
                newaccount.push_back(account_template[i + 1]);
            } else {
                this_log->Warninglog("Not define Account info line.");
            }
        }
        accounts_info[account] = newaccount;
        log += " success.";
        this_log->Infolog(log);
        return true; //Regsiter success.\n
    }
    log += "fail, already have same email.";
    this_log->Infolog(log);
    return false;
}

bool Account_Parse::ChangePassword(std::string account, std::string oldpassword, std::string password) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " change password ";
    auto account_it = accounts_info.find(account);
    if (account_it == accounts_info.end()) {
        log += "fail, Not this account.";
        this_log->Infolog(log);
        return false; //Not this account.; 
    } else {
        if (account_it->second.at(info_line.password + 1) == oldpassword) {
            account_it->second.at(info_line.password + 1) = password;
            auto fail_it = fail_counts.find(account);
            fail_it->second = 0;
            log += "success.";
            this_log->Infolog(log);
            return true; //Change password success.\n;
        } else {
            log += "fail, Error oldpassword.";
            this_log->Infolog(log);
            return false; //Error old password.\n;
        }
    }
}

bool Account_Parse::ChangeAccountInfo(std::string account, std::string password,
                                      std::string info_name, std::string new_info) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " change info " + info_name;
    auto it = accounts_info.find(account);
    if (it != accounts_info.end()) {
        if (it->second.at(info_line.password + 1) == password) {
            for (size_t i = 0; i < it->second.size(); i += 2) {
                if (it->second.at(i) == info_name) {
                    it->second.at(i + 1) = new_info;
                    log += "success.";
                    this_log->Infolog(log);
                    return true;
                }
            }
            log += "fail, error password.";
            this_log->Infolog(log);
            return false;
        }
        log += "fail, not this option.";
        this_log->Infolog(log);
        return false;
    }
    log += "fail, not this account.";
    this_log->Infolog(log);
    return false;
}

std::string Account_Parse::GetAccountInfo(std::string account) {
    if (!read_success) { return ""; }
    auto it = accounts_info.find(account);
    if (it != accounts_info.end()) {
        return JsonSpliced(it->second);
    }
    return "";
}

Account_Parse::~Account_Parse() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
    SaveAccountInfo();
}

#endif