#include "Httprespone.h"

std::string Str_httpstate(int codenum) {
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

std::string Filetype(std::string filename) {
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
    }
    else if (suffix == ".css") {
        return "text/css";
    }
    else if (suffix == ".js") {
        return "text/javascript";
    }
    else if (suffix == ".data") {
        return "application/json";
    }
    else if (suffix == ".png") {
        return "image/png";
    }
    else if (suffix == ".svg") {
        return "image/svg+xml";
    }
    else if (suffix == ".ico") {
        return "image/x-icon";
    }
    else {
        return "text/plain";
    }
}

void Create_respone_head(std::string *responehead, std::string filetype, int state, int bodylength) {
    *responehead += ("HTTP/1.1 " + std::to_string(state) + Str_httpstate(state)); 
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += ("Content-Type:" + filetype + "\r\n");
    *responehead += ("Content-Length:" + std::to_string(bodylength) + "\r\n");
    *responehead += (Timer::Nowtime_str() + "\r\n");
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

REQUESTYPE Requestparse(std::string *readbuf) {
    if(readbuf->find_first_of("GET") == 0) {
        return GET;
    }
    else if(readbuf->find_first_of("POST") == 0) {
        return POST;
    }
    else {
        return TYPENONE;
    }
}

int GETparse(std::string readbuf,std::string *filename) { //return GET request filename
    std::string filedir = FILEDIR;
    std::string request_file = Substr(readbuf, 5, READMAX, ' ');
    if (request_file == "0") {
        request_file = "index.html";

    }
    if (request_file == "-1") {
        Savelog(INFO, "Get request fail");
        return -1;
    }
    *filename = filedir + request_file;
    std::string tmp = "Get request:" + request_file;
    Savelog(INFO, tmp);
    return 0;
}
 
int GETprocess(std::string filename,Filestate *file) {
    if(!SERV::Readfile(filename, file)) {
        return -1;
    }
    else {
        return 0;
    }
}

int POSTparse(std::string request, std::string *post_type, std::string *post_data) { //return POST request type and date
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

POSTYPE POSTchoose(std::string post_type) {
    if (post_type == "login") {
        return POSTLogin;
    }
    else if (post_type == "reset") {
        return POSTReset;
    }
    else if (post_type == "register") {
        return POSTRegister;
    }
    else if (post_type == "vote_up") {
        return POSTVoteup;
    }
    else if (post_type == "vote_down") {
        return POSTVotedown;
    }
    else if (post_type == "comment") {
        return POSTComment;
    }
    else if (post_type == "content") {
        return POSTContent;
    }
    else if (post_type == "readcount") {
        return POSTReadcount;
    }
    else if (post_type == "verification") {
        return POSTVerifi;
    }
    else {
        return POSTERR;
    }
}

int POSTprocess(std::string post_type, std::string post_data) {
    POSTYPE post = POSTchoose(post_type);
    switch (post) {
    case POSTLogin: {
        std::string tmp = "Login " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    case POSTReset: {
        std::string tmp = "Reset password " + post_data;
        Savelog(INFO, tmp);
        break;
    }
    
    case POSTRegister: {
        std::string tmp = "Register " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    case POSTVoteup: {
        std::string tmp = "Vote up " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    case POSTVotedown: {
        std::string tmp = "Vote down " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    case POSTComment: {
        std::string tmp = "Comment " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    case POSTContent: {
        std::string tmp = "Content " + post_data;
        Savelog(INFO, tmp);
        break;
    }
    
    case POSTReadcount: {
        Savelog(INFO, "Readcount add");
        break;
    }

    case POSTVerifi: {
        std::string tmp = "Verifi " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    default: {
        std::string tmp = "Login " + post_data;
        Savelog(INFO, tmp);
        break;
    }

    }
    return 0;
}