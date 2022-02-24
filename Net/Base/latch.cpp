#include "Head/latch.h"

using namespace Wasi::Base;

Latch::Latch(int count_) :
    mtx(),
    cond(),
    count(count_) {}

void Latch::Wait() {
    std::unique_lock<std::mutex> lk(mtx);
    while (count > 0) {
        cond.wait(lk);
    }
}

void Latch::Count_down() {
    std::lock_guard<std::mutex> lk(mtx);
    --count;
    if (count == 0) {
        cond.notify_all();
    }
}

int Latch::Get_count() const {
    std::lock_guard<std::mutex> lk(mtx);
    return count;
}