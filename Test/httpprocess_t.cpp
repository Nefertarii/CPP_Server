#include "Include/Web/Http/httpprocess.h"
#include "Include/Net/tcpconnection.h"
#include "Include/Web/Http/httpcontext.h"
//#include "Include/Web/Http/httprespone.h"
//#include "Include/Web/Http/httprequest.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include <iostream>

using namespace wasi;
using namespace wasi::web;

void ParseRequest_t() {
    std::string request_string = "GET / HTTP/1.1\r\n"
                                 "Host: www.example.com\r\n"
                                 "User-Agent: Mozilla/5.0\r\n"
                                 "Accept: text/html,application/xhtml+xml\r\n"
                                 "Accept-Language: en-US,en;q=0.5\r\n"
                                 "Accept-Encoding: gzip, deflate\r\n"
                                 "Connection: keep-alive\r\n"
                                 "Referer: http://www.example.com/\r\n"
                                 "Cookie: name=value; name2=value2\r\n"
                                 "\r\n"
                                 "i'm request body, hi";
    loop::EventLoop loop;
    net::InetAddress local_addr("127.0.0.1", 9999);
    net::InetAddress peer_addr("127.0.0.1", 8888);
    net::TcpConnectionPtr conn = std::make_shared<net::TcpConnection>(&loop, "name", 20, local_addr, peer_addr);
    HttpContext context;
    conn->SetOutputString(request_string);
    context.SetRequest(HttpRequest());
    context.SetRespone(HttpRespone());
    conn->SetContext(context);
    HttpProcess::ProcessRequest(conn);
}

void SetBad_t() {
    std::string request_string = "GET /index.html HTTP/1.1\r\n"
                                 "Host: www.example.com\r\n"
                                 "User-Agent: Mozilla/5.0\r\n"
                                 "Accept: text/html,application/xhtml+xml\r\n"
                                 "Accept-Language: en-US,en;q=0.5\r\n"
                                 "Accept-Encoding: gzip, deflate\r\n"
                                 "Connection: keep-alive\r\n"
                                 "Referer: http://www.example.com/\r\n"
                                 "Cookie: name=value; name2=value2\r\n"
                                 "\r\n"
                                 "i'm request body, hi";
    loop::EventLoop loop;
    net::InetAddress local_addr("127.0.0.1", 9999);
    net::InetAddress peer_addr("127.0.0.1", 8888);
    net::TcpConnectionPtr conn = std::make_shared<net::TcpConnection>(&loop, "name", 20, local_addr, peer_addr);
    HttpContext context;
    conn->SetOutputString(request_string);
    context.SetRequest(HttpRequest());
    context.SetRespone(HttpRespone());
    conn->SetContext(context);
    HttpProcess::SetBadRespone(conn);
    HttpProcess::SetBadPage(conn);
}

void PrepareResponeHead_t() {
    loop::EventLoop loop;
    net::InetAddress local_addr("127.0.0.1", 9999);
    net::InetAddress peer_addr("127.0.0.1", 8888);
    net::TcpConnectionPtr conn = std::make_shared<net::TcpConnection>(&loop, "name", 20, local_addr, peer_addr);
    HttpContext context;
    HttpRequest request;
    HttpRequestHead request_data;
    request_data.version = HttpVersion::kHttp10;
    FileStat file_stat;
    file_stat.target_path = "/home/ubuntu/Server/Resource/HTML/Errorpage/Page404.html";
    file_stat.SetFileStat();
    request.SetRequestData(request_data);
    request.SetFileStat(file_stat);
    request.SetHttpRequest("request_string");
    context.SetRequest(request);
    context.SetRespone(HttpRespone());
    conn->SetContext(context);
    HttpProcess::PrepareResponeHead(conn);

    context                            = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRespone> respone_ptr = context.GetRespone();
    std::shared_ptr<HttpRespone> respone   = respone_ptr.lock();
    std::cout << respone->GetResponeHead();
    std::cout << "\n";
}

int main() {
    log::Logger::Init();
    ParseRequest_t();
    // SetBad_t();
    // PrepareResponeHead_t();
}