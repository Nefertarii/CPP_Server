#include "Servercontrol.h"

void Epolladd(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
    Savelog(INFO,"epoll control add");
}

void Epolldel(int socketfd, int epollfd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
    Savelog(INFO,"epoll control delete");
}

void Epollread(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    Savelog(INFO,"epoll control read");
}

void Epollwrite(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
    Savelog(INFO,"epoll control write");
}

void Server_start_Epollcontrol() {
    int concurrent = std::thread::hardware_concurrency();
    int MAXCLIENT = concurrent * SINGLECLIENTS;
    Gthreadpool threadpool(MAXCLIENT);
    std::vector<Clientinfo> clients;
    Httpconnect connectctrl;
    Httprocess processctrl;
    
    //initialization
    threadpool.init();
    clients.resize(MAXCLIENT);
    int epollfd = epoll_create(MAXCLIENT);
    struct epoll_event event, events[MAXCLIENT];
    Epolladd(connects.Listenfd(), epollfd);
    Savelog(INFO, "Server initialization complete.");

    //main control
    for (;;)
    {
        int nfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if(nfds < 0 && errno != EINTR) { //epoll create fail
            Savelog(FATAL, "Cann't create epoll control.");
            return; 
        }
        //all set
        for (int i = 0; i < nfds; i++) { 
            event = events[i];
            if (event.data.ptr == nullptr) {
                int connectfd = SERV::Accept(connects.Listenfd());
                if(!connectctrl.Client_accept()) {
                    clients[i].clientfd = connectfd;
                    processctrl.Set_client(clients[i]);
                    Epolladd(connectfd, epollfd);
                }
                //change until here
            }
            else if(event.events & EPOLLIN) {
                std::string readbuf, filename;
                struct Filestate file;
                REQUESTYPE request;
                process[i].Read(&readbuf);
                if(!readbuf.empty()) {
                    request = Requestparse(&readbuf);
                    switch (request) {
                    case GET: {
                        if(!GETparse(readbuf, &filename)) {
                            GETprocess(filename, &file);
                            Create_respone_head(&client_respone_head[i], Filetype(filename), 200, file.filelength);
                            client_respone_filefd[i] = file.filefd;
                            Epollwrite(process[i].Clientfd(), epollfd);
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
            else if(event.events & EPOLLOUT) {
                threadpool.submit(process[i].Send, client_respone_head[i]);
                if (client_respone_filefd[i]) {
                    threadpool.submit(process[i].Sendfile, process[i]);
                }
                if (!client_respone_body.empty()) {
                    threadpool.submit(process[i].Send, process[i]);
                }
                client_respone_filefd[i] = 0;
                client_respone_head[i].clear();
                client_respone_body[i].clear();
            }
        }
    }
}
