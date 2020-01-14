#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/updatemessage.h"
#include "../../../shared/networking/netmessagehandler.h"


class UpdateMessageHandler : public NetMessageHandler {
private:
    World *world;
public:
    explicit UpdateMessageHandler(World *world);
    void handle(const NetMessage &message) override;
};

#endif // NETMESSAGEHANDLER_H