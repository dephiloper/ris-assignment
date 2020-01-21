#ifndef SERVER_H
#define SERVER_H

#include <zmq.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <typeindex>
#include <typeinfo>
#include <set>

#include "networking/servernetmanager.h"
#include "networking/handlers/loginmessagehandler.h"
#include "networking/handlers/logoutmessagehandler.h"
#include "networking/handlers/inputmessagehandler.h"
#include "../shared/physics/collision.h"
#include "../shared/physics/raycast.h"
#include "../shared/physics/helpers.h"
#include "../shared/networking/netmessagehandler.h"
#include "../shared/networking/messages/netmessage.h"
#include "../shared/networking/messages/loginmessage.h"
#include "../shared/networking/messages/logoutmessage.h"
#include "../shared/networking/messages/updatemessage.h"
#include "../shared/networking/messages/inputmessage.h"
#include "../shared/world.h"
#include "../shared/utils/direction.h"

class Server {
private:
    ServerNetManager netManager;
    int64_t lastFrame{};
    bool isRunning = true;
    World world;
    std::map<std::type_index, std::shared_ptr<NetMessageHandler>> listeners;
    std::map<std::pair<int, int>, Tile> globalTiles;
    std::map<std::string, InputMessage> playerInputs;
    std::map<std::string, std::pair<int, int>> playerLocations;

    void processMessages();
    void updatePlayers(float deltaTime);
    void updatePotions(float deltaTime);
    RayCast shootAndCollide(const glm::vec3& origin, const glm::vec3& direction, const std::string& playerId);
    glm::vec3 moveAndSlide(const glm::vec3& position, const glm::vec3& direction);
    bool checkObstaclesForCollision(const glm::vec3& destination, float playerRadius);
    void publishWorld();
    void checkForRespawn(Player* p);
    void removeObsoleteTiles();
public:
    Server();
    void mainLoop();
};

#endif // SERVER_H