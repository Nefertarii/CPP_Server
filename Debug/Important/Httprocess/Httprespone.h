#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_

#include "Httperrno.h"

void Http_signal_handle();

//only encapsulates read and write operations
//need other function to control
class Http_Respone {
private:
    Log* this_log;
    bool have_upper;
    size_t writemax;
    size_t readmax;
public:
    Http_Respone() {}
    void SetLog(Log* log_p, size_t buffer_size);
    void SetWritemax(size_t writemax_) { writemax = writemax_; }
    void SetReadmax(size_t readmax_) { readmax = readmax_; }
    int Send(int socketfd, std::string* message);
    int SendFile(int socketfd, Filestate *file);
    ~Http_Respone();
    //static int Sendfile(int clientfd, std::string filename);
    //static int Sendfile(int socketfd, int filefd, off_t offset);
};

void Http_Respone::SetLog(Log* log_p, size_t buffer_size) {
    if (log_p == nullptr) {
        this_log = new Log("Http_Respone_Log.txt", buffer_size);
        have_upper = false;
    } else {
        this_log = log_p;
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

Http_Respone::~Http_Respone() {
    if (have_upper == false && this_log != nullptr) {
        delete this_log;
        this_log = nullptr;
    }
}

#endif









