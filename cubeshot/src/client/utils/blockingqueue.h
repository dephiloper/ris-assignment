#include <mutex>
#include <condition_variable>
#include <queue>

// blocking queue from https://stackoverflow.com/a/12805690
template <typename T>
class BlockingQueue
{
private:
    std::mutex              mutex;
    std::condition_variable cvCanPop;
    std::queue<T>           queue;
    std::atomic<bool> shutdown;

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
        T rc(std::move(this->queue.back()));
        this->queue.pop();
        return rc;
    }
    void requestShutdown() {
        {   
            std::unique_lock<std::mutex> lock(mutex);
            shutdown = true;
        }
    cvCanPop.notify_all();
    }
};