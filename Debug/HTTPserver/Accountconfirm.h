#ifndef ACCOUNTCONFIRM_H_
#define ACCOUNTCONFIRM_H_

#include "../Important/Gfilefunc.h"
#include "../Important/Glog.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>

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
    bool ReadAccountFile(std::string account_file_);
    bool ReadAccountInfoFile(std::string account_info_file_);
    void SetLog(Log* log_p, size_t buffer_size);
    //Login
    bool Login(std::string account, std::string password);
    bool Regsiter(std::string account, std::string password);
    bool ChangePassword(std::string account, std::string oldpassword, std::string password);
    bool ChangeInfo(std::string account, std::string info_name, std::string new_info);
    ~Account_Parse();
};

bool Account_Parse::SaveAccount() {
    return Gfile::SaveData(account_file, &accounts);
}

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

bool Account_Parse::ReadAccountFile(std::string account_file_) {
    account_file = account_file_;
    if (Gfile::ReadConfig(account_file, &accounts)) {
        for (auto i : accounts) {
            fail_counts[i.first] = 0;
        }
        return true;
    }
    std::cout << "Can't read account file:" << account_file << "\n";
    read_success = false;
    return false;
}

bool Account_Parse::ReadAccountInfoFile(std::string account_info_file_) {
    account_info_file = account_info_file_;
    if (Gfile::ReadConfig(account_info_file, &accounts_info)) {
        read_success = true;
        return true;
    }
    std::cout << "Can't read account info file:" << account_info_file << "\n";
    read_success = false;
    return false;
}

bool Account_Parse::Login(std::string account, std::string password) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " Login ";
    auto account_it = accounts.find(account);
    if (account_it == accounts.end()) {
        log += "Not this account.";
        this_log->Infolog(log);
        return false; //Not this account.; 
    } else {
        auto fail_it = fail_counts.find(account);
        if (account_it->second == password && fail_it->second <= 5) {
            log += "success.";
            this_log->Infolog(log);
            return true; //Login success;   
        } else if (fail_it->second <= 5) {
            log += "fail. try times:" + std::to_string(fail_it->second);
            this_log->Infolog(log);
            fail_it->second += 1;
            return false; //Error password.;
        } else {
            log += "Maximum try";
            this_log->Infolog(log);
            fail_it->second += 1;
            return false; //Error time maximum.;                    
        }
    }
    log += "wtf.";
    this_log->Infolog(log);
    return false;
}

bool Account_Parse::Regsiter(std::string account, std::string password) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " regsiter ";
    auto account_it = accounts.find(account);
    if (account_it == accounts.end()) {
        accounts[account] = password;
        log += " success.";
        this_log->Infolog(log);
        return true; //Regsiter success.\n
    } else {
        log += "fail, already have same account.";
        this_log->Infolog(log);
        return true; //Regsiter success.\n
    }

}

bool Account_Parse::ChangePassword(std::string account, std::string oldpassword, std::string password) {
    if (!read_success) { return false; }
    std::string log = "User:" + account + " change password ";
    auto account_it = accounts.find(account);
    if (account_it == accounts.end()) {
        log += "fail, Not this account.";
        this_log->Infolog(log);
        return false; //Not this account.; 
    } else {

        if (account_it->second == oldpassword) {
            account_it->second = password;
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

bool Account_Parse::ChangeInfo(std::string account, std::string info_name, std::string new_info) {
    std::string log = "User:" + account + " change info ";
    auto it = accounts_info.find(account);
    if (it != accounts_info.end()) {
        for (size_t i = 0; i <= it->second.size(); i += 2) {
            if (it->second.at(i) == info_name) {
                it->second.at(i + 1) = new_info;
                log += "success.";
                this_log->Infolog(log);
                return true;
            }
        }
        return false;
        log += "fail, not this option.";
        this_log->Infolog(log);
    }
    log += "fail, not this account.";
    this_log->Infolog(log);
    return false;
}

Account_Parse::~Account_Parse() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
    SaveAccount();
    SaveAccountInfo();
}

#endif