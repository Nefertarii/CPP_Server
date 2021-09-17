#ifndef GMETA_LOCAL_H_
#define GMETA_LOCAL_H_

#include "../../Important/Gsocketctrl.h"
#include "../../Important/Gtime.h"
#include "Accountconfirm.h"

struct Serverinfo {
    std::string name = "";
    std::string ip = "";
    std::string port = "";
    std::string token = "";
    std::string meassage = "";
    int socketfd = -1;
    void Reset() {
        ip.clear();
        port.clear();
        meassage.clear();
        token.clear();
        socketfd = -1;
    }
};

//read input and processing with local graph server
//p2p
class Metalocal {
private:
    Log* this_log;
    bool have_upper;
    Serverinfo server;
    Account_Parse accountctrl;
    std::string input;
    std::string account, oldpassword, password;
    bool isleagle(std::string str);
    bool Input(std::string& str);
    bool PasswordInput(std::string& str);
    bool Httpconnect();
    bool AccountConfirm();
    bool Read();
    bool Send();
public:
    Metalocal();
    void SetLog(Log* upper, size_t buffer_size);
    void Interaction();
    ~Metalocal();
};

bool isleagle(std::string str) {
    for (size_t i = 0;i != input.size();i++) {
        char c = input[i];
        if (c < 48 || 122 < c) {
            return false;
        }
        if (97 <= c && c <= 122) {  //97-122 = "a~z"
            return true;
        }
        if (48 <= c && c <= 57) {   //48-57'0-9'
            return true;
        }
        if (65 <= c && c <= 90) {   //65-90 = "A~Z"
            return true;
        }
        if (c == 95) {              //95 = '_'
            return true;
        }
        return false;
    }
}

bool Metalocal::Input(std::string& str) {
    str.clear();
    str = getpass("input:");
    std::getline(std::cin, str);
    if (isleagle(str)) {
        return true;
    } else {
        str.clear();
        return false;
    }
}

bool Metalocal::PasswordInput(std::string& str) {
    str.clear();
    str = getpass("input:");
    if (isleagle(str)) {
        return true;
    } else {
        str.clear();
        return false;
    }
}

Metalocal::Metalocal() {
    ;
}

void Metalocal::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        this_log = upper;
        have_upper = true;
    } else {
        this_log = new Log("LocalMeta_Log.txt", 200);
        have_upper = false;
    }
}

void Metalocal::Interaction() {
    while (1) {
        std::cout << "Method ";
        Input(input);
        if (input == "quit") {
            break;
        } else if (info == "login") {
            std::cout << "Login... Account ";
            Input(account);
            std::cout << "Password ";
            PasswordInput(password);
            T.MetaLogin(account, password);
        } else if (info == "regsiter") {
            std::cout << "Regsiter... Account ";
            Input(account);
            std::cout << "Password ";
            PasswordInput(password);
            T.MetaRegsiter(account, password);
        } else if (info == "change") {
            std::cout << "Change password... Account ";
            Input(account);
            std::cout << "Old password ";
            PasswordInput(oldpassword);
            std::cout << "New password ";
            PasswordInput(password);
            T.MetaChangePassword(account, oldpassword, password);
        } else {
            std::cout << "Bad input";
        }
        std::cout << std::endl;
    }
}

Metalocal::~Metalocal() {
    ;
}


#endif