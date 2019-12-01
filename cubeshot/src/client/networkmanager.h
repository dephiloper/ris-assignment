#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <thread>
#include <atomic>
#include <string>
#include <iostream>

#include "zmq.hpp"
#include "utils/blockingqueue.h"
#include "../shared/world.h"

class NetworkManager {
public:
    explicit NetworkManager(const std::string& ipAddress);
    BlockingQueue<int> inputQueue;
    World world;
    
    void start();
    void stop();
private:
    static constexpr unsigned int PLAYER_BUFFER_SIZE = sizeof(int) + sizeof(float) * 3;
    zmq::context_t context;
    std::thread inputTask;
    std::thread updateWorldTask;
    std::atomic<bool> isRunning{};
    std::string ipAddress;

    void pushInput();
    void receiveWorld();
};

#endif