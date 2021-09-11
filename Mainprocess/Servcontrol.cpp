#include "Servcontrol.h"

Servercontrol_epoll::Servercontrol_epoll() {
    listenfd = 0;
    concurrent = std::thread::hardware_concurrency();
    MAXCLIENT = concurrent * connectctrl.Single_concurrent_client();
    REWRITEMAX = 5;

    //initialization
    //threadpool.init();
    clients.resize(MAXCLIENT * 2);
    int epollfd = epoll_create(MAXCLIENT);
    listenfd = connectctrl.Connectlisten();
    epollctrl.Set_epollfd(epollfd);
    //Gthreadpool threadpool(MAXCLIENT);

    Infolog("epoll control add.");
    Infolog("Server initialization complete.");
}

void Servercontrol_epoll::Server_start_Epollcontrol() {
    struct epoll_event ev, events[MAXCLIENT];
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    int epollfd = epollctrl.Epollfd();
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

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

void Servercontrol_epoll::Server_stop() {
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

void Servercontrol_epoll::Connect_accept() {
    int connectfd = Servfunc::Accept(listenfd);
    if (!connectctrl.Canconnect()) {
        int index = connectctrl.Connect_nums();
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

void Servercontrol_epoll::Connect_method_get(Clientinfo *client, std::string *readbuf) {
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

void Servercontrol_epoll::Connect_method_post(Clientinfo *client, std::string *readbuf) {
    Infolog("Into post request.");
    badrequest.Respone404(&client->respone_head);
    epollctrl.Epollwrite(client->clientfd, client);
    return;
}

void Servercontrol_epoll::Connect_disconnect(Clientinfo *client) {
    epollctrl.Epolldel(client->clientfd);
    connectctrl.Disconnect(client);
}

void Servercontrol_epoll::Send_responehead(Clientinfo *client) {
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

void Servercontrol_epoll::Send_responebody(Clientinfo *client) {
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

void Servercontrol_epoll::Send_responefile(Clientinfo *client) {
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

Servercontrol_epoll::~Servercontrol_epoll() {
    Server_stop();
}
