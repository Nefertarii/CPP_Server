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

bool Account_Parse::ReadAccount() {
    if (Gfile::ReadConfig(accountfile, &accounts)) {
        for (auto i : accounts) {
            fail_counts[i.first] = 0;
        }
        read_success = true;
        return true;
    }
    read_success = false;
    return false;
}

bool Account_Parse::SaveAccount() {
    return Gfile::SaveData(accountfile, accounts);
}

Account_Parse::Account_Parse(std::string accountfile_) {
    accountfile = accountfile_;
    read_success = false;
    if (!ReadAccount()) {
        std::cout << "Can't read account file:" << accountfile << "\n";
        return;
    }
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

bool Account_Parse::ReadAccountFile(std::string accountfile_) {
    accountfile = accountfile_;
    if (!ReadAccount()) {
        std::cout << "Can't read account file:" << accountfile << "\n";
        return false;
    }
    return true;
}

bool Account_Parse::MetaLogin(std::string account, std::string password) {
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

bool Account_Parse::MetaRegsiter(std::string account, std::string password) {
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

bool Account_Parse::MetaChangePassword(std::string account, std::string oldpassword, std::string password) {
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

Account_Parse::~Account_Parse() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
    SaveAccount();
}

#endif