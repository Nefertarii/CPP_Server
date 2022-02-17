#include "Head/eventloop.h"
#include "Head/channel.h"
#include "Head/poll.h"
#include <iostream>

using namespace Wasi::Net;
const int poll_time_ms = 10000;

EventLoop::EventLoop() :looping(false), thread_id(gettid()) { ; }

void EventLoop::Loop() {
	assert(!looping);
	looping = true;
	quit = false;
	while (!quit) {
		active_channels.clear();
		poller->Poll(poll_time_ms, &active_channels);
		for (ChannelList::iterator it = active_channels.begin();
			 it != active_channels.end(); ++it) {
			(*it)->Handle_event();
		}
	}
	std::cout << "loop end.\n";
	looping = false;
}

void EventLoop::Quit() {
	quit = true;
}

bool EventLoop::Is_in_loop_thread() {
	//return thread_id == getpid();
	if (thread_id != getpid()) {
		assert("two or more event loop.");
		return false;
	}
	return true;
}

void EventLoop::Assert_in_loop_thread() {
	if (!Is_in_loop_thread()) {
		//abortNotInLoopThread();
		std::cout << "EventLoop:" << thread_id << "not current.\n"
			<< "should be:" << gettid() << "\n";
	}
}

void EventLoop::Update_channel(Channel* channel) {
	assert(channel->Owner_loop() == this);
	Assert_in_loop_thread();
	poller->Update_channel(channel);
}

void EventLoop::Remove_channel(Channel* channel) {

}

bool EventLoop::Has_channel(Channel* channel) {
	assert(channel->Owner_loop() == this);
	Assert_in_loop_thread();
	return poller->Has_channel(channel);
}

EventLoop::~EventLoop() {
	;
}
