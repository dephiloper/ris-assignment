#include "client.h"

Client client;

int main() {
    client.mainLoop();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    client.handleMouseInput(xPos, yPos);
}

Client::Client() : camera(glm::vec3(0.0f, 1.0f, 0.0f)), netManager("localhost", 5555) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use only subset of core functionality

    // glfw window creation, check if window is successfully created, otherwise error
    window = glfwCreateWindow(renderer.SCREEN_WIDTH, renderer.SCREEN_HEIGHT, "Cubeshot", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    //glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers, check if glad is successfully included
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;

    glEnable(GL_DEPTH_TEST);

    stbi_set_flip_vertically_on_load(true);

    renderer.init();
    netManager.start(netManager);
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(InitMessage), std::make_unique<InitMessageHandler>(&playerId, &world)));
    listeners.insert(std::pair<std::type_index, std::unique_ptr<NetMessageHandler>>(typeid(UpdateMessage), std::make_unique<UpdateMessageHandler>(&world)));
    netManager.login();
}

void Client::mainLoop() {
    while(!glfwWindowShouldClose(window)) {
        auto currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using (uses current state to retrieve color)

        processInput(deltaTime);
        processMessages();
        
        // update player
        auto localPlayer = world.players[playerId];
        camera.updatePosition(Vector3::toGlm(localPlayer.position));
        
        renderer.render(camera);
        renderer.render(world, playerId);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    netManager.logout();
    netManager.stop();
}

void Client::processInput(float deltaTime) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // http://disq.us/p/1nt0anm
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
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

    //if (input.direction != direction || camera.pitch) {
    netManager.queueOut.push(std::make_shared<InputMessage>(direction, camera.front));
    //}

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Client::handleMouseInput(double xPos, double yPos) {
    float xOffset = ((float)xPos) - mouseX;
    float yOffset = mouseY - ((float)yPos); // reversed since y-coordinates go from bottom to top

    mouseX = (float)xPos;
    mouseY = (float)yPos;

    camera.processMouseMovement(xOffset, yOffset);
}

void Client::processMessages() {
    auto queueCount = netManager.queueIn.size();
    // process only currently received messages
    for (auto i = 0; i < queueCount; i++) {
        auto msg = netManager.queueIn.pop();
        listeners[typeid(*msg)]->handle(*msg);
    }
}