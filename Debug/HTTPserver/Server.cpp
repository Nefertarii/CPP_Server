#include "Servcontrol.h"

void SIG_handler(int sig) {
    if (sig == SIGINT) {
        std::cout << "capture signal SIGINT.\n";
        //server.Server_stop(); auto run
        exit(2);
    } else {
        exit(0);
    }
}

int main() {
    static Server_Control_Epoll server;
    std::cout << "Http server start.\n";
    signal(SIGINT, SIG_handler);
    server.ServerStart();
    return 0;
}