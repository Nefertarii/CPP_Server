#include "Servercontrol.h"

void Server_start_Epollcontrol() {
    int concurrent = std::thread::hardware_concurrency();
    int listenfd = 0;
    int MAXCLIENT = concurrent * SINGLECLIENTS;
    Gthreadpool threadpool(MAXCLIENT);
    std::vector<Clientinfo> clients;
    Httpconnect connectctrl;
    Httprocess processctrl;
    struct epoll_event event, events[MAXCLIENT];
    
    //initialization
    threadpool.init();
    connectctrl.Connectlisten(&listenfd);
    clients.resize(MAXCLIENT);
    int epollfd = epoll_create(MAXCLIENT);
    Epolladd(listenfd, epollfd);
    Savelog(INFO, "Server initialization complete.");

    //main control
    for (;;)
    {
        int nfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if(nfds < 0 && errno != EINTR) { //epoll create fail
            Savelog(FATAL, "Cann't create epoll control.");
            //signal notify manage process...
            return; 
        }
        //all set
        for (int i = 0; i < nfds; i++) { 
            event = events[i];
            if (event.data.ptr == nullptr) {
                int connectfd = SERV::Accept(listenfd);
                if(!connectctrl.Canconnect()) {
                    clients[i].clientfd = connectfd;
                    processctrl.Set_client(clients[i]);
                    Epolladd(connectfd, epollfd);
                }
            }
            else if(event.events & EPOLLIN) {
                std::string readbuf, filename;
                struct Filestate file;
                REQUESTYPE request;
                processctrl.Read(clients[i].clientfd, &readbuf);
                if (!readbuf.empty()) {
                    request = Requestparse(&readbuf);
                    switch (request) {
                    case GET: {
                        if(!GETparse(readbuf, &filename)) {
                            GETprocess(filename, &file);
                            Create_respone_head(&clients[i].respone_head, Filetype(filename), 200, file.filelength);
                            clients[i].filefd = file.filefd;
                            Epollwrite(clients[i].clientfd, epollfd);
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
                threadpool.submit(processctrl.Send, clients[i].clientfd, clients[i].respone_head);
                if (clients[i].filefd) {
                    //threadpool.submit(processctrl.Sendfile, clients[i].clientfd, clients[i].filefd);
                    processctrl.Sendfile(clients[i].clientfd, clients[i].filefd);
                }
                if (!clients[i].respone_body.empty()) {
                    //threadpool.submit(processctrl.Send, clients[i].clientfd, clients[i].respone_body);
                    processctrl.Send(clients[i].clientfd, clients[i].respone_body);
                }
            }
        }
    }
}
