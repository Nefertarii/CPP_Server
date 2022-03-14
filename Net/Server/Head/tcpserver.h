#ifndef SERVER_TCPSERVER_H_
#define SERVER_TCPSERVER_H_

#include "../../../Class/noncopyable.h"
#include "../../Sockets/Head/inetaddress.h"
#include "../../Sockets/Head/acceptor.h"
#include "tcpconnection.h"
#include <memory>
#include <atomic>
#include <map>

namespace Wasi {
    namespace Poll {
        class EventLoop;
        class EventLoopThread;
    }

    namespace Server {
        using ConnectionMap = std::map<std::string, TcpConnectionPtr>;
        using ThreadInitCallback = std::function<void(Poll::EventLoop*)>;

        class TcpServer : Noncopyable {
        private:
            void New_connection(int sockfd, const Sockets::InetAddress& peeraddr);
            void Remove_connection(const TcpConnectionPtr& conn);
            void Remove_connection_in_loop(const TcpConnectionPtr& conn);
            Poll::EventLoop* loop;
            const std::string name;
            const std::string ip_port;
            std::unique_ptr<Sockets::Acceptor> acceptor;
            //std::shared_ptr<Poll::EventLoop> threadloop;
            //threadpoll loop
            ConnectionCallback connection_callback;
            MessageCallback message_callback;
            WriteCompleteCallback write_complete_callback;
            std::atomic<int> started;
            ConnectionMap conntions;
            int next_conn_id;
        public:
            enum OptReusePort {
                NOREUSEPORT,
                REUSEPORT
            };
            TcpServer(Poll::EventLoop* loop_, const Sockets::InetAddress& listenaddr,
                      const std::string& name_, OptReusePort opt);
            const std::string& Get_ip_port();
            const std::string& Get_name();
            Poll::EventLoop* Get_loop();
            //void Set_thread_num(int num_threads);
            //void Set_thread_init_callback(const ThreadInitCallback& callback_)
            void Set_connection_callback(const ConnectionCallback& callback_);
            void Set_message_callback(const MessageCallback& callback_);
            void Set_write_complete_callback(const WriteCompleteCallback& callback_);
            void Start();
            ~TcpServer();

        };
    }
}

#endif