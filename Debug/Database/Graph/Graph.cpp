#include "Graphserver.h"
#include "GQLparse.h"
#include <string>

void SIG_handler(int sig) {
    if (sig == SIGINT) {
        std::cout << "\ncapture signal SIGINT.\n";
        //server.Server_stop(); auto run
        exit(2);
    } else {
        exit(0);
    }
}

int main(int argc, const char* argv[]) {
    std::string config_file;
    if (argc != 2) {
        config_file = "/home/cs18/vscode/Webserver/Graph.conf";
    } else {
        config_file = argv[1];
    }
    signal(SIGINT, SIG_handler);
    static Graph_Server_Control server(config_file);
    std::cout << "Server start.\n";
    server.ServerStart();
}