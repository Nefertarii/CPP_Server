#include "Servprocess.h"

void Epolladd(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
    Savelog(INFO,"epoll control add");
}

void Epolldel(int socketfd, int epollfd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
    Savelog(INFO,"epoll control delete");
}

void Epollread(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    Savelog(INFO,"epoll control read");
}

void Epollwrite(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    Savelog(INFO,"epoll control write");
}