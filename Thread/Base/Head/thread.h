#ifndef BASE_THREAD_H_
#define BASE_THREAD_H_

#include "../../../Class/noncopyable.h"
#include "latch.h"
#include <atomic>
#include <functional>
#include <string>
#include <thread>

namespace Wasi {
namespace Base {

using Function = std::function<void()>;

class Thread : Noncopyable {
private:
    void Set_default_name();
    bool started;
    bool joined;
    std::string name;
    Function func;
    Latch latch;
    pthread_t pthread_id;
    pid_t tid;
    static std::atomic<int> num_created;

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