#ifndef METACONFIRM_H_
#define METACONFIRM_H_

#include "../../Important/Gfilefunc.h"
#include "../../Important/Glog.h"
#include <fstream>
#include <iostream>
#include <algorithm>

class Meta_Parse {
private:
    Log* this_log;
    bool have_upper;
    std::string accountfile;
    std::map<std::string, std::string> accounts;
    std::vector<int> fail_counts;
    void SetLog(Log* log_p);
    void SaveAccount();
    void ReadAccount();

public:
    Meta_Parse();
    //Login
    bool MetaLogin(std::string account, std::string password);
    bool MetaRegsiter(std::string account, std::string password);
    bool MetaChangePassword(std::string account, std::string oldpassword, std::string password);
    ~Meta_Parse();
};

void Meta_Parse::SaveAccount() {
    //std::sort(accounts.begin(), accounts.end());
    std::fstream file;
    file.open(accountfile, std::ios::out);
    for (size_t i = 0; i != accounts.size(); i++) {
        ;
    }
    meta_log.Infolog("Account info save.");
}

void Meta_Parse::ReadAccount() {
    
}


Meta_Parse::Meta_Parse() {

}

//little data, not use sort algorithm
bool Meta_Parse::MetaLogin(std::string account, std::string password) {
    std::string log = "User:" + account + " Login";
    for (size_t i = 0; i != accounts.size(); i++) {
        if (accounts[i] == account) {
            if (passwords[i] == password && fail_counts[i] <= 5) {
                log += " success.";
                meta_log.Infolog(log);
                return true; //Login success;                    
            } else if (fail_counts[i] <= 5) {
                log += " fail. try times:" + std::to_string(fail_counts[i]);
                meta_log.Infolog(log);
                fail_counts[i] += 1;
                return false; //Error password.;
            } else {
                log += " Maximum try";
                meta_log.Infolog(log);
                fail_counts[i] += 1;
                return false; //Error time maximum.;                    
            }
        }
    }
    log += " Not this account.";
    meta_log.Infolog(log);
    return false; //Not this account.;
}

bool Meta_Parse::MetaRegsiter(std::string account, std::string password) {
    std::string log = "User:" + account + " regsiter";
    for (size_t i = 0; i != accounts.size(); i++) {
        if (accounts[i] == account) {
            log += " fail.";
            meta_log.Infolog(log);
            return false; //Already have same account.\n;
        }
    }
    accounts.push_back(account);
    passwords.push_back(password);
    log += " success.";
    meta_log.Infolog(log);
    return true; //Regsiter success.\n
}

bool Meta_Parse::MetaChangePassword(std::string account, std::string oldpassword, std::string password) {
    std::string log = "User:" + account + " change password";
    for (size_t i = 0; i != accounts.size(); i++) {
        if (accounts[i] == account) {
            if (passwords[i] == oldpassword) {
                passwords[i] = password;
                log += " success.";
                meta_log.Infolog(log);
                return true; //Change password success.\n;  
            } else {
                log += " fail.";
                meta_log.Infolog(log);
                return false; //Not curret old password.\n;
            }
        }
    }
    log += " fail.";
    meta_log.Infolog(log);
    return false; //Not this user.\n;
}

Meta_Parse::~Meta_Parse() {
    if (!have_upper) {
        delete this_log;
        this_log = nullptr;
    }
}

#endif