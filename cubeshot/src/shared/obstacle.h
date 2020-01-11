#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <nop/structure.h>
#include "vector3.h"
#include "intersectable.h"

struct Obstacle : Intersectable {
    Obstacle() : Intersectable(OBSTACLE) {}
    float radius = 0.5f;
    
    NOP_STRUCTURE(Obstacle, position);
};

#endif //OBSTACLE_H