#include "Head/eventloop.h"
using namespace Wasi::Net;

Event_Loop::Event_Loop() :looping(false), thread_id(0) {;}

void Event_Loop::Loop() {
	assert(!looping);
	std::cout << "start loop.\n";
	looping = true;
	sleep(5);
	std::cout << "loop end.\n";
	looping = false;
}

bool Event_Loop::IsInLoopThread() { 
	//return thread_id == getpid();
	if (thread_id != getpid()) {
		assert("two or more event loop.");
	}
	return true;
}

void Event_Loop::UpdateChannel() {
	
}

void Event_Loop::RemoveChannel() {
	
}

bool Event_Loop::HasChannel() {
	
}

Event_Loop::~Event_Loop() {
	;
}
