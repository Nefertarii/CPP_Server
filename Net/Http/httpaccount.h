#ifndef HTTP_ACCOUNT_H_
#define HTTP_ACCOUNT_H_

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
    AccountSet login_account;  // already logged account
    AccountInfo tmp_account;   // process use
    std::string account_file;

public:
    HttpAccount();
    bool Login();
    bool Regsiter();
    bool Change_passwd();
    bool Change_other();
    ~HttpAccount();
};
}
} // namespace Wasi::Http

#endif // !HTTP_ACCOUNT_H_