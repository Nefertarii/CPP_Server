#ifndef SERVERRNO_H_
#define SERVERRNO_H_

#include "../Gsocketfunc.h"
#include <cstddef>
#include <string>

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
    void Reset() {
        port.clear();
        ip.clear();
        respone_head.clear();
        respone_body.clear();
        clientfd = 0;
        rewrite_count = 0;
        fileinfo.filefd = 0;
        fileinfo.filelength = 0;
        fileinfo.offset = 0;
        err_code = ERRNONE;
        state_code = STATENONE;
        requset_type = TYPENONE;
    }
};
//reset httpconnect
//clear httprocess

class Strerr {
public:
    const char* Str_error(SERVERR err_code);
    const char* Str_state(SERVSTATE state_code);
    const char* Str_type(REQUESTYPE requset_type);
};











const char* Serverr_map[] = {
    [-ERRNONE] = "undefine server error.",
    [-WRITEFAIL] = "send respone fail."
};

const char* Servstate_map[] = {
    [STATENONE] = "undefine server state."
};

const char* Request_map[] = {
    [TYPENONE] = "undefine requeset type.",
    [GET] = "GET",
    [POST] = "POST",
};

const char* Strerr::Str_error(SERVERR err_code) { return Serverr_map[-err_code % ERREND]; }

const char* Strerr::Str_state(SERVSTATE state_code) { return Servstate_map[state_code % STATEND]; }

const char* Strerr::Str_type(REQUESTYPE requset_type) { return Request_map[requset_type % TYPEND]; }




#endif