#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include <iostream>

#include "../../shared/networking/netmessage.h"
#include "../../shared/networking/netmessagehandler.h"

class LoginMessageHandler: public NetMessageHandler {
public:
    void handle(const NetMessage& message);
};

#endif // NETMESSAGEHANDLER_H