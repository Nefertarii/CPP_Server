#include "Head/connector.h"
#include "../Poll/Head/channel.h"
#include "../Poll/Head/eventloop.h"
#include "../Sockets/Head/socketapi.h"
#include "../../Timer/Head/timerid.h"
#include <cassert>
#include <iostream>

using namespace Wasi;
using namespace Wasi::Sockets;

const int Connector::max_retry_delay_ms = 30 * 1000;
const int Connector::init_retry_delay_ms = 500;

void Connector::Set_state(States state_) { state = state_; }

void Connector::Start_in_loop() {
    loop->Assert_in_loop_thread();
    assert(state == DISCONNECTED);
    if (connect) { Connect(); }
    else { std::cout << "Do not Connect\n"; }
}

void Connector::Stop_in_loop() {
    loop->Assert_in_loop_thread();
    if (state == CONNECTING) {
        Set_state(DISCONNECTED);
        Retry(Remove_and_Reset());
    }
}

void Connector::Connect() {
    int sockfd = Sockets::Create_socket(servaddr.Family());
    int ret = Sockets::Connect(sockfd, servaddr.Get_sockaddr());
    int tmp_errno = (ret == 0) ? 0 : errno;
    switch (tmp_errno) {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        Connecting(sockfd);
        break;
    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        Retry(sockfd);
        break;
    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        std::cout << "Connector::ConnectConnect error" << tmp_errno << "\n";
        Sockets::Close(sockfd);
        break;
    default:
        std::cout << "Connector::Connect:Unexpected error" << tmp_errno << "\n";
        Sockets::Close(sockfd);
        break;
    }
}

void Connector::Connecting(int sockfd) {
    Set_state(CONNECTING);
    assert(!channel);
    channel.reset(new Poll::Channel(loop, sockfd));
    channel->Set_write_callback(std::bind(&Connector::Handle_write, this));
    channel->Set_error_callback(std::bind(&Connector::Handle_error, this));
    channel->Enable_writing();
}

void Connector::Handle_write() {
    std::cout << "Connector::Handle_write " << state << "\n";
    if (state == CONNECTING) {
        int sockfd = Remove_and_Reset();
        int err = Sockets::Get_socket_error(sockfd);
        if (err) {
            std::cout << "Connector::Handle_write SO_ERROR = "
                << err << " " << String_error(err) << "\n";
            Retry(sockfd);
        }
        else {
            Set_state(CONNECTED);
            if (connect) { new_connection_callback(sockfd); }
            else { Sockets::Close(sockfd); }
        }
    }
}

void Connector::Handle_error() {
    std::cout << "Connector::Handle_error state:" << state << "\n";
    if (state == CONNECTING) {
        int sockfd = Remove_and_Reset();
        int err = Sockets::Get_socket_error(sockfd);
        std::cout << "Connector::Handle_error SO_ERROR = " << state
            << " " << String_error(err) << "\n";
    }
}

void Connector::Retry(int sockfd) {
    Sockets::Close(sockfd);
    Set_state(DISCONNECTED);
    if (connect) {
        std::cout << "Connector::Retry Retry connecting to " << servaddr.To_string_ip_port()
            << " in " << retry_delay_ms << " milliseconds.\n";
        loop->Run_after(retry_delay_ms / 1000.0,
                        std::bind(&Connector::Start_in_loop, shared_from_this()));
        retry_delay_ms = std::min(retry_delay_ms * 2, max_retry_delay_ms);
    }
    else {
        std::cout << "Do not connect\n";
    }
}

void Connector::Reset_channel() { channel.reset(); }

int Connector::Remove_and_Reset() {
    channel->Disable_all();
    channel->Remove();
    int sockfd = channel->Fd();
    loop->Queue_in_loop(std::bind(&Connector::Reset_channel, this));
    return sockfd;
}

Connector::Connector(Poll::EventLoop* loop_,
                     const InetAddress& servaddr_) :
    loop(loop_),
    servaddr(servaddr_),
    retry_delay_ms(init_retry_delay_ms),
    connect(false),
    state(DISCONNECTED) {
    std::cout << "Connector ctor [" << this << "]\n";
}

void Connector::Set_new_connection_callback(const NewConnectionCallback& callback) {
    new_connection_callback = callback;
}
void Connector::Start() {
    connect = true;
    loop->Run_in_loop(std::bind(&Connector::Start_in_loop, this));
}

void Connector::Restart() {
    loop->Assert_in_loop_thread();
    Set_state(DISCONNECTED);
    retry_delay_ms = init_retry_delay_ms;
    connect = true;
    Start_in_loop();
}

void Connector::Stop() {
    connect = false;
    loop->Queue_in_loop(std::bind(&Connector::Stop_in_loop, this));
}

const InetAddress& Connector::Get_servaddr() const { return servaddr; }

Connector::~Connector() {
    std::cout << "Connector dtor[" << this << "]\n";
    assert(!channel);
}
/*


    Poll::EventLoop* loop;
    InetAddress servaddr;
    int retry_delay_ms;
    bool connect;
    States state;
    std::unique_ptr<Poll::Channel> channel;
    NewConnectionCallback new_connection_callback;
public:

*/