#ifndef GRAPHSERVER_H_
#define GRAPHSERVER_H_

#include "../../Important/Gepollctrl.h"
#include "../../Important/Gsocketctrl_server.h"
#include "GQLparser.h"
#include <fstream>
#include <string>

struct Connectinfo {
    std::string ip = "";
    std::string port = "";
    std::string meassage = "";
    std::string token = "";
    int socketfd = -1;
    void Reset()
};

class Graph_Server_Control {
private:
    Epoll_Control epollctrl;
    Socket_Control_Server socketctrl;
    Socket_Config socket_settings;
    std::vector<Connectinfo> clients;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    Log graph_server_log;
    Timer server_clock;
    GQLparser GQLctrl;
    int listenfd;
    bool init_complite;
    void ConnectAccept(Connectinfo* client, int connectfd);
    void ConnectDel(Connectinfo* client);
    void MeassgeParse(Connectinfo* client);
    void ResultSend(Connectinfo* client);
    bool ReadConfig(std::string config_file);

public:
    Graph_Server_Control(std::string config_file);
    void ServerStart();
    void ServerReboot();
    void ServerStop();
    ~Graph_Server_Control();
};

#endif