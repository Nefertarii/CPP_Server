#ifndef METALOCAL_H_
#define METALOCAL_H_

#include "../../Important/Gtime.h"
#include "Accountconfirm.h"

//read input and processing with local graph server
//p2p
class Meta_Local {
private:
    Log* this_log;
    bool have_upper;
    bool read_success;
    Account_Parse accountctrl;
    std::string input;
    std::string account, oldpassword, password;
    bool IsLeagle(std::string str);
    bool Input(std::string* str);
    bool PasswordInput(std::string* str);
    bool AccountConfirm();
public:
    Meta_Local() {}
    void SetLog(Log* upper, size_t buffer_size);
    void ReadAccountConfirm(std::string accountfile);
    bool ConfirmInteraction();
    void GraphInteraction();
    ~Meta_Local();
};

#endif