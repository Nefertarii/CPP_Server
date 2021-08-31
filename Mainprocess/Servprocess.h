#ifndef SERVPROCESS_H_
#define SERVPROCESS_H_


#include "../Important/Glog.h"
#include <sys/epoll.h>
#include <vector>
#include <string>

class Epollcontrol {
private:
    void *hasconnect;
    int epollfd;

public:
    Epollcontrol(){}
    void Set_epollfd(int epollfd_);
    void Epolladd(int socketfd);
    void Epolldel(int socketfd);
    void Epollread(int socketfd);
    void Epollwrite(int socketfd);
    ~Epollcontrol(){}
};


#endif