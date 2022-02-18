#include "Head/timerid.h"
#include "Head/timer.h"

using namespace Wasi::Time;

TimerId::TimerId() :timer(nullptr), sequence(0) {}

TimerId::TimerId(Timer* timer_, int sequence_) :
    timer(timer_), sequence(sequence_) {}