#include "Servcontrol.h"

int main() {
    std::cout << "Server start.\n";
    Servercontrol_epoll server;
    signal(SIGINT, server.SIGINT_handler);
    server.Server_start_Epollcontrol();
    return 0;
}