#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_

#include "../Gservfunc.h"
#include "Httperrno.h"

class Httprespone {
private:
    ;
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










extern const size_t READMAX;

std::string Httprespone::Str_httpstate(int codenum) {
    switch (codenum) {
    case 200:
        return " OK\r\n";
    case 400:
        return " Bad Request\r\n";
    case 401:
        return " Unauthorized\r\n";
    case 403:
        return " Forbidden\r\n";
    case 404:
        return " Not Found\r\n";
    default:
        return " \r\n";
    }
}

std::string Httprespone::Filetype(std::string filename) {
    int i = filename.length();
    while (i > 0) {
        if (filename[i] == '.' || i == 0) {
            break;
        }
        i--;
    }
    std::string suffix(filename, i, filename.length());
    if (suffix == ".html") {
        return "text/html";
    } else if (suffix == ".css") {
        return "text/css";
    } else if (suffix == ".js") {
        return "text/javascript";
    } else if (suffix == ".data") {
        return "application/json";
    } else if (suffix == ".png") {
        return "image/png";
    } else if (suffix == ".svg") {
        return "image/svg+xml";
    } else if (suffix == ".ico") {
        return "image/x-icon";
    } else {
        return "text/plain";
    }
}

void Httprespone::Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength) {
    *responehead += ("HTTP/1.1 " + std::to_string(state) + Str_httpstate(state)); 
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += ("Content-Type:" + filetype + "\r\n");
    *responehead += ("Content-Length:" + std::to_string(bodylength) + "\r\n");
    *responehead += (Timer::Nowtime_str() + "\r\n");
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

REQUESTYPE Httprespone::Requestparse(std::string *readbuf) {
    if(readbuf->find_first_of("GET") == 0) {
        return GET;
    }
    if(readbuf->find_first_of("POST") == 0) {
        return POST;
    } else {
        return TYPENONE;
    }
}

int Httprespone::GETparse(std::string readbuf,std::string *filename) { //return GET request filename
    *filename = Substr(readbuf, 5, READMAX, ' ');
    if (*filename == "0") {
        *filename = "index.html";
    }
    if (*filename == "-1") {
        Infolog("Get request fail");
        return -1;
    }
    std::string tmp = "Get request:" + *filename;
    Infolog(tmp);
    return 0;
}
 
int Httprespone::GETprocess(std::string filename,Filestate *file) {
    if(!Servfunc::Readfile(filename, file)) {
        return -1;
    } else {
        return 0;
    }
}

int Httprespone::POSTparse(std::string request, std::string *post_type, std::string *post_data) { //return POST request type and date
    std::string type = Substr(request, 5, READMAX, ' ');
    type = Substr_Revers(type, 30, '/');
    if(type == "-1" || type == "0") {
        return -1;
    }
    std::string data = Substr_Revers(request, READMAX, '\n');
    if(data == "-1" || data == "0") {
        return -1;
    }
    *post_type = type;
    *post_data = data;
    return 0;
}

POSTYPE Httprespone::POSTchoose(std::string post_type) {
    if (post_type == "login") {
        return POSTLogin;
    } else if (post_type == "reset") {
        return POSTReset;
    } else if (post_type == "register") {
        return POSTRegister;
    } else if (post_type == "vote_up") {
        return POSTVoteup;
    } else if (post_type == "vote_down") {
        return POSTVotedown;
    } else if (post_type == "comment") {
        return POSTComment;
    } else if (post_type == "content") {
        return POSTContent;
    } else if (post_type == "readcount") {
        return POSTReadcount;
    } else if (post_type == "verification") {
        return POSTVerifi;
    } else {
        return POSTERR;
    }
}

int Httprespone::POSTprocess(std::string post_type, std::string post_data) {
    POSTYPE post = POSTchoose(post_type);
    switch (post) {
    case POSTLogin: {
        std::string tmp = "Login " + post_data;
        Infolog(tmp);
        break;
    } case POSTReset: {
        std::string tmp = "Reset password " + post_data;
        Infolog(tmp);
        break;
    } case POSTRegister: {
        std::string tmp = "Register " + post_data;
        Infolog(tmp);
        break;
    } case POSTVoteup: {
        std::string tmp = "Vote up " + post_data;
        Infolog(tmp);
        break;
    } case POSTVotedown: {
        std::string tmp = "Vote down " + post_data;
        Infolog(tmp);
        break;
    } case POSTComment: {
        std::string tmp = "Comment " + post_data;
        Infolog(tmp);
        break;
    } case POSTContent: {
        std::string tmp = "Content " + post_data;
        Infolog(tmp);
        break;
    } case POSTReadcount: {
        Infolog("Readcount add");
        break;
    } case POSTVerifi: {
        std::string tmp = "Verifi " + post_data;
        Infolog(tmp);
        break;
    } default: {
        std::string tmp = "Login " + post_data;
        Infolog(tmp);
        break;
    }} //switch end
    return 0;
}


#endif