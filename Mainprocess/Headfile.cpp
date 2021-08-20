#include "Headfile.h"

int SERV::Socket(int family, int type, int protocol) {
    int sockfd = socket(family, type, protocol);
    if (sockfd < 0) {
        SERV::Syserrlog("Socket error.", errno);
        return -1;
    }
    return sockfd;
}
int SERV::Bind(int fd, const struct sockaddr *sa, socklen_t salen) {
    if (bind(fd, sa, salen) < 0) {
        SERV::Syserrlog("Bind error.", errno);
        return -1;
    }
    return 0;
}
int SERV::Listen(int fd, int backlog) {
    if (listen(fd, backlog) < 0) {
        SERV::Syserrlog("Listen error.", errno);
        return -1;
    }
    return 0;
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
            SERV::Syserrlog("Accept error.", errno);
            return -1;
            }
        Syserrlog("Get accept from", 0);
        Syserrlog(inet_ntoa(cliaddr.sin_addr), 0);
        return 0;
    }
    return -1;
}
int SERV::Close(int fd) {
    if (close(fd) < 0) {
        SERV::Syserrlog("Close error.", errno);
        return -1;
    }
    return 0;
}
void SERV::Syserrlog(const char *fmt, int err) {
    const char *tmperr = strerror(err);
    int tid_index = SERV::Pthreadindex();
    if(err) {
        Savelog(fmt, 0);
        Savelog(tmperr, 0);
    }
    else {
        Savelog(fmt, 0);
    }
}
ULL SERV::Pthreadid() {
    std::ostringstream buf;
    buf << std::this_thread::get_id();
    std::string stid = buf.str();
    ULL tid = std::stoull(stid);
    return tid;
}
int SERV::Pthreadindex() {
    int tid_index = Pthreadid() % MAXthread;
    return tid_index;
}