#ifndef NETMESSAGE_H
#define NETMESSAGE_H

#include <cstring>
#include <vector>
#include <string>
#include <iostream>

#include "../utils/serialization.h"

class NetMessage {
public:
    NetMessage();
    NetMessage(std::string senderId);
    
    std::string senderId;

protected:
    static std::vector<unsigned char> serialize(const NetMessage& msg);
    static NetMessage deserialize(const std::vector<unsigned char>& buffer);
};

#endif // NETMESSAGE_H