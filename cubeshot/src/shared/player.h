#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include "vector3.h"

struct Player {
    Vector3 position;
    Vector3 front;

    NOP_STRUCTURE(Player, position, front);
};

#endif //PLAYER_H