#include "httpaccount.h"
#include "httpenum.h"
#include <Log/logging.h>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Http;

std::string HttpAccount::Space_fill(int n) {
    return std::string(n, '=');
}

HttpAccount::HttpAccount() :
    user_id_now(0) {
}

HttpAccount::HttpAccount(std::string account_file_name) :
    tmp_str() {
    account_file.Open(account_file_name);
    std::string tmp_str;
    account_file.Read(tmp_str, 2, 7);
    user_id_now = std::stoi(tmp_str);
    account_file.Close();
}

void HttpAccount::Select_file(std::string account_file_name) {
    account_file.Open(account_file_name);
    std::string tmp_str;
    account_file.Read(tmp_str, 2, 7);
    user_id_now = std::stoi(tmp_str);
    account_file.Close();
}

std::string HttpAccount::Login(std::string email, std::string password) {
    if (user_id_now == 0) { return ""; }
    std::string log = "email:" + email;
    size_t ret      = account_file.Find("E:" + email);
    if (ret != std::string::npos) {
        // 40 = email line length
        size_t beg                    = 40 + ret;
        std::string contrast_password = "P:" + password + Space_fill(17 - password.size());
        if (account_file.Find(contrast_password, beg, 20) != std::string::npos) {
            log += " login success";
            LOG_INFO(log);
            std::string user_id;
            account_file.Read(user_id, ret - 8, 7);
            return user_id;
        }
    }
    log += " try login fail";
    LOG_INFO(log)
    return "";
}

bool HttpAccount::Regsiter(std::string email, std::string password, std::string username) {
    if (user_id_now == 0) { return false; }
    std::string log = "email:" + email;
    size_t ret      = account_file.Find("E:" + email);
    if (ret == std::string::npos) {
        std::string account_info;
        // ID
        account_info += "I:" + std::to_string(user_id_now) + "\n";
        // Email
        int space_num = 37 - email.size();
        account_info += "E:" + email;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Password
        space_num = 17 - password.size();
        account_info += "P:" + password;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Alias
        space_num = 17 - username.size();
        account_info += "A:" + username;
        account_info += Space_fill(space_num);
        account_info += "\n";
        // Empty
        account_info += "N:none===\n";
        account_info += "N:none===\n";
        account_info += "N:none===\n";
        account_info += "=========\n";
        account_file.Write(account_info);
        log += " regsiter success\n";
        LOG_INFO(log);
        user_id_now++;
        account_file.Write(std::to_string(user_id_now), 2);
        return true;
    }
    log += " regsiter fail\n";
    LOG_INFO(log);
    return false;
}

bool HttpAccount::Change_passwd(std::string email, std::string oldpassword, std::string newpassword) {
    if (user_id_now == 0) { return false; }
    std::string log = "email:" + email;
    size_t ret      = account_file.Find("E:" + email);
    if (ret != std::string::npos) {
        size_t beg                    = 40 + ret;
        std::string contrast_password = "P:" + oldpassword + Space_fill(17 - oldpassword.size());
        if (account_file.Find(contrast_password, beg, 20) != std::string::npos) {
            account_file.Write("P:" + newpassword, beg);
            log += " change password success";
            LOG_INFO(log)
            return true;
        }
    }
    log += " change password fail";
    LOG_INFO(log);
    return false;
}

bool HttpAccount::Change_other() { return false; }

bool HttpAccount::Find_account_id(std::string id) {
    std::string log = "user:" + id;
    if (account_file.Find("I:" + id) != std::string::npos) {
        log += " found";
        LOG_DEBUG(log);
        return true;
    }
    log += " not found";
    LOG_DEBUG(log);
    return false;
}

bool HttpAccount::Find_account_email(std::string email) {
    std::string log = "email:" + email;
    if (email.length() < 17 && email.length() > 4) {
        if (account_file.Find("E:" + email) != std::string::npos) {
            log += " found";
            LOG_DEBUG(log);
            return true;
        }
    }
    log += " not found";
    LOG_DEBUG(log);
    return false;
}

AccountInfo HttpAccount::Get_account_by_id(std::string id) {
    if (user_id_now == 0) { return AccountInfo(); }
    std::string log = "user:" + id;
    size_t ret      = account_file.Find("I:" + id);
    if (ret != std::string::npos) {
        size_t beg = 70 + ret + 2;
        account_file.Read(tmp_account.user_alias, beg, 17);
        int length = tmp_account.user_alias.find_first_of('=');

        tmp_account.user_id = id;
        tmp_account.user_alias.assign(tmp_account.user_alias, 0, length);
        // tmp_account.user_image = user_image_dir + "/" + id + ".png";
        tmp_account.user_image = user_image_dir + "/1000001.png";

        log += " found";
        LOG_INFO(log)
        return tmp_account;
    }
    log += " not found";
    LOG_INFO(log);
    return AccountInfo();
}

HttpAccount::~HttpAccount() {}