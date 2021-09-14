#ifndef SERVCONTROL_H_
#define SERVCONTROL_H_

#include "../Important/Httprocess/Httphandler.h"
#include "../Important/Gepollcontrol.h"
#include <fstream>
#include <map>

class Server_Control_Epoll {
private:
    Socket_Control socketctrl;
    Epoll_Control epollctrl;
    HTTP_Handler httpctrl;
    Log server_log;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    std::vector<Clientinfo> clients;
    Socket_Config socket_settings;
    size_t logbuf_size;
    bool ReadConfig();
public:
    Server_Control_Epoll();
    void ServerStart()
    void ServerStop();
    ~Server_Control_Epoll();
};

bool Server_Control_Epoll::ReadConfig() {
    std::fstream file;
    std::string fileline;
    std::string key, value_str;
    size_t value_ul;
    bool stringdone = false;
    file.open("Http.conf", std::ios::in);
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
    } else {
        std::cout<<"Can't open Http.conf."
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

        httpctrl.Init(&server_log, logbuf_size, socket_settings);
        clients.resize(socket_settings.connect_max * 2);
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
    struct epoll_event ev, events[socket_settings.connect_max];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epoll_create(socket_settings.connect_max);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    epollctrl.SetEpollfd(epollfd);

    for (;;) {
        int readyfds = epoll_wait(epollfd, events, socket_settings.connect_max, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            server_log.Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                int connectfd;
                if (connectfd < 0) {
                    server_log.Errorlog("Bad connect.");
                } else {
                    clients[socket_settings.connect_nums].clientfd = connectfd;
                    epollctrl.Epolladd(connectfd, &clients[i]);
                    socket_settings.connect_nums += 1;
                }
            } else if (ev.events & EPOLLIN) {
                Clientinfo *client = static_cast<Clientinfo *>(ev.data.ptr);
                std::string readbuf;
                int ret = socketctrl.SocketRead(client->clientfd, &readbuf);
                if (!ret) {
                    httpctrl.RequestParse(&client, readbuf);
                } else if (ret > 0) {
                    //read error or read FIN
                    socketctrl.Disconnect();
                }
            } else if (ev.events & EPOLLOUT) {
                Clientinfo *client = static_cast<Clientinfo *>(ev.data.ptr);
                Send_responehead(client);
                Send_responefile(client);
                Send_responebody(client);
            }
        }
    }
}
void Server_Control_Epoll::ServerStop() {
    ;
}

void Server_start_Epollcontrol() {


    //main control
    for (;;) {
        int readyfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if (readyfds < 0 && errno != EINTR) { //epoll create fail
            Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return;
        }
        for (int i = 0; i < readyfds; i++) {
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                Connect_accept();
            } else if (ev.events & EPOLLIN) {
                Clientinfo *client = static_cast<Clientinfo *>(ev.data.ptr);
                std::string readbuf;
                int ret = processctrl.Read(client->clientfd, &readbuf);
                if (!ret) {
                    switch (responectrl.Requestparse(&readbuf)) {
                    case GET: {
                        Connect_method_get(client, &readbuf);
                        break;
                    } case POST: {
                        Connect_method_post(client, &readbuf);
                        break;
                    } default: {
                        //bad request disconnect;
                        break;
                    }} //switch end
                } else if (ret > 0) {
                    //read error or read FIN
                    Connect_disconnect(client);
                }
            } else if (ev.events & EPOLLOUT) {
                Clientinfo *client = static_cast<Clientinfo *>(ev.data.ptr);
                Send_responehead(client);
                Send_responefile(client);
                Send_responebody(client);
            }
        }
    }
}

void Server_stop() {
    Warninglog("Server closeing.");
    for (size_t i = 0; i != MAXCLIENT; i++) {
        if (clients[i].clientfd > 0) {
            Connect_disconnect(&clients[i]);
        }
    }
    //threadpool.shutdown();
    Servfunc::Close(listenfd);
    Servfunc::Close(epollctrl.Epollfd());
    std::string log = "Server total run time:" + serverclock.Runtime_str() + " sec";
    Savetofile();
    Infolog(log);
    Infolog("Server is close.");
}

void Connect_accept() {
    int connectfd = Servfunc::Accept(listenfd);
    if (!socketctrl.Canconnect()) {
        int index = socketctrl.Connect_nums();
        clients[index].clientfd = connectfd;
        processctrl.Set_client(&clients[index]);
        std::string log = clients[index].ip + ":" + clients[index].port;
        epollctrl.Epolladd(connectfd, &clients[index]);
        log = log + " accept success";
        Infolog(log);
    } else {
        Infolog("Clients is maximum, Waiting disconnect.");
    }
}

void Connect_method_get(Clientinfo *client, std::string *readbuf) {
    std::string filename;
    if (!responectrl.GETparse(*readbuf, &filename)) {
        responectrl.GETprocess(filename, &client->fileinfo);
        responectrl.Create_respone_head(&client->respone_head,
                                        responectrl.Filetype(filename),
                                        200, client->fileinfo.filelength);
    } else {
        badrequest.Respone404(&client->respone_head);
    }
    epollctrl.Epollwrite(client->clientfd, client);
}

void Connect_method_post(Clientinfo *client, std::string *readbuf) {
    Infolog("Into post request.");
    badrequest.Respone404(&client->respone_head);
    epollctrl.Epollwrite(client->clientfd, client);
    return;
}

void Connect_disconnect(Clientinfo *client) {
    epollctrl.Epolldel(client->clientfd);
    socketctrl.Disconnect(client);
}

void Send_responehead(Clientinfo *client) {
    //ret
    //-1 errno
    // 0 success
    // 1 signal interruption
    // 2 cache full
    int socketfd = client->clientfd;
    if (client->respone_head.empty()) {
        return;
    } else {
        int ret = processctrl.Send(socketfd, &client->respone_head);
        switch (ret) {
        case -1: {
            Connect_disconnect(client);
            return;
        } case 0: {
            client->respone_head.clear();
            return;
        } case 1: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        } case 2: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        }} //switch end
    }
}

void Send_responebody(Clientinfo *client) {
    //can only be send file or info(json/data)
    int socketfd = client->clientfd;
    if (client->respone_body.empty()) {
        return;
    }
    else {
        int ret = processctrl.Send(socketfd, &client->respone_body);
        switch (ret) {
        case -1: {
            Connect_disconnect(client);
            return;
        } case 0: {
            if (client->respone_body.empty()) {
                epollctrl.Epollread(socketfd, client);
            } else {
                epollctrl.Epollwrite(socketfd, client);
            }
            return;
        } case 1: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        } case 2: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        }} //switch end
    }
}

void Send_responefile(Clientinfo *client) {
    //send file
    int socketfd = client->clientfd;
    if (client->fileinfo.filefd == 0) {
        Connect_disconnect(client);
        return;
    } else {
        if (client->fileinfo.offset == 0) {
            std::string log = "Write file to: " + std::to_string(socketfd);
            std::string writetimes = std::to_string((client->fileinfo.filelength / WRITEMAX) + 1);
            log += " write count: " + writetimes;
            Infolog(log);
        }
        int ret = processctrl.Sendfile(socketfd, &client->fileinfo);
        switch (ret) {
        case -1:{
            Connect_disconnect(client);
            return;
        } case 0: {
            if (client->fileinfo.filefd == 0) {
                epollctrl.Epollread(socketfd, client);
            } else {
                epollctrl.Epollwrite(socketfd, client);
            }
            return;
        } case 1: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        } case 2: {
            client->rewrite_count += 1;
            if (client->rewrite_count > REWRITEMAX) {
                Connect_disconnect(client);
            }
            return;
        }} //switch end
    }
}

~Servercontrol_epoll() {
    Server_stop();
}

#endif