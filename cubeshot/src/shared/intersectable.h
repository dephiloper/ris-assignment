#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include <nop/structure.h>
#include "vector3.h"

enum Type { PLAYER, OBSTACLE };

struct Intersectable {
    Intersectable(Type type) : type(type) {}
    
    Type type;
    Vector3 position;
    Vector3 front;

    NOP_STRUCTURE(Intersectable, position, front);
};

#endif //INTERSECTABLE_H