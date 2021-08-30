#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_

#include "Servfunc.h"

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

std::string Str_httpstate(int codenum);
std::string Filetype(std::string filename);
void Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength);
REQUESTYPE Requestparse(std::string *readbuf);
int GETparse(std::string readbuf, std::string *filename);
int GETprocess(std::string filename, Filestate *file);
int POSTparse(std::string request, std::string *post_type, std::string *post_data);
POSTYPE POSTchoose(std::string post_type);
int POSTprocess(std::string post_type, std::string post_data);

#endif