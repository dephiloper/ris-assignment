#ifndef WORLD_H
#define WORLD_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb/stb_image.h"

#include "map"
#include "vector"
#include "filesystem"

#include "utils/shader.h"

class World {
private:
    Shader* shader{};
    enum BlueprintType {CUBE_0, CUBE_1};
    std::map<BlueprintType, std::tuple<unsigned int, unsigned int>> blueprints;
    unsigned int loadObject(std::vector<float> vertices, bool hasColor = false, bool hasTexture = false);
    unsigned int loadTexture(unsigned int vao, const std::string& texturePath, bool alphaChannel);
public:
    const std::string ASSETS_DIR = std::filesystem::current_path().string() + "/assets/";
    explicit World(Shader* shader);
    void setup();
    void render();
    void update(float deltaTime);
};

#endif //WORLD_H