#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>

template <typename T> 
// blocking queue from https://stackoverflow.com/a/12805690
class BlockingQueue
{
private:
    std::mutex mutex;
    std::condition_variable cvCanPop;
    std::queue<T> queue;
    std::atomic<bool> shutdown{};

public:
    void push(T const& value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            queue.push(value);
        }
        this->cvCanPop.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->cvCanPop.wait(lock, [=]{ return !this->queue.empty() || this->shutdown; });
        if (!this->shutdown) {
            T value(std::move(this->queue.back()));
            this->queue.pop();
            return value;
        } else {
            return T();
        }
    }
    void requestShutdown() {
        {   
            std::unique_lock<std::mutex> lock(mutex);
            shutdown = true;
        }
    cvCanPop.notify_all();
    }
};

#endif