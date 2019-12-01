#include "networkmanager.h"

NetworkManager::NetworkManager(const std::string& ipAddress) : isRunning(false) {
    this->ipAddress = ipAddress;
}

void NetworkManager::start() {
    this->inputTask = std::thread(&NetworkManager::pushInput, this);
    this->updateWorldTask = std::thread(&NetworkManager::receiveWorld, this);
}

void NetworkManager::stop() {
    this->isRunning = false;
    this->inputQueue.requestShutdown();
    this->inputTask.join();
}

void NetworkManager::pushInput() {
    zmq::socket_t sock(context, zmq::socket_type::push);
    sock.connect("tcp://" + ipAddress + ":5555");
    isRunning.store(true);

    while (isRunning.load()) {
        int input = inputQueue.pop();
        std::array<int, 1> buf{};
        buf[0] = input;
        sock.send(zmq::buffer(buf), zmq::send_flags::none);
    }

    sock.disconnect("tcp://" + ipAddress + ":5555");
}

void NetworkManager::receiveWorld() {
    zmq::socket_t sock(context, zmq::socket_type::sub);
    sock.connect("tcp://" + ipAddress + ":5556");
    sock.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    while (1) {
        zmq::message_t msg;
        sock.recv(msg, zmq::recv_flags::none);
        World w;
        unsigned int playerCount = msg.size() / PLAYER_BUFFER_SIZE;
        
        for (int i = 0; i < playerCount; i++) {
            Player p = *reinterpret_cast<const Player*>(msg.data() + PLAYER_BUFFER_SIZE * i);
            w.players.push_back(p);
        }

        this->world = w;
    }
    
    sock.disconnect("tcp://" + ipAddress + ":5556");
}