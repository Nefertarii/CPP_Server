#ifndef SERVCONTROL_H_
#define SERVCONTROL_H_

#include "../Important/Httprocess/Httphandler.h"
#include "../Important/Gsocketctrl.h"
#include "../Important/Gepollcontrol.h"
#include <fstream>
#include <map>

class Server_Control_Epoll {
private:
    Socket_Control socketctrl;
    Epoll_Control epollctrl;
    HTTP_Handler httpctrl;
    Log server_log;
    Timer server_clock;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    std::vector<Clientinfo> clients;
    Socket_Config socket_settings;
    size_t logbuf_size;
    bool ReadConfig();
public:
    Server_Control_Epoll();
    void ServerStart();
    void ServerStop();
    ~Server_Control_Epoll();
};

bool Server_Control_Epoll::ReadConfig() {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool stringdone = false;
    file.open("/home/cs18/vscode/Webserver/Http.conf", std::ios::in);
    if (file) {
        while (std::getline(file, fileline)) {
            if (fileline == "----") {
                stringdone = true;
            }
            if (!stringdone) {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        key = fileline.substr(0, i);
                        value_str = fileline.substr(i + 1, fileline.size());
                        global_string_settings[key] = value_str;
                    }
                }
            } else {
                for (size_t i = 0; i != fileline.size(); i++) {
                    if (fileline[i] == ' ') {
                        key = fileline.substr(0, i);
                        value_ul = std::stoul(fileline.substr(i + 1, fileline.size()));
                        global_value_settings[key] = value_ul;
                    }
                }
            }
        }
        file.close();
        return true;
    } else {
        std::cout << "Can't open file \"Http.conf\"\n";
    }
    return false;
}

Server_Control_Epoll::Server_Control_Epoll() {
    if (ReadConfig()) {
        auto map_it = global_value_settings.find("MaxLogBuffer");
        logbuf_size = map_it->second;
        map_it = global_value_settings.find("MaxClients");
        socket_settings.connect_max = map_it->second;
        socket_settings.connect_nums = 0;
        map_it = global_value_settings.find("WriteMax");
        socket_settings.write_max = map_it->second;
        map_it = global_value_settings.find("ReadMax");
        socket_settings.read_max = map_it->second;
        map_it = global_value_settings.find("Listen");
        socket_settings.listen_port = map_it->second;
        socket_settings.listenfd = 0;
        auto map_it2 = global_string_settings.find("DocumentRoot");
        std::string document_root = map_it2->second;
        httpctrl.Init(&server_log, logbuf_size, document_root, socket_settings);
        clients.resize(socket_settings.connect_max);
        //socket
        socketctrl.SetLog(&server_log, logbuf_size);
        socketctrl.SetConfig(socket_settings);
        //epoll
        epollctrl.SetLog(&server_log, logbuf_size);
        server_log.Infolog("Server initialization complete.");
        return;
    }
    server_log.Errorlog("Server initialization Fail!");
}

void Server_Control_Epoll::ServerStart() {
    socket_settings.listenfd = socketctrl.SocketListen();
    if (socket_settings.listenfd < 0) {
        return;
    }
    
    struct epoll_event ev, events[socket_settings.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epoll_create(socket_settings.connect_max);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_settings.listenfd, &ev);
    epollctrl.SetEpollfd(epollfd);

    for (;;) {
        int readyfds = epoll_wait(epollfd, events, (int)socket_settings.connect_max, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            server_log.Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                int connectfd = socketctrl.SocketAccept();
                if (connectfd < 0) {
                    server_log.Errorlog("Bad connect.");
                } else {
                    clients[socket_settings.connect_nums].clientfd = connectfd;
                    epollctrl.Epolladd(connectfd, &clients[i]);
                    socket_settings.connect_nums += 1;
                }
            } else if (ev.events & EPOLLIN) {
                Clientinfo* client = static_cast<Clientinfo*>(ev.data.ptr);
                std::string readbuf;
                int result = socketctrl.SocketRead(client->clientfd, &readbuf);
                if (result == 0) {
                    httpctrl.RequestParse(client, readbuf);
                    epollctrl.Epollwrite(client->clientfd, client);
                } else {
                    epollctrl.Epolldel(client->clientfd);
                    socketctrl.SocketDisconnet(client->clientfd);
                    socket_settings.connect_nums -= 1;
                }
            } else if (ev.events & EPOLLOUT) {
                Clientinfo* client = static_cast<Clientinfo*>(ev.data.ptr);
                int result = httpctrl.SendRespone(client);
                if (result < 0) {
                    epollctrl.Epolldel(client->clientfd);
                    socketctrl.SocketDisconnet(client->clientfd);
                    socket_settings.connect_nums -= 1;
                } else if (result == 0) {
                    epollctrl.Epollwrite(client->clientfd, client);
                } else {
                    epollctrl.Epollread(client->clientfd, client);
                }
            }
        }
    }
}

void Server_Control_Epoll::ServerStop() {
    server_log.Warninglog("Server closeing.");
    for (size_t i = 0; i != socket_settings.connect_nums; i++) {
        if (clients[i].clientfd > 0) {
            epollctrl.Epolldel(clients[i].clientfd);
            socketctrl.SocketDisconnet(clients[i].clientfd);
        }
    }
    socketctrl.SocketDisconnet(epollctrl.Epollfd());
    socketctrl.SocketDisconnet(socket_settings.listenfd);
    std::string log = "Server total run time:" + server_clock.Runtime_str() + " sec";
    server_log.Infolog(log);
    server_log.Infolog("Server is close now.");
    server_log.Savetofile();
}

Server_Control_Epoll::~Server_Control_Epoll() {
    ServerStop();
}

#endif