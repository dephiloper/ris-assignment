#ifndef WORLD_H
#define WORLD_H

#include <nop/structure.h>
#include <utility>
#include <map>

#include "player.h"

struct World {
    std::map<std::string, Player> players;

    NOP_STRUCTURE(World, players);
};

#endif //WORLD_H