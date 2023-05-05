#include "timerid.h"
#include "timer.h"

using namespace Wasi::Time;

TimerId::TimerId() :
    timer(nullptr), sequence(0) {}

TimerId::TimerId(Timer* timer_) :
    timer(timer_), sequence(0) {}

TimerId::TimerId(Timer* timer_, int sequence_) :
    timer(timer_), sequence(sequence_) {}