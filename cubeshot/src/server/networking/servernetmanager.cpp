#include "servernetmanager.h"

ServerNetManager::ServerNetManager(int port) : NetManager(zmq::socket_type::router, "*", port) {}

void ServerNetManager::publishData() {
    while (isRunning.load()) {
        auto msg = queueOut.pop(); // TODO currently returns just new object when queue is stopped
        if (isRunning.load()) {
            socket.send(zmq::message_t(msg->senderId), zmq::send_flags::sndmore);
            socket.send(zmq::message_t(msg->serialize()), zmq::send_flags::dontwait);
        }
    }
}

void ServerNetManager::receiveData() {
    while (isRunning.load()) {
        zmq::message_t idMsg;
        socket.recv(idMsg, zmq::recv_flags::none); // timeouts after 10ms
        if (idMsg.empty()) continue;
        std::string id = std::string(static_cast<char *>(idMsg.data()), idMsg.size());

        zmq::message_t zmqMsg;
        socket.recv(zmqMsg, zmq::recv_flags::none);

        std::string data = std::string(static_cast<char *>(zmqMsg.data()), zmqMsg.size());
        Command cmd = NetMessage::readCommand(data);
        std::shared_ptr<NetMessage> msg;

        switch (cmd) {
            case Command::LOGIN: {
                msg = std::make_shared<LoginMessage>(LoginMessage::deserialize(data));
                break;
            }
            case Command::LOGOUT: {
                msg = std::make_shared<LogoutMessage>(LogoutMessage::deserialize(data));
                break;
            }
            case Command::MOVE: {
                msg = std::make_shared<InputMessage>(InputMessage::deserialize(data));
                break;
            }
            case INIT:
            case UPDATE: {
                std::cout << "Error: the command can't get processed by the server." << std::endl;
                break;
            }
        }
        assert(msg != nullptr);


        msg->senderId = id;
        queueIn.push(msg);
    }
}

void ServerNetManager::connect() {
    socket.bind("tcp://" + ipAddress + ":5555");
}

void ServerNetManager::disconnect() {
    socket.unbind("tcp://" + ipAddress + ":5555");
}