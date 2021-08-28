#include "Servercontrol.h"

void Epolladd(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
}

void Epolldel(int socketfd, int epollfd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, socketfd, nullptr);
}

void Epollread(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Epollwrite(int socketfd, int epollfd) {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = Epoll_has_connect;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, socketfd, &ev);
}

void Server_start_Epollcontrol() {
    int concurrent = std::thread::hardware_concurrency();
    int MAXCLIENT = concurrent * SINGLECLIENTS;
    Httpconnect connects;
    std::vector<Httprocess> process;
    std::vector<std::string> client_respone_head;
    std::vector<std::string> client_respone_body;
    std::vector<std::string> client_ip;
    std::vector<std::string> client_port;
    std::vector<int> client_socketfd;
    std::vector<int> client_respone_filefd;
    process.resize(MAXCLIENT);
    client_respone_head.resize(MAXCLIENT);
    client_respone_body.resize(MAXCLIENT);
    client_respone_filefd.resize(MAXCLIENT);
    client_ip.resize(MAXCLIENT);
    client_port.resize(MAXCLIENT);
    client_socketfd.resize(MAXCLIENT);
    Gthreadpool threadpool(MAXCLIENT);

    threadpool.init();
    int epollfd = epoll_create(MAXCLIENT);
    struct epoll_event event, events[MAXCLIENT];
    Epolladd(connects.Listenfd(), epollfd);
    
#ifdef DEBUG
    std::cout << "Server initialize complete.\n";
#else
    Savelog(INFO, "Server initialize complete.", 0);
#endif

    for (;;) {
        int nfds = epoll_wait(epollfd, events, MAXCLIENT, 0);
        if(nfds < 0 && errno != EINTR) {
        #ifdef DEBUG
            std::cout << "Server start fail.\n";
        #else
            Savelog(INFO, "Server start fail.", 0);
        #endif
            return; 
        }
        for (int i = 0; i < nfds; i++) {
            event = events[i];
            if (event.data.ptr == nullptr) {
                int connectfd = SERV::Accept(connects.Listenfd());
                if(!connects.Client_accept()) {
                    process[i].Set_clientfd(connectfd);
                    Epolladd(connectfd, epollfd);
                }
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
