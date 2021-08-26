#include "Httprocess.h"

Httprocess::Httprocess(int socketfd) {
    clientfd = socketfd;
    Reset();
    char *tmpip;
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(clientfd, (struct sockaddr *)&client_address, &address_length);
    Client_port = std::to_string(ntohs(client_address.sin_port));
    Client_ip = inet_ntoa(client_address.sin_addr);
}

void Httprocess::Reset() {
    err_code = ERRNONE;
    state_code = STATENONE;
    requset_type = TYPENONE;
}

void Httprocess::Set_clientfd(int socketfd) {
    clientfd = socketfd;
    Reset();
    char *tmpip;
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(clientfd, (struct sockaddr *)&client_address, &address_length);
    Client_port = std::to_string(ntohs(client_address.sin_port));
    Client_ip = inet_ntoa(client_address.sin_addr);
}

int Httprocess::Clientfd() { 
    return clientfd; 
}

int Httprocess::Send(std::string message, this) {
    int ret = SERV::Write(Httprocess::clientfd, message);
    if (ret == 0) {
        std::string tmp = "Send for " + Httprocess::Client_ip  + " done.";
        #ifdef DEBUG
            std::cout << tmp << "\n";
        #else
            Savelog(INFO, tmp, 0);
        #endif
    }
    else if(ret >= 1) {
        ;
    }
    else {
        ;
    }
    Httprocess::Reset();
    return 0;
}

int Httprocess::Sendfile(std::string filename) {
    struct Filestate file;
    SERV::Readfile(filename, &file);
    int ret = SERV::Writefile(Httprocess::clientfd, file.filefd, 10);
    if (ret == 0) {
        std::string tmp = "Send file for " + Client_ip + " done.";
        #ifdef DEBUG
            std::cout << tmp << "\n";
        #else
            Savelog(INFO, tmp, 0);
        #endif
    }
    else if(ret >= 1) {
        ;
    }
    else {
        ;
    }
    Httprocess::Reset();
    return 0;
}

int Httprocess::Sendfile(int filefd) {
    int ret = SERV::Writefile(Httprocess::clientfd, filefd, 10);
    if (ret == 0) {
        std::string tmp = "Send file for " + Httprocess::Client_ip + " done.";
        #ifdef DEBUG
            std::cout << tmp << "\n";
        #else
            Savelog(INFO, tmp, 0);
        #endif
    }
    else if(ret >= 1) {
        ;
    }
    else {
        ;
    }
    Httprocess::Reset();
    return 0;
}

int Httprocess::Read(std::string *read_buf) {
    int ret = SERV::Read(clientfd, read_buf);
    if (ret == 0) {
        std::string tmp = "Read for " + Client_ip + " done.";
        #ifdef DEBUG
            std::cout << tmp << "\n";
        #else
            Savelog(INFO, tmp, 0);
        #endif
            return 0;
    }
    else if (ret >= 1) {
        return -1;
    }
    else {
        return -1;
    }
}

void Httprocess::Disconnect() {
    shutdown(clientfd, SHUT_WR);
    std::string tmp = "Close " + Client_ip + " done.";
    #ifdef DEBUG
        std::cout << tmp << "\n";
    #else
        Savelog(INFO, tmp, 0);
    #endif
    Reset();
}


Httpconnect::Httpconnect() {
#ifdef DEBUG
    std::cout << "Create HTTP listen.\n"; 
#else
    Savelog(INFO, "HTTP listening", 0);
#endif
    Connectlisten();
}

void Httpconnect::Connectlisten() {
    concurrent_count = std::thread::hardware_concurrency();
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(LISTENPORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    listenfd = SERV::Socket(AF_INET, SOCK_STREAM, 0);
    SERV::Bind(listenfd, (struct sockaddr *)&server_address, sizeof(server_address));
    SERV::Listen(listenfd, concurrent_count * SINGLECLIENTS);
    signal(SIGPIPE, SIG_IGN);
}

int Httpconnect::Client_accept() {
    int MAXCLIENT = concurrent_count * SINGLECLIENTS;
    if (connect_count == MAXCLIENT) {
#ifdef DEBUG
    std::cout << "Too many client\n"; 
#else
    Savelog(INFO, "HTTP client is full", 0);
#endif
        return -1;
        
    }
    else {
    connect_count++;
#ifdef DEBUG
    std::cout << "Can create client\n"; 
#else
    Savelog(INFO, "Request create client", 0);
#endif
    return 0;  
    }
}

const int Httpconnect::Socketfd() { 
    return socketfd; 
}

const int Httpconnect::Listenfd() { 
    return listenfd; 
}

const int Httpconnect::Concurrentcount() { 
    return concurrent_count; 
}

const int Httpconnect::Clientcount() { 
    return connect_count; 
}



