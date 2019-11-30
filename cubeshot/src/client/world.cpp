#include "world.h"
#include <iostream>
#include <GLFW/glfw3.h>

World::World() { 
    Player p(glm::vec3(2.0f, 0.0f, 2.0f));
    players.push_back(p);
}

void World::update(float deltaTime) {
    players[0].position.y = (1 + sin((float)glfwGetTime()*2)) / 2;
}