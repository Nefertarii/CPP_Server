#ifndef GRAPHSERV_H_
#define GRAPHSERV_H_

#include "../../Important/Gepollcontrol.h"
#include "../../Important/Gsocketctrl.h"
#include <string>
#include <vector>

struct Connectinfo {
    std::string ip = "";
    std::string port = "";
    std::string meassage = "";
    int socketfd = 0;

};

class Graph_Server_Control {
private:
    Epoll_Control epollctrl;
    Socket_Control socketctrl;
    Socket_Config config;
    std::vector<Connectinfo> connects;
    Log graph_server_log;
    int listenfd;
    void ConnectAccept();
    void MeassgeParse();

public:
    Graph_Server_Control();
    void ServerStart();
    void ServerReboot();
    void ServerStop();
    ~Graph_Server_Control();
};

Graph_Server_Control::Graph_Server_Control() {
    graph_server_log.Set("graph_server_log", 200);
    epollctrl.SetLog(&graph_server_log);
    socketctrl.SetLog(&graph_server_log);
    config.connect_max = 4;
    config.listen_port = 6888;
    config.write_max = 1024 * 4;
    socketctrl.SetConfig(config);
    connects.resize(config.connect_max * 2);
}

void Graph_Server_Control::ServerStart() {
    listenfd = socketctrl.SocketListen();
    struct epoll_event ev, events[config.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epollctrl.Epollfd();
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    for (;;) {
        int readyfds = epoll_wait(epollfd, events, config.connect_max, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                ConnectAccept();
            } else if (ev.events & EPOLLIN) {
                Connectinfo *client = static_cast<Connectinfo *>(ev.data.ptr);
                MeassgeParse();
            } else if (ev.events & EPOLLOUT) {
                Connectinfo *client = static_cast<Connectinfo *>(ev.data.ptr);
                ResultSend();
            }
        }
    }
}

void Graph_Server_Control::ServerReboot() {
    ServerStop();
    ServerStart();
}
void Graph_Server_Control::ServerStop() {
    ;
}
Graph_Server_Control::~Graph_Server_Control() {
    ;
}














#endif