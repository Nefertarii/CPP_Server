#ifndef SERVER_TCPCONNECTION_H_
#define SERVER_TCPCONNECTION_H_

#include <memory>
#include <functional>
#include "../../../Class/noncopyable.h"
#include "../../../Timer/Head/timestamp.h"
#include "../../Sockets/Head/inetaddress.h"
#include "../../Base/Head/buffer.h"

struct tcp_info;

namespace Wasi {
    namespace Poll {
        class Channel;
        class EventLoop;
    }
    namespace Sockets {
        class Socket;
    }
    namespace Server {
        class TcpConnection;
        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
        using CloseCallback = std::function<void(const TcpConnectionPtr&)>;
        using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
        using MessageCallback = std::function<void(const TcpConnectionPtr&, Base::Buffer*, Time::TimeStamp)>;
        using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

        class TcpConnection :
            Noncopyable,
            public std::enable_shared_from_this<TcpConnection> {
        private:
            enum ConnState {
                DISCONNECTED,
                CONNECTING,
                CONNECTED,
                DISCONNECTING
            };
            void Handle_read(Time::TimeStamp receive_time);
            void Handle_write();
            void Handle_close();
            void Handle_error();
            void Send_in_loop();
            void Shutdown_in_loop();
            void Force_close_in_loop();
            void Set_state(ConnState state_);
            void Start_read_in_loop();
            void Stop_read_in_loop();
            const char* State_to_string();
            Poll::EventLoop* loop;
            const std::string name;
            ConnState state;
            bool reading;
            size_t high_water_mark;
            Base::Buffer input_buffer;
            Base::Buffer output_buffer;
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
            TcpConnection(Poll::EventLoop* loop, const std::string& name_, int sockfd_,
                          const Sockets::InetAddress& local_addr_,
                          const Sockets::InetAddress& peer_addr_);
            Poll::EventLoop* Get_loop() const;
            const std::string& Get_name() const;
            const Sockets::InetAddress& Get_local_address() const;
            const Sockets::InetAddress& Get_peer_address() const;
            Base::Buffer* Get_input_buffer();
            Base::Buffer* Get_output_buffer();
            std::string Get_tcp_info() const;
            bool Get_tcp_info(tcp_info* tcpi) const;
            bool Connected() const;
            bool Disconnected() const;
            bool Is_reading() const;
            void Send(const std::string message);
            void Send(const std::string message, size_t len);
            void Send(const char* message, size_t len);
            void Shutdown();
            void Force_close();
            void Force_close_delay(double seconds);
            void Start_read();
            void Stop_read();
            void Set_no_delay(bool on);
            void Set_connection_callback(const ConnectionCallback& cb);
            void Set_message_callback(const MessageCallback& cb);
            void Set_write_complete_callback(const WriteCompleteCallback& cb);
            void Set_high_water_mark_callback(const HighWaterMarkCallback& cb, size_t high_water_mark_);
            void Set_close_callback(const CloseCallback& cb);
            void Connect_established();
            void Connect_destroyed();
            ~TcpConnection();
        };
    }
}


#endif