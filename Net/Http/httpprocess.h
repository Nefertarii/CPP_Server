#ifndef HTTP_PROCESS_H_
#define HTTP_PROCESS_H_

#include <memory>
#include <string>

namespace Wasi {
namespace Server {
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<Server::TcpConnection>;
} // namespace Server

namespace Http {
static std::string local_dir = "/home/nefertarii/vscode/HTML";

int Parse_request(const Server::TcpConnectionPtr& conn);
int Get_process(const Server::TcpConnectionPtr& conn);
int Post_process(const Server::TcpConnectionPtr& conn);
int Process_request(const Server::TcpConnectionPtr& conn);
int Prepare_respone(const Server::TcpConnectionPtr& conn);
int Send_respone(const Server::TcpConnectionPtr& conn);
int Set_404_page(const Server::TcpConnectionPtr& conn);
int Send_bad_respone(const Server::TcpConnectionPtr& conn);
void Request_process(const Server::TcpConnectionPtr& conn);

} // namespace Http
} // namespace Wasi

#endif // !HTTP_PROCESS_H_