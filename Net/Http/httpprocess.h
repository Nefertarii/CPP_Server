#ifndef HTTP_PROCESS_H_
#define HTTP_PROCESS_H_

#include "httpaccount.h"
#include "httpenum.h"
#include <memory>
#include <string>

namespace Wasi {
namespace Server {
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<Server::TcpConnection>;
} // namespace Server

namespace Http {

class HttpProcess {
public:
    static HttpAccount account;
    static int Parse_request(const Server::TcpConnectionPtr& conn);
    static int Get_process(const Server::TcpConnectionPtr& conn);
    static int Post_process(const Server::TcpConnectionPtr& conn);
    static int Process_request(const Server::TcpConnectionPtr& conn);
    static int Prepare_respone(const Server::TcpConnectionPtr& conn);
    static int Send_respone(const Server::TcpConnectionPtr& conn);
    static int Set_404_page(const Server::TcpConnectionPtr& conn);
    static int Send_bad_respone(const Server::TcpConnectionPtr& conn);
    static void Request_process(const Server::TcpConnectionPtr& conn);
};

} // namespace Http
} // namespace Wasi

#endif // !HTTP_PROCESS_H_