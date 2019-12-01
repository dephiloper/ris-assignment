#include "world.h"
#include <iostream>
#include <stdlib.h>    

World::World() { 

}

void World::update(float deltaTime) {

}

std::vector<char> World::toBuffer() {
    char data[sizeof(Player) * players.size()];

    for (int i = 0; i < players.size(); i++)
        memcpy(data + i * sizeof(Player), &players[i], sizeof(Player));

    std::vector<char> buf;
    buf.insert(buf.end(), data, data + sizeof(Player) * players.size());
    return buf;
}

World World::fromBuffer(char* buffer) {
    Player p;
    std::cout << sizeof(char) << std::endl;
    p.id = *reinterpret_cast<const int*>(buffer);
    p.position.x = *reinterpret_cast<const float*>(buffer + sizeof(int));
    p.position.y = *reinterpret_cast<const float*>(buffer + sizeof(int) + sizeof(float));
    p.position.z = *reinterpret_cast<const float*>(buffer + sizeof(int)  + sizeof(float) * 2);

    World w;
    w.players.push_back(p);

    return w;
}