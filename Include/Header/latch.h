#ifndef HEADER_LATCH_H_
#define HEADER_LATCH_H_

#include <condition_variable>
#include <mutex>

namespace wasi {
namespace head {

class Latch {
private:
    mutable std::mutex mtx_;
    std::condition_variable cond_;
    int count_;

public:
    explicit Latch(int count);
    void Wait();
    void CountDown();
    int GetCount() const;
};

}
} // namespace wasi::head

#endif