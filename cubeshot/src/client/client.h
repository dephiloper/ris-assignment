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
#include "networking/handlers/loginmessagehandler.h"


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

    void processInput(float deltaTime);
    void processMessages();

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