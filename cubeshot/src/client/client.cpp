#include "client.h"

Client client;

int main() {
    client.mainLoop();
    return 0;
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    client.handleResizeViewport(width, height);
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos) {
    client.handleMouseInput(xPos, yPos);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    client.handleMouseButtonInput(button, action, mods);
}

Client::Client() : camera(glm::vec3(0.0f, 1.0f, 0.0f)), netManager("localhost", 5555) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use only subset of core functionality

    // glfw window creation, check if window is successfully created, otherwise error
    window = glfwCreateWindow(renderer.screenWidth, renderer.screenHeight, "Cubeshot", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    //glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers, check if glad is successfully included
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);

    renderer.init();
    netManager.start();
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(InitMessage),
                                                                                    std::make_unique<InitMessageHandler>(
                                                                                            &playerId, &world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(UpdateMessage),
                                                                                    std::make_unique<UpdateMessageHandler>(
                                                                                            &world)));
    netManager.login();
}

void Client::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = (float) glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using (uses current state to retrieve color)

        processInput(deltaTime);
        processMessages();

        // update player
        auto localPlayer = world.players[playerId];
        camera.updatePosition(static_cast<glm::vec3>(localPlayer.position));

        glm::vec3 right = glm::normalize(
                glm::cross(static_cast<glm::vec3>(localPlayer.front), glm::vec3(0.0, 1.0, 0.0)));
        glm::vec3 yIgnoredFront = glm::normalize(glm::cross(WORLD_UP, right));
        glm::vec2 playerPos = glm::vec2(localPlayer.position.x, localPlayer.position.z);
        glm::vec2 target;
	bool targetFound = false;	
	
    	for (const auto&[id, p] : world.players) {
	   if (id != playerId) {
	      target = glm::vec2(p.position.x, p.position.z);
	      targetFound = true;
	   }
        }

        /*if (world.potion.isActive) {
            target = glm::vec2(world.potion.position.x, world.potion.position.z);
        } else if (!localPlayer.hasPotion) {
            for (const auto&[id, p] : world.players) {
                if (p.hasPotion) {
                    target = glm::vec2(p.position.x, p.position.z);
                }
            }
        }*/

        glm::vec2 direction = glm::normalize(target - playerPos);
        float angle = acos(glm::dot(direction, glm::normalize(glm::vec2(yIgnoredFront.x, yIgnoredFront.z))));
        angle *= glm::dot(direction, glm::vec2(right.x, right.z)) < 0 ? 1 : -1;
        renderer.renderUi(angle, glm::distance(target, playerPos), targetFound);

        renderer.render(camera);
        renderer.render(world, playerId);

        auto laserVisibility = [](const Laser &laser) {
            auto timeLeft = laser.spawnTime + 0.5e6 - currentMillis();
            float visibility = glm::max(timeLeft / 0.5e6, 0.0);
            return visibility;
        };

        // & turns stateless to stateful so referencing locale variables is possible 
        world.lasers.erase(std::remove_if(world.lasers.begin(), world.lasers.end(), [&](const Laser &laser) {
            return laserVisibility(laser) <= 0.01;
        }), world.lasers.end());


        for (const auto &laser : world.lasers) {
            renderer.render(laser, laserVisibility(laser));
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    netManager.logout();
    netManager.stop();
}

void Client::processInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // http://disq.us/p/1nt0anm
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    char direction = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction |= FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction |= BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction |= LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction |= RIGHT;

    netManager.queueOut.push(std::make_shared<InputMessage>(direction, camera.front, shoot));
    shoot = false;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Client::handleMouseInput(double xPos, double yPos) {
    float xOffset = ((float) xPos) - mouseX;
    float yOffset = mouseY - ((float) yPos); // reversed since y-coordinates go from bottom to top

    mouseX = (float) xPos;
    mouseY = (float) yPos;

    camera.processMouseMovement(xOffset, yOffset);
}

void Client::handleMouseButtonInput(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        shoot = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        camera.zoom /= 2.0f;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        camera.zoom = ZOOM;
}

void Client::handleResizeViewport(int width, int height) {
    renderer.screenWidth = width;
    renderer.screenHeight = height;
}

void Client::processMessages() {
    auto queueCount = netManager.queueIn.size();
    // process only currently received messages
    for (auto i = 0; i < queueCount; i++) {
        auto msg = netManager.queueIn.pop();
        listeners[typeid(*msg)]->handle(*msg);
    }
}
