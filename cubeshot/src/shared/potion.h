#ifndef POTION_H
#define POTION_H

#include <nop/structure.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "vector3.h"
#include "physics/helpers.h"
#include "physics/collision.h"

struct Potion {
    Vector3 position;
    static constexpr float COLLISION_RADIUS = 0.2f;

    static constexpr float MAX_FORCE = 0.2f;
    static constexpr float MAX_SPEED = 2.0f;
    static constexpr float WANDER_CIRCLE_DISTANCE = 2.0f;
    static constexpr float WANDER_CIRCLE_RADIUS = 2.0f;
    static constexpr float ANGLE_CHANGE = 0.5f;
    static constexpr float MAX_SEE_AHEAD = 2.0f;
    static constexpr float MAX_AVOID_FORCE = 100.0f;

    glm::vec3 scale{0.4, 0.4, 0.4};
    glm::vec3 velocity{0.0, 0.0, 1.0};
    float wanderAngle = 0.0f;

    glm::vec3 wander() {
        glm::vec3 circleCenter = glm::normalize(velocity) * MAX_SPEED;
        circleCenter *= WANDER_CIRCLE_DISTANCE;

        glm::vec3 displacement = glm::vec3(0.0f, 0.0f, -WANDER_CIRCLE_RADIUS);
        displacement = glm::rotate(displacement, wanderAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        wanderAngle += (frand() * ANGLE_CHANGE) - (ANGLE_CHANGE * 0.5f);
        glm::vec3 wanderForce = circleCenter + displacement;
        return wanderForce; // wander force
    }

    glm::vec3 collisionAvoidance(const std::vector<Obstacle>& obstacles) {
        float dynamicLength = glm::length(velocity) / MAX_SPEED; 
        glm::vec3 ahead = static_cast<glm::vec3>(position) + glm::normalize(velocity) * MAX_SEE_AHEAD * dynamicLength; // calculate the ahead vector
        glm::vec3 ahead2 = static_cast<glm::vec3>(position) + glm::normalize(velocity) * MAX_SEE_AHEAD * dynamicLength / 2.0f;
        std::optional<Obstacle> obstacle = findObstacleToAvoid(ahead, ahead2, obstacles);
        glm::vec2 avoidance = glm::vec2(0);
    
        if (obstacle.has_value()) {
            avoidance.x = ahead.x - obstacle->position.x + frand() * 0.2f - 0.1f;
            avoidance.y = ahead.z - obstacle->position.z + frand() * 0.2f - 0.1f;
            avoidance = glm::normalize(avoidance);
            avoidance *= (MAX_AVOID_FORCE);
        } 

        return glm::vec3(avoidance.x, 0.0f, avoidance.y);
    }

    std::optional<Obstacle> findObstacleToAvoid(glm::vec3 ahead, glm::vec3 ahead2, const std::vector<Obstacle>& obstacles) {
        std::optional<Obstacle> closestObstacle = std::nullopt;
        for (const auto& o : obstacles) {
            bool collision = Collision::lineCircleCollision(static_cast<glm::vec3>(position), ahead, ahead2, o);
            glm::vec2 pos = glm::vec2(position.x, position.z);
            glm::vec2 oPos = glm::vec2(o.position.x, o.position.z);

            if (collision) {
                // no closest obstalce or this one is closer than the previous one
                if (closestObstacle == std::nullopt || glm::distance(pos, oPos) < glm::distance(pos, glm::vec2(closestObstacle->position.x, closestObstacle->position.z))) {
                    closestObstacle = o;
                }
            }
        }

        return closestObstacle;
    }

    NOP_STRUCTURE(Potion, position);
};

#endif //POTION_H