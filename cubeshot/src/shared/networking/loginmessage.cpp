#include "loginmessage.h"

LoginMessage::LoginMessage() {}

LoginMessage::LoginMessage(std::string senderId, std::string username) : NetMessage(Command::LOGIN, senderId) {
    this->username = username;
}

std::vector<char> LoginMessage::toBuffer() {
    char data[sizeof(Command) + sizeof(LoginMessage)];
    std::memcpy(data, (char *)Command::LOGIN, sizeof(Command::LOGIN));
    std::memcpy(data + sizeof(Command::LOGIN), this, sizeof(LoginMessage));

    std::vector<char> buf;
    buf.insert(buf.end(), data, data + sizeof(Command::LOGIN) + sizeof(LoginMessage));

    return buf;
}

NetMessage LoginMessage::fromBuffer(char* buffer) {
    return *reinterpret_cast<const LoginMessage*>(buffer + sizeof(Command::LOGIN));
}