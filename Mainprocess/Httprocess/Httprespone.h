#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_


#include "Servfunc.h"

class Httprespone {
private:
    const size_t READMAX = 1024 * 4;                            //once read max length;
    const char *FILEDIR = "/home/cs18/vscode/Webserver/Blog/";           //Default location for read file

public:
    std::string Str_httpstate(int codenum);
    std::string Filetype(std::string filename);
    void Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength);
    REQUESTYPE Requestparse(std::string *readbuf);
    int GETparse(std::string readbuf, std::string *filename);
    int GETprocess(std::string filename, Filestate *file);
    int POSTparse(std::string request, std::string *post_type, std::string *post_data);
    POSTYPE POSTchoose(std::string post_type);
    int POSTprocess(std::string post_type, std::string post_data);
};


#endif