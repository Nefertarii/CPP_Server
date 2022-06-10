#ifndef HTTP_ACCOUNT_H_
#define HTTP_ACCOUNT_H_

#include <Base/filehandler.h>
#include <fstream>
#include <set>
#include <string>

namespace Wasi {
namespace Http {

struct AccountInfo {
    std::string user_id;
    std::string user_email;
    std::string user_password;
    std::string user_alias;
    std::string user_image;
    // image = dir + id
    // ...
};

using AccountSet = std::set<size_t, AccountInfo>;

class HttpAccount {
private:
    int user_id_now; // total create id
    int block_size = 130;
    /*
     * ID       I id        length=10
     * email    E email     length=40
     * passwd   P passwd    length=20
     * alias    A alias     length=20
     * none     N none      length=10
     * none     N none      length=10
     * none     N none      length=10
     * read separate        length=10
     * total = 130
     */
    AccountInfo tmp_account; // process use
    Base::FileHandler account_file;
    std::string tmp_str;
    std::string Space_fill(int n);

public:
    HttpAccount();
    HttpAccount(std::string account_file_name);
    void Select_file(std::string account_file_name);
    bool Login(std::string email, std::string password);
    bool Regsiter(std::string email, std::string password, std::string username);
    bool Change_passwd(std::string email, std::string oldpassword, std::string newpassword);
    bool Change_other();
    AccountInfo Get_account(std::string id);
    ~HttpAccount();
};

}
} // namespace Wasi::Http

#endif // !HTTP_ACCOUNT_H_