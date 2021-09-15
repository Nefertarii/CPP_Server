#ifndef SERVCONTROL_H_
#define SERVCONTROL_H_

#include "../Important/Httprocess/Httphandler.h"
#include "../Important/Gsocketctrl.h"
#include "../Important/Gepollcontrol.h"
#include "../Important/Gtime.h"
#include <fstream>
#include <map>

class Server_Control_Epoll {
private:
    Socket_Control socketctrl;
    Epoll_Control epollctrl;
    HTTP_Handler httpctrl;
    Log server_log;
    Timer server_clock;
    std::map<std::string, std::string> global_string_settings;
    std::map<std::string, size_t> global_value_settings;
    std::vector<Clientinfo> clients;
    Socket_Config socket_settings;
    size_t logbuf_size;
    bool ReadConfig(std::string config_file);
    void ConnectAdd(Clientinfo* client, int connectfd);
    void ConnectDel(Clientinfo* client);
public:
    Server_Control_Epoll(std::string config_file);
    void ServerStart();
    void ServerStop();
    ~Server_Control_Epoll();
};


#endif