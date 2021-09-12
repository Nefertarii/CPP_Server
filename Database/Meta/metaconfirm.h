#ifndef METACONFIRM_H_
#define METACONFIRM_H_

#include "../../Important/Gtime.h"
#include "../../Important/Glog.h"
#include "../../Important/Gstring.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>

class Meta_Parse {
private:
    bool is_connect; //connect Graph server
    int socketfd;
    Log meta_log;
    std::string accountfile;
    std::string GQL;
    std::vector<std::string> accounts; //read form file 
    std::vector<int> fail_counts;
    std::vector<std::string> passwords;//read form file
    void HttpInit();
    void SaveAccount();
    bool SendGQL();
    bool ReadResult();
        
public:
    Meta_Parse();
    //Login
    bool Send(std::string GQLrequest);
    bool MetaLogin(std::string account, std::string password);
    bool MetaRegsiter(std::string account, std::string password);
    bool MetaChangePassword(std::string account, std::string oldpassword, std::string password);
    ~Meta_Parse();
};




#endif