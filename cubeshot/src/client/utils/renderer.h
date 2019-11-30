#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include "map"
#include "vector"
#include "filesystem"

#include "../world.h"
#include "camera.h"
#include "shader.h"

class Renderer {
public:
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    std::string assetsDir;
    std::string shaderDir;

    void init();
    void render(World &world);
    void render(Camera &camera);
private:
    Shader shader{};
    enum BlueprintType {CUBE_0, CUBE_1};
    std::map<BlueprintType, std::tuple<unsigned int, unsigned int>> blueprints;
    
    unsigned int loadObject(std::vector<float> vertices, bool hasColor = false, bool hasTexture = false);
    unsigned int loadTexture(unsigned int vao, const std::string& texturePath, bool alphaChannel);
};

#endif