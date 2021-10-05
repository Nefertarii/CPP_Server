#ifndef SERVCTRL_H_
#define SERVCTRL_H_

#include "../Important/Httprocess/Httphandler.h"
#include "../Important/Gsocketctrl_server.h"
#include "../Important/Gsocketctrl_client.h"
#include "../Important/Gepollctrl.h"

class Server_Control_Epoll {
private:
    Socket_Control_Server clientctrl;
    //Socket_Control_Client graphctrl;
    Socket_Config server_settings, connect_settings;
    std::vector<Clientinfo> clients;
    int listenfd;

    Epoll_Control epollctrl;
    HTTP_Handler httpctrl;
    Log http_server_log;
    
    Timer server_clock;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    size_t logbuf_size;
    bool init_complite;
    void ConnectAdd(Clientinfo* client, int connectfd);
    void ConnectDel(Clientinfo* client);
public:
    Server_Control_Epoll(std::string config_file);
    void ServerStart();
    void ServerReboot();
    void ServerStop();
    ~Server_Control_Epoll();
};

void Server_Control_Epoll::ConnectAdd(Clientinfo* client, int connectfd) {
    client->clientfd = connectfd;
    Gsocket::GetAddress(connectfd, &client->ip, &client->port, &http_server_log);
    epollctrl.Epolladd(connectfd, client);
    server_settings.connect_nums += 1;
}

void Server_Control_Epoll::ConnectDel(Clientinfo* client) {
    epollctrl.Epolldel(client->clientfd);
    clientctrl.SocketDisconnect(client->clientfd);
    client->Reset();
    server_settings.connect_nums -= 1;
}

Server_Control_Epoll::Server_Control_Epoll(std::string config_file) {
    if (Gfile::ReadConfig(config_file, &global_string_settings, &global_value_settings)) {
        logbuf_size = global_value_settings.find("MaxLogBuffer")->second;
        server_settings.connect_max = global_value_settings.find("MaxClients")->second;
        server_settings.connect_nums = 0;
        server_settings.write_max = global_value_settings.find("WriteMax")->second;
        server_settings.read_max = global_value_settings.find("ReadMax")->second;
        server_settings.port = global_value_settings.find("Listen")->second;
        server_settings.reuseaddr = global_value_settings.find("ReuseAddress")->second;
        server_settings.reuseport = global_value_settings.find("ReusePort")->second;
        //server settings
        http_server_log.Set("HTTP_Server_Log.txt", logbuf_size);
        clients.resize(server_settings.connect_max);
        //server http control init
        httpctrl.SetLog(&http_server_log, logbuf_size);
        httpctrl.Init(&global_string_settings, &global_value_settings);
        //server socket control init
        clientctrl.SetLog(&http_server_log, logbuf_size);
        clientctrl.SetConfig(&server_settings);
        //server epoll control init
        epollctrl.SetLog(&http_server_log, logbuf_size);
        //...//
        /*
        //connect_settings.write_max = global_value_settings.find("WriteMax")->second;
        //connect_settings.read_max = global_value_settings.find("ReadMax")->second;
        //graph connect control init
        graphctrl.SetLog(&http_server_log, logbuf_size);
        graphctrl.SetConfig(&connect_settings);
        std::string ip = global_string_settings.find("GraphIP")->second;
        int port = global_value_settings.find("GraphPort")->second;
        graphctrl.SetConnect(ip, port);
        */

        http_server_log.Infolog("Server initialization complete.");
        init_complite = true;        
    } else {
        std::cout << "Server initialization Fail!\n";
        init_complite = false;
    }
}

void Server_Control_Epoll::ServerStart() {
    if (init_complite == false) {
        std::cout << "Server not initialization, Can't Start.";
        return;
    }
    listenfd = clientctrl.SocketListen();
    if (listenfd < 0) {
        std::cout << "Server can't create.\n";
        return;
    }
    struct epoll_event ev, events[server_settings.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epoll_create(server_settings.connect_max);
    epollctrl.SetEpollfd(epollfd);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    for (;;) {
        int readyfds = epoll_wait(epollfd, events, (int)server_settings.connect_max, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            http_server_log.Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                int connectfd = clientctrl.SocketAccept();
                if (connectfd < 0) {
                    http_server_log.Errorlog("BCreateBadHeadad connect.");
                } else {
                    ConnectAdd(&clients[server_settings.connect_nums], connectfd);
                }
            } else if (ev.events & EPOLLIN) {
                Clientinfo* client = static_cast<Clientinfo*>(ev.data.ptr);
                std::string readbuf;
                if (clientctrl.SocketRead(client->clientfd, &readbuf) == 0) {
                    httpctrl.RequestParse(client, readbuf);
                    epollctrl.Epollwrite(client->clientfd, client);
                } else {
                    ConnectDel(client);
                }
            } else if (ev.events & EPOLLOUT) {
                Clientinfo* client = static_cast<Clientinfo*>(ev.data.ptr);
                int result = httpctrl.SendRespone(client);
                if (result < 0) {
                    ConnectDel(client);
                } else if (result == 0) {
                    epollctrl.Epollread(client->clientfd, client);
                } else {
                    epollctrl.Epollwrite(client->clientfd, client);
                }
            }
        }
    }
}

void Server_Control_Epoll::ServerReboot() {
    ServerStop();
    ServerStart();
}

void Server_Control_Epoll::ServerStop() {
    http_server_log.Warninglog("Server closeing.");
    for (size_t i = 0; i != server_settings.connect_max; i++) {
        if (clients[i].clientfd > 0) {
            ConnectDel(&clients[i]);
        }
    }
    clientctrl.SocketDisconnect(epollctrl.Epollfd());
    clientctrl.SocketDisconnect(listenfd);
    std::string log = "Server total run time:" + server_clock.Runtime_str() + " sec";
    http_server_log.Infolog(log);
    http_server_log.Infolog("Server is close now.");
    server_clock.Reset();
}

Server_Control_Epoll::~Server_Control_Epoll() {
    ServerStop();
}

#endif