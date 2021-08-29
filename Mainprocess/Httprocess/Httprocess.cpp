#include "Httprocess.h"




void Httprocess::Reset_client(struct Clientinfo client) {
    client.port.clear();
    client.ip.clear();
    client.respone_head.clear();
    client.respone_body.clear();
    client.clientfd = 0;
    client.socketfd = 0;
    client.filefd = 0;
    client.err_code = ERRNONE;
    client.state_code = STATENONE;
    client.requset_type = TYPENONE;
}

void Httprocess::Set_client(struct Clientinfo client) {
    char *tmpip;
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    getpeername(client.clientfd, (struct sockaddr *)&client_address, &address_length);
    client.port = std::to_string(ntohs(client_address.sin_port));
    client.ip = inet_ntoa(client_address.sin_addr);
}

int Httprocess::Send(int clientfd, std::string message) {
    int ret = SERV::Write(clientfd, message);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {  //error code set
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Sendfile(int clientfd, std::string filename) {
    struct Filestate file;
    SERV::Readfile(filename, &file);
    int ret = SERV::Writefile(clientfd, file.filefd, 10);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Sendfile(int clientfd, int filefd) {
    int ret = SERV::Writefile(clientfd, filefd, 10);
    if (ret == 0) {
        return 0;
    }
    else if(ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

int Httprocess::Read(int clientfd, std::string *read_buf) {
    int ret = SERV::Read(clientfd, read_buf);
    if (ret == 0) {
        return 0;
    }
    else if (ret >= 1) {
        return 1;
    }
    else {
        return -1;
    }
}

void Httprocess::Clear(struct Clientinfo client) {
    client.respone_head.clear();
    client.respone_body.clear();
    client.filefd = 0;
}

void Httprocess::Disconnect(struct Clientinfo client) {
    shutdown(client.clientfd, SHUT_WR);
    Reset_client(client);
}




void Httpconnect::Connectlisten(int *listenfd) {
    concurrent_count = std::thread::hardware_concurrency();
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(LISTENPORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    *listenfd = SERV::Socket(AF_INET, SOCK_STREAM, 0);
    SERV::Bind(*listenfd, (struct sockaddr *)&server_address, sizeof(server_address));
    SERV::Listen(*listenfd, concurrent_count * SINGLECLIENTS);
    signal(SIGPIPE, SIG_IGN);
}

int Httpconnect::Canconnect() {
    if(connect_nums + 1 > concurrent_count * SINGLECLIENTS) {
        return -1;
    }
    else {
        connect_nums++;
        return 0;
    }
}

const int Httpconnect::Concurrent_count() {
    return concurrent_count;
}

const int Httpconnect::Connect_nums() {
    return connect_nums;
}
