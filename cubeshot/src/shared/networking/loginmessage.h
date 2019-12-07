#ifndef LOGINMESSAGE_H
#define LOGINMESSAGE_H

#include "netmessage.h"
#include <string>

struct LoginMessage : NetMessage {
    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::LOGIN);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static LoginMessage deserialize(std::string data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data);
        LoginMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(LoginMessage, senderId);
};

#endif // LOGINMESSAGE_H