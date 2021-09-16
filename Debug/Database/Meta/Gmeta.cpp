#include "Metaconfirm.h"

using namespace std;
int main(int argc, const char* argv[]) {
    std::string file;
    if (argc != 2) {
        file = "/home/cs18/vscode/Webserver/Accountinfo.txt";
    } else {
        file = argv[1];
    }
    Meta_Parse T(file);
    T.SetLog(nullptr, 200);
    string info;
    string username;
    string oldpasswd, passwd;
    while (1) {
        cout << "input method:";
        getline(cin, info);
        if (info == "quit") {
            break;
        } else if (info == "login") {
            cout << "login.\n";
            cout << "input account:\n";
            getline(cin, username);
            cout << "input password:\n";
            getline(cin, passwd);
            T.MetaLogin(username, passwd);
        } else if (info == "regsiter") {
            cout << "regsiter.\n";
            cout << "input account:\n";
            getline(cin, username);
            cout << "input password:\n";
            getline(cin, passwd);
            T.MetaRegsiter(username, passwd);
        } else if (info == "change") {
            cout << "change.\n";
            cout << "input account:\n";
            getline(cin, username);
            cout << "input old password:\n";
            getline(cin, oldpasswd);
            cout << "input new password:\n";
            getline(cin, passwd);
            T.MetaChangePassword(username, oldpasswd, passwd);
        } else {
            cout << "Bad input.\n";
        }
    }
}