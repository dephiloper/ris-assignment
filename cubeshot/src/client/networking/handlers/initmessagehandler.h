#ifndef INITHANDLER_H
#define INITHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/initmessage.h"
#include "../../../shared/networking/netmessagehandler.h"

class InitMessageHandler: public NetMessageHandler {
private:
    std::string* id;
    World* world;
public:
    InitMessageHandler(std::string* id, World* world);
    void handle(NetMessage& message);
};

#endif // INITHANDLER_H