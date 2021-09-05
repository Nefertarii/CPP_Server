#ifndef SERVFUNC_H_
#define SERVFUNC_H_

#include "../../Important/Gstring.h"
#include "../../Important/Gtime.h"
#include "../../Important/Glog.h"
#include "../../Important/Gthread.h"
#include "Serverrno.h"
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
/*  Globalvalue 
    const size_t SINGLECLIENTS = 8;                      //once thread max client
    const char *FILEDIR = "/home/http/server/";          //Default location for read file
    const size_t MAXLOG = 16;                            //Log buffer maximum
    const char *PROTOCOL = "HTTP";                       //server used protocol
    const size_t LISTENPORT = 80;                        //listen port
    const size_t READMAX = 1024 * 4;                     //once read max length
    const size_t WRITEMAX = 1024 * 4;                    //once send max length
    const size_t REWRITEMAX = 10;                        //signle EINTR rewrite nums
*/

//using
using ULL = unsigned long long;

//System function rewrite
namespace SERV {
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr *sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    int Read(int socketfd, std::string *str);
    int Readfile(std::string filename_,struct Filestate *filestat_);
    int Write(int socketfd, std::string *str);
    int Writefile(int socketfd, int filefd, off_t offset);
}

#endif