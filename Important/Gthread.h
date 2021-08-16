#ifndef GTHREAD_H_
#define GTHREAD_H_

#include <mutex>
#include <atomic>
#include <memory>
#include <thread>
#include <future>
#include <algorithm>


class Lock
{
public:
    void operator=() = delete;
    //void std::unique_lock(std::mutex) lock(std::mutex resource);
}


#endif