#ifndef WORLD_H
#define WORLD_H

#include <nop/structure.h>
#include <utility>
#include <map>

#include "player.h"
#include "obstacle.h"
#include "floor.h"

struct World {
    std::map<std::string, Player> players;
    std::vector<Obstacle> obstacles;
    std::vector<Floor> tiles;

    NOP_STRUCTURE(World, obstacles, tiles);
};

#endif //WORLD_H