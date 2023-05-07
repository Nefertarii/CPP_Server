#include "Include/Eventloop/channel.h"
#include "Include/Eventloop/eventloop.h"
#include "Include/Logger/logger.h"
#include <sys/poll.h>

using namespace wasi;
using namespace wasi::loop;

void Channel::Update() {
    eventloop_->UpdateChannel(this);
}

Channel::Channel(EventLoop* eventloop, int fd) :
    fd_(fd),
    looping_(false),
    handling_(false),
    events_(0),
    revents_(0),
    index_(-1),
    eventloop_(eventloop),
    read_callback_(nullptr),
    write_callback_(nullptr),
    error_callback_(nullptr),
    close_callback_(nullptr) {}

void Channel::HandleEvent(timer::TimeStamp receive_time) {
    handling_ = true;
    // peer close connection
    if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        LOG_WARN("channel:" + std::to_string(fd_) + " POLLHUP.");
        if (close_callback_) { close_callback_(); }
    }
    // invalid file descriptor
    if (revents_ & POLLNVAL) {
        LOG_WARN("channel:" + std::to_string(fd_) + " POLLNVAL.");
    }
    // error
    if (revents_ & (POLLERR | POLLNVAL)) {
        if (error_callback_) { error_callback_(); }
    }
    // readble
    if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (read_callback_) { read_callback_(receive_time); }
    }
    // writable
    if (revents_ & POLLOUT) {
        if (write_callback_) { write_callback_(); }
    }
    handling_ = false;
}

void Channel::SetRevents(int revents) { revents_ = revents; }

void Channel::SetIndex(int index) { index_ = index; }

void Channel::SetReadCallback(ReadEventCallback callback) {
    read_callback_ = std::move(callback);
}

void Channel::SetWriteCallback(EventCallback callback) {
    write_callback_ = std::move(callback);
}

void Channel::SetErrorCallback(EventCallback callback) {
    error_callback_ = std::move(callback);
}

void Channel::SetCloseCallback(EventCallback callback) {
    close_callback_ = std::move(callback);
}

void Channel::EnableReading() {
    events_ |= READEVENT;
    Update();
}

void Channel::EnableWriting() {
    events_ |= WRITEVENT;
    Update();
}

void Channel::DisableReading() {
    events_ &= ~READEVENT;
    Update();
}

void Channel::DisableWriting() {
    events_ &= ~WRITEVENT;
    Update();
}

void Channel::DisableAll() {
    events_ = NONEVENT;
    Update();
}


void Channel::Remove() {
    if (NoneEvent()) {
        eventloop_->RemoveChannel(this);
    }
}

bool Channel::NoneEvent() { return events_ == NONEVENT; }

bool Channel::Writing() { return events_ & WRITEVENT; }

bool Channel::Reading() { return events_ & READEVENT; }

int Channel::GetFd() { return fd_; }

int Channel::GetIndex() { return index_; }

int Channel::GetEvents() { return events_; }

Channel::~Channel() {
    if (handling_) {
        LOG_WARN("channel:" + std::to_string(fd_) + " close in handling");
    }
}