#ifndef LOGINMESSAGE_H
#define LOGINMESSAGE_H

#include "netmessage.h"

class LoginMessage : public NetMessage {
public:
    LoginMessage();
    LoginMessage(std::string senderId, std::string username);
    std::string username;

    std::vector<char> toBuffer();
    static NetMessage fromBuffer(char* buffer);
};

#endif // LOGINMESSAGE_H