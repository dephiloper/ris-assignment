#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <iostream>
#include <filesystem>

#include "utils/camera.h"
#include "utils/renderer.h"
#include "networkmanager.h"
#include "world.h"

class Client {
private:
    GLFWwindow* window{};
    NetworkManager networkManager;
    Camera camera;
    Renderer renderer;
    World world;
    float lastFrame{};

    void processInput(float deltaTime);

public:
    float mouseX{};
    float mouseY{};

    Client();
    void mainLoop();
    void handleMouseInput(double xPos, double yPos);

};

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif //CLIENT_H