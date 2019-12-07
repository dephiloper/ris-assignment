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
        socket.recv(zmqMsg, zmq::recv_flags::none); // blocking receive with timeout of 200 ms
        if (zmqMsg.empty()) continue;

        std::cout << "receiving message" << std::endl;

        std::string data = std::string(static_cast<char*>(zmqMsg.data()), zmqMsg.size());
        Command cmd = NetMessage::readCommand(&data);
        std::shared_ptr<NetMessage> msg;

        switch(cmd) {
            case Command::LOGIN:
            {
                msg = std::make_shared<LoginMessage>(LoginMessage::deserialize(data));
                std::cout << "login message received" << std::endl;
                break;
            }
            case Command::LOGOUT:
            {
                msg = std::make_shared<LogoutMessage>(LogoutMessage::deserialize(data));
                std::cout << "logout message received" << std::endl;
                break;
            }
            case Command::UPDATE:
            {
                std::cout << "update" << std::endl;
                break;
            }
        }

        queueIn.push(msg);
    }
}

void ClientNetManager::connect() {
        socket.connect("tcp://" + ipAddress + ":5555");
}

void ClientNetManager::disconnect() {
        socket.disconnect("tcp://" + ipAddress + ":5555");
}