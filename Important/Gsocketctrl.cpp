#include "Gsocketctrl.h"

void Socket_Control::SetLog(Log* upper, size_t buffer_size) {
    if (upper != nullptr) {
        this_log = upper;
        have_upper = true;
    } else {
        this_log = new Log("Socket_Control_Log.txt", 200);
        have_upper = false;
    }
}

int Socket_Control::SocketListen() {
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(socket_config->listen_port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    int listenfd = socket_config->listenfd;
    listenfd = Gsocket::Socket(AF_INET, SOCK_STREAM, 0, this_log);
    if (listenfd < 0) { return -1; }
    if (socket_config->reuseaddr) {
        if (Gsocket::ReuseAddress(listenfd, &socket_config->reuseaddr, this_log) < 0) {
            return -1;
        }
    }
    if (socket_config->reuseport) {
        if (Gsocket::ReusePort(listenfd, &socket_config->reuseport, this_log) < 0) {
            return -1;
        }
    }
    if (Gsocket::Bind(listenfd, (struct sockaddr*)&server_address,
                      sizeof(server_address), this_log) < 0) { return -1; }
    if (Gsocket::Listen(listenfd, 0, this_log) < 0) { return -1; }
    signal(SIGPIPE, SIG_IGN);
    socket_config->listenfd = listenfd;
    return listenfd;
}

int Socket_Control::SocketAccept() {
    if (socket_config->connect_nums == socket_config->connect_max) {
        return -1;
    } else {
        return Gsocket::Accept(socket_config->listenfd, this_log);
    }
}

int Socket_Control::SocketRead(int socketfd, std::string* readbuf) {
    return Gsocket::Read(socketfd, readbuf, socket_config->read_max, this_log);
}

int Socket_Control::SocketWrite(int socketfd, std::string* message) {
    int ret = 0;
    size_t writemax = socket_config->write_max;
    if (message->size() > writemax) {
        std::string tmp(*message, 0, writemax);
        *message = message->substr(writemax, (message->size() - writemax));
        ret = Gsocket::Write(socketfd, &tmp, this_log);
    } else {
        ret = Gsocket::Write(socketfd, message, this_log);
        message->clear();
    }
    return ret;
}

int Socket_Control::SocketDisconnet(int socketfd) {
    return Gsocket::Close(socketfd, this_log);
}

Socket_Control::~Socket_Control() {
    if (!have_upper) {
        delete this_log;
        this_log = nullptr;
    }
}
