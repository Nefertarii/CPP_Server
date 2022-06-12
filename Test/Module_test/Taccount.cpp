#include <Log/logging.h>
#include <Net/Http/httpaccount.h>
#include <iostream>

using namespace Wasi;
using namespace std;

void login_test() {
    Http::HttpAccount accounts("/home/nefertarii/vscode/CPP_Server/Test/Userinfo/account.txt");
    std::cout << "get id:" << accounts.Login("hncsgwc@gmail.com", "654321") << "\n";
}

void reg_test() {
    Http::HttpAccount accounts("account.txt");
    if (accounts.Regsiter("hncsgwc4@gmail.com", "123456", "wasi"))
        cout << "ture\n";
}

void change_test() {
    Http::HttpAccount accounts("account.txt");
    if (accounts.Change_passwd("hncsgwc@gmail.com", "123456", "654321"))
        cout << "ture\n";
}

int main() {
    login_test();
}