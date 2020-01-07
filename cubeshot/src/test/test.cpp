#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float cube(const glm::vec3& p, float r) {
    glm::vec3 p2 = glm::abs(p) - r;
    return glm::length(glm::max(p2, glm::vec3(0)));
}

bool intersectPlane(const glm::vec3 &n, const glm::vec3 &p0, const glm::vec3 &l0, const glm::vec3 &l, float &t) 
{ 
    // assuming vectors are all normalized
    float denom = glm::dot(n, l); 
    if (denom > 1e-6) { 
        auto p0l0 = p0 - l0; 
        t = glm::dot(p0l0, n) / denom; 
        return (t >= 0); 
    } 
 
    return false; 
}

int main() {
    glm::vec3 planeNormal = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)); // why does this need to point in the opposite direction?
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 2.4f, 0.0f));
    glm::vec4 planeOrigin = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    planeOrigin = model * planeOrigin;
    std::cout << "plane origin: " << planeOrigin.x << ", " << planeOrigin.y << ", " << planeOrigin.z << std::endl;
    planeOrigin = glm::inverse(model) * planeOrigin;
    std::cout << "undo translation: " << planeOrigin.x << ", " << planeOrigin.y << ", " << planeOrigin.z << std::endl;
    glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rayDirection = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

    float t;
    std::cout << "intersection testing" << std::endl;
    if (intersectPlane(planeNormal, planeOrigin, rayOrigin, rayDirection, t)) {
        glm::vec4 intersectionPoint = glm::vec4(glm::vec3(rayOrigin + rayDirection * t), 1.0f);
        std::cout << "intersection point: " << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << std::endl;
        intersectionPoint = glm::inverse(model) * intersectionPoint;
        std::cout << "intersection point: " << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << std::endl;
        float distance = cube(glm::vec3(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z), 0.4);
        std::cout << "distance " << distance << std::endl; // why is distance not 0?
    }
}