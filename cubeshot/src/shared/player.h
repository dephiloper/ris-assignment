#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include "vector3.h"

struct Player {
    Vector3 position;
    Vector3 front;
    std::vector<Vector3> hitPoints;

    bool enteredNewLocation = false;
    NOP_STRUCTURE(Player, position, front, hitPoints);
};

#endif //PLAYER_H