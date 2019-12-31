#ifndef VECTOR3_H
#define VECTOR3_H

#include <nop/structure.h>
#include <glm/glm.hpp>

struct Vector3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    static glm::vec3 toGlm(Vector3 vec) {
        return glm::vec3(vec.x, vec.y, vec.z);
    }

    static Vector3 from(glm::vec3 vec) {
        return Vector3{vec.x, vec.y, vec.z};
    }


    NOP_STRUCTURE(Vector3, x, y, z);
};

#endif //VECTOR3_H