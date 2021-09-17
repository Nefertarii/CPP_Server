#include "Httprespone.h"

void Http_Respone::SetLog(Log* log, size_t buffer_size) {
    if (log == nullptr) {
        this_log = new Log("Http_Respone_Log.txt", buffer_size);
        have_upper = false;
    } else {
        this_log = log;
        have_upper = true;
    }
}

int Http_Respone::Send(int socketfd, std::string *message) {
    int ret = 0;
    if (message->size() > writemax) {
        std::string submessage(*message, 0, writemax);
        *message = message->substr(writemax, (message->size() - writemax));
        ret = Gsocket::Write(socketfd, &submessage, this_log);
    } else {
        ret = Gsocket::Write(socketfd, message, this_log);
        message->clear();
    }
    return ret;
}

int Http_Respone::SendFile(int socketfd, Filestate *file) {
    int ret = 0;
    if((file->filelength - file->offset) > writemax) {
        ret = Gsocket::Writefile(socketfd, file->filefd, file->offset, writemax, this_log);
        file->offset += writemax;
    } else {
        ret = Gsocket::Writefile(socketfd, file->filefd, file->offset, writemax, this_log);
        file->filefd = 0;
        file->filelength = 0;
        file->offset = 0;
    }
    return ret;
}

void Http_Respone::BadRequest404(std::string* responehead) {
    responehead->clear();
    *responehead += "HTTP/1.1 404 Bad Request\r\n";
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += "Content-Length:0\r\n";
    *responehead += Timer::Nowtime_str() + "\r\n";
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

void Http_Respone::BadRequest403(std::string* responehead) {
    responehead->clear();
    *responehead += "HTTP/1.1 403 Forbidden\r\n";
    *responehead += "Constent_Charset:utf-8\r\n";
    *responehead += "Content-Language:zh-CN\r\n";
    *responehead += "Content-Length:0\r\n";
    *responehead += Timer::Nowtime_str() + "\r\n";
    *responehead += "Server version:Gserver/1.0 (C++) \r\n\r\n";
}

Http_Respone::~Http_Respone() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}
