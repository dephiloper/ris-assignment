#ifndef SERVER_H
#define SERVER_H

#include <zmq.hpp>

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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Server {
private:
    const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
    const float MOVEMENT_SPEED = 4.0f;

    ServerNetManager netManager;
    int64_t lastFrame{};
    bool isRunning = true;
    World world;
    std::map<std::type_index, std::shared_ptr<NetMessageHandler>> listeners;
    std::map<std::string, InputMessage> playerInputs;

    void processMessages();
    void updatePlayers(float deltaTime);

public:
    Server();
    void mainLoop();
};

#endif // SERVER_H