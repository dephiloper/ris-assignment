#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../intersectible.h"
#include "raycast.h"

const std::vector<glm::vec3> faceNormals = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f) };

class Collision {
private:
    // distance function
    static float cubeDistance(const glm::vec3& p, float r) {
        glm::vec3 p2 = glm::abs(p) - r;
        //return glm::length(glm::max(p2, glm::vec3(0)));
        return glm::max(p2.x, glm::max(p2.y, p2.z));
    }

    // see https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    static bool linePlaneIntersection(const glm::vec3 &n, const glm::vec3 &p0, const glm::vec3 &l0, const glm::vec3 &l, float &t) { 
        // assuming vectors are all normalized
        float denom = glm::dot(n, l); 
        if (denom > 1e-6) { 
            auto p0l0 = p0 - l0; 
            t = glm::dot(p0l0, n) / denom; 
            return (t >= 0); 
        } 
        return false; 
    }

public:
    static float calculateParametricDistance(const Intersectible& intersectable, RayCast& ray) {
        float halfWidth = (intersectable.type == PLAYER ? Player::SCALE : Obstacle::SCALE) / 2.0f; 

        // calculate translation and rotation matrix of the player
        glm::mat4 translation = glm::mat4(1.0f);
        translation = glm::translate(translation, static_cast<glm::vec3>(intersectable.position));
        glm::vec3 target = static_cast<glm::vec3>(intersectable.front);
        glm::vec3 r = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), target);
        glm::mat4 rotation = glm::inverse(glm::lookAt(glm::vec3(0), target, glm::vec3(0, 1, 0)));

        // translate and rotate the ray origin/ray direction according to the possible player to hit (player to hit at the origin of the coordinate system)
        glm::vec4 translatedOrigin = glm::inverse(translation * rotation) * glm::vec4(ray.origin, 1.0f); // translate and rotate ray origin
        glm::vec4 rotatedDirection = glm::normalize(glm::inverse(rotation) * glm::vec4(ray.direction, 1.0f)); // just rotate the ray direction

        // check all possible face normals if an intersection is possible
        std::vector<float> distances;
        std::vector<glm::vec3> modelIntersections;
        std::vector<glm::vec3> worldIntersections;

        for (auto const& face : faceNormals) {
            float parametricDistance;
            if (linePlaneIntersection(-face, face * halfWidth, translatedOrigin, rotatedDirection, parametricDistance)) {
                glm::vec3 modelIntersection = (translatedOrigin + parametricDistance * rotatedDirection);
                if (Collision::cubeDistance(modelIntersection, halfWidth) < 0.001) { // apply distance function between intersection point an face plane
                    modelIntersections.push_back(modelIntersection);
                    // transform back
                    auto origin = translation * rotation * translatedOrigin;
                    auto direction = rotation * rotatedDirection;
                    worldIntersections.push_back(origin + direction * parametricDistance);
                    distances.push_back(parametricDistance);
                }
            }
        }

        if (distances.size() > 0) {
            int minIndex = std::min_element(distances.begin(),distances.end()) - distances.begin();
            ray.modelIntersection = modelIntersections[minIndex];
            ray.worldIntersection = worldIntersections[minIndex];
            ray.length = distances[minIndex];
            return ray.length;
        }
        
        return -1;
    }

    static bool lineCircleCollision(const glm::vec2& lineOrigin, const glm::vec2& ahead, const glm::vec2& ahead2, const glm::vec2& circleOrigin, float radius) {
        return glm::distance(lineOrigin, circleOrigin) <= radius * 2 || glm::distance(ahead, circleOrigin) <= radius * 2 || glm::distance(ahead2, circleOrigin) <= radius * 2;
    }

    static bool squareCircleCollision(const glm::vec2& squarePosition, const glm::vec2& circlePosition, float squareWidth, float circleRadius) {
        float testX = circlePosition.x, testZ = circlePosition.y;
        if (circlePosition.x < squarePosition.x - squareWidth)       testX = squarePosition.x - circleRadius; // left edge
        else if (circlePosition.x > squarePosition.x + squareWidth)  testX = squarePosition.x + circleRadius; // right edge
        if (circlePosition.y < squarePosition.y - squareWidth)       testZ = squarePosition.y - circleRadius; // top edge
        else if (circlePosition.y > squarePosition.y + squareWidth)  testZ = squarePosition.y + circleRadius; // bottom edge

        float distance = glm::distance(circlePosition, glm::vec2(testX, testZ));
        
        return (distance <= circleRadius);
    }
};

#endif //COLLISION_H