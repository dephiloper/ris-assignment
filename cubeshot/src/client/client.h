#ifndef CLIENT_H
#define CLIENT_H

#include <vector>
#include <iostream>
#include <filesystem>
#include <typeindex>
#include <typeinfo>
#include <map>

#include "utils/camera.h"
#include "utils/renderer.h"
#include "../shared/networking/messages/inputmessage.h"
#include "networking/clientnetmanager.h"
#include "../shared/world.h"
#include "../shared/networking/netmessagehandler.h"
#include "networking/handlers/updatemessagehandler.h"
#include "networking/handlers/initmessagehandler.h"
#include "numeric"

class Client {
private:
    GLFWwindow* window{};
    ClientNetManager netManager;
    Camera camera;
    Renderer renderer;
    World world;
    float lastFrame{};
    std::map<std::type_index, std::shared_ptr<NetMessageHandler>> listeners;
    std::string playerId;
    InputMessage input;
    glm::vec2 target;
    int64_t nextUpdateTimestamp = std::numeric_limits<int64_t>::max();

    float mouseX{};
    float mouseY{};
    bool shoot{};

    void processInput(float deltaTime);
    void processMessages();

public:
    Client();
    void mainLoop();
    void handleMouseInput(double xPos, double yPos);
    void handleMouseButtonInput(int button, int action, int mods);
    void handleResizeViewport(int width, int height);
};

void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif //CLIENT_H