#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socketapi.h"

using namespace Wasi::Http;
using namespace Wasi;

/*
    Server::TcpServer listen_server;
    Server::TcpServer process_server;
    Base::ThreadPool process_threadpool;
*/

HttpServer::HttpServer() {
    
}

void HttpServer::Start() {
    listen_server.Start();
}

void HttpServer::Set_thread_num(int num) {
    threadpool.Set_thread_count(num);
}