#include "Accountconfirm.h"
#include <iostream>


using namespace std;

int main() {
    Account_Parse accountctrl;
    accountctrl.SetLog(nullptr, 200);
    accountctrl.ReadAccountFile("/home/cs18/vscode/Webserver/Config/Account.txt");
    accountctrl.ReadAccountInfoFile("/home/cs18/vscode/Webserver/Config/Accountinfo.txt");
    accountctrl.ChangeInfo("tempaccount0", "AccountAlias", "UserTest");
    accountctrl.ChangePassword("tempaccount0", "password0", "password00");
}
