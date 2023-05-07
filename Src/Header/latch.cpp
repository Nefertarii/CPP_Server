#include "Include/Header/latch.h"

using namespace wasi::head;

Latch::Latch(int count) :
    mtx_(),
    cond_(),
    count_(count) {}

void Latch::Wait() {
    std::unique_lock<std::mutex> lk(mtx_);
    while (count_ > 0) {
        cond_.wait(lk);
    }
}

void Latch::CountDown() {
    std::lock_guard<std::mutex> lk(mtx_);
    --count_;
    if (count_ == 0) {
        cond_.notify_all();
    }
}

int Latch::GetCount() const {
    std::lock_guard<std::mutex> lk(mtx_);
    return count_;
}