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

#endif









