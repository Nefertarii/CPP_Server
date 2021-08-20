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

//using
extern const size_t MAXthread;
using ULL = unsigned long long;
//typedef unsigned long long ULL

//System function rewrite
namespace SERV
{
    int Socket(int family, int type, int protocal);
    int Bind(int fd, const struct sockaddr *sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    int Listen(int fd, int backlog);
    int Accept(int listenfd);
    int Close(int fd);
    void Syserrlog(const char *fmt, int err);
    ULL Pthreadid();
    int Pthreadindex();
}

class Threadlog
{
private:
    int write_index;
public:
    //save function;
    ;
};

#endif