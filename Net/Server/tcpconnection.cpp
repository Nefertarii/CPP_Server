#include "Head/tcpconnection.h"
#include "../Poll/Head/channel.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socket.h"
#include <netinet/tcp.h>

using namespace Wasi::Server;

void TcpConnection::Handle_read(Time::TimeStamp receive_time) {
    loop->Assert_in_loop_thread();
    int tmp_errno = 0;
    char buf[65536];
    iovec vec[2];
    const size_t writalbe = 
}
void TcpConnection::Handle_write();
void TcpConnection::Handle_close();
void TcpConnection::Handle_error();
void TcpConnection::Send(const std::string message);
void TcpConnection::Send(const void* message, size_t len);
void TcpConnection::Shutdown();
void TcpConnection::Force_close();

void TcpConnection::Set_state(ConnState state_) { state = state_; }

void TcpConnection::Start_read();
void TcpConnection::Stop_read();
const char* TcpConnection::State_to_string();
/*
class TcpConnection : Noncopyable {
private:
    enum ConnState {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        DISCONNECTING,
    }
    Poll::EventLoop* loop;
    const std::string name;
    std::string input_buffer;
    std::string output_buffer;
    ConnState state;
    bool reading;
    std::unique_ptr<Sockets::Socket> socket;
    std::unique_ptr<Poll::Channel> channel;
    const Sockets::InetAddress local_addr;
    const Sockets::InetAddress peer_addr;
    ConnectionCallback connection_callback;
    MessageCallback message_callback;
    WriteCompleteCallback write_complete_callback;
    HighWaterMarkCallback high_water_mark_callback;
    CloseCallback close_callback;
public:
    TcpConnection(Event* loop, const string& name, int sockfd,
                  const Sockets::InetAddress& local_addr_,
                  const Sockets::InetAddress& peer_addr_);
    Poll::EventLoop* Get_loop() const;
    const std::string& Get_name() const;
    const Sockets::InetAddress& Get_local_address() const;
    const Sockets::InetAddress& Get_peer_address() const;
    std::string* Get_input_buffer();
    std::string* Get_output_buffer();
    std::string Get_tcp_info() const;
    bool Get_tcp_info(tcp_info*) const;
    bool Connected() const;
    bool Disconnected() const;
    bool Is_reading() const;
    void Send(const void* message, int len);
    void Send(const std::string& message);
    void Shutdown();
    void Force_close();
    void Force_close(double seconds);
    void Start_read();
    void Stop_read();
    void Set_no_delay(bool on);
    void Set_connection_callback();
    void Set_message_callback();
    void Set_write_complete_callback();
    void Set_high_water_mark_callback();
    void Set_close_callback();
    void Connect_established();
    void Connect_destroyed();
    ~TcpConnection();
};

*/