#include "clientnetmanager.h"

ClientNetManager::ClientNetManager(const std::string& ipAddress, const int port) : NetManager(zmq::socket_type::dealer, ipAddress, port) { }

void ClientNetManager::publishData() {
    while (isRunning.load()) {
        auto msg = queueOut.pop(); // TODO currently returns just new object when queue is stopped
        if (isRunning.load()) {
            socket.send(zmq::buffer(msg->serialize()), zmq::send_flags::dontwait);
        }
    }
}

void ClientNetManager::receiveData() {
    while (isRunning.load()) {
        zmq::message_t zmqMsg;
        socket.recv(zmqMsg, zmq::recv_flags::none); // blocking receive with timeout of 10 ms
        if (zmqMsg.empty()) continue;

        std::string data = std::string(static_cast<char*>(zmqMsg.data()), zmqMsg.size());
        Command cmd = NetMessage::readCommand(data);
        std::shared_ptr<NetMessage> msg;

        switch(cmd) {
            case Command::INIT: {
                msg = std::make_shared<InitMessage>(InitMessage::deserialize(data));
                break;
            }
            case Command::UPDATE: {
                msg = std::make_shared<UpdateMessage>(UpdateMessage::deserialize(data));
                break;
            }
        }

        assert(msg != nullptr);
        queueIn.push(msg);
    }
}

void ClientNetManager::login() {
    queueOut.push(std::make_shared<LoginMessage>());
}

void ClientNetManager::logout() {
    queueOut.push(std::make_shared<LogoutMessage>());
    usleep(200);
}

void ClientNetManager::connect() {
    socket.connect("tcp://" + ipAddress + ":5555");
}

void ClientNetManager::disconnect() {
    socket.disconnect("tcp://" + ipAddress + ":5555");
}