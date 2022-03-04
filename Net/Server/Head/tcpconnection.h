#ifndef SERVER_TCPCONNECTION_H_
#define SERVER_TCPCONNECTION_H_

#include <memory>
#include <functional>

namespace Wasi {
    namespace Server {
        class TcpConection;
        using TcpConnectionPtr = std::shared_ptr<TcpConection>;
        using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
        using MeassageCallback = std::function<void(const TcpConnectionPtr&)>;
        using WriteCompleteCallback = std::function<void(const TcpConnectionPtr&)>;
        
        class TcpConnection {

        };
    }
}


#endif