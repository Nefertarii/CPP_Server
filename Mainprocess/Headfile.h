#ifndef HEADFILE_H_
#define HEADFILE_H_

//include head file
#include "../Important/Gstring.h"
#include "../Important/Gthread.h"
#include "../Important/Gtime.h"
#include "../Important/Glog.h"
#include "../Important/Gjson.h"
#include "Serverrno.h"

#include <arpa/inet.h>
#include <cstdarg>
#include <iostream>
#include <signal.h>
#include <string>
#include <sstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

//include head file
#include "../Important/Gstring.h"
#include "../Important/Gthread.h"
#include "../Important/Gtime.h"
#include "../Important/Glog.h"
#include "../Important/Gjson.h"
#include "Serverrno.h"

#include <arpa/inet.h>
#include <cstdarg>
#include <iostream>
#include <signal.h>
#include <string>
#include <sstream>
#include <dirent.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <vector>

//setting
static const size_t SINGLECLIENTS; //once thread max client
static const char *DIR;            //Default location for read file
static const char *PROTOCOL;       //server used protocol
static const size_t LISTENPORT;    //listen port
static const size_t READMAX;       //once read max length
static const size_t WRITEMAX;      //once send max length
static const size_t REWRITEMAX;    //signle EINTR rewrite nums

//using
using ULL = unsigned long long;

struct Filestate {
    int filefd = 0;
    int filelength = 0;
};

//System function rewrite
namespace SERV
{
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr *sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    int Read(int socketfd, std::string *str);
    int Readfile(std::string filename_,struct Filestate *filestat_);
    int Write(int socketfd, std::string str);
    int Writefile(int socketfd, int filefd, off_t offset);
    void Syserrlog(const char *fmt, int err);
    ULL Pthreadid();
    int Pthreadindex();
}

#endif