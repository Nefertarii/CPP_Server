#include "tcpclient.h"
#include "../../Base/Net/connector.h"
#include "../../Base/Net/inetaddress.h"
#include "../../Base/Net/socketapi.h"
#include "../../Base/Timer/timerid.h"
#include "../../Base/eventloop.h"
#include "../../Log/logging.h"
#include <cassert>
#include <sstream>

using namespace Wasi;
using namespace Wasi::Server;

void Wasi::Server::Remove_connection(Poll::EventLoop* loop,
                                     const TcpConnectionPtr& conn) {
    loop->Queue_in_loop(std::bind(&TcpConnection::Connect_destroyed, conn));
}

void Wasi::Server::Remove_connector(const ConnectorPtr& connector) {
}

void TcpClient::New_connection(int sockfd) {
    loop->Assert_in_loop_thread();
    Sockets::InetAddress peeraddr(Sockets::Get_peer_addr(sockfd));
    std::string buf;
    buf += ":" + peeraddr.To_string_ip_port()
           + "#" + std::to_string(next_conn_id);
    std::string conn_name = name + buf;
    Sockets::InetAddress localaddr(Sockets::Get_local_addr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(loop, conn_name, sockfd,
                                            localaddr, peeraddr));
    conn->Set_connection_callback(connection_callback);
    conn->Set_message_callback(message_callback);
    conn->Set_write_complete_callback(write_complete_callback);
    conn->Set_close_callback(std::bind(&TcpClient::Remove_connection, this, std::placeholders::_1));
    {
        std::lock_guard<std::mutex> lk(mtx);
        connection = conn;
    }
    conn->Connect_established();
}

void TcpClient::Remove_connection(const TcpConnectionPtr& conn) {
    loop->Assert_in_loop_thread();
    assert(loop == conn->Get_loop());
    {
        std::lock_guard<std::mutex> lk(mtx);
        assert(connection == conn);
        connection.reset();
    }
    loop->Queue_in_loop(std::bind(&TcpConnection::Connect_destroyed, conn));
    if (retry && connect) {
        std::string msg = "TcpClient::Connect[" + name + "] reconnecting to"
                          + connector->Get_servaddr().To_string_ip_port();
        LOG_INFO(msg);
        connector->Restart();
    }
}

TcpClient::TcpClient(Poll::EventLoop* loop_,
                     const Sockets::InetAddress& serveraddr,
                     const std::string& cli_name) :
    loop(loop_),
    connector(new Sockets::Connector(loop, serveraddr)),
    name(cli_name),
    connection_callback(),
    message_callback(),
    retry(false),
    connect(true),
    next_conn_id(1) {
    connector->Set_new_connection_callback(
        std::bind(&TcpClient::New_connection, this, std::placeholders::_1));
    const void* address = static_cast<const void*>(this);
    std::stringstream point;
    point << address;
    std::string msg = "TcpClient::TcpClient[" + point.str() + "] connector ";
    address         = static_cast<const void*>(connector.get());
    point << address;
    msg += point.str();
    LOG_INFO(msg);
}

void TcpClient::Connect() {
    std::string msg = "TcpClient::TcpClient[" + name + "] connector "
                      + connector->Get_servaddr().To_string_ip_port();
    LOG_INFO(msg);
    connect = true;
    connector->Start();
}

void TcpClient::Disconnect() {
    connect = false;
    {
        std::lock_guard<std::mutex> lk(mtx);
        if (connection) { connection->Shutdown(); }
    }
}

void TcpClient::Stop() {
    connect = false;
    connector->Stop();
}

void TcpClient::Enable_retry() { retry = true; }

void TcpClient::Set_message_callback(const MessageCallback& callback) {
    message_callback = callback;
}

void TcpClient::Set_write_complete_callback(const WriteCompleteCallback& callback) {
    write_complete_callback = callback;
}

void TcpClient::Set_connection_callback(const ConnectionCallback& callback) {
    connection_callback = callback;
}

TcpConnectionPtr TcpClient::Connection() {
    std::lock_guard<std::mutex> lk(mtx);
    return connection;
}

Poll::EventLoop* TcpClient::Get_loop() const { return loop; }

const std::string& TcpClient::TcpClient::Get_name() const { return name; }

bool TcpClient::Get_retry() const { return retry; }

TcpClient::~TcpClient() {
    const void* address = static_cast<const void*>(connector.get());
    std::stringstream point;
    point << address;
    std::string msg = "TcpClient::~TcpClient [" + name + "] connector " + point.str();
    LOG_DEBUG(msg);
    TcpConnectionPtr conn;
    bool unique = false;
    {
        std::lock_guard<std::mutex> lk(mtx);
        unique = connection.unique();
        conn   = connection;
    }
    if (conn) {
        assert(loop == conn->Get_loop());
        CloseCallback callback = std::bind(&Wasi::Server::Remove_connection, loop, std::placeholders::_1);
        loop->Run_in_loop(std::bind(&TcpConnection::Set_close_callback, conn, callback));
        if (unique) { conn->Force_close(); }
    } else {
        connector->Stop();
        loop->Run_after(1, std::bind(&Remove_connector, connector));
    }
}