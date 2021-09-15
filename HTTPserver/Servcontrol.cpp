#include "Servcontrol.h"

bool Server_Control_Epoll::ReadConfig(std::string config_file) {
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
    }
    return false;
}

void Server_Control_Epoll::ConnectAdd(Clientinfo* client, int connectfd) {
    client->clientfd = connectfd;
    Gsocket::GetAddress(connectfd, &client->ip, &client->port, &server_log);
    epollctrl.Epolladd(connectfd, client);
    socket_settings.connect_nums += 1;
}

void Server_Control_Epoll::ConnectDel(Clientinfo* client) {
    epollctrl.Epolldel(client->clientfd);
    socketctrl.SocketDisconnet(client->clientfd);
    client->Reset();
    socket_settings.connect_nums -= 1;
}

Server_Control_Epoll::Server_Control_Epoll(std::string config_file) {
    if (ReadConfig(config_file)) {
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
        map_it = global_value_settings.find("ReuseAddress");
        socket_settings.reuseaddr = map_it->second;
        map_it = global_value_settings.find("ReusePort");
        socket_settings.reuseport = map_it->second;
        socket_settings.listenfd = -1;

        auto map_it2 = global_string_settings.find("DocumentRoot");
        std::string document_root = map_it2->second;

        server_log.Set("Server_Log.txt", logbuf_size);
        httpctrl.Init(&server_log, logbuf_size, document_root, socket_settings);
        clients.resize(socket_settings.connect_max);
        //socket
        socketctrl.SetLog(&server_log, logbuf_size);
        socketctrl.SetConfig(&socket_settings);
        //epoll
        epollctrl.SetLog(&server_log, logbuf_size);
        server_log.Infolog("Server initialization complete.");
        return;
    }
    server_log.Errorlog("Server initialization Fail!");
}

void Server_Control_Epoll::ServerStart() {
    socket_settings.listenfd = socketctrl.SocketListen();
    if (socket_settings.listenfd < 0) { return; }
    struct epoll_event ev, events[socket_settings.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epoll_create(socket_settings.connect_max);
    epollctrl.SetEpollfd(epollfd);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_settings.listenfd, &ev);

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
                    ConnectAdd(&clients[socket_settings.connect_nums], connectfd);
                }
            } else if (ev.events & EPOLLIN) {
                Clientinfo* client = static_cast<Clientinfo*>(ev.data.ptr);
                std::string readbuf;
                int result = socketctrl.SocketRead(client->clientfd, &readbuf);
                if (result == 0) {
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

void Server_Control_Epoll::ServerStop() {
    server_log.Warninglog("Server closeing.");
    for (size_t i = 0; i != socket_settings.connect_max; i++) {
        if (clients[i].clientfd > 0) {
            ConnectDel(&clients[i]);
        }
    }
    socketctrl.SocketDisconnet(epollctrl.Epollfd());
    socketctrl.SocketDisconnet(socket_settings.listenfd);
    std::string log = "Server total run time:" + server_clock.Runtime_str() + " sec";
    server_log.Infolog(log);
    server_log.Infolog("Server is close now.");
}

Server_Control_Epoll::~Server_Control_Epoll() {
    ServerStop();
}
