#include <map>
#include "server.h"


int main() {
    Server server;
    server.mainLoop();
    return 0;
}

int64_t currentMillis() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Server::Server(): netManager(5555) {
    std::cout << "Server started." << std::endl;
    netManager.start(netManager);
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LoginMessage), std::make_unique<LoginMessageHandler>(&netManager, &world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(LogoutMessage), std::make_unique<LogoutMessageHandler>(&world)));
    netManager.queueIn.push(std::make_shared<LoginMessage>("id"));
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = currentMillis();
        auto deltaTime = currentFrame - lastFrame;
        processMessages();

        for(auto const& [id, p] : world.players) {
            auto msg = std::make_shared<UpdateMessage>();
            msg->world.players = this->world.players;
            msg->senderId = id;
            netManager.queueOut.push(msg);
        }

        auto timeLeft = currentFrame + (1e6 / 60) - currentMillis();
        usleep(timeLeft < 0 ? 0 : timeLeft);
        if (timeLeft <= 0) std::cout << "\033[1;33mWarning:\033[0m Server doing to much work (time left: " << timeLeft << "us)" << std::endl;
        lastFrame = currentFrame;
    }
    netManager.stop();
}

void Server::processMessages() {
    while (netManager.queueIn.size() != 0) { // TODO limit this!
        auto msg = netManager.queueIn.pop();
        // std::cout << typeid(*msg).name() << std::endl;
        listeners[typeid(*msg)]->handle(*msg);
    }
}