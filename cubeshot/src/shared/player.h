#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include "vector3.h"
#include "intersectable.h"

struct Player : Intersectable {
    Player() : Intersectable(PLAYER) {};

    std::vector<Vector3> hitPoints;
    bool enteredNewLocation = false;

    NOP_STRUCTURE(Player, position, front, hitPoints);
};

#endif //PLAYER_H