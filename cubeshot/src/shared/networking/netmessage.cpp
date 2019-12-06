#include "netmessage.h"

NetMessage::NetMessage() {}

NetMessage::NetMessage(Command command, std::string senderId) {
    this->command = command;
    this->senderId = senderId;
}

std::vector<char> NetMessage::toBuffer() {
    char data[sizeof(NetMessage)];
    std::memcpy(data, this, sizeof(NetMessage));

    std::vector<char> buf;
    buf.insert(buf.end(), data, data + sizeof(NetMessage));

    return buf;
}

NetMessage NetMessage::fromBuffer(char* buffer) {
    return *reinterpret_cast<const NetMessage*>(buffer);
}