#include "servernetmanager.h"

ServerNetManager::ServerNetManager(const int port) : NetManager(zmq::socket_type::router, "*", port) { }

void ServerNetManager::publishData() {
    while (isRunning.load()) {
        auto msg = queueOut.pop(); // TODO currently returns just new object when queue is stopped
        if (isRunning.load()) {
            socket.send(zmq::message_t(msg->id), zmq::send_flags::sndmore); // TODO send id msg
            socket.send(zmq::message_t(msg->toBuffer()), zmq::send_flags::dontwait); // TODO send net msg
        }
    }
}

void ServerNetManager::receiveData() {
    while (isRunning.load()) {
        zmq::message_t id;
        socket.recv(id, zmq::recv_flags::none);
        if (id.empty()) continue;

        zmq::message_t zmqMsg;
        socket.recv(zmqMsg, zmq::recv_flags::none);
        std::shared_ptr<NetMessage> msg(new NetMessage(MessageType::LOGIN, std::string(static_cast<char*>(id.data()), id.size())));
        std::cout << msg->type << std::endl;
        queueOut.push(msg);
    }
}

void ServerNetManager::connect() {
    socket.bind("tcp://" + ipAddress + ":5555");
}

void ServerNetManager::disconnect() {
    socket.unbind("tcp://" + ipAddress + ":5555");
}