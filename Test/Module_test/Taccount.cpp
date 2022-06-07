#include <Log/logging.h>
#include <Net/Http/httpaccount.h>
#include <iostream>

using namespace Wasi;
using namespace std;

void login_test() {
    Http::HttpAccount accounts("account.txt");
    if (accounts.Login("hncsgwc@gmail.com", "123456") == true)
        cout << "true\n";
}

void reg_test() {
    Http::HttpAccount accounts("account.txt");
    if (accounts.Regsiter("hncsgwc2@gmail.com", "123456", "wasi"))
        cout << "ture\n";
}

int main() {
    login_test();
}