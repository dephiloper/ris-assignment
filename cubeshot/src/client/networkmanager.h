#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <thread>
#include <atomic>
#include <string>
#include "utils/blockingqueue.h"
#include "zmq.hpp"

class NetworkManager {
public:
    explicit NetworkManager(const std::string& ipAddress);
    BlockingQueue<int> inputQueue;
    
    void start();
    void stop();
private:
    zmq::context_t context;
    std::thread task;
    std::atomic<bool> isRunning{};
    std::string ipAddress;

    void pushInput();
};

#endif