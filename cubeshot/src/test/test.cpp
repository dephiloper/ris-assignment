#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../shared/player.h"

int main() {
    Player p;
    p.hasPotion = true;

    Player* x = &p;
    x->hasPotion = false;
    std::cout << p.hasPotion << std::endl;
}