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
class EventLoopThread;
using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThreadPool : Noncopyable {
private:
    bool started;
    int index;
    int num_threads;
    std::string name;
    EventLoop* baseloop;
    std::vector<std::unique_ptr<EventLoopThread>> threads;
    std::vector<EventLoop*> loops;

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