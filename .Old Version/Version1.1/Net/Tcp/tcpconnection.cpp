#include "tcpconnection.h"
#include <Base/Net/socket.h>
#include <Base/Net/socketapi.h>
#include <Base/Poll/channel.h>
#include <Base/Poll/eventloop.h>
#include <Base/Timer/clock.h>
#include <Base/Timer/timerid.h>
#include <Log/logging.h>
#include <assert.h>
#include <cstring>
#include <netinet/tcp.h>
#include <sstream>
#include <sys/fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/uio.h>

using namespace Wasi;
using namespace Wasi::Server;

void TcpConnection::Handle_read(Time::TimeStamp receive_time) {
    loop->Assert_in_loop_thread();
    int tmp_errno = 0;
    ssize_t read  = input_buffer.Read_fd(channel->Fd(), &tmp_errno);
    if (read > 0) {
        message_callback(shared_from_this(), &input_buffer, receive_time);
        // message_callback(shared_from_this(), &output_buffer, receive_time);
    } else if (read == 0) {
        Handle_close();
    } else {
        errno = tmp_errno;
        LOG_ERROR("TcpConnection::Handle_read error");
        Handle_error();
    }
}

void TcpConnection::Handle_write() {
    loop->Assert_in_loop_thread();
    if (channel->Is_writing()) {
        ssize_t write = Sockets::Write(channel->Fd(),
                                       output_buffer.Content().c_str(),
                                       output_buffer.Content().length());
        if (write > 0) {
            output_buffer.Add_index(write);
            if (output_buffer.Index() == 0) {
                channel->Disable_writing();
                if (write_complete_callback) { loop->Queue_in_loop(std::bind(write_complete_callback, shared_from_this())); }
                if (state == DISCONNECTING) { Shutdown(); }
            }
        } else {
            LOG_ERROR("TcpConnection::Handle_write error");
        }
    } else {
        LOG_ERROR("Connection fd:" + std::to_string(channel->Fd()) + "no more writing");
    }
}

void TcpConnection::Handle_close() {
    loop->Assert_in_loop_thread();
    std::string msg = "Channel fd:" + std::to_string(channel->Fd()) + " state:" + State_to_string();
    LOG_INFO(msg);
    assert(state == CONNECTED || state == DISCONNECTING);
    Set_state(DISCONNECTED);
    channel->Disable_all();
    TcpConnectionPtr guard_this(shared_from_this());
    connection_callback(guard_this);
    close_callback(guard_this);
}

void TcpConnection::Handle_error() {
    int err         = Sockets::Get_socket_error(channel->Fd());
    std::string msg = "TcpConnection[" + name + "] SO_ERROR:"
                      + std::to_string(err) + ", " + Sockets::String_error(err);
    LOG_INFO(msg);
}

void TcpConnection::Send_in_loop() {
    loop->Assert_in_loop_thread();
    ssize_t wrote    = 0;
    size_t len       = output_buffer.Size();
    size_t remaining = output_buffer.Remaining();
    bool fault       = false;
    if (state == DISCONNECTED) {
        LOG_INFO("Disconnected, give up write.");
        return;
    }
    if (!channel->Is_writing() && remaining != 0) {
        wrote = Sockets::Write(channel->Fd(), output_buffer.Content().c_str(), len);
        if (wrote > 0) {
            remaining = len - wrote;
            output_buffer.Add_index(wrote);
            if (remaining == 0 && write_complete_callback) {
                output_buffer.Init();
                loop->Queue_in_loop(std::bind(write_complete_callback, shared_from_this()));
            }
        } else {
            if (errno != EWOULDBLOCK) {
                LOG_INFO("Error " + std::string(strerror(errno)));
                if (errno == EPIPE || errno == ECONNRESET) {
                    fault = true;
                }
            }
        }
    }
    assert(remaining <= len);
    // high_water_mark
    if (!fault && remaining > 0) {
        if (len + remaining >= high_water_mark
            && len < high_water_mark
            && high_water_mark_callback) {
            loop->Queue_in_loop(std::bind(high_water_mark_callback, shared_from_this(), len + remaining));
        }
        // output_buffer.Append()
        if (!channel->Is_writing()) { channel->Enable_writing(); }
    }
}

#include <iostream>

void TcpConnection::Send_file_in_loop() {
    if (state == DISCONNECTED) {
        LOG_INFO("Disconnected, give up write.");
        return;
    }
    if (!channel->Is_writing() && file.Remaining() > 0) {
        int ret = sendfile(socket->Fd(), file.filefd, &file.file_offset, 15000);
        if (ret > 0) {
            if (file.Remaining() > 0) {
                // std::this_thread::sleep_for(std::chrono::milliseconds(30));
                loop->Queue_in_loop(std::bind(&TcpConnection::Send_file_in_loop, shared_from_this()));
                return;
            }
            if (write_complete_callback) {
                close(file.filefd);
                file.Init();
                loop->Queue_in_loop(std::bind(write_complete_callback, shared_from_this()));
                return;
            }
            std::string msg = "request file:" + file.file_name + " send done";
            LOG_INFO(msg);
            close(file.filefd);
            file.Init();
        } else {
            std::string msg = "Send file:" + file.file_name + " to IP:" + peer_addr.To_string_ip() + " State:";
            if (errno == EPIPE || errno == ECONNRESET) {
                msg += "(high water mark)";
                LOG_DEBUG(msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                loop->Queue_in_loop(std::bind(&TcpConnection::Send_file_in_loop, shared_from_this()));
            } else if (errno == EAGAIN) {
                msg += "(resource in using)";
                LOG_DEBUG(msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                loop->Queue_in_loop(std::bind(&TcpConnection::Send_file_in_loop, shared_from_this()));
            } else {
                msg += '(' + errno + ':';
                msg += std::string(strerror(errno)) + ')';
                Connect_destroyed();
            }
        }
    }
}

/*
 * void TcpConnection::Send_file_in_loop2() {
 *     if (!channel->Is_writing() && file.Remaining() > 0) {
 *         int read_size = 15000;
 *         Base::FileHandler file_control;
 *         file_control.Open(file.file_name);
 *         char extrabuf[read_size];
 *         file.file_offset += file_control.Read(extrabuf, file.file_offset, read_size);
 *         iovec vec;
 *         vec.iov_base       = extrabuf;
 *         vec.iov_len        = sizeof(extrabuf);
 *         const int iovcnt   = 1;
 *         const size_t write = writev(socket->Fd(), &vec, iovcnt);
 *         if (write < 0) {
 *             if (errno == EPIPE || errno == ECONNRESET) {
 *                 LOG_WARN("High water");
 *                 std::this_thread::sleep_for(std::chrono::milliseconds(50));
 *                 loop->Queue_in_loop(std::bind(&TcpConnection::Send_file_in_loop2, shared_from_this()));
 *             }
 *         } else {
 *             output_buffer.Init();
 *             if (file.Remaining() > 0) {
 *                 loop->Queue_in_loop(std::bind(&TcpConnection::Send_file_in_loop2, shared_from_this()));
 *             } else {
 *                 file.Init();
 *                 loop->Queue_in_loop(std::bind(write_complete_callback, shared_from_this()));
 *             }
 *         }
 *     }
 * }
 */

void TcpConnection::Shutdown_in_loop() {
    loop->Assert_in_loop_thread();
    if (!channel->Is_writing()) {
        socket->Shutdown_write();
    }
}

void TcpConnection::Force_close_in_loop() {
    loop->Assert_in_loop_thread();
    if (state == CONNECTED || state == DISCONNECTING) {
        Handle_close();
    }
}

void TcpConnection::Set_state(ConnState state_) { state = state_; }

void TcpConnection::Start_read_in_loop() {
    loop->Assert_in_loop_thread();
    if (!reading || !channel->Is_reading()) {
        channel->Enable_reading();
        reading = true;
    }
}

void TcpConnection::Stop_read_in_loop() {
    loop->Assert_in_loop_thread();
    if (reading || channel->Is_reading()) {
        channel->Disable_reading();
        reading = false;
    }
}

const char* TcpConnection::State_to_string() {
    switch (state) {
    case DISCONNECTED:
        return "disconnected";
    case CONNECTING:
        return "connecting";
    case CONNECTED:
        return "connected";
    case DISCONNECTING:
        return "disconnecting";
    default:
        return "unknown state";
    }
}

TcpConnection::TcpConnection(Poll::EventLoop* loop_, const std::string& name_, int sockfd_,
                             const Sockets::InetAddress& local_addr_,
                             const Sockets::InetAddress& peer_addr_) :
    loop(loop_),
    name(name_),
    state(CONNECTING),
    reading(true),
    high_water_mark(64 * 1024 * 1024),
    input_buffer(Base::Buffer::BufferState::READ),
    output_buffer(Base::Buffer::BufferState::WRITE),
    file(),
    context(),
    socket(new Sockets::Socket(sockfd_)),
    channel(new Poll::Channel(loop, sockfd_)),
    local_addr(local_addr_),
    peer_addr(peer_addr_) {
    channel->Set_read_callback(std::bind(&TcpConnection::Handle_read, this, Time::TimeStamp(Time::Clock::Nowtime_us())));
    channel->Set_write_callback(std::bind(&TcpConnection::Handle_write, this));
    channel->Set_close_callback(std::bind(&TcpConnection::Handle_close, this));
    channel->Set_error_callback(std::bind(&TcpConnection::Handle_error, this));
    const void* address = static_cast<const void*>(this);
    std::stringstream this_point;
    this_point << address;
    std::string msg = "TcpConnection[" + name + "] ctor at " + this_point.str()
                      + " fd:" + std::to_string(sockfd_);
    socket->Set_keep_alive(true);
}

Poll::EventLoop* TcpConnection::Get_loop() const { return loop; }

const std::string& TcpConnection::Get_name() const { return name; }

const Sockets::InetAddress& TcpConnection::Get_local_address() const { return local_addr; }

const Sockets::InetAddress& TcpConnection::Get_peer_address() const { return peer_addr; }

Base::Buffer* TcpConnection::Get_input_buffer() { return &input_buffer; }

Base::Buffer* TcpConnection::Get_output_buffer() { return &output_buffer; }

Base::FileStat* TcpConnection::Get_file_stat() { return &file; }

std::string TcpConnection::Get_tcp_info() const {
    std::string tcp_info;
    socket->Get_tcp_info(&tcp_info);
    return tcp_info;
}

std::any TcpConnection::Get_context() {
    return context;
}

bool TcpConnection::Get_tcp_info(tcp_info* tcpi) const {
    return socket->Get_tcp_info(tcpi);
}

bool TcpConnection::Connected() const { return state == CONNECTED; }

bool TcpConnection::Disconnected() const { return state == DISCONNECTED; }

bool TcpConnection::Is_reading() const { return reading; }

void TcpConnection::Send(const std::string message) {
    output_buffer = message;
    if (state == CONNECTED) {
        if (loop->Is_in_loop_thread()) {
            Send_in_loop();
        } else {
            loop->Run_in_loop(std::bind(&TcpConnection::Send_in_loop, this));
        }
    }
}

void TcpConnection::Send(const std::string message, size_t len) {
    std::string clip_str = message.substr(0, len);
    Send(clip_str);
}

void TcpConnection::Send(const char* message, size_t len) {
    std::string tmp_str(message, 0, len);
    Send(tmp_str);
}

void TcpConnection::Sendfile(const std::string filename) {
    if (filename.empty()) { return; }
    struct stat sys_file_stat;
    stat(filename.c_str(), &sys_file_stat);
    file.file_name = filename.c_str();
    file.file_size = sys_file_stat.st_size;
    file.filefd    = open(filename.c_str(), O_RDONLY);
    if (state == CONNECTED) {
        if (loop->Is_in_loop_thread()) {
            Send_file_in_loop();
        } else {
            loop->Run_in_loop(std::bind(&TcpConnection::Send_file_in_loop, this));
        }
    }
}

/*
 * void TcpConnection::Sendfile2(const std::string filename) {
 *     if (filename.empty()) { return; }
 *     struct stat sys_file_stat;
 *     stat(filename.c_str(), &sys_file_stat);
 *     file.file_name = filename.c_str();
 *     file.file_size = sys_file_stat.st_size;
 *     if (state == CONNECTED) {
 *         if (loop->Is_in_loop_thread()) {
 *             Send_file_in_loop2();
 *         } else {
 *             loop->Run_in_loop(std::bind(&TcpConnection::Send_file_in_loop2, this));
 *         }
 *     }
 * }
 */

void TcpConnection::Shutdown() {
    if (state == CONNECTED) {
        Set_state(DISCONNECTING);
        loop->Run_in_loop(std::bind(&TcpConnection::Shutdown_in_loop, this));
    }
}

void TcpConnection::Force_close() {
    if (state == CONNECTED || state == DISCONNECTING) {
        Set_state(DISCONNECTING);
        loop->Queue_in_loop(std::bind(&TcpConnection::Force_close_in_loop, shared_from_this()));
    }
}

void TcpConnection::Force_close_delay(double seconds) {
    if (state == CONNECTED || state == DISCONNECTING) {
        Set_state(DISCONNECTING);
        loop->Run_after(seconds, std::bind(&TcpConnection::Force_close, shared_from_this()));
    }
}

void TcpConnection::Start_read() {
    loop->Run_in_loop(std::bind(&TcpConnection::Start_read_in_loop, this));
}

void TcpConnection::Stop_read() {
    loop->Run_in_loop(std::bind(&TcpConnection::Stop_read_in_loop, this));
}

void TcpConnection::Set_no_delay(bool on) { socket->Set_tcp_delay(on); }

void TcpConnection::Set_context(const std::any& context_) {
    context = context_;
}

void TcpConnection::Set_connection_callback(const ConnectionCallback& cb) {
    connection_callback = cb;
}

void TcpConnection::Set_message_callback(const MessageCallback& cb) {
    message_callback = cb;
}

void TcpConnection::Set_write_complete_callback(const WriteCompleteCallback& cb) {
    write_complete_callback = cb;
}

void TcpConnection::Set_high_water_mark_callback(const HighWaterMarkCallback& cb, size_t high_water_mark_) {
    high_water_mark_callback = cb;
    high_water_mark          = high_water_mark_;
}

void TcpConnection::Set_close_callback(const CloseCallback& cb) {
    close_callback = cb;
}

void TcpConnection::Connect_established() {
    loop->Assert_in_loop_thread();
    assert(state == CONNECTING);
    Set_state(CONNECTED);
    channel->Enable_reading();
    connection_callback(shared_from_this());
}

void TcpConnection::Connect_destroyed() {
    loop->Assert_in_loop_thread();
    if (state == CONNECTED) {
        Set_state(DISCONNECTED);
        channel->Disable_all();
        connection_callback(shared_from_this());
    }
    channel->Remove();
}

TcpConnection::~TcpConnection() {
    const void* address = static_cast<const void*>(this);
    std::stringstream this_point;
    this_point << address;
    std::string msg = "TcpConnection[" + name + "] dtor at "
                      + this_point.str() + " fd:" + std::to_string(channel->Fd())
                      + " state:" + State_to_string();
    LOG_INFO(msg);
    assert(state == DISCONNECTED);
}
