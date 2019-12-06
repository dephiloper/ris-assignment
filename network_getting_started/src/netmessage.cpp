#include "netmessage.h"

NetMessage::NetMessage(MessageType type, std::string id) {
    this->type = type;
    this->id = id;
}

std::vector<char> NetMessage::toBuffer() {
    char data[sizeof(NetMessage)];
    std::memcpy(data, this, sizeof(NetMessage));

    std::vector<char> buf;
    buf.insert(buf.end(), data, data + sizeof(NetMessage));

    return buf;
}