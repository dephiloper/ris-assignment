#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "../shared/tile.h"

int main() {
    auto area = Tile::calculateLocationArea(std::pair(2,2));
    for (auto loc : area) {
        std::cout << loc.first << ", " << loc.second << std::endl;
    }
}