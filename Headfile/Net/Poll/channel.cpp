#include "Head/channel.h"
#include "Head/eventloop.h"
#include <sys/poll.h>

using namespace Wasi::Net;

const int Channel::none_event = 0;
const int Channel::read_event = POLLIN | POLLPRI;
const int Channel::write_event = POLLOUT;

Channel::Channel(EventLoop* loop_, int fd_) :
    loop(loop_), fd(fd_), events(0), revents(0), index(-1) { ; }

int Channel::Fd() { return fd; }

int Channel::Index() { return index; }

int Channel::Events() { return events; }

int Channel::Revents() { return revents; }

void Channel::Set_revents(int revents_) { revents = revents_; }

void Channel::Set_index(int index_) { index = index_; }

void Channel::Set_read_callback(const EventCallBack& cb) {
    read_callback = std::move(cb);
}

void Channel::Set_write_callback(const EventCallBack& cb) {
    write_callback = std::move(cb);
}

void Channel::Set_error_callback(const EventCallBack& cb) {
    error_callback = std::move(cb);
}

void Channel::Set_close_callback(const EventCallBack& cb) {
    close_callback = std::move(cb);
}

void Channel::Enable_reading() { events |= read_event; Update(); }
void Channel::Disable_reading() { events &= ~read_event; Update(); }
void Channel::Enable_writing() { events |= write_event; Update(); }
void Channel::Disable_writing() { events &= ~write_event; Update(); }

void Channel::Handle_event() { 
    if (revents & POLLNVAL) {
        //log warning;
        //std::cout << "Channel no value\n";
    }
    if (revents & (POLLERR | POLLNVAL)) {
        if (error_callback) { error_callback(); }
    }
    if (revents & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (read_callback) { read_callback(); }
    }
    if (revents & POLLOUT) {
        if (write_callback) { write_callback(); }
    }
}

void Channel::Update() { loop->Update_channel(this); }

bool Channel::Is_none_event() { return events == none_event; }

EventLoop* Channel::Owner_loop() { return loop; }

Channel::~Channel() {
    
}