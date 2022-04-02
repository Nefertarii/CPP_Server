#ifndef BASE_LATCH_H_
#define BASE_LATCH_H_

#include "../../../Class/noncopyable.h"
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

}
} // namespace Wasi::Base

#endif // !BASE_LATCH_H_