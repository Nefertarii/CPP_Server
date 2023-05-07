#include "Include/Web/Http/httpserver.h"
#include "Include/Eventloop/eventloopthreadpool.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Web/Http/httprespone.h"
#include "Include/Logger/logger.h"
#include "Include/Timer/clock.h"
#include <signal.h>

using namespace wasi;
using namespace wasi::web;

void HttpServer::SendRespone(const net::TcpConnectionPtr& conn) {
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRespone> respone_ptr = context.GetRespone();
    std::shared_ptr<HttpRespone> respone   = respone_ptr.lock();
    size_t repeat_max                      = context.GetRepeatMax();
    if (context.GetRepeatNum() < repeat_max) {
        while (context.GetRepeatNum() < repeat_max) {
            context.AddRepeatNum();
            switch (context.GetState()) {
            case ResponeSendState::kWait:
                std::this_thread::sleep_for(wasi::timer::Timer_Ms(context.GetRepeatTime()));
                break;
            case ResponeSendState::kHead:
                if (respone->GetResponeHead().length() > 0) {
                    conn->Send(respone->GetResponeHead());
                }
                if (respone->GetResponeFile().file_length > 0) {
                    conn->SendFile(respone->GetResponeFile().target_path);
                }
                context.SetState(ResponeSendState::kDone);
                context.SetRepeatNum(repeat_max);
                break;
            case ResponeSendState::kError:
                context.SetState(ResponeSendState::kDone);
                conn->Shutdown();
                context.SetRepeatNum(repeat_max);
                break;
            case ResponeSendState::kDone:
            default:
                context.SetRepeatNum(repeat_max);
            }
        }
        context.ResetRepeatNum();
    } else {
        context.SetState(ResponeSendState::kDone);
        conn->Shutdown();
        context.SetRepeatNum(repeat_max);
    }
    conn->SetContext(context);
}

void HttpServer::Message(const net::TcpConnectionPtr& conn) {
    //loop::EventLoop* thread_pool_loop      = thread_pool_->GetLoop();
    HttpContext context                    = std::any_cast<HttpContext>(conn->GetContext());
    std::weak_ptr<HttpRespone> respone_ptr = context.GetRespone();
    std::shared_ptr<HttpRespone> respone   = respone_ptr.lock();
    switch (context.GetState()) {
    case ResponeSendState::kWait:
        HttpProcess::ProcessRequest(conn);
        //thread_pool_loop->RunInLoop(std::bind(&HttpProcess::ProcessRequest, conn));
        conn->GetLoop()->RunInLoop(std::bind(&HttpServer::SendRespone, this, conn));
        break;
    case ResponeSendState::kHead:
        conn->GetLoop()->RunInLoop(std::bind(&HttpServer::SendRespone, this, conn));
        break;
    case ResponeSendState::kBody:
        conn->GetLoop()->RunInLoop(std::bind(&HttpServer::SendRespone, this, conn));
        break;
    case ResponeSendState::kError:
        conn->Shutdown();
        break;
    case ResponeSendState::kDone:
    default:
        break;
    }
}

void HttpServer::Connection(const net::TcpConnectionPtr& conn) {
    if (conn->Connected()) {
        HttpContext context;
        context.SetRepeatMax(fail_repeat_num_);
        context.SetRepeatTime(fail_repeat_time_);
        conn->SetContext(context);
    }
}

void HttpServer::WriteComplete(const net::TcpConnectionPtr& conn) {
    HttpContext context = std::any_cast<HttpContext>(conn->GetContext());

    context.Clear();
    conn->SetContext(context);
    // Message(conn);
}

HttpServer::HttpServer(loop::EventLoop* loop,
                       std::map<std::string, std::string> setting) :
    listen_server_(nullptr),
    thread_pool_(nullptr) {
    std::string server_name = setting["server_name"];
    std::string listen_addr = setting["listen_addr"];
    int listen_port         = std::stoi(setting["listen_port"]);
    int server_thread_num   = std::stoi(setting["server_thread_num"]);
    fail_repeat_num_        = std::stoi(setting["fail_repeat_num"]);
    fail_repeat_time_       = std::stoi(setting["fail_repeat_time"]);
    // bool reuse_port         = (setting["reuse_port"] == "true") ? true : false;
    HttpProcess::SetHtmlDir(setting["html_resource_directory"]);

    net::InetAddress addr(listen_addr, listen_port);
    listen_server_ = std::make_shared<net::TcpServer>(loop, addr, server_name);

    thread_pool_ = listen_server_->GetThreadPool();
    listen_server_->SetThreadNum(server_thread_num);

    listen_server_->SetConnectionCallback(std::bind(
        &HttpServer::Connection, this, std::placeholders::_1));
    listen_server_->SetMessageCallback(std::bind(
        &HttpServer::Message, this, std::placeholders::_1));
    listen_server_->SetWriteCompleteCallback(std::bind(
        &HttpServer::WriteComplete, this, std::placeholders::_1));
    signal(SIGPIPE, SIG_IGN);
}

void HttpServer::Start() {
    LOG_INFO("HttpServer [" + listen_server_->GetName()
             + "] start listening on " + listen_server_->GetIpPort())
    if (thread_pool_->GetThreadNum() == 0) {
        thread_pool_->SetThreadNum(std::thread::hardware_concurrency());
    }
    listen_server_->Start();
    thread_pool_->Start();
}

void HttpServer::SetThreadNum(int num) {
    thread_pool_->SetThreadNum(num);
}
