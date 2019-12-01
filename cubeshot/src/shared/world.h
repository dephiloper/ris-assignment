#ifndef WORLD_H
#define WORLD_H

#include <utility>
#include <vector>
#include "player.h"

class World {
private:
   
public:
    World();
    void update(float deltaTime);
    std::vector<Player> players;
    std::vector<char> toBuffer();
    static World fromBuffer(char* buffer);
};

#endif //WORLD_H