#ifndef POLL_EVENTLOOP_THREADLOOP_H_
#define POLL_EVENTLOOP_THREADLOOP_H_

#include "../../../Thread/Head/threadpool.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Wasi {
namespace Poll {

class EventLoop;
using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThreadPool {
private:
    EventLoop* baseloop;
    Base::ThreadPool thread_pool;
    std::vector<std::unique_ptr<EventLoop>> loops;
    std::string name;
    bool started;
    int num_threads;
    int index;
    // std::vector<std::unique_ptr<EventLoopThread>> threads;

public:
    EventLoopThreadPool(EventLoop* baseloop_, const std::string& name_);
    void Set_thread_num(int num_threads_);
    void Start(const ThreadInitCallback& cb = ThreadInitCallback());
    EventLoop* Get_loop();
    std::string Get_name() const;
    bool Started();
    ~EventLoopThreadPool();
};

}
} // namespace Wasi::Poll

#endif // !POLL_EVENTLOOP_THREADLOOP_H_