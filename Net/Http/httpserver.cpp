#include "Head/httpserver.h"
#include "../../Log/Head/logging.h"
#include "../../Timer/Head/clock.h"
#include "../Base/Head/filestat.h"
#include "../Poll/Head/eventloop.h"
#include "../Poll/Head/eventloopthreadpool.h"
#include "../Sockets/Head/socketapi.h"
#include "Head/httprequest.h"
#include "Head/httprespone.h"
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

using namespace Wasi::Http;
using namespace Wasi;

static std::string local_dir = "home/nefertarii/vscode/HTML/";

int Parse_request(const Server::TcpConnectionPtr& conn) {
    // Preaccess phase: IP control(black list)
    // ...
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    conn_request->Parse(conn->Get_input_buffer()->Content());
    return 0;
}

int Get_process(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    if (conn_request->is_file) {
        struct stat sys_file_stat;
        conn_respone->respone_file = conn_request->path + local_dir + conn_request->target;
        if (stat(conn_respone->respone_file.c_str(), &sys_file_stat) < 0) {
            LOG_ERROR("not find file:" + conn_respone->respone_file);
            return -1;
        }
        size_t dot = conn_request->target.find_first_of('.');
        std::string file_type(conn_request->target, dot);
        if (file_type == ".html") {
            conn_respone->respone_head.content_type = FileType::HTML;
        } else if (file_type == ".css") {
            conn_respone->respone_head.content_type = FileType::CSS;
        } else if (file_type == ".js") {
            conn_respone->respone_head.content_type = FileType::JAVASCRIPT;
        } else if (file_type == ".json") {
            conn_respone->respone_head.content_type = FileType::JSON;
        } else if (file_type == ".png") {
            conn_respone->respone_head.content_type = FileType::PNG;
        } else if (file_type == ".svg") {
            conn_respone->respone_head.content_type = FileType::SVG;
        } else if (file_type == ".ico") {
            conn_respone->respone_head.content_type = FileType::ICON;
        } else {
            conn_respone->respone_head.content_type = FileType::PLAIN;
        }
        timespec file_modify_time                 = sys_file_stat.st_mtim;
        conn_respone->respone_head.last_modified  = file_modify_time.tv_sec;
        conn_respone->respone_head.date           = Time::Clock::Nowtime_sec();
        conn_respone->respone_head.code_num       = HttpCode::CODE200;
        conn_respone->respone_head.content_length = sys_file_stat.st_size;
        return 0;
    } else {
        conn_respone->respone_head.code_num = HttpCode::CODE404;
        return 0;
    }
}

int Post_process(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    // fill HttpRequest body
    // fill HttpRequest
    return 0;
}

int Process_request(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    if (conn_request->version == HttpVersion::HTTP11) {
        conn_respone->respone_head.http_version       = HttpVersion::HTTP11;
        conn_respone->respone_head.connection         = ConnectionType::KEEPALIVE;
        conn_respone->respone_head.keep_alive_timeout = 5;
        conn_respone->respone_head.keep_alive_max     = 1000;
    } else if (conn_request->version == HttpVersion::HTTP10) {
        conn_respone->respone_head.http_version = HttpVersion::HTTP10;
    }
    if (conn_request->method == Method::GET) {
        if (Get_process(conn) < 0) {
            LOG_ERROR("Get request process fail");
            return -1;
        }
        return 0;
    } else if (conn_request->method == Method::POST) {
        if (Post_process(conn) < 0) {
            LOG_ERROR("Post request process fail");
            return -1;
        }
        return 0;
    }
    return -1;
}

int Prepare_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRequest> conn_request = conn_context.Get_request();
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    std::string tmp_string;
    conn_respone->prepare_respone_head.clear();
    // version code
    tmp_string.clear();
    switch (conn_respone->respone_head.http_version) {
    case HttpVersion::HTTP10: tmp_string = "HTTP/1.0 "; break;
    case HttpVersion::HTTP11: tmp_string = "HTTP/1.1 "; break;
    case HttpVersion::HTTP20: tmp_string = "HTTP/2.0 "; break;
    default: tmp_string = "HTTP/1.0 "; break;
    }
    switch (conn_respone->respone_head.code_num) {
    case HttpCode::CODE200:
        tmp_string += "200 OK\r\n";
        break;
    case HttpCode::CODE301:
        tmp_string += "301 Moved Permanently\r\n";
        break;
    case HttpCode::CODE304:
        tmp_string += "304 Not Modified\r\n";
        break;
    case HttpCode::CODE400:
        tmp_string += "400 Bad Request\r\n";
        break;
    case HttpCode::CODE403:
        tmp_string += "403 Forbidden\r\n";
        break;
    case HttpCode::CODE404:
        tmp_string += "404 Not Found\r\n";
        break;
    case HttpCode::CODE411:
        tmp_string += "411 Length Required\r\n";
        break;
    case HttpCode::CODE500:
        tmp_string += "500 Internal Server Error\r\n";
        break;
    case HttpCode::CODE501:
        tmp_string += "501 Method Not Implemented\r\n";
        break;
    default: tmp_string += "404 Not Found\r\n"; break;
    }
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "Connection: ";
    switch (conn_respone->respone_head.connection) {
    case ConnectionType::KEEPALIVE:
        tmp_string += "Keep-Alive\r\n";
        tmp_string += "Keep-Alive: timeout=";
        tmp_string += std::to_string(conn_respone->respone_head.keep_alive_timeout);
        tmp_string += ", max=";
        tmp_string += std::to_string(conn_respone->respone_head.keep_alive_max);
        tmp_string += "\r\n";
        break;
    default: tmp_string = ""; break;
    }
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "Content-Type: ";
    switch (conn_respone->respone_head.content_type) {
    case FileType::HTML:
        tmp_string += "text/html; ";
        break;
    case FileType::CSS:
        tmp_string += "text/css; ";
        break;
    case FileType::JAVASCRIPT:
        tmp_string += "text/javascript; ";
        break;
    case FileType::JSON:
        tmp_string += "application/json; ";
        break;
    case FileType::PNG:
        tmp_string += "image/png; ";
        break;
    case FileType::SVG:
        tmp_string += "image/svg+xml; ";
        break;
    case FileType::ICON:
        tmp_string += "image/x-icon; ";
        break;
    case FileType::PLAIN:
        tmp_string += "text/plain; ";
        break;
    default:
        tmp_string += "text/plain; ";
        break;
    }
    switch (conn_respone->respone_head.charset) {
    case HttpCharset::UTF8:
        tmp_string += "charset=UTF-8\r\n";
        break;
    default:
        tmp_string += "charset=UTF-8\r\n";
        break;
    }
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "Content-Length: ";
    tmp_string += std::to_string(conn_respone->respone_head.content_length);
    tmp_string += "\r\n";
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "Last-Modified: ";
    tmp_string += Time::Clock::To_string_sec(conn_respone->respone_head.last_modified, "%a, %d %b %G %T GMT\r\n");
    conn_respone->prepare_respone_head += tmp_string;

    tmp_string = "date: ";
    tmp_string += Time::Clock::To_string_sec(conn_respone->respone_head.date, "%a, %d %b %G %T GMT\r\n");
    conn_respone->prepare_respone_head += tmp_string;

    return 0;
}

int Send_respone(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    conn->Send(conn_respone->prepare_respone_head);
    return 0;
}

void Request_process(const Server::TcpConnectionPtr& conn) {
    std::string msg;
    // parse_request
    if (Parse_request(conn) < 0) {
        LOG_INFO("parse_request fail");
        return;
    }
    // process_request
    if (Process_request(conn) < 0) {
        LOG_INFO("process_request fail");
        return;
    }
    // prepare_respone
    if (Prepare_respone(conn) < 0) {
        LOG_INFO("prepare_respone fail");
        return;
    }
    // send_respone
    if (Send_respone(conn) < 0) {
        LOG_INFO("send_respone fail");
        return;
    }

    LOG_INFO("success");
}

void HttpServer::Connection(const Server::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        LOG_INFO("Get connection");
        // use reserve and index can set max client;
        HttpContext new_conn_context;
        contexts.push_back(new_conn_context);
        conn->Set_context(new_conn_context);
        // conn->Send("Get connection");
    }
}

void HttpServer::Message(const Server::TcpConnectionPtr& conn) {
    Base::Buffer* read                = conn->Get_input_buffer();
    std::string msg                   = "From " + conn->Get_peer_address().To_string_ip_port() + " get message: " + read->Content();
    Poll::EventLoop* thread_pool_loop = thread_pool->Get_loop();
    thread_pool_loop->Run_in_loop(std::bind(&Request_process, conn));
    LOG_DEBUG(msg);
}

void HttpServer::Write_complete(const Server::TcpConnectionPtr& conn) {
    HttpContext conn_context                  = std::any_cast<HttpContext>(conn->Get_context());
    std::shared_ptr<HttpRespone> conn_respone = conn_context.Get_respone();
    Base::FileStat* conn_file_stat            = conn->Get_file_stat();
    // head write complete
    if (conn_respone->respone_file.empty()) {
        conn->Send(conn_respone->respone_body);
        conn_respone->
    }

    // write body

    // write file
    if (conn_file_stat->filefd == 0) { return; }
    if (conn_file_stat->Remaning() == 0) { return; }
    if (!conn_respone->respone_file.empty()) {
        conn->Sendfile(conn_respone->respone_file);
        conn_respone->respone_file.clear();
    }
}

HttpServer::HttpServer(Poll::EventLoop* loop,
                       const Sockets::InetAddress& listen_addr,
                       const std::string& name,
                       Server::TcpServer::OptReusePort option) :
    listen_server(loop, listen_addr, name, option),
    thread_pool(listen_server.Get_thread_pool()) {
    listen_server.Set_connection_callback(std::bind(
        &HttpServer::Connection, this, std::placeholders::_1));
    listen_server.Set_message_callback(std::bind(
        &HttpServer::Message, this, std::placeholders::_1));
    listen_server.Set_write_complete_callback(std::bind(
        &HttpServer::Write_complete, this, std::placeholders::_1));
}

void HttpServer::Start() {
    std::string msg = "HttpServer [" + listen_server.Get_name()
                      + "] starts listening on " + listen_server.Get_ip_port();
    LOG_INFO(msg);
    thread_pool->Set_thread_num(std::thread::hardware_concurrency());
    listen_server.Start();
    thread_pool->Start();
}

void HttpServer::Set_thread_num(int num) {
    thread_pool->Set_thread_num(num);
}