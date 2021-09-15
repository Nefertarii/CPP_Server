#ifndef SERVERRNO_H_
#define SERVERRNO_H_

#include <cstddef>
#include <string>
#include "../Gsocketfunc.h"

enum REQUESTYPE {
    TYPENONE = 0,
    GET,
    POST,
    TYPEND = (1 << 30)
};

enum SERVERR {
    ERRNONE = 0,
    WRITEFAIL = -1,
    ERREND = -(1 << 30)
};

enum SERVSTATE {
    STATENONE = 0,
    STATEND = (1 << 30)
};

enum HTTPSTATE {
    OK = 200,
    Badrequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    Notfound = 404
};

enum POSTYPE {
    POSTERR = 0,
    POSTLogin,
    POSTReset,
    POSTRegister,
    POSTVoteup,
    POSTVotedown,
    POSTComment,
    POSTContent,
    POSTReadcount,
    POSTVerifi
};



struct Clientinfo {
    std::string port = "";
    std::string ip = "";
    std::string respone_head = "";
    std::string respone_body = "";
    //client's socketfd
    int clientfd = 0;
    size_t rewrite_count = 0;
    Filestate fileinfo;
    SERVERR err_code = ERRNONE;
    SERVSTATE state_code = STATENONE;
    REQUESTYPE requset_type = TYPENONE;
    void Reset();
};
//reset httpconnect
//clear httprocess

class Strerr {
public:
    const char* Str_error(SERVERR err_code);
    const char* Str_state(SERVSTATE state_code);
    const char* Str_type(REQUESTYPE requset_type);
};

#endif