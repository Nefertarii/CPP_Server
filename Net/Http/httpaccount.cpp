#include "httpaccount.h"
#include <Base/filehandler.h>
#include <Log/logging.h>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Http;

std::string HttpAccount::Space_fill(int n) {
    return std::string(n, ' ');
}

HttpAccount::HttpAccount(std::string account_file_name) :
    tmp_str() {
    account_file.Open(account_file_name);
    std::string tmp_str;
    account_file.Read(tmp_str, 2, 7);
    user_id_now = std::stoi(tmp_str);
    account_file.Close();
}

bool HttpAccount::Login(std::string email, std::string password) {
    std::string log = "email:" + email;
    size_t ret      = account_file.Find("E " + email);
    if (ret != std::string::npos) {
        size_t beg = 40 + ret;
        if (account_file.Find("P " + password, beg, 20) != std::string::npos) {
            log += " login success";
            LOG_INFO(log)
            return true;
        }
    }
    log += " try login fail";
    LOG_INFO(log)
    return false;
}

bool HttpAccount::Regsiter(std::string email, std::string password, std::string username) {
    std::string log = "email:" + email;
    size_t ret      = account_file.Find("E " + email);
    if (ret == std::string::npos) {
        std::string account_info;
        // ID
        account_info += "I " + std::to_string(user_id_now) + "\n";
        // Email
        int space_num = 37 - email.size();
        account_info += "E " + email;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Password
        space_num = 17 - password.size();
        account_info += "P " + password;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Alias
        space_num = 17 - username.size();
        account_info += "A " + username;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Empty
        account_info += "N none   ";
        account_info += "N none   ";
        account_info += "N none   ";
        account_info += "----------";
        account_file.Write(account_info);
        log += " regsiter success\n";
        LOG_INFO(log);
        return true;
    }
    log += " regsiter fail\n";
    LOG_INFO(log);
    return false;
}

bool HttpAccount::Change_passwd(std::string email, std::string oldpassword, std::string password) {
    // while
    // read file: i*130 ~ i+1*130
    // find E\r
    // compare email
    // find P\r
    // compare password
    // change password
    // change file
    return true;
}

bool HttpAccount::Change_other() { return false; }

HttpAccount::~HttpAccount() {}