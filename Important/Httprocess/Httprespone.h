#ifndef HTTPRESPONE_H_
#define HTTPRESPONE_H_

#include "../Gsocketfunc.h"
#include "Httperrno.h"

class Http_Respone {
private:
    Log* this_log;
    bool have_upper;
    size_t writemax;
    size_t readmax;
public:
    Http_Respone() {}
    void SetLog(Log* log, size_t buffer_size);
    void SetWritemax(size_t writemax_) { writemax = writemax_; }
    void SetReadmax(size_t readmax_) { readmax = readmax_; }
    void BadRequest404(std::string* responehead);
    void BadRequest403(std::string* responehead);
    int Send(int socketfd, std::string* message);
    int SendFile(int socketfd, Filestate *file);
    ~Http_Respone();
    //static int Sendfile(int clientfd, std::string filename);
    //static int Sendfile(int socketfd, int filefd, off_t offset);
};


#endif









