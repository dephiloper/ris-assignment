#ifndef LOGOUTHANDLER_H
#define LOGOUTHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/logoutmessage.h"
#include "../../../shared/networking/netmessagehandler.h"

class LogoutMessageHandler: public NetMessageHandler {
private:
    World* world;
public:
    explicit LogoutMessageHandler(World* world);
    void handle(const NetMessage& message) override;
};

#endif // LOGOUTHANDLER_H