#include "Head/poll.h"
#include "Head/eventloop.h"
#include "Head/channel.h"
#include <sys/poll.h>

using namespace Wasi::Net;

Poller::Poller(EventLoop* loop):ownerloop(loop) {}

Wasi::Time::TimeStamp  Poller::Poll(int timeout_ms, std::vector<Channel*> channels) {
    int num_events = Poll(&*pollfds.begin(), pollfds.size(), timeout_ms);
    //Ms
}

Poller::~Poller() {
    
}
