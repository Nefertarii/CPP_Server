#include "Servprocess.h"

void Epollcontrol::Set_epollfd(int epollfd_) { epollfd = epollfd_; }

void Epollcontrol::Epolladd(int socketfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev); 
}

void Epollcontrol::Epolldel(int socketfd) { epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr); }

void Epollcontrol::Epollread(int socketfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Epollcontrol::Epollwrite(int socketfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}