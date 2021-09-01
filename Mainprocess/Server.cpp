#include "Servcontrol.h"

static Servercontrol_epoll server;

void SIG_handler(int sig) {
    if (sig == SIGINT) {
        Infolog("capture signal SIGINT.");
        //server.Server_stop(); auto run
        exit(2);
    }
    else {
        exit(0);
    }
}

int main() {
    std::cout << "Server start.\n";
    signal(SIGINT, SIG_handler);
    server.Server_start_Epollcontrol();
    return 0;
}