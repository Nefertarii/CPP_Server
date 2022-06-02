#ifndef HTTP_ACCOUNT_H_
#define HTTP_ACCOUNT_H_

#include <Base/filehandler.h>
#include <fstream>
#include <set>
#include <string>

namespace Wasi {
namespace Http {

struct AccountInfo {
    size_t user_id;
    std::string user_alias;
    std::string user_email;
    std::string user_password;
    // image = dir + id
    // ...
};

using AccountSet = std::set<size_t, AccountInfo>;

class HttpAccount {
private:
    static size_t user_id_now; // total create id
    AccountSet login_account;  // already login account
    AccountInfo tmp_account;   // process use
    Base::FileHandler account_file;

public:
    HttpAccount(std::string account_file_name);
    bool Login(std::string email, std::string password);
    bool Regsiter(std::string email, std::string password, std::string username);
    bool Change_passwd(std::string email, std::string oldpassword, std::string password);
    bool Change_other();
    ~HttpAccount();
};

}
} // namespace Wasi::Http

#endif // !HTTP_ACCOUNT_H_