#ifndef HTTPROCESS_H_
#define HTTPROCESS_H_

#include "../Gsocketfunc.h"
#include "Httperrno.h"

class Http_Process {
private:
    size_t readmax;
    std::string document_root;
    Log* this_log;
    bool have_upper;

public:
    Http_Process() {}
    void SetLog(Log* log, size_t buffer_size);
    void SetReadmax(size_t readmax_) { readmax = readmax_; }
    void SetDocumentRoot(std::string document_root_) { document_root = document_root_; }

    std::string StrHttpState(int codenum);
    std::string FileType(std::string filename);
    void CreateResponeHead(std::string* responehead, std::string filetype, int state, int bodylength);
    REQUESTYPE RequestType(std::string* readbuf);
    //Use GET type parse readbuf and set fileinfo
    int GETParse(std::string readbuf, std::string* filename, Filestate* file);
    //Use POST type parse readbuf and set respone data
    int POSTParse(std::string request, std::string* post_type, std::string* post_data);
    //string to int for use switch
    POSTYPE POSTChoose(std::string post_type);
    ~Http_Process();
};


void Http_Process::SetLog(Log* log, size_t buffer_size) {
    if (log == nullptr) {
        this_log = new Log("Http_Process_Log.txt", buffer_size);
        have_upper = false;
    } else {
        this_log = log;
        have_upper = true;
    }
}

std::string Http_Process::StrHttpState(int codenum) {
    switch (codenum) {
    case 200:
        return "OK\r\n";
    case 400:
        return "Bad Request\r\n";
    case 401:
        return "Unauthorized\r\n";
    case 403:
        return "Forbidden\r\n";
    case 404:
        return "Not Found\r\n";
    default:
        return "\r\n";
    }
}

std::string Http_Process::FileType(std::string filename) {
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

void Http_Process::CreateResponeHead(std::string* responehead, std::string filetype, int state, int bodylength) {
    *responehead += ("HTTP/1.1 " + std::to_string(state) + " " + StrHttpState(state));
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += ("Content-Type:" + filetype + "\r\n");
    *responehead += ("Content-Length:" + std::to_string(bodylength) + "\r\n");
    *responehead += (Timer::Nowtime_str() + "\r\n");
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

REQUESTYPE Http_Process::RequestType(std::string* readbuf) {
    if (readbuf->find_first_of("GET") == 0) {
        return GET;
    }
    if (readbuf->find_first_of("POST") == 0) {
        return POST;
    }
    return TYPENONE;
}

int Http_Process::GETParse(std::string readbuf, std::string* filename, Filestate* fileinfo) { //return GET request filename
    *filename = Substr(readbuf, 5, readmax, ' ');
    if (*filename == "-1") {
        return -1;
    }
    if (*filename == "0") {
        *filename = "index.html";
    }
    *filename = document_root + *filename;
    if (Gsocket::Readfile(*filename, fileinfo, this_log) < 0) {
        return -1;
    }
    return 0;
}

int Http_Process::POSTParse(std::string request, std::string* post_type, std::string* post_data) { //return POST request type and date
    //Get POST type
    std::string type = Substr(request, 5, readmax, ' ');
    type = Substr_Revers(type, 30, '/');
    if (type == "-1" || type == "0") {
        return -1;
    }
    //Get readbuf data
    std::string data = Substr_Revers(request, readmax, '\n');
    if (data == "-1" || data == "0") {
        return -1;
    }
    //Handler post request
    std::string log;
    switch (POSTChoose(type)) {
    case POSTLogin: {
        //Login
        log = "Login:" + data;
        this_log->Infolog(log);
        break;
    } case POSTReset: {
        //Reset password
        log = "Reset password:" + data;
        this_log->Infolog(log);
        break;
    } case POSTRegister: {
        //Register
        log = "Register:" + data;
        this_log->Infolog(log);
        break;
    } case POSTVoteup: {
        //Vote up
        log = "Vote up:" + data;
        this_log->Infolog(log);
        break;
    } case POSTVotedown: {
        //Vote down
        log = "Vote down:" + data;
        this_log->Infolog(log);
        break;
    } case POSTComment: {
        //Comment
        log = "Comment:" + data;
        this_log->Infolog(log);
        break;
    } case POSTContent: {
        //Content
        log = "Content:" + data;
        this_log->Infolog(log);
        break;
    } case POSTReadcount: {
        //Readcount add
        log = "Readcount add:" + data;
        this_log->Infolog(log);
        break;
    } case POSTVerifi: {
        //Verification code
        log = "Verification:" + data;
        this_log->Infolog(log);
        break;
    } default: {
        //Error type
        break;
    }
    } //switch end


    *post_type = type;
    *post_data = data;
    return 0;

}

POSTYPE Http_Process::POSTChoose(std::string post_type) {
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

Http_Process::~Http_Process() {
    if (!have_upper) {
        delete this_log;
        this_log = nullptr;
    }
}

#endif