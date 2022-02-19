#include "Head/eventloop.h"
#include "Head/channel.h"
#include "Head/poll.h"
#include "../../Timer/Head/clock.h"
#include "../../Timer/Head/timerid.h"
#include "../../Timer/Head/timestamp.h"
#include "../../Timer/Head/timerqueue.h"
#include <iostream>
#include <cassert>
#include <sys/eventfd.h>

using namespace Wasi;
using namespace Wasi::Net;
const int poll_time_ms = 10000;

EventLoop::EventLoop() :
	looping(false),
	quit(false),
	calling_pending_function(false),
	wakeup_fd(Create_event()),
	thread_id(gettid()),
	poller(Poller::New_default_poller(this)),
	wakeup_channel(new Channel(this, wakeup_fd)),
	timer_queue(new Time::TimerQueue(this))
	 { ; }

void EventLoop::Handle_read() {
	
}

void EventLoop::Do_pending_functions() {
	std::vector<std::function<void()>> functions;
	calling_pending_function = true;
	{
		std::lock_guard<std::mutex> lk(mtx);
		functions.swap(pending_functions);
	}
	for (size_t i = 0;i < functions.size();++i) {
		functions[i]();
	}
	calling_pending_function = false;
}

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
		Do_pending_functions();
	}
	std::cout << "loop end.\n";
	looping = false;
}

void EventLoop::Quit() {
	quit = true;
	if (!Is_in_loop_thread()) { Wakeup(); }
}

void EventLoop::Wakeup() {
	int one = 1;
	size_t ret = ::write(wakeup_fd, &one, sizeof(one));
	if (ret != sizeof(one)) {
		std::cout << "ERR,wakeup() writes:" << ret << " bytes, should is 8";
	}
}

Time::TimerId EventLoop::Run_at(const Time::TimeStamp& time, const std::function<void()>& callback) {
	return timer_queue->Add_timer(std::move(callback), time, 0.0);
}

Time::TimerId EventLoop::Run_after(double delay, const std::function<void()>& callback) {
	Time::TimeStamp time(Time::Time_stamp_add(Time::Clock::Nowtime_us(), delay));
	return Run_at(time, callback);
}

Time::TimerId EventLoop::Run_every(double interval, const std::function<void()>& callback) {
	Time::TimeStamp time(Time::Time_stamp_add(Time::Clock::Nowtime_us(), interval));
	return timer_queue->Add_timer(callback, time, interval);
}

void EventLoop::Run_in_loop(const std::function<void()>& callback) {
	if (Is_in_loop_thread()) { callback(); }
	else { Queue_in_loop(std::move(callback)); }
}

void EventLoop::Queue_in_loop(const std::function<void()>& callback) {
	{
		std::lock_guard<std::mutex> lk(mtx);
		pending_functions.push_back(callback);
	}
	if (!Is_in_loop_thread() || calling_pending_function) { Wakeup(); }
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

int EventLoop::Create_event() {
	int eventfd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventfd_ < 0) { /*std::cout<<"event creat fail.";*/ }
	return eventfd_;
}

bool EventLoop::Has_channel(Channel* channel) {
	assert(channel->Owner_loop() == this);
	Assert_in_loop_thread();
	return poller->Has_channel(channel);
}

bool EventLoop::Is_in_loop_thread() {
	//return thread_id == getpid();
	if (thread_id != gettid()) {
		assert("two or more event loop.");
		return false;
	}
	return true;
}

EventLoop::~EventLoop() {
	//delete poller;
}
