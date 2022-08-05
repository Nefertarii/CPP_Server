#include <Log/logging.h>
#include <Net/Http/httpaccount.h>
#include <iostream>
#include <Base/exception.h>

using namespace Wasi;
using namespace std;

void login_test() {
    Http::HttpAccount accounts("home/ubuntu/Webserver/Test/Userinfo/account.txt");
    std::cout << "get id:" << accounts.Login("hncsgwc@gmail.com", "654321") << "\n";
}

void find_test() {
    Http::HttpAccount accounts("home/ubuntu/Webserver/Test/Userinfo/account.txt");
    if (accounts.Find_account_email("hncsgwc@gmail.com"))
        cout << "ture\n";
}

void reg_test() {
    Http::HttpAccount accounts("home/ubuntu/Webserver/Test/Userinfo/account.txt");
    if (accounts.Regsiter("hncsgwc4@gmail.com", "123456", "wasi"))
        cout << "ture\n";
}

void change_test() {
    Http::HttpAccount accounts("home/ubuntu/Webserver/Test/Userinfo/account.txt");
    if (accounts.Change_passwd("hncsgwc@gmail.com", "123456", "654321"))
        cout << "ture\n";
}

int main() {
    try {
        find_test();
    } catch (Wasi::Exception e) {
        e.What();
    }
}