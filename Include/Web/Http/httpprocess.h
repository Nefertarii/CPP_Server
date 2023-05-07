#ifndef WEB_HTTP_HTTPPROCESS_H_
#define WEB_HTTP_HTTPPROCESS_H_

#include <memory>

namespace wasi {
namespace net {
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
} // namespace net

namespace web {

class HttpProcess {
private:
    static std::string html_dir_;
    static std::string CRLF_;

public:
    HttpProcess();
    static void SetHtmlDir(std::string dir);
    static std::string GetHtmlDir();
    static bool ParseRequest(net::TcpConnectionPtr conn);
    static bool ProcessRequestGet(net::TcpConnectionPtr conn);
    static bool ProcessRequestPost(net::TcpConnectionPtr conn);
    static bool SetBadRespone(net::TcpConnectionPtr conn);
    static bool SetBadPage(net::TcpConnectionPtr conn); // special pages
    static bool PrepareResponeHead(net::TcpConnectionPtr conn);
    static bool ProcessRequest(net::TcpConnectionPtr conn); // Integrating all operations
    ~HttpProcess();
};

} // namespace web
} // namespace wasi

#endif