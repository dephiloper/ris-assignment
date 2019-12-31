#include "netmanager.h"

NetManager::NetManager(zmq::socket_type socketType, const std::string& ipAddress, const int port) : isRunning(false) {
    this->socketType = socketType;
    this->ipAddress = ipAddress;
    this->port = port;
}

void NetManager::start(NetManager& instance) {
    socket = zmq::socket_t(context, socketType);
    socket.setsockopt(ZMQ_RCVTIMEO, 10);
    //socket.setsockopt(ZMQ_LINGER, 500);
    connect();
    isRunning.store(true);
    publishTask = std::thread(&NetManager::publishData, std::ref(*this));
    receiveTask = std::thread(&NetManager::receiveData, std::ref(*this));
}

void NetManager::stop() {
    isRunning.store(false);
    queueOut.requestShutdown();
    queueIn.requestShutdown();

    if (publishTask.joinable())
        publishTask.join();
    if (receiveTask.joinable())
        receiveTask.join();
    
    socket.setsockopt(ZMQ_LINGER, 0);
    disconnect();
    socket.close();
    context.close();
}