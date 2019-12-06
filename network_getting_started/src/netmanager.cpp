#include "netmanager.h"

NetManager::NetManager(zmq::socket_type socketType, const std::string& ipAddress, const int port) : isRunning(false) {
    this->socketType = socketType;
    this->ipAddress = ipAddress;
    this->port = port;
}

void NetManager::start(NetManager& instance) {
    socket = zmq::socket_t(context, socketType);
    connect();
    isRunning.store(true);
    publishTask = std::thread(&NetManager::publishData, std::ref(instance));
    receiveTask = std::thread(&NetManager::receiveData, std::ref(instance));
}

void NetManager::stop() {
    isRunning.store(false);
    queueOut.requestShutdown();
    queueIn.requestShutdown();
    
    if (publishTask.joinable())
        publishTask.join();
    if (receiveTask.joinable())
        receiveTask.join();

    disconnect();
}