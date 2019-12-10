#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H

#include <iostream>

#include "../../../shared/networking/messages/loginmessage.h"
#include "../../../shared/networking/netmessagehandler.h"

class LoginMessageHandler: public NetMessageHandler {
private:
    std::string* id;
public:
    LoginMessageHandler(std::string* id);
    void handle(NetMessage& message);
};

#endif // NETMESSAGEHANDLER_H