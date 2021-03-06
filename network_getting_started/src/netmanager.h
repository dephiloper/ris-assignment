#ifndef NETMANAGER_H
#define NETMANAGER_H

#include "zmq.hpp"

#include <thread>
#include <atomic>
#include <string>
#include <iostream>

#include "blockingqueue.h"
#include "netmessage.h"


class NetManager {
public:
    BlockingQueue<std::shared_ptr<NetMessage>> queueOut;
    BlockingQueue<std::shared_ptr<NetMessage>> queueIn;
    
    void start(NetManager& instance);
    void stop();
protected:
    NetManager(zmq::socket_type socketType, const std::string& ipAddress, const int port);
    zmq::context_t context;
    zmq::socket_type socketType;
    zmq::socket_t socket;

    std::string ipAddress;
    int port;
    std::atomic<bool> isRunning{};

    std::thread publishTask;
    std::thread receiveTask;

    virtual void publishData() = 0;
    virtual void receiveData() = 0;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
};

#endif