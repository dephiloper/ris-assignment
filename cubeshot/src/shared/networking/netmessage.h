#ifndef NETMESSAGE_H
#define NETMESSAGE_H

#include <cstring>
#include <vector>
#include <string>
#include <iostream>

enum Command : char { LOGIN, LOGOUT, UPDATE };

class NetMessage {
public:
    NetMessage();
    NetMessage(Command command, std::string senderId);
    Command command;
    std::string senderId;

    std::vector<char> toBuffer();
    static NetMessage fromBuffer(char* buffer);
};

#endif // NETMESSAGE_H