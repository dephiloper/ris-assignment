#ifndef LOGINMESSAGE_H
#define LOGINMESSAGE_H

#include "netmessage.h"
#include <string>

struct LoginMessage : NetMessage {
    LoginMessage() = default;
    LoginMessage(const std::string &senderId) : NetMessage(senderId) {}

    std::string receiverId;

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::LOGIN);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static LoginMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        LoginMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(LoginMessage, senderId, receiverId);
};

#endif // LOGINMESSAGE_H