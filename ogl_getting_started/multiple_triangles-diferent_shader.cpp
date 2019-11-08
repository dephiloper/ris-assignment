#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

const std::string PROJECT_PATH = "/home/phil/Development/realtime-interactive-systems/ogl_getting_started/";

std::string readFile(const std::string& filepath);
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * https://stackoverflow.com/a/2602258
 */
std::string readFile(const std::string& filepath) {
    std::ifstream t(filepath);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // http://disq.us/p/1nt0anm
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT)  ;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use only subset of core functionality

    // glfw window creation, check if window is successfully created, otherwise error
    GLFWwindow* window = glfwCreateWindow(800, 600, "OGL_GETTING_STARTED", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers, check if glad is successfully included
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // render triangle
    // -------------------------------------------------------------------

    // create vertex shader
    // -------------------------------------------------------------------
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShader == 0) std::cout << "Error occurred while processing vertex shader." << std::endl;

    // 1: shader object, 2: how many shader strings, 3: shader source code
    std::string vertexShaderSource = readFile(PROJECT_PATH + "shaders/vertex.glsl");
    const GLchar* sourcePtr = vertexShaderSource.c_str(); // https://stackoverflow.com/a/30804288
    glShaderSource(vertexShader, 1, &sourcePtr, nullptr);
    glCompileShader(vertexShader);

    // check compilation for errors
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // create fragment shader
    // -------------------------------------------------------------------
    unsigned int orangeFragmentShader;
    orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (orangeFragmentShader == 0) std::cout << "Error occurred while processing fragment shader." << std::endl;

    // 1: shader object, 2: how many shader strings, 3: shader source code
    std::string fragmentShaderSource = readFile(PROJECT_PATH + "shaders/fragment.glsl");
    sourcePtr = fragmentShaderSource.c_str();
    glShaderSource(orangeFragmentShader, 1, &sourcePtr, nullptr);
    glCompileShader(orangeFragmentShader);

    // check compilation for errors
    glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(orangeFragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int yellowFragmentShader;
    yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (yellowFragmentShader == 0) std::cout << "Error occurred while processing fragment shader." << std::endl;

    // 1: shader object, 2: how many shader strings, 3: shader source code
    std::string yellowFragmentShaderSource = readFile(PROJECT_PATH + "shaders/yellow_fragment.glsl");
    sourcePtr = yellowFragmentShaderSource.c_str();
    glShaderSource(yellowFragmentShader, 1, &sourcePtr, nullptr);
    glCompileShader(yellowFragmentShader);

    // check compilation for errors
    glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(yellowFragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // linking shaders into shader program
    // -------------------------------------------------------------------
    unsigned int orangeShaderProgram;
    orangeShaderProgram = glCreateProgram();
    if (orangeShaderProgram == 0) std::cout << "Error occurred while creating shader program." << std::endl;

    glAttachShader(orangeShaderProgram, vertexShader);
    glAttachShader(orangeShaderProgram, orangeFragmentShader);
    glLinkProgram(orangeShaderProgram);

    glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(orangeShaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(orangeShaderProgram);
    // after creating the program we delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragmentShader);


    unsigned int yellowShaderProgram;
    yellowShaderProgram = glCreateProgram();
    if (yellowShaderProgram == 0) std::cout << "Error occurred while creating shader program." << std::endl;

    glAttachShader(yellowShaderProgram, vertexShader);
    glAttachShader(yellowShaderProgram, yellowFragmentShader);
    glLinkProgram(yellowShaderProgram);

    glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(yellowShaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(yellowShaderProgram);
    // after creating the program we delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(yellowFragmentShader);



    // Attributes of glVertexAttribPointer
    /* The first parameter specifies which vertex attribute we want to configure. Remember that we specified the
     * location of the position vertex attribute in the vertex shader with layout (location = 0). This sets the
     * location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
     *
     * The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed
     * of 3 values.
     *
     * The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating
     * point values).
     *
     * The next argument specifies if we want the data to be normalized. If we're inputting integer data types
     * (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 (or -1 for signed data)
     * and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
     *
     * The fifth argument is known as the stride and tells us the space between consecutive vertex attributes.
     * Since the next set of position data is located exactly 3 times the size of a float away we specify that
     * value as the stride. Note that since we know that the array is tightly packed (there is no space between
     * the next vertex attribute value) we could've also specified the stride as 0 to let OpenGL determine the
     * stride (this only works when values are tightly packed). Whenever we have more vertex attributes we have to
     * carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
     *
     * The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data
     * begins in the buffer. Since the position data is at the start of the data array this value is just 0. We will
     * explore this parameter in more detail later on
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    // vertices of an triangle
    float vertices_right[] = {
         0.5,  0.5, .0, // right triangle top
         1.0, -0.5, .0, // right triangle bottom right
         0.0, -0.5, .0, // right triangle bottom left
    };

    float vertices_left[] = {
         0.0, -0.5, .0, // left triangle bottom right
        -1.0, -0.5, .0, // left triangle bottom left
        -0.5,  0.5, .0  // left triangle top
    };


    unsigned int VBO_0, VBO_1, VAO_0, VAO_1; // create unique id
    glGenBuffers(1, &VBO_0); // create buffer object to corresponding id
    glGenBuffers(1, &VBO_1); // create buffer object to corresponding id
    glGenVertexArrays(1, &VAO_0); // create vertex array object
    glGenVertexArrays(1, &VAO_1); // create vertex array object

    // 1. bind buffers and vertex array object
    // -------------------------------------------------------------------
    glBindVertexArray(VAO_0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_0); // bind created buffer to buffer type GL_ARRAY_BUFFER (type of vertex buffer)

    // after binding, every buffer calls on GL_ARRAY_BUFFER will be used to configure VBO

    // 2. copy vertices array in a buffer for opengl to use
    // copies vertex data into buffer memory
    // 0: type of buffer we want copy data into
    // 1: size of the data
    // 2: the data
    // 3: specifies how the graphics card should manage the given data
    // 3.1: GL_STATIC_DRAW (data does not change or very rarely)
    // 3.2: GL_DYNAMIC_DRAW (data changes a lot)
    // 3.3: GL_STREAM_DRAW (data changes on every draw) -> 3.2 & 3.3 data is placed in memory for faster drawing
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_right), vertices_right, GL_STATIC_DRAW);

    // 3. set vertex attribute pointers (need explanation)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindVertexArray(VAO_0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(VAO_1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_left), vertices_left, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(VAO_1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting
        glClear(GL_COLOR_BUFFER_BIT); // state-using (uses current state to retrieve color)

        // double buffer
        /*
         * When an application draws in a single buffer the resulting image might display flickering issues.
         * This is because the resulting output image is not drawn in an instant, but drawn pixel by pixel and
         * usually from left to right and top to bottom. Because this image is not displayed at an instant to
         * the user while still being rendered to, the result may contain artifacts. To circumvent these issues,
         * windowing applications apply a double buffer for rendering. The front buffer contains the final output
         * image that is shown at the screen, while all the rendering commands draw to the back buffer. As soon
         * as all the rendering commands are finished we swap the back buffer to the front buffer so the image
         * is instantly displayed to the user, removing all the aforementioned artifacts.
         */

        glUseProgram(orangeShaderProgram);
        glBindVertexArray(VAO_0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0); // no need to unbind it every time
        glUseProgram(yellowShaderProgram);
        glBindVertexArray(VAO_1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0); // no need to unbind it every time


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources
    glDeleteVertexArrays(1, &VAO_0);
    glDeleteVertexArrays(1, &VAO_1);
    glDeleteBuffers(1, &VBO_0);
    glDeleteBuffers(1, &VBO_1);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}
#pragma clang diagnostic pop