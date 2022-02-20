#include "Head/thread.h"
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <string>
#include <cassert>

using namespace Wasi::Net;

//pid_t gettid() 

void Thread::Set_default_name() {
    int num = num_created.fetch_add(1);
    if (name.empty()) {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name = buf;
    }
}

Thread::Thread(ThreadFunc func, const string& name_ = string()) :
    started(false),
    joined(false),
    pthread_id(0),
    tid(0),
    func(std::move(func)),
    name(name_) { Set_default_name(); }

void Thread::Start() {
    assert(!started);
    started = true;
    //if(pthread_create(&pthread_id,nullptr,data))
}

int Thread::Join();

bool Thread::Started() const { return started; }

pid_t Thread::Tid() const { return tid; }

const string& Thread::Name() const { return name; }

static int Thread::Num_created() { return num_created.load(); }

Thread::~Thread() {
    if (started && !joined) {
        pthread_detach(pthread_id);
    }
}