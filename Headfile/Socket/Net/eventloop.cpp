#include "Head/eventloop.h"
using namespace Wasi::Socket;

Event_Loop::Event_Loop() :looping(false), thread_id(0) {;}

void Event_Loop::Loop() {
	assert(!looping);
	std::cout << "start loop.\n";
	looping = true;
	sleep(5);
	std::cout << "loop end.\n";
	looping = false;
}

bool Event_Loop::Is_in_loop_thread() { 
	//return thread_id == getpid();
	if (thread_id != getpid()) {
		assert("two or more event loop.");
	}
	return true;
}

Event_Loop::~Event_Loop() {
	;
}
