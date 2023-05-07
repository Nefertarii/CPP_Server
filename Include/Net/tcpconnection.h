#ifndef NET_TCPCONNECTION_H_
#define NET_TCPCONNECTION_H_

#include "Include/Net/inetaddress.h"
#include "Include/Timer/timestamp.h"
#include "Include/Header/filestat.h"
#include "netinet/tcp.h"
#include <functional>
#include <memory>
#include <any>

namespace wasi {
namespace loop {
class EventLoop;
class Channel;
} // namespace loop

namespace net {
class Socket;
class TcpConnection;

using TcpConnectionPtr      = std::shared_ptr<TcpConnection>;
using ConnectionCallback    = std::function<void(const TcpConnectionPtr&)>;
using CloseCallback         = std::function<void(const TcpConnectionPtr&)>;
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
using MessageCallback       = std::function<void(const TcpConnectionPtr&, std::string*, timer::TimeStamp)>;
using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

enum ConnectState {
    kConnectdisconnected,
    kConnectconnecting,
    kConnectconnected,
    kConnectdisconnecting
};

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
private:
    loop::EventLoop* loop_;
    const std::string name_;
    ConnectState state_;
    bool reading_;
    size_t high_water_mark_;
    std::string input_string_;  // input send out
    std::string output_string_; // output read in
    std::any context_;
    head::FileStat file_;
    std::unique_ptr<net::Socket> socket_;
    std::unique_ptr<loop::Channel> channel_;
    const net::InetAddress local_addr_;
    const net::InetAddress peer_addr_;
    ConnectionCallback connection_callback_;
    MessageCallback message_callback_;
    WriteCompleteCallback write_complete_callback_;
    HighWaterMarkCallback high_water_mark_callback_;
    CloseCallback close_callback_;
    void HandleRead(timer::TimeStamp receive_time);
    void HandleWrite();
    void HandleClose();
    void HandleError();
    void SendInLoop();
    void SendFileInLoop();
    void ShutdownInLoop();
    void ForceCloseInLoop();
    void StartReadInLoop();
    void StopReadInLoop();
    void SetState(ConnectState state);
    std::string StringState(ConnectState state);

public:
    TcpConnection(loop::EventLoop* loop, const std::string& name, int sockfd,
                  const net::InetAddress& local_addr,
                  const net::InetAddress& peer_addr);
    loop::EventLoop* GetLoop() const;
    const std::string& GetName() const;
    const net::InetAddress& GetLocalAddress() const;
    const net::InetAddress& GetPeerAddress() const;
    std::string GetInputString();
    std::string GetOutputString();
    std::string GetTcpInfo() const;
    std::any GetContext();
    head::FileStat GetFileStat();
    bool GetTcpInfo(tcp_info* tcpi) const;
    bool Connected() const;
    bool Disconnected() const;
    bool Reading() const;
    void Send(const std::string message);
    void Send(const std::string message, size_t len);
    void Send(const char* message, size_t len);
    void SendFile(const std::string filename);
    void Shutdown();
    void ForceClose();
    void ForceCloseDelay(double seconds);
    void StartRead();
    void StopRead();
    void SetNoDelay(bool flag);
    void SetContext(const std::any& context);
    void SetOutputString(std::string message); //debug
    void SetConnectionCallback(const ConnectionCallback& cb);
    void SetMessageCallback(const MessageCallback& cb);
    void SetWriteCompleteCallback(const WriteCompleteCallback& cb);
    void SetHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t high_water_mark);
    void SetCloseCallback(const CloseCallback& cb);
    void ConnectEstablished();
    void ConnectDestroyed();
    ~TcpConnection();
};

} // namespace net
} // namespace wasi

#endif