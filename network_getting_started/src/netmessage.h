#ifndef NETMESSAGE_H
#define NETMESSAGE_H

#include <cstring>
#include <vector>
#include <string>

enum MessageType { LOGIN, LOGOUT, UPDATE };

class NetMessage {
public:
    NetMessage(MessageType type, std::string id);
    MessageType type;
    std::string id;

    std::vector<char> toBuffer();
};

#endif