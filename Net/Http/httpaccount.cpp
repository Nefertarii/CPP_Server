#include "httpaccount.h"
#include <Base/filehandler.h>

#include <iostream>

using namespace Wasi;
using namespace Wasi::Http;

HttpAccount::HttpAccount(std::string account_file_name) :
    tmp_str() {
    account_file.Open(account_file_name);
    std::string tmp_str;
    account_file.Read(tmp_str, 2, 10);
    user_id_now = std::stoi(tmp_str);
    account_file.Close();
}

bool HttpAccount::Find_id(std::string str, size_t beg, size_t end) {
    return true;
}

bool HttpAccount::Find_email(std::string str, size_t beg, size_t end) {
    return true;
}

bool HttpAccount::Find_passwd(std::string str, size_t beg, size_t end) {
    return true;
}

bool HttpAccount::Login(std::string email, std::string password) {
    account_file.Reopen();
    size_t file_size = account_file.Get_file_size();
    int amount       = 10;
    int account      = file_size / block_size;
    int times        = (account / amount) + 1;
    for (int i = 1; i <= times; i++) {
        account_file.Read(tmp_str, (block_size * amount * (i - 1)), (block_size * amount * i));
        size_t email_beg = 0, email_end = 0;
        size_t passwd_beg = 0, passwd_end = 0;
        for (int j = 0; j < amount; j++) {
            email_beg = tmp_str.find_first_of("E ", email_beg) + 2;
            email_end = tmp_str.find_first_not_of(' ', email_beg);
            std::string save_email(tmp_str, email_beg, email_end - email_beg);
            if (save_email == email) {
                passwd_beg = tmp_str.find_first_of("P ", passwd_beg) + 2;
                passwd_end = tmp_str.find_first_not_of(' ', passwd_beg);
                std::string save_passwd(tmp_str, passwd_beg, passwd_end - passwd_beg);
                if (save_passwd == password) { return true; }
                return false;
            }
            email_beg = email_end;
        }
        return false;
    }
    account_file.Close();
    return true;
}

bool HttpAccount::Regsiter(std::string email, std::string password, std::string username) {
    account_file.Reopen();
    // tmp str
    // add info
    // write to end
    account_file.Close();
    return true;
}

bool HttpAccount::Change_passwd(std::string email, std::string oldpassword, std::string password) {
    account_file.Reopen();
    // while
    // read file: i*130 ~ i+1*130
    // find E\r
    // compare email
    // find P\r
    // compare password
    // change password
    // change file
    account_file.Close();
    return true;
}

bool HttpAccount::Change_other() { return false; }

HttpAccount::~HttpAccount() {}