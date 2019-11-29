#include "client.h"

Client client;

int main() {
    client.mainLoop();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    client.handleMouseInput(xPos, yPos);
}

Client::Client() : camera(glm::vec3(0.0f, 0.0f, 0.0f)), world(&shader) {
    init();
    networkManager.start();
    world.setup();
}

void Client::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use only subset of core functionality

    // glfw window creation, check if window is successfully created, otherwise error
    window = glfwCreateWindow(800, 600, "OGL_GETTING_STARTED", nullptr, nullptr);
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

    // load shaders
    if (shader.load(SHADER_DIR + "vertex.glsl", SHADER_DIR + "fragment.glsl") != 0)
        std::cout << "Error loading shaders" << std::endl;

    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
}

void Client::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using (uses current state to retrieve color)
    
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);
    world.render();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(window);
}

void Client::mainLoop() {
    while(!glfwWindowShouldClose(window)) {
        auto currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(deltaTime);
        world.update(deltaTime);
        render();

        glfwPollEvents();
    }

    networkManager.stop();
}

void Client::processInput(float deltaTime) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // http://disq.us/p/1nt0anm
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT)  ;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        networkManager.inputQueue.push(0);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        networkManager.inputQueue.push(1);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        networkManager.inputQueue.push(2);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        networkManager.inputQueue.push(3);

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.ProcessKeyboard(FORWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     camera.ProcessKeyboard(BACKWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     camera.ProcessKeyboard(LEFT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     camera.ProcessKeyboard(RIGHT, deltaTime);

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

    camera.ProcessMouseMovement(xOffset, yOffset);
}