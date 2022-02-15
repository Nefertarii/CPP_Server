#include "Head/eventloop.h"

using namespace Wasi::Socket;

void Event_Loop::Abort_not_in_loop_thread() {
	if (!Is_in_loop_thread()) {
		;
	}
}

bool Event_Loop::Is_in_loop_thread() { return thread_id == getpid(); }
