#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include <iostream>
#include <stdlib.h>

#include "../../../shared/networking/messages/loginmessage.h"
#include "../../../shared/networking/netmessagehandler.h"
#include "../../../shared/networking/netmanager.h"

class LoginMessageHandler: public NetMessageHandler {
private:
    World* world;
    NetManager* netManager;
public:
    LoginMessageHandler(NetManager* netManager, World* world);
    void handle(NetMessage& message);
};

#endif // LOGINHANDLER_H