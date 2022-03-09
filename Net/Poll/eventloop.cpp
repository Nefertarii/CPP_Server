#include "Head/eventloop.h"
#include "Head/channel.h"
#include "Head/poller.h"
#include "../../Timer/Head/clock.h"
#include "../../Timer/Head/timerid.h"
#include "../../Timer/Head/timestamp.h"
#include "../../Timer/Head/timerqueue.h"
#include <iostream>
#include <cassert>
#include <sys/eventfd.h>

using namespace Wasi;
using namespace Wasi::Poll;
const int poll_timeout_ms = 5000;

int Create_event_fd() {
	int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0) {
		std::cout << "event fd create fail.\n";
	}
	return evtfd;
}

void EventLoop::Abort_not_in_loop_thread() {
	if (!Is_in_loop_thread()) {
		std::cout << "EventLoop:" << thread_id << " not current,"
			<< " Should be:" << gettid() << "\n";
	}
}

void EventLoop::Do_pending_functors() {
	std::vector<Functors> functors;
	calling_pending_functors = true;
	{
		std::lock_guard<std::mutex> lk(mtx);
		functors.swap(pending_functors);
	}
	for (const Functors& functor : functors) { functor(); }
	calling_pending_functors = false;
}

void EventLoop::Handle_read() {
	ssize_t tmp = 1;
	ssize_t ret = ::read(wake_up_fd, &tmp, sizeof(tmp));
	if (ret != sizeof(tmp)) {
		std::cout << "EventLoop:Handle_read() read:" << ret << " bytes, Should be 8.\n";
	}
}

EventLoop::EventLoop() :
	wake_up_fd(Create_event_fd()),
	looping(false),
	event_handling(false),
	quit(false),
	calling_pending_functors(false),
	thread_id(gettid()),
	current_active_channel(nullptr),
	poller(new Poller(this)),
	timer_queue(new Time::TimerQueue(this)),
	wakeup_channel(new Channel(this, wake_up_fd)) {
	std::cout << "EventLoop created:" << this << " in thread:" << thread_id << "\n";
	wakeup_channel->Set_read_callback(std::bind(&EventLoop::Handle_read, this));
	wakeup_channel->Enable_reading();
}

void EventLoop::Loop() {
	assert(!looping);
	Assert_in_loop_thread();
	looping = true;
	quit = false;
	//timer_queue->Get_expired();
	while (!quit) {
		active_channels.clear();
		poll_return_time = poller->Poll(poll_timeout_ms, &active_channels);
		event_handling = true;
		for (Channel* channel : active_channels) {
			current_active_channel = channel;
			current_active_channel->Handle_event(poll_return_time);
		}
		event_handling = false;
		current_active_channel = nullptr;
		Do_pending_functors();
	}
	looping = false;
}

void EventLoop::Quit() {
	quit = true;
	if (!Is_in_loop_thread()) { Wake_up(); }
}

Time::TimeStamp EventLoop::Poll_return_time() const {
	return poll_return_time;
}

Time::TimerId EventLoop::Run_at(const Time::TimeStamp& time, Functors callback) {
	return timer_queue->Add_timer(std::move(callback), time, 0.0);
}

Time::TimerId EventLoop::Run_after(double delay, Functors callback) {
	Time::TimeStamp time(Time::Time_stamp_add(Time::Clock::Nowtime_us(), delay));
	return Run_at(time, std::move(callback));
}

Time::TimerId EventLoop::Run_every(double interval, Functors callback) {
	Time::TimeStamp time(Time::Time_stamp_add(Time::Clock::Nowtime_us(), interval));
	return timer_queue->Add_timer(std::move(callback), time, interval);
}

void EventLoop::Update_channel(Channel* channel) {
	assert(channel->Owner_loop() == this);
	Assert_in_loop_thread();
	poller->Update_channel(channel);
}

void EventLoop::Remove_channel(Channel* channel) {
	assert(channel->Owner_loop() == this);
	Assert_in_loop_thread();
	if (event_handling) {
		assert(current_active_channel == channel ||
			   std::find(active_channels.begin(), active_channels.end(), channel) ==
			   active_channels.end());
	}
	poller->Remove_channel(channel);
}

void EventLoop::Assert_in_loop_thread() {
	if (!Is_in_loop_thread()) { Abort_not_in_loop_thread(); }
}

bool EventLoop::Is_in_loop_thread() const {
	return thread_id == gettid();
}

void EventLoop::Run_in_loop(Functors callback) {
	if (Is_in_loop_thread()) { callback(); }
	else { Queue_in_loop(callback); }
}

void EventLoop::Queue_in_loop(Functors callback) {
	{
		std::lock_guard<std::mutex> lk(mtx);
		pending_functors.push_back(std::move(callback));
	}
	if (!Is_in_loop_thread() || calling_pending_functors) { Wake_up(); }
}

void EventLoop::Wake_up() {
	ssize_t tmp = 1;
	ssize_t ret = ::write(wake_up_fd, &tmp, sizeof(tmp));
	if (ret != sizeof(tmp)) {
		std::cout << "EventLoop:Wake_up() writes:" << ret << " bytes, should be 8\n";
	}
}

EventLoop::~EventLoop() {
	assert(!looping);
	close(wake_up_fd);
}