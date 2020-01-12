#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/updatemessage.h"
#include "../../../shared/networking/netmessagehandler.h"
#include "../../../shared/utils/constants.h"


class UpdateMessageHandler: public NetMessageHandler {
private:
    World* world;
public:
    UpdateMessageHandler(World* world);
    void handle(NetMessage& message);
};

#endif // NETMESSAGEHANDLER_H