#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <iostream>
#include <filesystem>

#include "utils/shader.h"
#include "utils/camera.h"
#include "world.h"

class Client {
private:
    const std::string SHADER_DIR = std::filesystem::current_path().string() + "/src/client/shaders/";
    
    GLFWwindow* window;
    Shader shader;
    Camera camera;
    World world;
    float lastFrame;

    void init();
    void render(float deltaTime);
    void processInput(float deltaTime);

public:
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 600;
    
    float mouseX;
    float mouseY;

    Client();
    void mainLoop();
    void handleMouseInput(double xPos, double yPos);

};

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif //CLIENT_H