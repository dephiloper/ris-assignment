#ifndef INITMESSAGE_H
#define INITMESSAGE_H

#include "netmessage.h"
#include "../../world.h"

struct InitMessage : NetMessage {
    InitMessage() = default;
    InitMessage(const std::string &senderId) : NetMessage(senderId) {}

    std::string receiverId;
    World world;

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::INIT);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static InitMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        InitMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(InitMessage, receiverId, world);
};

#endif // INITMESSAGE_H