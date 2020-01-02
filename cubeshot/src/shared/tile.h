#ifndef TILE_H
#define TILE_H

#include <nop/structure.h>
#include "vector3.h"
#include "obstacle.h"

struct Tile {
    Vector3 position;
    Vector3 scale{ 20.0f, 0.2f, 20.0f };
    std::vector<Obstacle> obstacles;
    NOP_STRUCTURE(Tile, position, obstacles);
};

#endif //TILE_H