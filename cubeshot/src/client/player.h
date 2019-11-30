#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Player {
private:
public:
    Player(glm::vec3 position);
    unsigned int id;
    glm::vec3 position;
    void update(float deltaTime);
};

#endif //PLAYER_H