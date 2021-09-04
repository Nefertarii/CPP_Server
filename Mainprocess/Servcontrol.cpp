#include "Servcontrol.h"

Servercontrol_epoll::Servercontrol_epoll() {
    listenfd = 0;
    concurrent = std::thread::hardware_concurrency();
    MAXCLIENT = concurrent * connectctrl.Single_concurrent_client();
    REWRITEMAX = 5;

    //initialization
    //threadpool.init();
    clients.resize(MAXCLIENT);
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
    for (;;)
    {
        int readyfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if(readyfds < 0 && errno != EINTR) { //epoll create fail
            Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return; 
        }
        
        for (int i = 0; i < readyfds; i++) { 
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                Connect_accept(&clients[i]);
            }
            else if(ev.events & EPOLLIN) {
                std::string readbuf;
                processctrl.Read(clients[i].clientfd, &readbuf);
                if (!readbuf.empty()) {
                    switch (responectrl.Requestparse(&readbuf)) {
                    case GET: {
                        Connect_getmethod(&clients[i], &readbuf);
                        break;
                    } case POST: {
                        Connect_postmethod(&clients[i], &readbuf);
                        break;
                    } default: {
                        break;
                    }}//switch end
                }
            }
            else if(ev.events & EPOLLOUT) {
                Send_responehead(&clients[i]);
                Send_responefile(&clients[i]);
                Send_responebody(&clients[i]);
            }
        }
    }
}

void Servercontrol_epoll::Server_stop() {
    Warninglog("Server closeing.");
    for (size_t i = 0; i != MAXCLIENT; i++) {
        if (clients[i].clientfd > 0) {
            epollctrl.Epollwrite(clients[i].clientfd);
            processctrl.Disconnect(&clients[i]);
        }
    }
    //threadpool.shutdown();
    SERV::Close(listenfd);
    SERV::Close(epollctrl.Epollfd());
    std::string log = "Server total run time:" + serverclock.Runtime_str() + " sec";
    Savetofile();
    Infolog(log);
    Infolog("Server is close.");
    
}

void Servercontrol_epoll::Connect_accept(Clientinfo *client) {
    int connectfd = SERV::Accept(listenfd);
    if(!connectctrl.Canconnect()) {
        client->clientfd = connectfd;
        processctrl.Set_client(client);
        epollctrl.Epolladd(connectfd);
        std::string log = client->ip + ":" + client->port;
        log = "Accept success " + log;
        Infolog(log);
    }
}

void Servercontrol_epoll::Connect_getmethod(Clientinfo *client, std::string *readbuf) {
    std::string filename;
    if(!responectrl.GETparse(*readbuf, &filename)) {
        responectrl.GETprocess(filename, &client->fileinfo);
        responectrl.Create_respone_head(&client->respone_head, 
                                        responectrl.Filetype(filename), 
                                        200, client->fileinfo.filelength);
    } else {
        badrequest.Respone200(&client->respone_head);
    }
    epollctrl.Epollwrite(client->clientfd);
    return;
}

void Servercontrol_epoll::Connect_postmethod(Clientinfo *client, std::string *readbuf) {
    Infolog("Into post request.");
    badrequest.Respone200(&client->respone_head);
    epollctrl.Epollwrite(client->clientfd);
    return;
}

void Servercontrol_epoll::Send_responehead(Clientinfo *client) {
    //ret
    //-1 errno
    // 0 success
    // 1 signal interruption
    // 2 cache full
    int socketfd = client->clientfd;
    if(client->respone_head.empty()) {
        return;
    } else {
        int ret = processctrl.Send(socketfd, &client->respone_head);
        switch (ret) {
        case -1: {
            processctrl.Disconnect(client);
            epollctrl.Epolldel(socketfd);
            return;
        } case 0: {
            client->respone_head.clear();
            return;
        } case 1: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(socketfd);
            }
            return;
        } case 2: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(socketfd);
            }
            return;
        }} //switch end 
    }
}

void Servercontrol_epoll::Send_responebody(Clientinfo *client) {
    //can only be send file or info(json/data)
    int socketfd = client->clientfd;
    if(client->respone_body.empty()) {
        //processctrl.Disconnect(client);
        //epollctrl.Epolldel(client->clientfd);
        return;
    } else {
        int ret = processctrl.Send(socketfd, &client->respone_body);
        switch (ret) {
        case -1: {
            processctrl.Disconnect(client);
            epollctrl.Epolldel(socketfd);
            return;
        } case 0: {
            if(client->respone_body.empty()) {
                //processctrl.Disconnect(client);
                //epollctrl.Epolldel(socketfd);
                epollctrl.Epollread(socketfd);
            } else {
                epollctrl.Epollwrite(socketfd);
            }
            return; 
        } case 1: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(socketfd);
            }
            return;
        } case 2: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(client->clientfd);
            }
            return;
        }}//switch end
    }
}

void Servercontrol_epoll::Send_responefile(Clientinfo *client) {
    //send file
    int socketfd = client->clientfd;
    if(client->fileinfo.filefd == 0) {
        processctrl.Disconnect(client);
        epollctrl.Epolldel(socketfd);
        return;
    } else {
        int ret = processctrl.Sendfile(socketfd, &client->fileinfo);
        switch (ret) {
        case -1: {
            processctrl.Disconnect(client);
            epollctrl.Epolldel(socketfd);
            return;
        } case 0: {
            if(client->fileinfo.filefd == 0) {
                //processctrl.Disconnect(client);
                //epollctrl.Epolldel(socketfd);
                epollctrl.Epollread(socketfd);
            } else {
                epollctrl.Epollwrite(socketfd);
            }
            return;
        } case 1: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(socketfd);
            }
            return;
        } case 2: {
            client->writecount += 1;
            if(client->writecount > REWRITEMAX) {
                processctrl.Disconnect(client);
                epollctrl.Epolldel(socketfd);
            }
            return;
        }}//switch end
    }
}

Servercontrol_epoll::~Servercontrol_epoll() {
    Server_stop();
}

