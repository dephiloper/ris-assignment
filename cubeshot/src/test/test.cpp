#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Intersectable {
    glm::vec3 position;
};

struct Player : Intersectable {
    glm::vec3 hitPoints;
    bool enteredNewLocation = false;
};

int main() {
    Player p;
    p.position = glm::vec3(5,4,5);
    std::vector<Intersectable*> ins;
    ins.push_back(&p);
}