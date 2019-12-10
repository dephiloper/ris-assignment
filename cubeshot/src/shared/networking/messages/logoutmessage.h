#ifndef LOGOUTMESSAGE_H
#define LOGOUTMESSAGE_H

#include "netmessage.h"

struct LogoutMessage : NetMessage {
    LogoutMessage() = default;
    LogoutMessage(const std::string &senderId) : NetMessage(senderId) {}

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::LOGOUT);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static LogoutMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        LogoutMessage msg;
        deserializer.Read(&msg);
        return msg;
    }

    NOP_STRUCTURE(LogoutMessage, senderId);
};

#endif // LOGOUTMESSAGE_H