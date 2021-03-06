#ifndef SERVFUNC_H_
#define SERVFUNC_H_

//headfile include
#include "Glog.h"
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdarg>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <dirent.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <vector>

//setting
//Globalvalue
//const size_t MAXLOG = 16;                            //Log buffer maximum
//const char *PROTOCOL = "HTTP";                       //server used protocol

struct Filestate {
    int filefd = 0;
    size_t filelength = 0;
    size_t offset = 0;
};

struct Socket_Config {
    size_t connect_max = 0;
    size_t connect_nums = 0;
    size_t write_max = 0;
    size_t read_max = 0;
    int port = 0;
    int reuseaddr = 0;
    int reuseport = 0;
    Socket_Config& operator=(Socket_Config tmp);
};

namespace Gsocket {
    int Socket(int family, int type, int protocal, Log* log_p);
    int Bind(int fd, const struct sockaddr* sa, socklen_t salen, Log* log_p);
    int Listen(int fd, int backlog, Log* log_p);
    int Accept(int listenfd, Log* log_p);
    int Close(int fd, Log* log_p);
    int Connect(int socketfd, const struct sockaddr* sa, socklen_t salen, Log* log_p);
    int Read(int socketfd, std::string* str, size_t readmax, Log* log_p);
    int Readfile(std::string filename_, struct Filestate* filestat_, Log* log_p);
    int Write(int socketfd, std::string* str, Log* log_p);
    int Writefile(int socketfd, int filefd, off_t offset, size_t writemax ,Log* log_p);
    int ReuseAddress(int socketfd, int *flag, Log* log_p);
    int ReusePort(int socketfd, int* flag, Log* log_p);
    void GetAddress(int socketfd, std::string* ip, std::string* port, Log* log_p);
};

#endif