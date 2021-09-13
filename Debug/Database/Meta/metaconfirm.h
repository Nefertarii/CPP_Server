#ifndef METACONFIRM_H_
#define METACONFIRM_H_

#include "../../Important/Gtime.h"
#include "../../Important/Glog.h"
#include "../../Important/Gstring.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>

class Meta_Parse {
private:
    bool is_connect; //connect Graph server
    int socketfd;
    Log meta_log;
    std::string accountfile;
    std::string GQL;
    std::vector<std::string> accounts; //read form file 
    std::vector<int> fail_counts;
    std::vector<std::string> passwords;//read form file
    void HttpInit();
    void SaveAccount();
    bool SendGQL();
    bool ReadResult();

public:
    Meta_Parse();
    //Login
    bool Send(std::string GQLrequest);
    bool MetaLogin(std::string account, std::string password);
    bool MetaRegsiter(std::string account, std::string password);
    bool MetaChangePassword(std::string account, std::string oldpassword, std::string password);
    ~Meta_Parse();
};

size_t READMAX = 1024 * 40;

void Meta_Parse::HttpInit() {
    struct sockaddr_in servaddr;
    std::string recv, send;
    memset(&servaddr, 0, sizeof(servaddr));
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8848);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    signal(SIGPIPE, SIG_IGN);
    if (connect(socketfd, (sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        meta_log.Errorlog("connect Graph server error.", errno);
        is_connect = false;
    } else {
        is_connect = true;
    }
}

void Meta_Parse::SaveAccount() {
    //std::sort(accounts.begin(), accounts.end());
    std::fstream file;
    file.open(accountfile, std::ios::out);
    for (size_t i = 0; i != accounts.size(); i++) {
        file.write(accounts[i].c_str(), accounts[i].size());
        file.write("\n", 1);
        file.write(passwords[i].c_str(), passwords[i].size());
        file.write("\n", 1);
    }
    meta_log.Infolog("Account info save.");
}

bool Meta_Parse::SendGQL() {
    if (is_connect) {
        if (write(socketfd, GQL.c_str(), GQL.size()) < 0) {
            meta_log.Errorlog("Write GQL error.", errno);
        } else {
            std::string log = "Send GQL:" + GQL;
            meta_log.Infolog(log);
            return true;
        }
        GQL.clear();
    } else {
        std::cout << "Meta not connect Graph server.\n";
    }
    return false;
}

bool Meta_Parse::ReadResult() {
    if (is_connect) {
        char readbuf[READMAX] = { 0 };
        while (1) {
            int n = read(socketfd, readbuf, READMAX);
            if (n <= 0) {
                if (n == 0) {
                    meta_log.Infolog("Read done.");
                    return true;
                } else {
                    meta_log.Infolog("Read error.", errno);
                    return false;
                }
            } else {
                GQL += readbuf;
            }
        }
    }
    return false;
}

Meta_Parse::Meta_Parse() {
    accountfile = "Accountinfo.txt";
    meta_log.Set("login_log.txt", 200);
    std::fstream file;
    std::string account, password;
    std::string tmpstr;
    is_connect = false;
    int index = 0;
    file.open(accountfile, std::ios::in);
    if (file) {
        while (std::getline(file, tmpstr)) {
            if (index % 2 == 0) {
                accounts.push_back(tmpstr);
            } else {
                passwords.push_back(tmpstr);
            }
            index++;
        }
        fail_counts.resize(accounts.size());
        for (size_t i = 0; i != fail_counts.size(); i++) {
            fail_counts[i] = 1;
        }
        std::cout << "Database Meta running.\n";
    } else {
        std::cout << "can't open " << accountfile << "\n";
    }
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
    SaveAccount();
}

#endif