#ifndef MOVEMESSAGE_H
#define MOVEMESSAGE_H

#include "netmessage.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct MoveMessage : NetMessage {
    MoveMessage() = default;
    MoveMessage(glm::vec2 dir) : xDir(dir.x), zDir(dir.y) { }
    int xDir = 0;
    int zDir = 0;

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::MOVE);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static MoveMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        MoveMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(MoveMessage, xDir, zDir);
};

#endif // MOVEMESSAGE_H