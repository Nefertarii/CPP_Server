#ifndef SERVCTRL_H_
#define SERVCTRL_H_

#include "../Important/Httprocess/Httphandler.h"
#include "../Important/Gsocketctrl_server.h"
#include "../Important/Gsocketctrl_client.h"
#include "../Important/Gepollctrl.h"

class Server_Control_Epoll {
private:
    Socket_Control_Server clientctrl;
    //Socket_Control_Client graphctrl;
    Socket_Config server_settings, connect_settings;
    std::vector<Clientinfo> clients;
    int listenfd;

    Epoll_Control epollctrl;
    HTTP_Handler httpctrl;
    Log http_server_log;
    
    Timer server_clock;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    size_t logbuf_size;
    bool init_complite;
    void ConnectAdd(Clientinfo* client, int connectfd);
    void ConnectDel(Clientinfo* client);
public:
    Server_Control_Epoll(std::string config_file);
    void ServerStart();
    void ServerReboot();
    void ServerStop();
    ~Server_Control_Epoll();
};

#endif