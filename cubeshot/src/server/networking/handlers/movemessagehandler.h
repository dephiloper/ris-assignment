#ifndef MOVEHANDLER_H
#define MOVEHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/movemessage.h"
#include "../../../shared/networking/netmessagehandler.h"
#include "../../../shared/networking/netmanager.h"

class MoveMessageHandler: public NetMessageHandler {
public:
    void handle(NetMessage& message);
};

#endif // MOVEHANDLER_H