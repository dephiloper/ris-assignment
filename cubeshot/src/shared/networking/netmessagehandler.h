#ifndef NETMESSAGEHANDLER_H
#define NETMESSAGEHANDLER_H

#include "messages/netmessage.h"
#include "../world.h"

class NetMessageHandler {
public:
    virtual void handle(const NetMessage& message) = 0;
};

#endif // NETMESSAGEHANDLER_H