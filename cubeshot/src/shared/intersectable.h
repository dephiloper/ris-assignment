#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include <nop/structure.h>
#include "vector3.h"

enum Type { PLAYER, OBSTACLE };

struct Intersectable {
    Intersectable(const Type& type) : type(type) {}
    
    Type type;
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 front{0.0f, 0.0f, 0.0f};

    NOP_STRUCTURE(Intersectable, position, front);
};

#endif //INTERSECTABLE_H