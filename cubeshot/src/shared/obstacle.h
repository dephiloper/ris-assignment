#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <nop/structure.h>
#include "vector3.h"

struct Obstacle {
    Vector3 position;
    float radius = 0.5f;
    NOP_STRUCTURE(Obstacle, position);
};

#endif //OBSTACLE_H