#ifndef BASE_LATCH_H_
#define BASE_LATCH_H_

#include "../../Class/noncopyable.h"
#include <condition_variable>
#include <mutex>

namespace Wasi {
namespace Base {

class Latch : Noncopyable {
private:
    mutable std::mutex mtx;
    std::condition_variable cond;
    int count;

public:
    explicit Latch(int count_);
    void Wait();
    void Count_down();
    int Get_count() const;
};

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

}
} // namespace Wasi::Base

#endif // !BASE_LATCH_H_