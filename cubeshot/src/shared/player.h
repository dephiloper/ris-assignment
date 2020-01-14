#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include "vector3.h"
#include "intersectible.h"

struct Player : Intersectible {
    Player() : Intersectible(PLAYER) {};
    static constexpr float MOVEMENT_SPEED = 4.0f;
    static constexpr float SCALE = 0.8f;
    // correct collision radius would be std::sqrt(std::pow(PLAYER_SCALE, 2.0f) * 2.0f) / 2.0f;
    // but this looks way better!
    static constexpr float COLLISION_RADIUS = SCALE / 2.0f;

    std::vector<Vector3> hitPoints;
    bool hasPotion = false;
    bool enteredNewLocation = false;

    NOP_STRUCTURE(Player, position, front, hitPoints, hasPotion);
};

#endif //PLAYER_H