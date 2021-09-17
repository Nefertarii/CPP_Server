#include "Accountconfirm.h"
#include "Metalocal.h"
#include "../../Important/Gsocketctrl.h"

struct Serverinfo {
    std::string name = "";
    std::string ip = "";
    std::string port = "";
    std::string token = "";
    std::string meassage = "";
    int socketfd = -1;
    void Reset() {
        ip.clear();
        port.clear();
        meassage.clear();
        token.clear();
        socketfd = -1;
    }
};

using namespace std;
int main(int argc, const char* argv[]) {
    std::string file;
    if (argc != 2) {
        file = "/home/cs18/vscode/Webserver/Config/Accountinfo.txt";
    } else {
        file = argv[1];
    }
    Log meta_local_log;
    meta_local_log.Set("Meta_Local_Log.txt", 200);
    Meta_Local Meta;
    Socket_Control socketctrl;
    Socket_Config config;
    config.is_server = false;
    config.read_max = 4096;
    config.write_max = 4096;
    config.socketfd = -1;
    socketctrl.SetLog(&meta_local_log, 200);
    socketctrl.SetConfig(&config);
    
    Meta.ReadAccountConfirm(file);
    Meta.SetLog(&meta_local_log, 200);
    if (Meta.ConfirmInteraction()) {
        std::string str = "hello.";
        std::cout << "connecting graph server...";
        socketctrl.SocketConnect("127.0.0.1", 8100);
        socketctrl.SocketWrite(config.socketfd, &str);
        socketctrl.SocketRead(config.socketfd, &str);
        std::cout << "\n" << str << "\n";
    } else {
        std::cout << "login fail.";
    }
}