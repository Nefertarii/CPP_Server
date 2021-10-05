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
    HttpHeadConfig& operator=(const HttpHeadConfig& new_config) {
        this->Constent_Charset = new_config.Constent_Charset;
        this->Content_Language = new_config.Content_Language;
        this->Server_Name = new_config.Server_Name;
        return *this;
    }
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
    void Reset() {
        port.clear();
        ip.clear();
        postdata.clear();
        respone_head.clear();
        respone_body.clear();
        clientfd = 0;
        rewrite_count = 0;
        fileinfo.filefd = 0;
        fileinfo.filelength = 0;
        fileinfo.offset = 0;
        requset_type = TYPENONE;
        http_state = HTTPNONE;
    }
};
//reset httpconnect
//clear httprocess

namespace StrCode {
    std::string StrErrorCode(SERVERR err_code);
    std::string StrStateCode(SERVSTATE state_code);
    std::string StrTypeCode(REQUESTYPE requset_type);
    std::string StrHttpCode(HTTPSTATE http_state);
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
    [POSTLogin] = "Login",
    [POSTReset] = "Reset password",
    [POSTRegister] = "Register",
    [POSTVoteup] = "Card vote up",
    [POSTVotedown] = "Card vote down",
    [POSTComment] = "Card comment",
    [POSTContent] = "Card show detail",
    [POSTReadcount] = "Website connect count add",
    [POSTVerifi] = "Verify code"
};
const char* Http100_map[] = {
    [0] = "Continue",               //100
};
const char* Http200_map[] = {
    [0] = "OK",                     //200
};
const char* Http300_map[] = {
    [0] = "Multiple Choices",       //300
};
const char* Http400_map[] = {
    [0] = "Bad Request",            //400
    [1] = "Unauthorized",           //401
    [2] = "Payment Required",       //402
    [3] = "Forbidden",              //403
    [4] = "Not Found",              //404
    [5] = "Method Not Allowed",     //405
};
const char* Http500_map[] = {
    [0] = "Internal Server Error",  //500
};

std::string StrCode::StrErrorCode(SERVERR err_code) { return Serverr_map[-err_code % ERREND]; }

std::string StrCode::StrStateCode(SERVSTATE state_code) { return Servstate_map[state_code % STATEND]; }

std::string StrCode::StrTypeCode(REQUESTYPE requset_type) { return Request_map[requset_type % TYPEND]; }

std::string StrCode::StrHttpCode(HTTPSTATE state_code) {
    switch (state_code / 100) {
    case 1: {
        return Http100_map[state_code - 100 % HTTPEND];
    }
    case 2: {
        return Http200_map[state_code-200 % HTTPEND];
    }
    case 3: {
        return Http300_map[state_code-300 % HTTPEND];
    }
    case 4: {
        return Http400_map[state_code-400 % HTTPEND];
    }
    case 5: {
        return Http500_map[state_code-500 % HTTPEND];
    }
    default: {
        return "";
    }
    }//swicth end
}











#endif