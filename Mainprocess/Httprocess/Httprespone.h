#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_

#include "../Headfile.h"

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

static std::string Str_httpstate(int codenum);
static std::string Filetype(std::string filename);
static void Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength);
static REQUESTYPE Requestparse(std::string *readbuf);
static int GETparse(std::string readbuf, std::string *filename);
static int GETprocess(std::string filename, Filestate *file);
static int POSTparse(std::string request, std::string *post_type, std::string *post_data);
static POSTYPE POSTchoose(std::string post_type);
static int POSTprocess(std::string post_type, std::string post_data);

#endif