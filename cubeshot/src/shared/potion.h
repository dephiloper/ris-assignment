#ifndef POTION_H
#define POTION_H

#include <nop/structure.h>
#include "vector3.h"

struct Potion {
    Vector3 position;

    float radius = 0.3f;
    glm::vec3 scale{0.4, 0.4, 0.4};
    glm::vec3 velocity{0.0, 0.0, 0.0};

    glm::vec3 seek(glm::vec3 target) {
        glm::vec3 desiredVelocity = glm::normalize(target - static_cast<glm::vec3>(position)) * 10.0f;
        return desiredVelocity - velocity; // return steering
    }

    NOP_STRUCTURE(Potion, position);
};

#endif //POTION_H