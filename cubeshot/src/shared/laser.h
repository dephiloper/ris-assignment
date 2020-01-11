#ifndef LASER_H
#define LASER_H

#include <nop/structure.h>
#include "vector3.h"

struct Laser {
    Vector3 origin;
    Vector3 target;
    int64_t spawnTime{};
    NOP_STRUCTURE(Laser, origin, target, spawnTime);
};

#endif //LASER_H