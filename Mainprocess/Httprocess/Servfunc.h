#ifndef SERVFUNC_H_
#define SERVFUNC_H_

//headfile include
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
//Globalvalue
static const size_t READMAX = 1024 * 40;                            //Once read max length
static const size_t WRITEMAX = 1024 * 40;                           //Once write max length
static const size_t REWRITEMAX = 5;                                 //Error retry max times
static const size_t LISTENPORT = 8000;                                //listen port
static const size_t SINGLECLIENTS = 8;                              //once thread max client
static const std::string FILEDIR = "/home/cs18/vscode/Webserver/Blog/";   //Default location for read file
//const size_t MAXLOG = 16;                            //Log buffer maximum
//const char *PROTOCOL = "HTTP";                       //server used protocol

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