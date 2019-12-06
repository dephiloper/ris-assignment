#ifndef NETMESSAGEHANDLER_H
#define NETMESSAGEHANDLER_H

#include "netmessage.h"

class NetMessageHandler {
public:
    virtual void handle(const NetMessage& message) = 0;
};

#endif // NETMESSAGEHANDLER_H