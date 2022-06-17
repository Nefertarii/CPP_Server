#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_

#include "../noncopyable.h"
#include "../latch.h"
#include <atomic>
#include <functional>
#include <future>
#include <string>
#include <thread>

namespace Wasi {
namespace Base {
using Function = std::function<void()>;

// struct ThreadData {
//     std::string name;
//     Function thread_func;
//     Latch* latch;
//     pid_t* tid;
//     ThreadData(std::string name_, Function func_, Latch* latch_, pid_t* tid_) :
//         name(name_),
//         thread_func(func_),
//         latch(latch_),
//         tid(tid_) {}
// };

class Thread : Noncopyable {
private:
    bool started;
    bool joined;
    std::string name;
    Function thread_func;
    // std::packaged_task<void> thread_func;
    pthread_t pid;
    pid_t tid;
    std::thread thread_core;
    static std::atomic<int> num_created;
    void Set_default_name();

public:
    explicit Thread(Function func_, const std::string& name_ = std::string());
    void Start();
    int Join();
    bool Started() const;
    pid_t Get_tid() const;
    const std::string& Get_name() const;
    static int Num_created() { return num_created.load(); }
    ~Thread();
};
}
} // namespace Wasi::Base

#endif // !BASE_THREAD_H_