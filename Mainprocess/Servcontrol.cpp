#include "Servcontrol.h"

/*
    Savelog(INFO,"epoll control add");
    Savelog(INFO,"epoll control delete");
    Savelog(INFO,"epoll control read");
    Savelog(INFO,"epoll control write");
*/

void Server_start_Epollcontrol() {
    std::vector<Clientinfo> clients;
    Httpconnect connectctrl;
    Httprocess processctrl;
    Httprespone responectrl;
    Epollcontrol epollctrl;

    int listenfd = 0;
    int concurrent = std::thread::hardware_concurrency();
    int MAXCLIENT = concurrent * connectctrl.Single_concurrent_client();   
    //Gthreadpool threadpool(MAXCLIENT);
    struct epoll_event event, events[MAXCLIENT]; 

    //initialization
    //threadpool.init();
    clients.resize(MAXCLIENT);
    int epollfd = epoll_create(MAXCLIENT);
    listenfd = connectctrl.Connectlisten();
    epollctrl.Set_epollfd(epollfd);

    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = nullptr;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    Infolog("epoll control add");
    Infolog("Server initialization complete.");

    //main control
    for (;;)
    {
        int readyfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if(readyfds < 0 && errno != EINTR) { //epoll create fail
            Fatalog("Cann't create epoll control.");
            //signal notify manage process...
            return; 
        }
        //all set
        for (int i = 0; i < readyfds; i++) { 
            Infolog("Have connecting");
            event = events[i];
            if (event.data.ptr == nullptr) {
                int connectfd = SERV::Accept(listenfd);
                if(!connectctrl.Canconnect()) {
                    clients[i].clientfd = connectfd;
                    processctrl.Set_client(clients[i]);
                    epollctrl.Epolladd(connectfd);
                    Infolog("epoll control add");
                }
            }
            else if(event.events & EPOLLIN) {
                std::string readbuf, filename;
                struct Filestate file;
                REQUESTYPE request;
                processctrl.Read(clients[i].clientfd, &readbuf);
                if (!readbuf.empty()) {
                    request = responectrl.Requestparse(&readbuf);
                    switch (request) {
                    case GET: {
                        if(!responectrl.GETparse(readbuf, &filename)) {
                            responectrl.GETprocess(filename, &file);
                            responectrl.Create_respone_head(&clients[i].respone_head, 
                                                            responectrl.Filetype(filename), 
                                                            200, file.filelength);
                            clients[i].filefd = file.filefd;
                            epollctrl.Epollwrite(clients[i].clientfd);
                            Infolog("epoll control write");
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
                //threadpool.submit(processctrl.Send, clients[i].clientfd, clients[i].respone_head);
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
