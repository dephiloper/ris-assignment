#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "player.h"

class World {
private:
   
public:
    World();
    void update(float deltaTime);
    std::vector<Player> players;
};

#endif //WORLD_H