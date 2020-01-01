#ifndef FLOOR_H
#define FLOOR_H

#include <nop/structure.h>
#include "vector3.h"

struct Floor {
    Vector3 position;
    Vector3 scale{ 10.0f, 0.2f, 10.0f };
    NOP_STRUCTURE(Floor, position);
};

#endif //FLOOR_H