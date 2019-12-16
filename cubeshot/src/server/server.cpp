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
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(InputMessage), std::make_unique<InputMessageHandler>(&playerInputs)));
    //netManager.queueIn.push(std::make_shared<LoginMessage>("id"));
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = currentMillis();
        auto deltaTime = (currentFrame - lastFrame) / 1e6f;
        processMessages();

        updatePlayers(deltaTime);

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
        listeners[typeid(*msg)]->handle(*msg);
    }
}


void print(std::vector<float> const &input)
{
	std::copy(input.begin(),
			input.end(),
			std::ostream_iterator<float>(std::cout, " "));
}

void Server::updatePlayers(float deltaTime) {
    for(auto & [id, p] : world.players) {
        auto input = playerInputs[id];
        float velocity = 1.0 * deltaTime;

        p.front = input.front;

        glm::vec3 front = glm::normalize(glm::vec3(p.front[0], p.front[1], p.front[2]));
        glm::vec3 right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

        std::cout << "front length " << glm::length(front) << std::endl;
        std::cout << "right length " << glm::length(right) << std::endl;

        glm::vec3 position(p.x, p.y, p.z);

        if ((input.direction & FORWARD) == FORWARD)
            position += front * velocity;
        if ((input.direction & BACKWARD) == BACKWARD)
            position -= front * velocity;
        if ((input.direction & RIGHT) == RIGHT)
            position += right * velocity;
        if ((input.direction & LEFT) == LEFT)
            position -= right * velocity;

        position.y = 1;
        p.x = position.x;
        p.y = position.y;
        p.z = position.z;
    }  

    
}
