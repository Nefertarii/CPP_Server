#include "Servctrl.h"

void SIG_handler(int sig) {
    if (sig == SIGINT) {
        std::cout << "capture signal SIGINT.\n";
        //server.Server_stop(); static auto run
        exit(2);
    } else {
        exit(0);
    }
}

int main(int argc, const char* argv[]) {
    std::string config_file;
    if (argc != 2) {
        config_file = "/home/cs18/vscode/Webserver/Config/Http.conf";
    } else {
        config_file = argv[1];
    }
    static Server_Control_Epoll server(config_file);
    signal(SIGINT, SIG_handler);
    std::cout << "Server Start.\n";
    server.ServerStart();
    return 0;
}