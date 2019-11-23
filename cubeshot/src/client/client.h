#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image/stb_image.h"

#include <vector>
#include <iostream>
#include <filesystem>

#include "utils/shader.h"
#include "utils/camera.h"

class Client {
private:
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    
    GLFWwindow* window;
    Shader shader;
    Camera camera;
    float deltaTime;
    float lastFrame;


public:
    unsigned int VAO;
    const std::string SHADER_DIR = std::filesystem::current_path().string() + "/src/client/shaders/";
    const std::string ASSETS_DIR = std::filesystem::current_path().string() + "/assets/";

    float mouseX;
    float mouseY;

    Client();
    void init();
    void mainLoop();
    void processInput();
    unsigned int loadObject(std::vector<float> vertices, bool hasColor = false, bool hasTexture = false);
    unsigned int loadTexture(unsigned int vao, std::string texturePath, bool alphaChannel);
    void handleMouseInput(double xPos, double yPos);
};

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);