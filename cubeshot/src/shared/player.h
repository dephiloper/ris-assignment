#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include <string>
#include <vector>

struct Player {
    float x;
    float y;
    float z;
    std::vector<float> front;


    NOP_STRUCTURE(Player, x, y, z, front);
};

#endif //PLAYER_H