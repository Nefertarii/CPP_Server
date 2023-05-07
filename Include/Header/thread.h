#ifndef HEADER_THREAD_H_
#define HEADER_THREAD_H_

#include <atomic>
#include <functional>
#include <future>
#include <string>
#include <thread>

namespace wasi {
namespace head {
using Function = std::function<void()>;

class Thread {
private:
    bool started_;
    bool joined_;
    std::string name_;
    Function thread_func_;
    // std::packaged_task<void> thread_func;
    pthread_t pid_;
    pid_t tid_;
    std::thread thread_core_;
    static std::atomic<int> num_created_;
    void SetDefaultName();

public:
    explicit Thread(Function func, const std::string& name = std::string());
    void Start();
    int Join();
    bool Started() const;
    pid_t GetTid() const;
    const std::string& GetName() const;
    static int NumCreated();
    ~Thread();
};
}
} // namespace wasi::head

#endif