#ifndef SERVERRNO_H_
#define SERVERRNO_H_

#include "../Gsocketfunc.h"
#include <cstddef>

enum REQUESTYPE {
    TYPENONE = 0,
    GET,
    POST,
    POSTLogin,
    POSTReset,
    POSTRegister,
    POSTVoteup,
    POSTVotedown,
    POSTComment,
    POSTContent,
    POSTReadcount,
    POSTVerifi,
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
    HTTPNONE = 0,
    Continue = 100,
    OK = 200,
    Multiplechoices = 300,
    Badrequest = 400,
    Unauthorized = 401,
    Paymentrequired = 402,
    Forbidden = 403,
    Notfound = 404,
    Methodnotallowed = 405,
    Internalservererror = 500,
    HTTPEND = (1 << 30)
};

struct HttpHeadConfig {
    std::string Constent_Charset = "";
    std::string Content_Language = "";
    std::string Server_Name = "";
    HttpHeadConfig& operator=(const HttpHeadConfig& new_config);
};

struct Clientinfo {
    std::string port = "";
    std::string ip = "";
    std::string postdata = "";
    std::string respone_head = "";
    std::string respone_body = "";
    int clientfd = 0;       //client socketfd
    size_t rewrite_count = 0;
    Filestate fileinfo;
    REQUESTYPE requset_type = TYPENONE;
    HTTPSTATE http_state = HTTPNONE;
    void Reset();
};

namespace StrCode {
    std::string StrErrorCode(SERVERR err_code);
    std::string StrStateCode(SERVSTATE state_code);
    std::string StrTypeCode(REQUESTYPE requset_type);
    std::string StrHttpCode(HTTPSTATE http_state);
};

#endif