#include "Servprocess.h"

extern const size_t WRITEMAX;

void Epollcontrol::Set_epollfd(int epollfd_) {
    std::string log = "Epoll set, fd:" + std::to_string(epollfd) + ".";
    Infolog(log);
    epollfd = epollfd_;
    hasconnect = &epollfd;
}

void Epollcontrol::Epolladd(int socketfd) {
    std::string log = "Epoll add, fd:" + std::to_string(socketfd) + ".";
    Infolog(log);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
}

void Epollcontrol::Epolldel(int socketfd) { 
    std::string log = "Epoll del, fd:" + std::to_string(socketfd) + ".";
    Infolog(log);
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
}

void Epollcontrol::Epollread(int socketfd) {
    std::string log = "epoll ready read, fd:" + std::to_string(socketfd) + ".";
    Infolog(log);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Epollcontrol::Epollwrite(int socketfd) {
    std::string log = "epoll ready write, fd:" + std::to_string(socketfd) + ".";
    Infolog(log);
    struct epoll_event ev;
    ev.events = EPOLLOUT | EPOLLET;
    ev.data.ptr = hasconnect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Epollcontrol::Epolladd(int socketfd, Clientinfo *client) {
    std::string log = "epoll add, fd:" + std::to_string(socketfd) + ".";
    Infolog(log);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = client;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
}

void Epollcontrol::Epollread(int socketfd, Clientinfo *client) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = client;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Epollcontrol::Epollwrite(int socketfd, Clientinfo *client) {
    if(client->fileinfo.offset == 0) {
        std::string log = "epoll ready write, fd:" + std::to_string(socketfd) + ".";
        Infolog(log);
    }
    struct epoll_event ev;
    ev.events = EPOLLOUT | EPOLLET;
    ev.data.ptr = client;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

const int Epollcontrol::Epollfd() {
    return epollfd;
}