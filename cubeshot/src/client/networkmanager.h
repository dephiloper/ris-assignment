#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <thread>
#include <atomic>
#include "utils/blockingqueue.h"
#include "zmq.hpp"

class NetworkManager {
public:
    BlockingQueue<int> inputQueue;
    
    void start();
    void stop();
private:
    zmq::context_t context;
    std::thread task;
    std::atomic<bool> isRunning;

    void pushInput();
};

#endif