#include "Graphserver.h"

void Connectinfo::Reset() {
    ip.clear();
    port.clear();
    meassage.clear();
    token.clear();
    socketfd = -1;
}

bool Graph_Server_Control::ReadConfig(std::string config_file) {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool stringdone = false;
    file.open(config_file, std::ios::in);
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
        std::string log = "Can't open file:" + config_file + "\n";
        std::cout << log;
        return false;
    }
}

void Graph_Server_Control::ConnectAccept(Connectinfo* client, int connectfd) {
    //AES
    client->socketfd = connectfd;
    Gsocket::GetAddress(connectfd, &client->ip, &client->port, &graph_server_log);
    epollctrl.Epolladd(connectfd, client);
    socket_settings.connect_nums += 1;
}

void Graph_Server_Control::ConnectDel(Connectinfo* client) {
    epollctrl.Epolldel(client->socketfd);
    socketctrl.SocketDisconnet(client->socketfd);
    client->Reset();
    socket_settings.connect_nums -= 1;
}

void Graph_Server_Control::MeassgeParse(Connectinfo* client) {
    //GQLparse 
    

    
    client->meassage = "Get your message\n Is:" + client->meassage;
    epollctrl.Epollwrite(client->socketfd, client);
}

void Graph_Server_Control::ResultSend(Connectinfo* client) {
    if (socketctrl.SocketWrite(client->socketfd, &client->meassage) < 0) {
        ConnectDel(client);
    }
    if (client->meassage.empty()) {
        epollctrl.Epollread(client->socketfd, client);
    } else {
        epollctrl.Epollwrite(client->socketfd, client);
    }
}

Graph_Server_Control::Graph_Server_Control(std::string config_file) {
    if (ReadConfig(config_file)) {
        size_t logbuf_size = global_value_settings.find("MaxLogBuffer")->second;
        socket_settings.connect_max = global_value_settings.find("MaxClients")->second;
        socket_settings.connect_nums = 0;
        socket_settings.write_max = global_value_settings.find("WriteMax")->second;
        socket_settings.read_max = global_value_settings.find("ReadMax")->second;
        socket_settings.port = global_value_settings.find("Listen")->second;
        socket_settings.reuseaddr = global_value_settings.find("ReuseAddress")->second;
        socket_settings.reuseport = global_value_settings.find("ReusePort")->second;
        socket_settings.socketfd = -1;

        graph_server_log.Set("Graph_Server_Log.txt", logbuf_size);
        socketctrl.SetLog(&graph_server_log, logbuf_size);
        socketctrl.SetConfig(&socket_settings);
        epollctrl.SetLog(&graph_server_log, logbuf_size);
        graph_server_log.Infolog("Server initialization complete.");
        clients.resize(socket_settings.connect_max);
        init_complite = true;
    } else {
        std::cout << "Server initialization Fail!\n";
        init_complite = false;
    }
}

void Graph_Server_Control::ServerStart() {
    if (init_complite == false) {
        std::cout << "Server not initialization, Can't Start.";
        return;
    }
    socket_settings.socketfd = socketctrl.SocketListen();
    if (socket_settings.socketfd < 0) {
        std::cout << "Server cant't create.\n";
        return;
    }
    struct epoll_event ev, events[socket_settings.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epoll_create(socket_settings.connect_max);
    epollctrl.SetEpollfd(epollfd);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_settings.socketfd, &ev);

    for (;;) {
        int readyfds = epoll_wait(epollfd, events, (int)socket_settings.connect_max, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            graph_server_log.Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                int connectfd = socketctrl.SocketAccept();
                if (connectfd < 0) {
                    graph_server_log.Errorlog("Bad connect.");
                } else {
                    ConnectAccept(&clients[socket_settings.connect_nums], connectfd);
                }
            } else if (ev.events & EPOLLIN) {
                Connectinfo* client = static_cast<Connectinfo*>(ev.data.ptr);
                socketctrl.SocketRead(client->socketfd, &client->meassage);
                MeassgeParse(client);
            } else if (ev.events & EPOLLOUT) {
                Connectinfo* client = static_cast<Connectinfo*>(ev.data.ptr);
                ResultSend(client);
            }
        }
    }
}

void Graph_Server_Control::ServerReboot() {
    ServerStop();
    ServerStart();
}

void Graph_Server_Control::ServerStop() {
    graph_server_log.Warninglog("Server closeing.");
    for (size_t i = 0; i != socket_settings.connect_max; i++) {
        if (clients[i].socketfd > 0) {
            ConnectDel(&clients[i]);
        }
    }
    socketctrl.SocketDisconnet(epollctrl.Epollfd());
    socketctrl.SocketDisconnet(socket_settings.socketfd);
    std::string log = "Server total run time:" + server_clock.Runtime_str() + " sec";
    graph_server_log.Infolog(log);
    graph_server_log.Infolog("Server is close now.");
}

Graph_Server_Control::~Graph_Server_Control() {
    void ServerStop();
}