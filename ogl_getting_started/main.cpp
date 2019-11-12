#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "lib/stb_image/stb_image.h"

const std::string PROJECT_PATH = "/home/phil/Development/realtime-interactive-systems/ogl_getting_started/";
float mixValue = 0.0f;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

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

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mixValue = mixValue - 0.01f <= 0.0f ? 0.0f : (mixValue - 0.01f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mixValue = mixValue + 0.01f >= 1.0f ? 1.0f : (mixValue + 0.01f);
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

    Shader shader(PROJECT_PATH + "shaders/vertex.glsl", PROJECT_PATH + "shaders/fragment.glsl");

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);

    glEnableVertexAttribArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

    // vertices of an triangle
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        3, 1, 2,   // second triangle
    };

    unsigned int VBO, VAO, EBO; // create unique id
    glGenBuffers(1, &VBO); // create buffer object to corresponding id
    glGenBuffers(1, &EBO); // create vertex array object
    glGenVertexArrays(1, &VAO); // create vertex array object

    // 1. bind buffers and vertex array object
    // -------------------------------------------------------------------
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind created buffer to buffer type GL_ARRAY_BUFFER (type of vertex buffer)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. set vertex attribute pointers (need explanation)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(VAO);

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load((PROJECT_PATH + "assets/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // glTexImage2D parameters
        /* The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
         * The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
         * The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
         * The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
         * The next argument should always be 0 (some legacy stuff).
         * The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
         * The last argument is the actual image data.
         */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }


    // free image resources
    stbi_image_free(data);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load((PROJECT_PATH + "assets/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // glTexImage2D parameters
        /* The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
         * The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
         * The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
         * The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
         * The next argument should always be 0 (some legacy stuff).
         * The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
         * The last argument is the actual image data.
         */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    shader.use();
    shader.setInt("texture0", 0);
    shader.setInt("texture1", 1);

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

        shader.use();
        shader.setFloat("mixValue", mixValue);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}