#ifndef RAYCAST_H
#define RAYCAST_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct RayCast {
    RayCast() = default;
    RayCast(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}
    glm::vec3 origin;
    glm::vec3 direction;
    bool hit = false;
    float length;
    glm::vec3 modelIntersection;
    glm::vec3 worldIntersection;
    std::string intersectibleId;
};

#endif //RAYCAST_H