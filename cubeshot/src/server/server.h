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
#include "networking/handlers/movemessagehandler.h"
#include "../shared/networking/netmessagehandler.h"
#include "../shared/networking/messages/netmessage.h"
#include "../shared/networking/messages/loginmessage.h"
#include "../shared/networking/messages/logoutmessage.h"
#include "../shared/networking/messages/updatemessage.h"
#include "../shared/networking/messages/movemessage.h"
#include "../shared/world.h"

class Server {
private:
    ServerNetManager netManager;
    int64_t lastFrame{};
    bool isRunning = true;
    World world;
    std::map<std::type_index, std::shared_ptr<NetMessageHandler>> listeners;

    void processMessages();

public:
    Server();
    void mainLoop();
};

#endif // SERVER_H