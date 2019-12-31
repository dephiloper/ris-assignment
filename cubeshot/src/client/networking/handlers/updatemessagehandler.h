#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/updatemessage.h"
#include "../../../shared/networking/netmessagehandler.h"
#include <chrono>


class UpdateMessageHandler: public NetMessageHandler {
private:
    World* world;
    std::chrono::system_clock::time_point lastTime;
public:
    UpdateMessageHandler(World* world);
    void handle(NetMessage& message);
};

#endif // NETMESSAGEHANDLER_H