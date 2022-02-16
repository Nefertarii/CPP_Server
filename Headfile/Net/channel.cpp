#include "Head/channel.h"
#include "Head/eventloop.h"
#include <sys/poll.h>

using namespace Wasi::Net;

const int Channel::none_event = 0;
const int Channel::read_event = POLLIN | POLLPRI;
const int Channel::write_event = POLLOUT;
Channel::Channel(EventLoop* loop_, int fd_):
    loop(loop_), fd(fd_), event(0), revent(0), index(-1) { ; }

void Channel::HandleEvent() {
    if (revents & POLLNVAL) {
        //log warning;
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

void Channel::Update() {
    //loop->Updatechannel(this);
}

Channel::~Channel() {
    
}