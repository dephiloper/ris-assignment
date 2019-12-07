#include "netmessage.h"

NetMessage::NetMessage() {}

NetMessage::NetMessage(std::string senderId) {
    this->senderId = senderId;
}

std::vector<unsigned char> NetMessage::serialize(const NetMessage& msg) {
    std::vector<unsigned char> buffer;
    buffer.resize(sizeof(msg.senderId));
    std::size_t index = 0;

    index = Serialization::binary_serialize(buffer, index, msg.senderId);

    return buffer;
}

NetMessage NetMessage::deserialize(const std::vector<unsigned char>& buffer) {
    NetMessage msg;
    std::size_t index = 0;

    index = Serialization::binary_deserialize(buffer, index, &msg.senderId);

    return msg;
}
