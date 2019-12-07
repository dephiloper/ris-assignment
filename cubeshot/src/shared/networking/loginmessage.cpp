#include "loginmessage.h"

LoginMessage::LoginMessage() {}

LoginMessage::LoginMessage(std::string senderId, std::string username) : NetMessage(senderId) {
    this->username = username;
}

std::vector<unsigned char> LoginMessage::serialize(const LoginMessage& msg)
{
    // serialize base type
    std::vector<unsigned char> buf = NetMessage::serialize(msg);
    std::size_t index = buf.size();

    // cast back to derived type and serialize this too
    buf.resize(buf.size() + sizeof(username));
    index = Serialization::binary_serialize(buf, index, this->username);

    return buf;
}
 
LoginMessage LoginMessage::deserialize(const std::vector<unsigned char>& buffer)
{
    // deserialize base type
    LoginMessage msg;
    static_cast<NetMessage&>(msg) = NetMessage::deserialize(buffer);
    std::size_t index = buffer.size() - sizeof(username);

    // deserialize derived type
    index = Serialization::binary_deserialize(buffer, index, &msg.username);

    return msg;
}