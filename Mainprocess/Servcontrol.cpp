#include "Servcontrol.h"

Servercontrol_epoll::Servercontrol_epoll() {
    listenfd = 0;
    concurrent = std::thread::hardware_concurrency();
    MAXCLIENT = concurrent * connectctrl.Single_concurrent_client();   

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
    epoll_ctl(epollctrl.Epollfd(), EPOLL_CTL_ADD, listenfd, &ev);
    //main control
    for (;;)
    {
        int readyfds = epoll_wait(epollctrl.Epollfd(), events, MAXCLIENT, 0);
        if(readyfds < 0 && errno != EINTR) { //epoll create fail
            Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return; 
        }
        
        for (int i = 0; i < readyfds; i++) { 
            ev = events[i];
            if (ev.data.ptr == nullptr) {
                int connectfd = SERV::Accept(listenfd);
                if(!connectctrl.Canconnect()) {
                    clients[i].clientfd = connectfd;
                    processctrl.Set_client(&clients[i]);
                    epollctrl.Epolladd(connectfd);
                    std::string log = clients[i].ip + ":" + clients[i].port;
                    log = "Accept success " + log;
                    Infolog(log);
                }
            }
            else if(ev.events & EPOLLIN) {
                std::string readbuf, filename;
                REQUESTYPE request;
                processctrl.Read(clients[i].clientfd, &readbuf);
                if (!readbuf.empty()) {
                    request = responectrl.Requestparse(&readbuf);
                    switch (request) {
                    case GET: {
                        if(!responectrl.GETparse(readbuf, &filename)) {
                            responectrl.GETprocess(filename, &clients[i].fileinfo);
                            responectrl.Create_respone_head(&clients[i].respone_head, 
                                                            responectrl.Filetype(filename), 
                                                            200, clients[i].fileinfo.filelength);
                            epollctrl.Epollwrite(clients[i].clientfd);
                        }
                        else {
                            ;
                        }
                        break;
                    }

                    case POST: {
                        
                        break;
                    }

                    default: {

                        break;
                    }

                    }
                }
            }
            else if(ev.events & EPOLLOUT) {
                //threadpool.submit(processctrl.Send, clients[i].clientfd, &clients[i].respone_head);
                processctrl.Send(clients[i].clientfd, &clients[i].respone_head);
                if (clients[i].fileinfo.filefd)
                {
                    //threadpool.submit(processctrl.Sendfile, clients[i].clientfd, &clients[i].fileinfo);
                    processctrl.Sendfile(clients[i].clientfd, &clients[i].fileinfo);
                    if(clients[i].fileinfo.filefd) {
                        epollctrl.Epollwrite(clients[i].clientfd);
                    }
                }
                else if (!clients[i].respone_body.empty()) {
                    //threadpool.submit(processctrl.Send, clients[i].clientfd, &clients[i].respone_body);
                    processctrl.Send(clients[i].clientfd, &clients[i].respone_body);
                    if(!clients[i].respone_body.empty()) {
                        epollctrl.Epollwrite(clients[i].clientfd);
                    }
                }
                else {
                    epollctrl.Epollread(clients[i].clientfd);
                }
            }
        }
    }
}

void Servercontrol_epoll::Server_stop() {
    Warninglog("Server closeing.");
    for (int i = 0; i != MAXCLIENT; i++) {
        if (clients[i].clientfd > 0) {
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

Servercontrol_epoll::~Servercontrol_epoll() {
    Server_stop();
}