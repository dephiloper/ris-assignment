#ifndef WORLD_H
#define WORLD_H

#include <nop/structure.h>
#include <utility>
#include <map>

#include "player.h"
#include "obstacle.h"
#include "tile.h"
#include "laser.h"

struct World {
    std::map<std::string, Player> players;
    std::vector<Tile> tiles;
    std::vector<Laser> lasers;

    NOP_STRUCTURE(World, tiles);
};

#endif //WORLD_H