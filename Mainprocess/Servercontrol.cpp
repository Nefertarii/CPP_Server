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
    std::vector<std::string> respone_head;
    std::vector<std::string> respone_body;
    std::vector<int> respone_file;
    process.resize(MAXCLIENT);
    respone_head.resize(MAXCLIENT);
    respone_body.resize(MAXCLIENT);
    respone_file.resize(MAXCLIENT);
    Gthreadpool threadpool(MAXCLIENT);

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
                            Create_respone_head(&respone_head[i], Filetype(filename), 200, file.filelength);
                            respone_file[i] = file.filefd;
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
                threadpool.submit(process[i].Send, respone_head[i]);
                if (respone_file[i]) {
                    threadpool.submit(process[i].Sendfile, respone_file[i]);
                }
                if (!respone_body.empty()) {
                    threadpool.submit(process[i].Send, respone_body[i]);
                }
                respone_file[i] = 0;
                respone_head[i].clear();
                respone_body[i].clear();
            }
        }
    }
}
