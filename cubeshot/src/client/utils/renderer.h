#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb/stb_image.h"

#include "map"
#include "vector"
#include "filesystem"
#include "algorithm"

#include "camera.h"
#include "shader.h"
#include "blueprint.h"
#include "../../shared/world.h"
#include "../../shared/utils/constants.h"

class Renderer {
public:
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    std::string assetsDir;
    std::string shaderDir;

    void init();
    void render(const World &world, const std::string &localPlayerId);
    void render(const Camera &camera);
    void render(const Laser &laser, float visibility);
private:
    Shader gameShader{};
    Shader uiShader{};
    enum BlueprintType {TILE, OBSTACLE, PLAYER, CROSS};
    std::map<BlueprintType, Blueprint> blueprints;
    
    unsigned int loadObject(std::vector<float> vertices, unsigned short dimensions, bool hasColor = false, bool hasTexture = false);
    unsigned int loadTexture(unsigned int vao, const std::string& texturePath, bool alphaChannel);
};

#endif