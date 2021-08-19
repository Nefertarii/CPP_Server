#ifndef HEADFILE_H_
#define HEADFILE_H_

//include head file
#include "../Important/Gstring.h"
#include "../Important/Gthread.h"
#include "../Important/Gtime.h"
#include "../Important/Log.h"
#include <arpa/inet.h>
#include <cstdarg>
#include <iostream>
#include <signal.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

//using
using Tid = std::thread::id;
const size_t MAXthread = 10;

//System function rewrite
namespace SERV {
    int Socket(int family, int type, int protocal);
    void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
    //backlog: number of connections successfully established and waiting to be accepted
    void Listen(int fd, int backlog);
    int Accept(int listenfd);
    void Close(int fd);
    void Syserrlog(const char *fmt, int err, int index);
    void Syslog(int count, ...);
    void Pthreadexit();
}

int SERV::Socket(int family, int type, int protocol) {
    int sockfd = socket(family, type, protocol);
    if (sockfd < 0) {
        Tid tid = std::thread::get_id();
        SERV::Syserrlog("Socket error.", errno, (tid % MAXthread));
        SERV::Pthreadexit();
    }
    return sockfd;
}
void SERV::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        Tid tid = std::thread::get_id();
        SERV::Syserrlog("Bind error.", errno, (tid % MAXthread));
        SERV::Pthreadexit();
    }
}
void SERV::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        Tid tid = std::thread::get_id();
        SERV::Syserrlog("Listen error.", errno, (tid % MAXthread));
        SERV::Pthreadexit();
    }
}
int SERV::Accept(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int cnt = 0;
    while (cnt != 10) {
        int connectfd = accept(listenfd, (sockaddr *)&cliaddr, &cliaddrlen);
        if (connectfd < 0) {
            if (errno == EINTR)
                continue;
            Tid tid = std::thread::get_id();
            SERV::Syserrlog("Accept error.", errno, (tid % MAXthread));
            Pthreadexit();
            return 0;
        }
        SERV::Syslog(2, "Get accept from", inet_ntoa(cliaddr.sin_addr));
    }
    return -1;
}
void SERV::Close(int fd) {
    if (close(fd) < 0)
        SERV::Syserrlog("Close error.", errno);
}
void SERV::Syserrlog(const char *fmt, int err) {
    const char *tmperr = strerror(err);
    Savelog(fmt);
    Savelog(tmperr);
}
void SERV::Syslog(int count, ...) {
    std::string *start_p = &count;
    int i = 1;
    std::vector<std::string> log;
    log.resize(count - 1);
    while (i != count)
    {
        start_p++;
        log[i] = *strat_p;
        i++;
    }
    Savelog(log);
}

#endif