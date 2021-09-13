#ifndef GRAPHSERV_H_
#define GRAPHSERV_H_

#include "../../Important/Gepollcontrol.h"
#include "../../Important/Gservfunc.h"
#include <string>

class Graph_Server_Control {
private:
    Epoll_Control epollcontrol("Graph_Server_Epoll.txt");
    Socket_Func socketfunc("Graph_Server_Socket.txt");
public:
    Graph_Server_Control();
    ~Graph_Server_Control();
};






#endif