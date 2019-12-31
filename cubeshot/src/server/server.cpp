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
    for (auto i = 0; i < 8; i++) {
        world.obstacles.push_back(Obstacle{ Vector3{ rand() % 10 - 5.0f, 1.0f, rand() % 10 - 5.0f } });
    }
}

void Server::mainLoop() {
    while(isRunning) {
        auto currentFrame = currentMillis();
        auto deltaTime = (currentFrame - lastFrame) / 1e6f;
        processMessages();

        updatePlayers(deltaTime);
        publishWorld();

        auto timeLeft = currentFrame + (1e6 / 60) - currentMillis();
        usleep(timeLeft < 0 ? 0 : timeLeft);
        if (timeLeft <= 0) std::cout << "\033[1;33mWarning:\033[0m Server doing to much work (time left: " << timeLeft << "us)" << std::endl;
        lastFrame = currentFrame;
    }
    netManager.stop();
}

void Server::processMessages() {
    auto queueCount = netManager.queueIn.size();
    // process only currently received messages
    for (auto i = 0; i < queueCount; i++) {
        auto msg = netManager.queueIn.pop();
        listeners[typeid(*msg)]->handle(*msg);
    }
}


void Server::updatePlayers(float deltaTime) {
    for(auto& [id, p] : world.players) {
        auto input = playerInputs[id];
        float velocity = MOVEMENT_SPEED * deltaTime;

        p.front = input.front;

        // calculate front and right vectors
        glm::vec3 front = glm::normalize(glm::vec3(p.front.x, 0, p.front.z));
        glm::vec3 right = glm::normalize(glm::cross(front, WORLD_UP));
        
        glm::vec3 direction(0);

        if ((input.direction & FORWARD) == FORWARD)
            direction += front;
        if ((input.direction & BACKWARD) == BACKWARD)
            direction -= front;
        if ((input.direction & RIGHT) == RIGHT)
            direction += right;
        if ((input.direction & LEFT) == LEFT)
            direction -= right;

        if (direction != glm::vec3(0))
            direction = glm::normalize(direction) * velocity;
        
        p.position = Vector3::from(moveAndSlide(Vector3::toGlm(p.position), direction));
    }  
}

glm::vec3 Server::moveAndSlide(glm::vec3 position, glm::vec3 direction) {
    glm::vec3 destination = position;

    glm::vec3 newPosX = glm::vec3(position.x + direction.x, 1, position.z);
    glm::vec3 newPosZ = glm::vec3(position.x, 1, position.z + direction.z);
    if (!checkForCollision(newPosX, 0.5f)) {
        destination.x = newPosX.x;
    } else {
        std::cout << "collision on x" << std::endl;
    }
    if (!checkForCollision(newPosZ, 0.5f)) {
        destination.z = newPosZ.z;
    } else {
        std::cout << "collision on y" << std::endl;
    }
    
    return destination;
}

bool Server::checkForCollision(glm::vec3 destination, float collisionRadius) {
    for (auto const& obstacle : world.obstacles) {
        float testX = destination.x, testZ = destination.z;
        
        if (destination.x < obstacle.position.x - collisionRadius)       testX = obstacle.position.x - collisionRadius; // left edge
        else if (destination.x > obstacle.position.x + collisionRadius)  testX = obstacle.position.x + collisionRadius; // right edge
        if (destination.z < obstacle.position.z - collisionRadius)       testZ = obstacle.position.z - collisionRadius; // top edge
        else if (destination.z > obstacle.position.z + collisionRadius)  testZ = obstacle.position.z + collisionRadius; // bottom edge

        float distance = glm::distance(glm::vec2(destination.x, destination.z), glm::vec2(testX, testZ));
        
        if (distance <= 0.5f) return true;
    }
    return false;
}

void Server::publishWorld() {
    for(auto const& [id, p] : world.players) {
        auto msg = std::make_shared<UpdateMessage>();
        msg->players = this->world.players;
        msg->senderId = id;
        netManager.queueOut.push(msg);
    }
}
