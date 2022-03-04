#ifndef SERVER_TCPSERVER_H_
#define SERVER_TCPSERVER_H_

#include "../../../Class/noncopyable.h"
#include "../../Sockets/Head/inetaddress.h"
#include "../../Sockets/Head/acceptor.h"
#include "tcpconnection.h"
#include <memory>
#include <atomic>

namespace Wasi {
    namespace Poll {
        class EventLoop;
        class EventLoopThread;
    }

    namespace Server {
        using ConnectionMap = std::map<std::string, TcpConnection>;
        using ThreadInitCallback = std::function<void(EventLoop*)>;
        
        class TcpServer : Noncopyable {
        private:
            Poll::EventLoop* loop;
            const std::string name;
            const std::string ip_port;
            std::unique_ptr<Sockets::Acceptor> acceptor;
            std::shared_ptr<Poll::EventLoopThread> threadloop;
            ConnectionCallback callback_connection;
            MeassageCallback callback_message;
            WriteCompleteCallback callback_write_complete;
            std::atomic<int> started;
            ConnectionMap conntions;
            int next_conn_id;
            void New_Connection(int sockfd, const Sockets::InetAddress peeraddr);
            void Remove_connection(const TcpConnectionPtr& conn);
            void Remove_connection_in_loop(const TcpConnectionPtr& conn);
        public:
            TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr
                      const std::string& name_, OptReusePort opt);
            const std::string& Get_ip_port();
            const std::string& Get_name();
            Poll::EventLoop* Get_loop();
            void Set_thread_num(int num_threads);
            void Set_thread_init_callback(const ThreadInitCallback& callback_)
            void Set_connection_callback(const ConnectionCallback& callback_);
            void Set_message_callback(const MeassageCallback& callback_);
            void Set_write_callback(const WriteCompleteCallback& callback_);
            void Start();
            ~TcpServer();
            enum OptReusePort {
                NOREUSEPORT,
                REUSEPORT
            };
        };
    }
}

#endif