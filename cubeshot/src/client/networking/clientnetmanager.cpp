#include "clientnetmanager.h"

ClientNetManager::ClientNetManager(const std::string& ipAddress, const int port) : NetManager(zmq::socket_type::dealer, ipAddress, port) { }

void ClientNetManager::publishData() {
    while (isRunning.load()) {
        auto msg = queueOut.pop(); // TODO currently returns just new object when queue is stopped
        if (isRunning.load()) {
            socket.send(zmq::buffer(msg->toBuffer()), zmq::send_flags::dontwait);
        }
    }
}

void ClientNetManager::receiveData() {
    while (isRunning.load()) {
        zmq::message_t zmqMsg;
        socket.recv(zmqMsg, zmq::recv_flags::none); // blocking receive with timeout of 200 ms
        if (zmqMsg.empty()) continue;

        std::cout << "receiving message" << std::endl;
        //std::unique_ptr<NetMessage> msg; // TODO convert from received message
        //queueIn.push(msg);
    }
}

void ClientNetManager::connect() {
        socket.connect("tcp://" + ipAddress + ":5555");
}

void ClientNetManager::disconnect() {
        socket.disconnect("tcp://" + ipAddress + ":5555");
}