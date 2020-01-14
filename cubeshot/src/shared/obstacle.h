#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <nop/structure.h>
#include "vector3.h"
#include "intersectible.h"

struct Obstacle : Intersectible {
    Obstacle() : Intersectible(OBSTACLE) {}
    static constexpr float SCALE = 1.0f;

    float radius = 0.5f;
    
    NOP_STRUCTURE(Obstacle, position);
};

#endif //OBSTACLE_H