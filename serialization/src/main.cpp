#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <nop/serializer.h>
#include <nop/structure.h>
#include <nop/utility/stream_writer.h>
#include <nop/utility/stream_reader.h>

struct Player {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    NOP_STRUCTURE(Player, x, y, z);
};

struct World {
    std::vector<Player> players;
    NOP_STRUCTURE(World, players);
};

struct Message {
  std::string senderId;
  NOP_STRUCTURE(Message, senderId);
};

struct LoginMessage : Message {
    std::string name;
    NOP_STRUCTURE(LoginMessage, senderId, name);
};

struct UpdateMessage : Message {
    Player player;
    NOP_STRUCTURE(UpdateMessage, senderId, player);
};


int main() {
        using Writer = nop::StreamWriter<std::stringstream>;
        using Reader = nop::StreamReader<std::stringstream>;
        nop::Serializer<Writer> serializer;
        nop::Deserializer<Reader> deserializer;

        UpdateMessage msg;
        World world;
        world.players = {Player{0.5f, 0.3f, 0.1f}, Player{0.9f, 0.1f, 0.7f}, Player{0.1f, 0.09f, 3.1f}};
        msg.senderId = "3";
        Player p;
        p.x = 0.5f;
        p.y = 1.3f;
        p.z = 7.5f;
        msg.player = p;
        //msg.name = "phil";
        std::cout << "id: " << msg.senderId << "name: " << msg.player.x  << std::endl;
        int x = 1;
        serializer.Write(x);
        serializer.Write(msg);
        const std::string data = serializer.writer().stream().str();
        deserializer.reader().stream().str(data);
        int type;
        UpdateMessage msg2;
        deserializer.Read(&type);
        deserializer.Read(&msg2);
        std::cout << "id: " << msg2.senderId << "name: " << msg2.player.x  << std::endl;
}