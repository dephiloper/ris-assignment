#ifndef MOVEMESSAGE_H
#define MOVEMESSAGE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "netmessage.h"
#include "../../../shared/utils/direction.h"
#include "../../../shared/vector3.h"

struct InputMessage : NetMessage {
    InputMessage() = default;
    InputMessage(char direction, glm::vec3 front, bool shoot) : direction(direction), front{front.x, front.y, front.z}, shoot(shoot) { }
    char direction;
    Vector3 front;
    bool shoot;

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::MOVE);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static InputMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        InputMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(InputMessage, direction, front, shoot);
};

#endif // MOVEMESSAGE_H