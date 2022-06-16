#ifndef HTTP_PROCESS_H_
#define HTTP_PROCESS_H_

#include "httpaccount.h"
#include "httpenum.h"
#include <memory>
#include <string>
#include <regex>

namespace Wasi {
namespace Server {
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<Server::TcpConnection>;
} // namespace Server

namespace Http {

class HttpProcess {
private:
    static HttpAccount account;
    static std::regex email_pattern;
    static std::regex passwd_pattern;
    static std::regex name_pattern;

public:
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