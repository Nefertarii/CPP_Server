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

int main(int argc, const char* argv[]) {
    std::string config_file = argv[1];
    static Server_Control_Epoll server(config_file);
    signal(SIGINT, SIG_handler);
    server.ServerStart();
    return 0;
}