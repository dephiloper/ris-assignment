#include "networkmanager.h"
#include <iostream>

NetworkManager::NetworkManager(const std::string& ipAddress) : ipAddress(ipAddress, isRunning = false) {
}

void NetworkManager::start() {
    this->task = std::thread(&NetworkManager::pushInput, this);
}

void NetworkManager::stop() {
    this->isRunning = false;
    this->inputQueue.requestShutdown();
    this->task.join();
}

void NetworkManager::pushInput() {
    zmq::socket_t pushSock(context, zmq::socket_type::push);
    pushSock.connect("tcp://" + ipAddress);

    isRunning.store(true);

    while (isRunning.load()) {
        int input = inputQueue.pop();
        std::array<int, 1> buf{};
        buf[0] = input;
        pushSock.send(zmq::buffer(buf), zmq::send_flags::none);
    }

    pushSock.disconnect("tcp://" + ipAddress);
}