#ifndef PLAYER_H
#define PLAYER_H

#include <nop/structure.h>
#include "vector3.h"

struct Player {
    Vector3 position;
    Vector3 front;

    friend std::ostream& operator <<(std::ostream& os, const Player& p)
    {
        os << "(" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")";
        return os;
    }

    NOP_STRUCTURE(Player, position, front);
};

#endif //PLAYER_H