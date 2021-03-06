#ifndef UPDATEMESSAGE_H
#define UPDATEMESSAGE_H

#include "netmessage.h"
#include "../../world.h"
#include <string>

struct UpdateMessage : NetMessage {
    UpdateMessage() = default;
    UpdateMessage(const std::string &senderId) : NetMessage(senderId) {}
    
    std::map<std::string, Player> players;
    std::vector<Tile> tiles;
    std::vector<Laser> lasers;
    Potion potion;

    std::string serialize() {
        nop::Serializer<nop::StreamWriter<std::stringstream>> serializer;
        serializer.Write(Command::UPDATE);
        serializer.Write(*this);
        return serializer.writer().stream().str();
    }

    static UpdateMessage deserialize(const std::string &data) {
        nop::Deserializer<nop::StreamReader<std::stringstream>> deserializer;
        deserializer.reader().stream().str(data.substr(1));
        UpdateMessage msg;
        deserializer.Read(&msg);
        return msg;
    }
    
    NOP_STRUCTURE(UpdateMessage, players, tiles, lasers, potion);
};

#endif // UPDATEMESSAGE_H