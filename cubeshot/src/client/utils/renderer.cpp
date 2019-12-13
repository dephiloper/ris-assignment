#include "renderer.h"

void Renderer::init() {
    assetsDir = std::filesystem::current_path().string() + "/assets/";
    shaderDir = std::filesystem::current_path().string() + "/src/client/shaders/";

    // load shaders
    if (shader.load(shaderDir + "vertex.glsl", shaderDir + "fragment.glsl") != 0)
        std::cout << "Error loading shaders" << std::endl;

    std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int vao = loadObject(vertices, false, true);
    unsigned int textureId = loadTexture(vao, assetsDir + "cube0_texture.png", false);

    blueprints.insert(std::pair(CUBE_0, std::pair(vao, textureId)));
    textureId = loadTexture(vao, assetsDir + "cube1_texture.jpg", false);

    blueprints.insert(std::pair(CUBE_1, std::pair(vao, textureId)));
}

void Renderer::render(const Camera &camera) {
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("view", view);
}

void Renderer::render(const World &world, const std::string &localPlayerId) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, std::get<1>(blueprints.at(CUBE_0)));

    glBindVertexArray(std::get<0>(blueprints.at(CUBE_0)));

    for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(j, 0, i));
            unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, std::get<1>(blueprints.at(CUBE_1)));

    glBindVertexArray(std::get<0>(blueprints.at(CUBE_1)));

    for (auto const& [id, p] : world.players)
    {
        if (id == localPlayerId) {
            continue;
        }
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(p.x, p.y, p.z));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    

}

unsigned int Renderer::loadObject(std::vector<float> vertices, bool hasColor, bool hasTexture) {
    unsigned int index = 0;
    unsigned int stride = 3;
    unsigned int offset = 0;

    if (hasColor)
        stride += 3;
    if (hasTexture)
        stride += 2;

    unsigned int VBO, VAO; // create unique id
    glGenBuffers(1, &VBO); // create buffer object to corresponding id
    glGenVertexArrays(1, &VAO); // create vertex array object   

    // 1. bind buffers and vertex array object
    // -------------------------------------------------------------------
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind created buffer to buffer type GL_ARRAY_BUFFER (type of vertex buffer)

    // after binding, every buffer calls on GL_ARRAY_BUFFER will be used to configure VBO

    // 2. copy vertices array in a buffer for opengl to use
    // copies vertex data into buffer memory
    // 0: type of buffer we want copy data into
    // 1: size of the data
    // 2: the data
    // 3: specifies how the graphics card should manage the given data
    // 3.1: GL_STATIC_DRAW (data does not change or very rarely)
    // 3.2: GL_DYNAMIC_DRAW (data changes a lot)
    // 3.3: GL_STREAM_DRAW (data> changes on every draw) -> 3.2 & 3.3 data is placed in memory for faster drawing
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. set vertex attribute pointers (need explanation)
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    offset += 3;

    if (hasColor) {
        index++;
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(index);
        offset += 3;
    }

    if (hasTexture) {
        index++;
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(index);
        offset += 2;
    }

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

unsigned int Renderer::loadTexture(unsigned int vao, const std::string& texturePath, bool alphaChannel) {
    GLenum format = alphaChannel ? GL_RGBA : GL_RGB;
    
    glBindVertexArray(vao);
    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
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
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture - path: " << texturePath << std::endl;
    }

    // free image resources
    stbi_image_free(data);

    // unbind buffer and vertex array so VAO calls don't accidentally modify VAO, but it's not necessary
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    return texture;
}