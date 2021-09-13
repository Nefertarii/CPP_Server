#ifndef GRAPHSERV_H_
#define GRAPHSERV_H_

#include "../../Important/Gepollcontrol.h"
#include "../../Important/Gsocketctrl.h"
#include <string>
#include <vector>

struct Connectinfo {

};

class Graph_Server_Control {
private:
    Epoll_Control epollcontrol;
    Socket_Control socketctrl;
    std::vector<Connectinfo> connects;
    Log graph_server_log;
public:
    Graph_Server_Control();
    void ServerStart();
    void ServerReboot();
    void ServerStop();
    ~Graph_Server_Control();
};

Graph_Server_Control::Graph_Server_Control() {
    graph_server_log.Set("graph_server_log", 200);
    epollcontrol.SetLog(&graph_server_log);
    socketctrl.SetLog(&graph_server_log);
    Socket_Config config;
    config.connect_max = 4;
    config.listen_port = 6888;
    config.write_max = 1024 * 4;
    socketctrl.SetConfig(config);
}

void Graph_Server_Control::ServerStart() {
    socketctrl.SocketListen();
}

void Graph_Server_Control::ServerReboot();
void Graph_Server_Control::ServerStop();
Graph_Server_Control::~Graph_Server_Control();














#endif