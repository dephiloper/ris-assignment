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
    for (auto i = -1; i <= 1; i++) {
        for (auto j = -1; j <= 1; j++) {
            Floor tile;
            tile.position = Vector3{ i * tile.scale.x, -0.1, j * tile.scale.z };
            world.tiles.push_back(tile);
        }
    }
    //netManager.queueIn.push(std::make_shared<LoginMessage>("id"));
    for (auto i = 0; i < 8; i++) {
        world.obstacles.push_back(Obstacle{ Vector3{ rand() % 10 - 5.0f, 0.5f, rand() % 10 - 5.0f } });
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
    float newX = position.x + direction.x;
    float newZ = position.z + direction.z;
    glm::vec3 newPosX = glm::vec3(newX, 1, position.z);
    glm::vec3 newPosZ = glm::vec3(position.x, 1, newZ);

    if (!checkForCollision(newPosX, COLLISION_RADIUS))
        destination.x = newPosX.x;
    if (!checkForCollision(newPosZ, COLLISION_RADIUS))
        destination.z = newPosZ.z;
    
    return destination;
}

bool Server::checkForCollision(glm::vec3 destination, float playerRadius) {
    for (auto const& obstacle : world.obstacles) {
        float testX = destination.x, testZ = destination.z;
        
        if (destination.x < obstacle.position.x - obstacle.radius)       testX = obstacle.position.x - obstacle.radius; // left edge
        else if (destination.x > obstacle.position.x + obstacle.radius)  testX = obstacle.position.x + obstacle.radius; // right edge
        if (destination.z < obstacle.position.z - obstacle.radius)       testZ = obstacle.position.z - obstacle.radius; // top edge
        else if (destination.z > obstacle.position.z + obstacle.radius)  testZ = obstacle.position.z + obstacle.radius; // bottom edge

        float distance = glm::distance(glm::vec2(destination.x, destination.z), glm::vec2(testX, testZ));
        
        if (distance <= playerRadius) return true;
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
