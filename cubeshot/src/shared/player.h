#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include <string>

struct Player {
    float x;
    float y;
    float z;

    NOP_STRUCTURE(Player, x, y, z);
};

#endif //PLAYER_H