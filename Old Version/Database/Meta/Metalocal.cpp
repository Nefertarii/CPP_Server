#include "Metalocal.h"

bool Meta_Local::IsLeagle(std::string str) {
    for (size_t i = 0;i != str.size();i++) {
        char c = str[i];
        if (c < 48 || 122 < c) {
            return false;
        }
        if (97 <= c && c <= 122) {  //97-122 = "a~z"
            continue;
        }
        if (48 <= c && c <= 57) {   //48-57'0-9'
            continue;
        }
        if (65 <= c && c <= 90) {   //65-90 = "A~Z"
            continue;
        }
        if (c == 95) {              //95 = '_'
            continue;
        }
        return false;
    }
    return true;
}

bool Meta_Local::Input(std::string* str) {
    str->clear();
    std::getline(std::cin, *str);
    if (IsLeagle(*str)) {
        return true;
    } else {
        str->clear();
        return false;
    }
}

bool Meta_Local::PasswordInput(std::string* str) {
    str->clear();
    //std::string tmp = ;
    *str = getpass("input:");
    if (IsLeagle(*str)) {
        return true;
    } else {
        str->clear();
        return false;
    }
}

void Meta_Local::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        this_log = upper;
        have_upper = true;
    } else {
        this_log = new Log("LocalMeta_Log.txt", 200);
        have_upper = false;
    }
    accountctrl.SetLog(this_log, buffer_size);
}

void Meta_Local::ReadAccountConfirm(std::string accountfile) {
    if (accountctrl.ReadAccountFile(accountfile)) {
        read_success = true;
    } else {
        read_success = false;
    }
}

bool Meta_Local::ConfirmInteraction() {
    if (read_success) {
        while (1) {
            std::cout << "Method quit, login, regsiter, change\n";
            Input(&input);
            if (input == "quit") {
                break;
            } else if (input == "login") {
                std::cout << "Login... Account:";
                Input(&account);
                std::cout << "Password ";
                PasswordInput(&password);
                if (accountctrl.MetaLogin(account, password)) {
                    return true;
                } else {
                    return false;
                }
            } else if (input == "regsiter") {
                std::cout << "Regsiter... Account:";
                Input(&account);
                std::cout << "Password ";
                if (PasswordInput(&password)) {
                    std::cout << "Success.";
                } else {
                    std::cout << "Fail.";
                }
                accountctrl.MetaRegsiter(account, password);
            } else if (input == "change") {
                std::cout << "Change password... Account:";
                Input(&account);
                std::cout << "Old password ";
                PasswordInput(&oldpassword);
                std::cout << "New password ";
                PasswordInput(&password);
                if (accountctrl.MetaChangePassword(account, oldpassword, password)) {
                    std::cout << "Success.";
                } else {
                    std::cout << "Fail.";
                }

            } else {
                std::cout << "Bad input";
            }
            std::cout << std::endl;
        }
        return false;
    } else {
        this_log->Warninglog("Read file is not work.");
        return false;
    }
}

Meta_Local::~Meta_Local() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}
