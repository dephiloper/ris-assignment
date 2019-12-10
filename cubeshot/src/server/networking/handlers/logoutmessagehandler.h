#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/logoutmessage.h"
#include "../../../shared/networking/netmessagehandler.h"

class LogoutMessageHandler: public NetMessageHandler {
private:
    World* world;
public:
    LogoutMessageHandler(World* world);
    void handle(NetMessage& message);
};

#endif // LOGOUTHANDLER_H