#ifndef WORLD_H
#define WORLD_H

#include <nop/structure.h>
#include <utility>
#include <map>

#include "player.h"
#include "obstacle.h"

struct World {
    std::map<std::string, Player> players;
    std::vector<Obstacle> obstacles;

    NOP_STRUCTURE(World, obstacles);
};

#endif //WORLD_H