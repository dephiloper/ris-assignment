#ifndef SERVER_H
#define SERVER_H

#include <zmq.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <typeindex>
#include <typeinfo>

#include "networking/servernetmanager.h"
#include "networking/handlers/loginmessagehandler.h"
#include "networking/handlers/logoutmessagehandler.h"
#include "networking/handlers/inputmessagehandler.h"
#include "../shared/networking/netmessagehandler.h"
#include "../shared/networking/messages/netmessage.h"
#include "../shared/networking/messages/loginmessage.h"
#include "../shared/networking/messages/logoutmessage.h"
#include "../shared/networking/messages/updatemessage.h"
#include "../shared/networking/messages/inputmessage.h"
#include "../shared/world.h"
#include "../shared/utils/direction.h"
#include "../shared/utils/constants.h"

class Server {
private:
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    const float MOVEMENT_SPEED = 4.0f;
    const float COLLISION_RADIUS = PLAYER_SCALE / 2.0f;//std::sqrt(std::pow(PLAYER_SCALE, 2.0f) * 2.0f) / 2.0f;

    ServerNetManager netManager;
    int64_t lastFrame{};
    bool isRunning = true;
    World world;
    std::map<std::type_index, std::shared_ptr<NetMessageHandler>> listeners;
    std::map<std::pair<int, int>, Tile> tiles;
    std::map<std::string, InputMessage> playerInputs;
    std::map<std::string, std::pair<int, int>> playerLocations;

    void processMessages();
    void updatePlayers(float deltaTime);
    void publishWorld();
    bool checkForCollision(const glm::vec3& destination, float playerRadius);
    glm::vec3 moveAndSlide(const glm::vec3& position, const glm::vec3& direction);
    Tile generateNewTile(int x, int z);
    std::vector<Tile> getTileArea(const std::pair<int,int>& location);
    std::pair<int,int> positionToTileLocation(const Vector3& position);
public:
    Server();
    void mainLoop();
};

#endif // SERVER_H