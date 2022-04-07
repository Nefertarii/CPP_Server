#ifndef POLL_EVENTLOOP_THREADLOOP_MUDUO_H_
#define POLL_EVENTLOOP_THREADLOOP_MUDUO_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace Wasi {
namespace Poll {

class EventLoop;
class EventLoopThread;
using ThreadInitCallback = std::function<void(EventLoop*)>;

class EventLoopThreadPool {
private:
    EventLoop* baseloop;
    std::string name;
    bool started;
    int num_threads;
    int next;
    std::vector<std::unique_ptr<EventLoopThread>> threads;
    std::vector<EventLoop*> loops;

public:
    EventLoopThreadPool(EventLoop* baseloop_, const std::string& name_);
    void Set_thread_num(int num_threads_);
    void Start(const ThreadInitCallback& cb = ThreadInitCallback());
    EventLoop* Get_next_loop();
    EventLoop* Get_loop_hash(size_t hash);
    std::string Get_name() const;
    std::vector<EventLoop*> Get_all_loops();
    bool Started();
    ~EventLoopThreadPool();
};

}
} // namespace Wasi::Poll

#endif // !POLL_EVENTLOOP_THREADLOOP_MUDUO_H_