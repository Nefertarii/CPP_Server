#include "httpaccount.h"
#include <Base/filehandler.h>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Http;

HttpAccount::HttpAccount(std::string account_file_name) {
    account_file.Open(account_file_name);
    // read file
    std::string accounts_info;
    account_file.Read(accounts_info, 0, 160);
    std::cout << accounts_info;
    // fill set
    account_file.Close();
}

bool HttpAccount::Login(std::string email, std::string password) {}

bool HttpAccount::Regsiter(std::string email, std::string password, std::string username) {}

bool HttpAccount::Change_passwd(std::string email, std::string oldpassword, std::string password) {}

bool HttpAccount::Change_other() {}

HttpAccount::~HttpAccount() {}